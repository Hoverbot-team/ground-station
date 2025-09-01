#include <RadioLib.h>
#include <SPI.h>

#define SS 15
#define RST 16
#define DIO0 5

#define FSK 0
#define SF7 1
#define SF8 2
#define SF9 3
#define SF10 4
#define SF11 5
#define SF12 6


SX1278 radio = new Module(SS, DIO0, RST, RADIOLIB_NC);
//uart structs
struct recieverData {
  int8_t protocol = 0;
  bool isConnected = false;
  float Rssi = 0;
  uint8_t status = 0;

} __attribute__((packed));
//robot structures
struct handshake {
  int8_t recID;
  int8_t protocolSet = 0;
} __attribute__((packed));

struct rPing {
  int8_t recID;
  int8_t pingID;
};

//struct declarations
recieverData RecieverData;
rPing ping;
rPing pong;
//enums
//vars
bool PingSent = false;
//functions
void setup() {
  Serial.begin(115200);
  int state = radio.begin(433.0, 125.0, 12, 5, 8, 0x12, 17);
  if (state == RADIOLIB_ERR_NONE) {
    RecieverData.status = -1;
    while (true) {
      Serial.write((uint8_t*)&RecieverData, sizeof(RecieverData));
    }
  }
  RecieverData.protocol = SF12;
  handshake hOUT;
  handshake hIN;
  hOUT.recID = random(255);
  ping.recID = hOUT.recID;
  while (true) {
    radio.transmit((uint8_t*)&hOUT, sizeof(hOUT));
    Serial.write((uint8_t*)&RecieverData, sizeof(RecieverData));
    radio.receive((uint8_t*)&hIN, sizeof(hIN));
    if (memcmp(&hOUT, &hIN, sizeof(handshake)) == 0) {
      RecieverData.isConnected = true;
      break;
    }
  }
}

void loop() {
  RecieverData.Rssi = radio.getRSSI();
  Serial.write((uint8_t*)&RecieverData, sizeof(RecieverData));
  if (!PingSent) {
    radio.transmit((uint8_t*)&ping, sizeof(ping));
    PingSent = true;
  } else {
    int state1 = radio.readData((uint8_t*)&pong, sizeof(pong));
    if (state1 == RADIOLIB_ERR_NONE) {
      RecieverData.isConnected = true;
    } else if (state1 == RADIOLIB_ERR_RX_TIMEOUT) {
      RecieverData.isConnected = false;
      if (memcmp(&ping, &pong, sizeof(rPing)) == 0) {
        RecieverData.isConnected = true;
      }
      if (RecieverData.isConnected = true) {
        PingSent = false;
      }
    }
  }
}
void changeProtocol(int protocol) {
  if (protocol == 0) {
    radio.beginFSK(433.0);
  } else if (protocol >= 1 && protocol <= 12) {
    radio.setSpreadingFactor(protocol);

  } else {
  }
}
