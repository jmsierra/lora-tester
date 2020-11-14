
void testRGB(void){
  display.drawString(0, 20, "RGB Testing");
  display.display();
  for(uint32_t i=0;i<=30;i++){
    turnOnRGB(i<<16,10);
  }
  for(uint32_t i=0;i<=30;i++){
    turnOnRGB(i<<8,10);
  }
  for(uint32_t i=0;i<=30;i++){
    turnOnRGB(i,10);
  }
  turnOnRGB(0,0);
}

void OnTxDone( void )
{
  Serial.print("TX done......");
  displayInof();
  turnOnRGB(0,0);
  state=RX;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.print("TX Timeout......");
    //state=TX;
}
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
    Rssi=rssi;
    rxSize=size;
    memcpy(rxpacket, payload, size );
    rxpacket[size]='\0';
    turnOnRGB(0x001000,100);
    turnOnRGB(0,0);
    Radio.Sleep( );

    Serial.printf("\r\nreceived packet \"%s\" with rssi %d , length %d\r\n",rxpacket,Rssi,rxSize);
    Serial.println("wait to send next packet");
    displayInof();

    state=TX;
}

void displayInof()
{
    display.clear();
    display.drawString(0, 0,  "Received Size" + String(rxSize,DEC) + " packages:");
    display.drawString(0, 15, rxpacket);
    display.drawString(0, 30, "With rssi " + String(Rssi,DEC));
    display.drawString(0, 50, "Packet " + String(txNumber,DEC) + " sent done");
    display.display();
}


void userKey(void)
{
  delay(10);
  if(digitalRead(P3_3)==0)
  {
    sleepMode = true;
  }
}
