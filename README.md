# Plant-Monitor

Example code for setting up the [Arduino Nano 33 BLE Sense](https://store.arduino.cc/arduino-nano-33-ble-sense) as a Environment Sensing peripheral.
Given use case is for measuring temperature and humidity in a room for plants.

Uses built-in [HTS221 capacitive digital sensor](https://content.arduino.cc/assets/Nano_BLE_Sense_HTS221.pdf):
- Humidity accuracy: ± 3.5% rH, 20 to +80% rH
- Temperature accuracy: ± 0.5 °C,15 to +40 °C

Bluetooth Low Energy (BLE) Sources:
* [UUID numbers](https://www.bluetooth.com/specifications/assigned-numbers/)
* [Environmental Sensing Service characteristics](https://www.bluetooth.com/specifications/assigned-numbers/environmental-sensing-service-characteristics/)
* [GATT specification](https://www.bluetooth.com/specifications/specs/) (GATT Specification Supplement)
