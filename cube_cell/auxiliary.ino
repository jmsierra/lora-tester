/*
 * Test RGB LED
 */
void testRGB(void){
  Serial.println("Testing RGB...");
  
  // Display Testing
  display.clear();
  display.drawString(0, 20, "RGB Testing");
  display.display();
  
  for(uint32_t i=0;i<=30;i++){
    // Red
    turnOnRGB(i<<16,15);
  }
  for(uint32_t i=0;i<=30;i++){
    // Green
    turnOnRGB(i<<8,15);
  }
  for(uint32_t i=0;i<=30;i++){
    // Blue
    turnOnRGB(i,15);
  }

  // Sleep 0.5s
  delay(500);

  // Clear display
  display.clear();
  display.display();

  // Switch RGB off
  turnOnRGB(0,0);
}

void OnTxDone( void )
{
  turnOnRGB(0x100000,100);
  Serial.println("TX done......");
  displayInof();
  turnOnRGB(0,0);
  state = TX;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    turnOnRGB(0x100000,100);
    Serial.println("TX Timeout......");
    turnOnRGB(0,0);
    state = TX;
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
    state = TX;
  }
}
