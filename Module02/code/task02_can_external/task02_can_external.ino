#include <Arduino.h>
#include "esp_twai.h"
#include "esp_twai_onchip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Classic ESP32 + SN65HVD230. No MCP2515 or third-party CAN library.
// Set NODE_B=true on the SECOND board: distinct publisher ID and payload.
constexpr bool NODE_B = false;
constexpr gpio_num_t TX_PIN = GPIO_NUM_5, RX_PIN = GPIO_NUM_4;
constexpr uint32_t SEND_ID = NODE_B ? 0x322 : 0x321;
struct ReceivedFrame {
  uint32_t id;
  uint8_t dlc;
  bool extended;
  bool remote;
  uint8_t data[8];
};
twai_node_handle_t node = nullptr;
QueueHandle_t rxQueue = nullptr, txQueue = nullptr, errorQueue = nullptr;
uint8_t txData[8] = {};
twai_frame_t txFrame = {};
bool pending = false, timeoutReported = false, recoveryRequested = false;
uint32_t sequence = 0, lastSend = 0, queuedAt = 0, lastStatus = 0;

static bool onReceive(twai_node_handle_t handle, const twai_rx_done_event_data_t *, void *) {
  ReceivedFrame item = {};
  twai_frame_t frame = {};
  frame.buffer = item.data;
  frame.buffer_len = sizeof(item.data);
  BaseType_t wake = pdFALSE;
  if (twai_node_receive_from_isr(handle, &frame) == ESP_OK) {
    item.id = frame.header.id; item.dlc = frame.header.dlc;
    item.extended = frame.header.ide; item.remote = frame.header.rtr;
    xQueueSendFromISR(rxQueue, &item, &wake);
  }
  return wake == pdTRUE;
}

static bool onTransmit(twai_node_handle_t, const twai_tx_done_event_data_t *event, void *) {
  BaseType_t wake = pdFALSE;
  const bool success = event->is_tx_success;
  xQueueSendFromISR(txQueue, &success, &wake);
  return wake == pdTRUE;
}

static bool onError(twai_node_handle_t, const twai_error_event_data_t *event, void *) {
  BaseType_t wake = pdFALSE;
  const uint32_t flags = event->err_flags.val;
  xQueueSendFromISR(errorQueue, &flags, &wake);
  return wake == pdTRUE;
}

void requireOK(esp_err_t result, const char *step) {
  if (result == ESP_OK) return;
  Serial.printf("INIT FAIL: %s: %s\n", step, esp_err_to_name(result));
  while (true) delay(1000);
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.printf("CAN external preparation: node %c, TX5 RX4, 500 kbit/s. Hardware test pending.\n", NODE_B ? 'B' : 'A');
  Serial.println("Normal mode: another ACTIVE CAN node must ACK; listen-only analyzer cannot ACK.");
  rxQueue = xQueueCreate(32, sizeof(ReceivedFrame));
  txQueue = xQueueCreate(4, sizeof(bool));
  errorQueue = xQueueCreate(32, sizeof(uint32_t));
  if (!rxQueue || !txQueue || !errorQueue) requireOK(ESP_ERR_NO_MEM, "queues");
  twai_onchip_node_config_t config = {};
  config.io_cfg.tx = TX_PIN; config.io_cfg.rx = RX_PIN;
  config.io_cfg.quanta_clk_out = GPIO_NUM_NC;
  config.io_cfg.bus_off_indicator = GPIO_NUM_NC;
  config.bit_timing.bitrate = 500000;
  config.tx_queue_depth = 1;
  config.fail_retry_cnt = 0;  // Single-shot: prevent endless retries with a missing peer.
  // self_test, loopback and listen_only deliberately remain FALSE.
  requireOK(twai_new_node_onchip(&config, &node), "create node");
  twai_mask_filter_config_t filter = {};
  filter.id = 0; filter.mask = 0; filter.is_ext = false;
  requireOK(twai_node_config_mask_filter(node, 0, &filter), "standard-ID filter");
  twai_event_callbacks_t callbacks = {};
  callbacks.on_rx_done = onReceive;
  callbacks.on_tx_done = onTransmit;
  callbacks.on_error = onError;
  requireOK(twai_node_register_event_callbacks(node, &callbacks, nullptr), "callbacks");
  requireOK(twai_node_enable(node), "enable");
  txFrame.header.id = SEND_ID; txFrame.header.dlc = 8;
  txFrame.buffer = txData; txFrame.buffer_len = sizeof(txData);
}

void loop() {
  bool sent = false;
  while (xQueueReceive(txQueue, &sent, 0) == pdTRUE) {
    pending = false; timeoutReported = false;
    Serial.println(sent ? "TX_ACKED: link-level completion; verify peer application log separately."
                        : "TX_FAILED: inspect error flags, active peer, bitrate and wiring.");
  }
  uint32_t flags = 0;
  while (xQueueReceive(errorQueue, &flags, 0) == pdTRUE) {
    twai_error_flags_t error = {}; error.val = flags;
    Serial.printf("ERROR flags=0x%08lX ACK=%u BIT=%u FORM=%u STUFF=%u ARB_LOST=%u\n",
                  (unsigned long)flags, error.ack_err, error.bit_err, error.form_err, error.stuff_err, error.arb_lost);
    if (error.ack_err) Serial.println("NO ACK: check second active node; no success inferred.");
    if (error.arb_lost) Serial.println("Arbitration lost: normal contention can cause this; not itself a wiring fault.");
  }
  ReceivedFrame rx = {};
  while (xQueueReceive(rxQueue, &rx, 0) == pdTRUE) {
    Serial.printf("RX %s ID=0x%lX DLC=%u%s DATA=", rx.extended ? "EXT" : "STD", (unsigned long)rx.id, rx.dlc, rx.remote ? " RTR" : "");
    if (!rx.remote) for (uint8_t i = 0; i < rx.dlc && i < 8; ++i) Serial.printf("%02X ", rx.data[i]);
    Serial.println();
  }
  twai_node_status_t status = {};
  twai_node_record_t record = {};
  if (twai_node_get_info(node, &status, &record) != ESP_OK) {
    Serial.println("STATUS FAIL: transmission paused."); delay(100); return;
  }
  const uint32_t now = millis();
  if (now - lastStatus >= 1000) {
    lastStatus = now;
    Serial.printf("STATE=%d TEC=%u REC=%u BUS_ERRORS=%lu\n", (int)status.state,
                  status.tx_error_count, status.rx_error_count, (unsigned long)record.bus_err_num);
    if (status.state == TWAI_ERROR_BUS_OFF) Serial.println("BUS OFF: TX paused. Correct fault, then send r to request recovery.");
  }
  while (Serial.available()) {
    if (Serial.read() == 'r' && status.state == TWAI_ERROR_BUS_OFF && !recoveryRequested) {
      const esp_err_t err = twai_node_recover(node);
      Serial.printf("Recovery request: %s; wait for active state, not an instant pass.\n", esp_err_to_name(err));
      recoveryRequested = err == ESP_OK;
    }
  }
  if (status.state != TWAI_ERROR_BUS_OFF) recoveryRequested = false;
  if (pending && !timeoutReported && now - queuedAt >= 750) {
    timeoutReported = true;
    Serial.println("TX completion TIMEOUT: no new TX until callback; buffer retained. Inspect bus/peer.");
  }
  if (!pending && status.state != TWAI_ERROR_BUS_OFF && now - lastSend >= 1000) {
    lastSend = now;
    txData[0] = NODE_B ? 0xB2 : 0xA1;
    for (int i = 0; i < 4; ++i) txData[i + 1] = static_cast<uint8_t>(sequence >> (8 * i));
    txData[5] = 0xCA; txData[6] = 0x55; txData[7] = 0xAA;
    const esp_err_t err = twai_node_transmit(node, &txFrame, 100);
    Serial.printf("TX queue ID=0x%03lX DLC=8 result=%s (queue acceptance is not delivery)\n", (unsigned long)SEND_ID, esp_err_to_name(err));
    if (err == ESP_OK) { pending = true; queuedAt = now; ++sequence; }
    else if (err == ESP_ERR_TIMEOUT) Serial.println("TX queue TIMEOUT: queue space unavailable.");
  }
  delay(1);
}
