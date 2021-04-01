void LoRaData(){
  Heltec.display->clear();
  Heltec.display->drawString(0 , 0 , "S "+ packSize + "B; " + rssi);
  Heltec.display->drawStringMaxWidth(0 , 15 , 128, packet);
  //Heltec.display->drawString(0, 0, rssi);  
  Heltec.display->display();
}

void cbk(int packetSize) {
  Serial.println("Packet received" + String(packSize));
  packet = "[" + String(messages_received) + "]";
  packSize = String(packetSize,DEC);
  for (int i = 0; i < packetSize; i++){ 
      packet += (char) LoRa.read(); 
  }
  rssi = "R " + String(LoRa.packetRssi(), DEC) ;

  Serial.println(packet);

  // Print data  
  LoRaData();
}
