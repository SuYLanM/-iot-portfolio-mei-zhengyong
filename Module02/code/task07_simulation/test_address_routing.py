"""SIMULATION ONLY: address-routing/protocol model; never accesses COM or I2C."""
import unittest

ADDRESS_LCD = 0x27
ADDRESS_TEST = 0x42

def packet(seq, uptime, requests):
    p = bytearray(b"T7") + bytes([1, seq]) + uptime.to_bytes(4, "little")
    p += requests.to_bytes(2, "little") + bytes([0])
    check = 0
    for v in p:
        check ^= v
    return bytes(p + bytes([check]))

def valid(data, seq):
    if len(data) != 12 or data[:3] != b"T7\x01" or data[3] != seq or data[10] != 0:
        return False
    check = 0
    for v in data[:11]:
        check ^= v
    return check == data[11]

class FakeBus:
    """Shared logical SDA/SCL model, not voltage/timing/arbitration simulation."""
    signals = ("SDA", "SCL")
    def __init__(self):
        self.targets = {}
    def attach(self, address, target):
        if not 0x08 <= address <= 0x77 or address in self.targets:
            raise ValueError("reserved/duplicate address configuration")
        self.targets[address] = target
    def scan(self):
        return sorted(self.targets)
    def read(self, address, command):
        if address not in self.targets:
            raise LookupError("SIMULATED address NACK")
        return self.targets[address](command)

class RoutingSimulation(unittest.TestCase):
    def setUp(self):
        self.calls = []
        self.bus = FakeBus()
        def lcd(command):
            self.calls.append(("LCD", command))
            return b"SIMULATED_DDRAM"
        def test(command):
            self.calls.append(("MCU_TEST", command))
            if len(command) != 2 or command[0] != 0xA5:
                raise ValueError("bad command")
            return packet(command[1], 123456, 7)  # explicit synthetic fixtures
        self.bus.attach(ADDRESS_LCD, lcd)
        self.bus.attach(ADDRESS_TEST, test)
    def test_two_addresses_one_logical_pair(self):
        self.assertEqual(self.bus.scan(), [0x27, 0x42])
        self.assertEqual(self.bus.signals, ("SDA", "SCL"))
    def test_routing_does_not_call_other_target(self):
        self.assertEqual(self.bus.read(0x27, b"r"), b"SIMULATED_DDRAM")
        self.assertEqual(self.calls, [("LCD", b"r")])
    def test_protocol_packet_and_little_endian_fixture(self):
        data = self.bus.read(0x42, bytes([0xA5, 9]))
        self.assertTrue(valid(data, 9))
        self.assertEqual(int.from_bytes(data[4:8], "little"), 123456)
        self.assertEqual(self.calls, [("MCU_TEST", bytes([0xA5, 9]))])
    def test_stale_sequence_is_rejected(self):
        self.assertFalse(valid(packet(8, 123456, 7), 9))
    def test_short_response_is_rejected(self):
        self.assertFalse(valid(packet(9, 123456, 7)[:-1], 9))
    def test_corruption_is_rejected(self):
        data = bytearray(packet(9, 123456, 7)); data[4] ^= 1
        self.assertFalse(valid(data, 9))
    def test_missing_device_and_isolation(self):
        del self.bus.targets[0x42]
        self.assertEqual(self.bus.scan(), [0x27])
        with self.assertRaises(LookupError):
            self.bus.read(0x42, bytes([0xA5, 9]))
        self.assertEqual(self.bus.read(0x27, b"r"), b"SIMULATED_DDRAM")
    def test_duplicate_configuration_is_rejected(self):
        with self.assertRaises(ValueError):
            self.bus.attach(0x27, lambda _: b"collision")
        # This checks configuration policy, not real open-drain collision behavior.

if __name__ == "__main__":
    print("SIMULATION ONLY - synthetic 0x27/0x42 responses; no physical I2C, COM port or sensor readings", flush=True)
    unittest.main(verbosity=2)
