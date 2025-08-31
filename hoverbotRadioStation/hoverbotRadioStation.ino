#include <RadioLib.h>
#include <SPI.h>


#define SS 15
#define RST 16
#define DIO0 5
SX1278 radio = new Module(SS, DIO0, RST, RADIOLIB_NC);
struct recieverData{
  int8_t protocol = 0;
  bool isConnected = false;
  uint8_t Rssi = 0;
  uint8_t status = 0;

}__attribute__((packed));
recieverData RecieverData;
void setup() {
  Serial.begin(115200);
  radio.begin();
  Serial.write((uint8_t*)&RecieverData, sizeof(RecieverData));
}

void loop() {
  Serial.write((uint8_t*)&RecieverData, sizeof(RecieverData));
}
