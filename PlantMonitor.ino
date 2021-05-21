
/*
  Plant Environment Monitor

  This code creates a BLE peripheral that outputs the temperature and humidity
  localised around the Arduino Nano 33 BLE Sense.

  Code is modified from the ArduinoBLE BatteryMonitor example.
*/

#include <ArduinoBLE.h>
#include <Arduino_HTS221.h>

// UUID number source: https://www.bluetooth.com/specifications/assigned-numbers/
// List of characteristics available for environmental sensing: https://www.bluetooth.com/specifications/assigned-numbers/environmental-sensing-service-characteristics/
#define SERVICE_UUID_ESS "181A"             // Environmental Sensing Service UUID       
#define CHAR_UUID_TEMPERATURE "2A6E"        // Temperature characteristic UUID
#define CHAR_UUID_HUMIDITY "2A6F"           // Humidity characteristic UUID

// BLE Environmental Service
BLEService envService(SERVICE_UUID_ESS);

// Characteristic data types taken from GATT Specification Supplement
// Source: https://www.bluetooth.com/specifications/specs/
// BLE Temperature Characteristic
BLEIntCharacteristic tempChar(CHAR_UUID_TEMPERATURE, BLERead | BLENotify);
//BLE Humidity Characteristic
BLEUnsignedIntCharacteristic humChar(CHAR_UUID_HUMIDITY, BLERead | BLENotify);

int16_t oldTemp = 0; //last temerature reading
uint16_t oldHum = 0; //last humidity reading
long previousMillis = 0; //last time sensors were checked, in ms

void setup() {
  Serial.begin(9600);    // initialize serial communication
//  while (!Serial);

  pinMode(LED_BUILTIN, OUTPUT); // initialize the built-in LED pin to indicate when a central is connected

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }

  //Sensor Setup
  if (!HTS.begin()) {
    Serial.println("Failed to initialize humidity temperature sensor!");
    while (1);
  }

  BLE.setLocalName("PlantMonitor");
  BLE.setAdvertisedService(envService); // Add service UUID
  envService.addCharacteristic(tempChar); // Add temperature characteristic
  envService.addCharacteristic(humChar); // Add humidity characteristic
  BLE.addService(envService); // Add the environmental service

  // Set initial values for Characteristics
  tempChar.writeValue(oldTemp);
  humChar.writeValue(oldHum);

  BLE.advertise();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  // Wait for a BLE central
  BLEDevice central = BLE.central();

  // if a central is connected to the peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address()); // print central BT address
    digitalWrite(LED_BUILTIN, HIGH); // turn on LED to indicate connection

    //check temp and humidity every 5 seconds
    while (central.connected()) {
      long currentMillis = millis();
      if (currentMillis - previousMillis >= 5000) {
        previousMillis = currentMillis;
        updateSensors();
      }
    }//end while

    // when central disconnecteds, turn off LED
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }//endif
}

void updateSensors() {
  float temp = HTS.readTemperature();
  float hum = HTS.readHumidity();

  // Multiply by 100 to get 0.01 precision float data stored as int
  int envTemp = temp * 100;
  unsigned int envHum = round(hum) * 100;

  if (envTemp != oldTemp) { // if temp has changed
    tempChar.writeValue(envTemp); // update temp characteristic
    oldTemp = envTemp; //save temp for next comparison
    Serial.print(envTemp/100.0);
    Serial.println(" °C");
  }

  if (envHum != oldHum) {
    humChar.writeValue(envHum);
    oldHum = envHum;
    Serial.print(envHum/100.0);
    Serial.println(" %");
  }

}
