#include "FastLED.h"

#define RAPPORTO_BIANCO_COLORE 100 //0 significa solo BIANCO - 255 significa SOLO COLORE

#define NUM_STRIPS 3
#define NUM_LEDS_PER_STRIP 213
#define NUM_LEDS NUM_LEDS_PER_STRIP * NUM_STRIPS

#define NUM_EXHIBIT 12

CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

uint16_t colorIndex[NUM_LEDS];
uint16_t lavaIndex[NUM_LEDS];

DEFINE_GRADIENT_PALETTE(heatmap_gp) {
    0,  255,  0,    0,  //orange
  125,    0,    255,    0,  //black
  255,    0,    0,    255,  //red
};

CRGBPalette16 myPal = heatmap_gp;

int brightWin[] = {0,1,3,5,8,12,20,35,50,95,150,200,255}; //12


void setup() {
  // put your setup code here, to run once:
  Serial3.begin(9600);
  Serial.begin(9600);
  Serial3.flush();
  
  // tell FastLED there's 60 NEOPIXEL leds on pin 10, starting at index 0 in the led array
  FastLED.addLeds<WS2812B, 7,GRB>(leds, 0, NUM_LEDS_PER_STRIP);
  // tell FastLED there's 60 NEOPIXEL leds on pin 11, starting at index 60 in the led array
  FastLED.addLeds<WS2812B, 11,GRB>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  // tell FastLED there's 60 NEOPIXEL leds on pin 12, starting at index 120 in the led array
  FastLED.addLeds<WS2812B, 12,GRB>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);

  for (int i=0; i<NUM_EXHIBIT; i++) {
    colorIndex[i] = random8();
  }
  for (int i=0; i<NUM_LEDS; i++) {
    lavaIndex[i] = random8();
  }
  FastLED.setBrightness(255);
  FastLED.setTemperature(Tungsten40W);
  FastLED.clear();
  FastLED.show();

}

String inSer;
char inBuff[12];

bool event=false;

uint16_t potM = 0;
uint16_t pos = 0;

bool a_up = false;
bool b_up = false;
bool c_up = false;
bool d_up = false;

uint8_t brightVal = 0;

uint16_t x = 0;
uint16_t beamIndex = 0;

bool play = false;

unsigned long gameTime = 0;
unsigned long gameSpeed = 2000;
int jj = 11;

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial3.available()) {
    inSer = Serial3.readStringUntil('\n');
    if (inSer.length() == 14) {
      inSer.toCharArray(inBuff, 14);
      if (inBuff[12] == 'q') {
        potM = inBuff[1] - '0';
        if (potM > 9) potM = potM - 7;
        pos = inBuff[3] - '0';
        if (pos > 9) pos = pos - 7;
        a_up = inBuff[5] - '0';
        b_up = inBuff[7] - '0';
        c_up = inBuff[9] - '0';
        d_up = inBuff[11] - '0';
        event = true;
        Serial.print(a_up);
        Serial.print(b_up);
        Serial.print(c_up);
        Serial.println(d_up);
      }
    }    
  }

  if (event) {
    FastLED.clear();
    
    if (a_up) {
      //MAN
      if (b_up) {
        //SPOT
        FastLED.setBrightness(255);
        for (uint16_t j=0; j<NUM_LEDS; j++) {
          if ((j >= NUM_LEDS/NUM_EXHIBIT*pos) && (j < NUM_LEDS/NUM_EXHIBIT*(pos+1))) {
            leds[j] = CHSV(colorIndex[pos],RAPPORTO_BIANCO_COLORE,255);
          }
          else {
            leds[j] = CHSV(0,0,brightVal);
          }
        }  
        FastLED.show();
        Serial3.flush();
        
      }
      else {
        
        //WHITE
        brightVal = map(potM,0,11,255,0);
        fill_solid(leds,NUM_LEDS,CRGB::White);
        FastLED.setBrightness(brightVal);  
        FastLED.show();
        Serial3.flush();

      }
      play = false;
    }
    else {
      play = true; 
    }
    event = false;
  }

  if (play) {
    // AUTO
    FastLED.clear();

    brightVal = 125;
    gameSpeed = 60000;

    
    for (uint16_t j=0;j<NUM_LEDS;j++) {
      leds[j] = CHSV(0,0,brightVal);
    }

    for (int j=0; j<NUM_LEDS; j++) {
      if ((j >= NUM_LEDS/NUM_EXHIBIT*jj) && (j < NUM_LEDS/NUM_EXHIBIT*(jj+1))) {
        leds[j] = CHSV(colorIndex[jj],RAPPORTO_BIANCO_COLORE,255);
      }
      else {
        leds[j] = CHSV(0,0,brightVal);
      }
    }

    if ((millis() - gameTime) > gameSpeed) {
      gameTime = millis();
      jj--;
      if (jj < 0) {
        jj = 11;
      }
    }
    FastLED.show();
  }

  
  
  
}
