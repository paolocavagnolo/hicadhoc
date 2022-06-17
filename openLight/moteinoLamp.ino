#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>//get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>      //comes with Arduino IDE (www.arduino.cc)
#include <SPIFlash.h> //get it here: https://www.github.com/lowpowerlab/spiflash

#define GATEWAYID     1
#define NETWORKID     39  //the same on all nodes that talk to each other
//Match frequency to the hardware version of the radio on your Moteino (uncomment one):
#define FREQUENCY     RF69_868MHZ
#define ENCRYPTKEY    "ninoLeporeLampad" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW_HCW  //uncomment only for RFM69HW/HCW! Leave out if you have RFM69W/CW!

#define LED           9 // Moteinos have LEDs on D9

RFM69 radio;

uint32_t packetCount = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED,OUTPUT);
  radio.initialize(FREQUENCY,GATEWAYID,NETWORKID);
  radio.setHighPower();
  radio.encrypt(ENCRYPTKEY);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (radio.receiveDone())
  {
    digitalWrite(LED,HIGH);
    for (byte i = 0; i < radio.DATALEN; i++) {
      Serial.print(" ");
      Serial.print(radio.DATA[i],HEX);
    }
    Serial.println();
    //Check
    if (radio.ACKRequested()) radio.sendACK();
  }
  digitalWrite(LED,LOW);

}
