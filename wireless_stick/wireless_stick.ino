#include "heltec.h" 
#include "lora_details.h"

String rssi = "RSSI --";
String packSize = "--";
String packet ;

void setup() { 
   //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, RF_FREQUENCY /*long BAND*/);
  //
  LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
    // put in standby mode
  LoRa.setSignalBandwidth(LORA_BANDWIDTH_HZ);
  LoRa.setCodingRate4(LORA_CODINGRATE);
  LoRa.setSyncWord(0x12);   //0x34
  LoRa.setPreambleLength(8);
  
  Heltec.display->init();
  Heltec.display->flipScreenVertically();  
  Heltec.display->setFont(ArialMT_Plain_10);

  delay(1500);
  Heltec.display->clear();
  
  Heltec.display->drawString(0, 0, "LoRa Initial success!");
  Heltec.display->drawString(0, 10, "Wait for RX data...");
  Heltec.display->display();
  delay(1000);
  //LoRa.onReceive(cbk);
  LoRa.receive();
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) { cbk(packetSize);  }
  delay(10);
}
