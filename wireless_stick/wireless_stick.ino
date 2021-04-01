#include "heltec.h" 
#include "lora_details.h"

typedef enum
{
    LOWPOWER,
    RECEIVE,
    TRANSMIT
}States_t;
States_t state;

String rssi = "RSSI --";
String packSize = "--";
int packetSize, messages_received;
String packet ;

void setup() { 
  // Setup Heltec
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Enable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, RF_FREQUENCY /*long BAND*/);
  Serial.println("Heltec initiated");

  // Setup LORA
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
  LoRa.setSignalBandwidth(LORA_BANDWIDTH_HZ);
  LoRa.setCodingRate4(LORA_CODINGRATE);
  LoRa.setSyncWord(0x12);   //0x34
  LoRa.setPreambleLength(LORA_PREAMBLE_LENGTH);
  Serial.println("LORA setup");

  // Setup display
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);
  delay(1500);
  
  Heltec.display->clear();
  Heltec.display->drawString(0, 0, "LoRa Initiated");
  Heltec.display->drawString(0, 10, "Wait for RX...");
  Heltec.display->display();
  delay(1000);
  Serial.println("Screen setup");

  // Setup LORA receiving callback function
  //LoRa.onReceive(cbk);
  state = RECEIVE;
  messages_received = 0;
}

void loop() {
  switch (state){
    case RECEIVE:
      LoRa.receive();
      while (state == RECEIVE){
        packetSize = LoRa.parsePacket();
  
        if (packetSize){ 
          messages_received ++;
          cbk(packetSize);
        }

        delay(10);
      }
      break;
    default:
      break; 
  }
  delay(10);
}
