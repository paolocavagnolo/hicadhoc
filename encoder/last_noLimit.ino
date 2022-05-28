/*
 * 0: ruota ferma
 * 1: ruota in movimento
 * 2: spin troppo lento
 * 3: spin ok
 * [char]: lettera corrispondente a fermata ruota
 */

#define btnIT 8
#define btnEN 9
#define ledIT 10
#define ledEN 11

#define encA 2
#define encB 3
#define enc0 7

int i = 0;
bool clkws = false;

int oldi = 0;
int I = 0;
int oldI = 0;
int oldoldI = 0;

#include "Keyboard.h"

void setup() {
  Keyboard.begin();
  
  pinMode(btnIT, INPUT_PULLUP);
  pinMode(btnEN, INPUT_PULLUP);
  pinMode(ledIT, OUTPUT);
  pinMode(ledEN, OUTPUT);
  
  pinMode(encA, INPUT);
  pinMode(encB, INPUT);
  pinMode(enc0, INPUT);
  digitalWrite(encA, LOW);
  digitalWrite(encB, LOW);
  digitalWrite(enc0, LOW);
  
  attachInterrupt(digitalPinToInterrupt(encB), chkB, RISING);
  attachInterrupt(digitalPinToInterrupt(enc0), chk0, RISING);
  
  while(!digitalRead(7));
  i = 0;
  
  //Serial.begin(9600);
  
}

long ledT = 0;
bool langEN = false;
bool btnITen = true;
bool btnENen = true;
long btnITtimer = 0;
long btnENtimer = 0;
int theV = 0;
float vel = 0;
long timeF = 0;

long wheelchk = 0;
bool isspinning = 0;
bool Ven = true;
bool timeFen = true;

long delta=0;

uint8_t w = 0;

bool ok = false;
uint8_t cc = 0;

int zero = -15;

float m = 0.038;
float q = 0.1;

void loop() {

  //LANGUAGE LED
  if (langEN) {
    digitalWrite(ledIT, LOW);
    digitalWrite(ledEN, HIGH);  
  }
  else {
    digitalWrite(ledEN, LOW);
    digitalWrite(ledIT, HIGH);
  }

  //LANGUAGE BUTTON
  if (!digitalRead(btnIT) && btnITen) {
    ////Serial.println("ITALIANO");
    langEN = false;
    btnITen = false;
    btnITtimer = millis();
  }
  if (!digitalRead(btnEN) && btnENen) {
    ////Serial.println("INGLESE");
    langEN = true;
    btnENen = true;
    btnENtimer = millis();
  }
  if ((millis() - btnITtimer) > 500) {
    btnITen = true;
  }
  if ((millis() - btnENtimer) > 500) {
    btnENen = true;
  }


  //WHEEL STATE
  if ((millis() - wheelchk) > 300) {
    
    if (i != oldi) {
      
      isspinning = true;
      timeFen = true;
      float f = (I-oldI)-(oldI-oldoldI);
      
      if (f < 0) {
        if (Ven){
          theV = (I-oldI);
          Ven = false;
          vel = float(theV)/0.3/512*60;
          timeF = millis();

          //Serial.print("Vel MAX: ");
          //Serial.print(vel);
          //Serial.println(" rpm");
          //Serial.print("\n\nGiro");
        }

      }
      else {
        Ven = true;
      }
      
      
    }
    else {

      isspinning  = false;
      I = 0;
      
      Ven = true;
      
      if (timeFen) {
        
        float mmin = ((vel*m)+q)*0.85;
        float mmax = ((vel*m)+q)*1.15;

        mmin = 0;
        mmax = 99;
        
        delta = millis() - timeF;
        timeFen = false;
        if ((float(delta)/1000 > mmin) && (float(delta)/1000 < mmax)) {
          
          if (vel > 0) {
            
            ok = true;
            if (clkws) {
              //orario - numeri decrescono
              if (i < 11) {
                cc = 17;
              }
              else {
                cc = map(i + zero,1,514,1,18);
              }
              
            }
            else {
              //antiorario - numeri aumentano
              if (i < 24) {
                cc = 17;
              }
              else {
                cc = map(i + zero - 6,1,514,1,18);
              }
              
            }
            if (cc == 18) {cc = 17;}
            if (langEN) {
              Keyboard.write(char(64 + cc));
            }
            else {
              Keyboard.write(char(96 + cc));
            }
          }
          else {
            ok = false;
          }
          
        }
        else {
          ok = false;
        }
        //Serial.print("\n0: ");
        //Serial.print("min: ");
        //Serial.print(mmin);
        //Serial.print(" eff: ");
        //Serial.print(float(delta)/1000);
        //Serial.print(" max: ");
        //Serial.println(mmax);
        Ven = false;
      }

      theV = 0;
    }
    oldi = i;
    
    oldoldI = oldI;
    oldI = I;
    
    wheelchk = millis();
  }
  
 
  
}

void chkB() {
  if (digitalRead(encA)) {
    clkws = true;
    if (i<1) {
      i = 512;
    }
    else {
      i--;
    }
  }
  else{
    clkws = false;
    if (i>512) {
      i = 1;
    }
    else {
      i++;
    }
  }
  I++;
  
}

void chk0() {
  i = 0;
}
