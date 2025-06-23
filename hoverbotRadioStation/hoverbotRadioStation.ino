#include <SPI.h>
#include <LoRa.h>

#define SS 15
#define RST 16
#define DIO0 5

void setup() {
  Serial.begin(115200);
  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) {
    while (1); // halt if LoRa init fails
  }
  LoRa.setSpreadingFactor(7);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setCodingRate4(5);
}

void loop() {
  // Read from LoRa and send to Serial
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    while (LoRa.available()) {
      Serial.write(LoRa.read());  // byte-by-byte forwarding
    }
  }

  // Read from Serial and send to LoRa
  if (Serial.available()) {
    LoRa.beginPacket();
    while (Serial.available()) {
      LoRa.write(Serial.read());  // byte-by-byte forwarding
    }
    LoRa.endPacket();
  }
}
