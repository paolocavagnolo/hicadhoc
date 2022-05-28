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
int oldi = 0;
int I = 0;
int oldI = 0;
int oldoldI = 0;

#include "Keyboard.h"

void setup() {
  //Keyboard.begin();
  
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
  
  Serial.begin(9600);
  
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

void loop() {

  //LANGUAGE LED
  if (langEN) {
    digitalWrite(ledIT, HIGH);
  
    if ((millis() - ledT) > 900) {
      digitalWrite(ledEN, LOW);
      if ((millis() - ledT) > 1000) {
        digitalWrite(ledEN, HIGH);
        ledT = millis();
      }
    }
    
  }
  else {
    digitalWrite(ledEN, HIGH);

    if ((millis() - ledT) > 900) {
      digitalWrite(ledIT, LOW);
      if ((millis() - ledT) > 1000) {
        digitalWrite(ledIT, HIGH);
        ledT = millis();
      }
    }
    
    
  }

  //LANGUAGE BUTTON
  if (!digitalRead(btnIT) && btnITen) {
    Serial.println("ITALIANO");
    langEN = false;
    btnITen = false;
    btnITtimer = millis();
  }
  if (!digitalRead(btnEN) && btnENen) {
    Serial.println("INGLESE");
    langEN = true;
    btnENen = false;
    btnENtimer = millis();
  }
  if ((millis() - btnITtimer) > 500) {
    btnITen = true;
  }
  if ((millis() - btnENtimer) > 500) {
    btnENen = true;
  }


  //WHEEL STATE
  if ((millis() - wheelchk) > 500) {
    
    if (i != oldi) {
      
      isspinning = true;
      timeFen = true;
      float f = (I-oldI)-(oldI-oldoldI);
      
      if (f < 0) {
        if (Ven){
          theV = (I-oldI);
          Ven = false;
          vel = float(theV)/0.5/512*60;
          timeF = millis();
          if (vel > 90) {
            Keyboard.write("3");
          }
          else {
            Keyboard.write("2");
          }
          
          Serial.print("Vel MAX: ");
          Serial.print(vel);
          Serial.println(" rpm");
          Serial.print("\n\nGiro");
        }
        else {
            Keyboard.write("1");
            Serial.print(".");
          
        }
      }
      else {
        Ven = true;
      }
      
      
    }
    else {
      Serial.println("\nFinito");
      isspinning  = false;
      I = 0;
      
      Ven = true;
      
      if (timeFen) {
        
        delta = millis() - timeF;
        timeFen = false;
        if ((float(delta)/1000 > float(theV*13)/510) && (float(delta)/1000 < float(theV*15)/480)) {
          if (vel > 90) {
            ok = true;
            uint8_t cc = map(i,1,512,1,18);
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
        Serial.print("\n0: ");
        Serial.print("min: ");
        Serial.print(float(theV*13)/494);
        Serial.print(" eff: ");
        Serial.print(float(delta)/1000);
        Serial.print(" max: ");
        Serial.println(float(theV*16)/494);
        Ven = false;
      }
      Keyboard.write("0");
      
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
    if (i<1) {
      i = 512;
    }
    else {
      i--;
    }
  }
  else{
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
