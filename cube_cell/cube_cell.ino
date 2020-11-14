
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include "cubecell_SH1107Wire.h"
#include "lora_details.h"

/*
 * set LoraWan_RGB to 1,the RGB active
 * RGB red means sending;
 * RGB green means received done;
 */
#ifndef LoraWan_RGB
#define LoraWan_RGB 0
#endif

extern SH1107Wire  display; 

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 32 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

static RadioEvents_t RadioEvents;
typedef enum
{
    LOWPOWER,
    RX,
    TX
}States_t;

int16_t txNumber;
States_t state;
bool sleepMode = false;
int16_t Rssi,rxSize;



void setup() {
    boardInitMcu( );
    Serial.begin(115200);
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW); 
    delay(100);
    display.init();
    //display.flipScreenVertically();
    
    testRGB();
    txNumber=0;
    Rssi=0;

    pinMode(P3_3,INPUT);
    attachInterrupt(P3_3,userKey,FALLING);
    RadioEvents.TxDone = OnTxDone;
    RadioEvents.TxTimeout = OnTxTimeout;
    RadioEvents.RxDone = OnRxDone;

    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                   LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                   LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );

    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                                   LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                                   LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                                   0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

    state=TX;
}



void loop()
{
  switch(state)
  {
    case TX:
      delay(1000);
      txNumber++;
      sprintf(txpacket,"%s","hello");
      sprintf(txpacket+strlen(txpacket),"%d",txNumber);
      sprintf(txpacket+strlen(txpacket),"%s"," rssi : ");
      sprintf(txpacket+strlen(txpacket),"%d",Rssi);
      turnOnRGB(0x100000,0);

      Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));

      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) );
      state=LOWPOWER;
      break;
    case RX:
      Serial.println("into RX mode");
      Radio.Rx( 0 );
      state=LOWPOWER;
      break;
    case LOWPOWER:
      if(sleepMode)
      {
        Radio.Sleep( );
        display.stop();
        detachInterrupt(RADIO_DIO_1);
        turnOffRGB();
        pinMode(Vext,ANALOG);
        pinMode(ADC,ANALOG);
      }
      lowPowerHandler();
      break;
    default:
      break;
  }
    Radio.IrqProcess( );
}
