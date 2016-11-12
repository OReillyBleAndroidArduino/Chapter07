#include "CurieBle.h"

static const char* bluetoothDeviceName = "MyDevice"; 

static const char* serviceUuid = "180C";
static const char* characteristicUuid = "2A56";
static const int   characteristicTransmissionLength = 20; 

char randomString[20] = {0};

BLEService service(serviceUuid); 
BLECharacteristic characteristic(
  characteristicUuid,
  BLERead, // readable from client's perspective
  characteristicTransmissionLength
);

BLEPeripheral blePeripheral; 

unsigned long lastBleCharacteristicUpdateTime_ms = 0;
unsigned long updateTimeout_ms = 5000; // update every 5 seconds


void generateRandomString(const int stringLength, char* outputString) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  int alphanumLength = 36;

  outputString[stringLength];
  for (int i = 0; i < stringLength; i++) {
    outputString[i] = alphanum[random(0, alphanumLength)];
  }

  outputString[stringLength] = '\0';
}

void setBleCharacteristicValue(char* output, int length) {
  characteristic.setValue((const unsigned char*) output, length); 
}


void setup() { 
  blePeripheral.setLocalName(bluetoothDeviceName); 

  blePeripheral.setAdvertisedServiceUuid(service.uuid()); 
  blePeripheral.addAttribute(service);
  blePeripheral.addAttribute(characteristic);

  blePeripheral.begin(); 
  
  randomSeed(analogRead(0)); // initialize random numbers
  
  lastBleCharacteristicUpdateTime_ms = millis();
}

void loop() {
  unsigned long currentTime_ms = millis();
  
  if ((currentTime_ms - lastBleCharacteristicUpdateTime_ms) > updateTimeout_ms) {
    lastBleCharacteristicUpdateTime_ms = currentTime_ms;
    
    int randomStringLength = random(1, characteristicTransmissionLength);
    generateRandomString(randomStringLength, randomString);
    
    setBleCharacteristicValue(randomString, randomStringLength);
  }
  
}

