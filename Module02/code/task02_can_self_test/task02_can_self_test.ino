#include <Arduino.h>
#include <cstring>
#include "esp_twai.h"
#include "esp_twai_onchip.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

// Classic ESP32 / Arduino Core 3.3.12 (ESP-IDF 5.5.5).
// GPIO matrix pad feedback: TX and RX use the SAME unconnected GPIO5.
// No jumper, transceiver or CAN bus. Disconnect everything from GPIO5.
constexpr gpio_num_t LOCAL_FEEDBACK_PIN = GPIO_NUM_5;
struct ReceivedFrame {
  uint32_t id;
  uint8_t dlc;
  bool extended;
  bool remote;
  uint8_t data[8];
};
twai_node_handle_t node = nullptr;
QueueHandle_t rxQueue = nullptr, txQueue = nullptr;
uint8_t txData[8] = {};
twai_frame_t txFrame = {};
uint32_t sequence = 0, lastSend = 0;

static bool onReceive(twai_node_handle_t handle, const twai_rx_done_event_data_t *, void *) {
  ReceivedFrame item = {};
  twai_frame_t frame = {};
  frame.buffer = item.data;
  frame.buffer_len = sizeof(item.data);
  BaseType_t wake = pdFALSE;
  if (twai_node_receive_from_isr(handle, &frame) == ESP_OK) {
    item.id = frame.header.id;
    item.dlc = frame.header.dlc;
    item.extended = frame.header.ide;
    item.remote = frame.header.rtr;
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

void requireOK(esp_err_t result, const char *step) {
  if (result == ESP_OK) return;
  Serial.printf("FAIL: %s: %s\n", step, esp_err_to_name(result));
  while (true) delay(1000);
}

void setup() {
  Serial.begin(115200);
  delay(300);
  Serial.println("CAN local self-test PREPARED; not a recorded hardware result.");
  Serial.println("Leave GPIO5 disconnected. GPIO-matrix feedback; no physical CAN validation.");
  rxQueue = xQueueCreate(4, sizeof(ReceivedFrame));
  txQueue = xQueueCreate(4, sizeof(bool));
  if (!rxQueue || !txQueue) requireOK(ESP_ERR_NO_MEM, "queues");
  twai_onchip_node_config_t config = {};
  config.io_cfg.tx = LOCAL_FEEDBACK_PIN;
  config.io_cfg.rx = LOCAL_FEEDBACK_PIN;
  config.io_cfg.quanta_clk_out = GPIO_NUM_NC;
  config.io_cfg.bus_off_indicator = GPIO_NUM_NC;
  config.bit_timing.bitrate = 500000;
  config.tx_queue_depth = 1;
  config.fail_retry_cnt = 0;
  config.flags.enable_self_test = true;  // No ACK required, local test ONLY.
  config.flags.enable_loopback = true;   // Receive our own transmitted frame.
  requireOK(twai_new_node_onchip(&config, &node), "create node");
  twai_mask_filter_config_t filter = {};
  filter.id = 0;
  filter.mask = 0;  // Accept all standard IDs.
  filter.is_ext = false;
  requireOK(twai_node_config_mask_filter(node, 0, &filter), "filter");
  twai_event_callbacks_t callbacks = {};
  callbacks.on_rx_done = onReceive;
  callbacks.on_tx_done = onTransmit;
  requireOK(twai_node_register_event_callbacks(node, &callbacks, nullptr), "callbacks");
  requireOK(twai_node_enable(node), "enable");
  txFrame.header.id = 0x123;
  txFrame.header.dlc = 8;
  txFrame.buffer = txData;
  txFrame.buffer_len = sizeof(txData);
  lastSend = millis() - 1000;
}

void loop() {
  if (static_cast<uint32_t>(millis() - lastSend) < 1000) { delay(1); return; }
  lastSend = millis();
  for (int i = 0; i < 4; ++i) txData[i] = static_cast<uint8_t>(sequence >> (8 * i));
  txData[4] = 0xCA; txData[5] = 0xFE; txData[6] = 0x55; txData[7] = 0xAA;
  Serial.printf("TX ID=0x%03lX DLC=%u DATA=", (unsigned long)txFrame.header.id, txFrame.header.dlc);
  for (uint8_t b : txData) Serial.printf("%02X ", b);
  Serial.println();
  requireOK(twai_node_transmit(node, &txFrame, 100), "queue TX");
  bool sent = false;
  // Do not reuse persistent TX buffer if the driver has not released it.
  if (xQueueReceive(txQueue, &sent, pdMS_TO_TICKS(250)) != pdTRUE) {
    requireOK(ESP_ERR_TIMEOUT, "TX completion; halted to preserve buffer lifetime");
  }
  ReceivedFrame received = {};
  const bool got = xQueueReceive(rxQueue, &received, pdMS_TO_TICKS(250)) == pdTRUE;
  bool match = got && !received.extended && !received.remote && received.id == 0x123 && received.dlc == 8;
  if (match) match = std::memcmp(received.data, txData, 8) == 0;
  if (got) {
    Serial.printf("RX ID=0x%03lX DLC=%u DATA=", (unsigned long)received.id, received.dlc);
    for (uint8_t i = 0; i < received.dlc && i < 8; ++i) Serial.printf("%02X ", received.data[i]);
    Serial.println();
  } else Serial.println("RX timeout");
  Serial.println(sent && match ? "PASS: local TX/RX match ONLY; physical CAN unverified."
                              : "FAIL: local TX/RX completion or payload mismatch.");
  ++sequence;
}
