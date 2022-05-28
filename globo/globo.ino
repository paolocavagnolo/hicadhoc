#include <Controllino.h>  

int tempo_acceso = 10; //secondi

void setup() {

  pinMode(CONTROLLINO_D3, OUTPUT);
  pinMode(CONTROLLINO_D4, OUTPUT);
  pinMode(CONTROLLINO_D5, OUTPUT);

  pinMode(CONTROLLINO_A0, INPUT);
  pinMode(CONTROLLINO_A2, INPUT);
  
}


void loop() {

  if (digitalRead(CONTROLLINO_A0)) {
    
    digitalWrite(CONTROLLINO_D3, HIGH);
    digitalWrite(CONTROLLINO_D5, HIGH);

    delay(200);

    digitalWrite(CONTROLLINO_D4, HIGH);
    
    delay(tempo_acceso*1000);
  }


  else if (digitalRead(CONTROLLINO_A2)) {
    
    digitalWrite(CONTROLLINO_D3, LOW);
    digitalWrite(CONTROLLINO_D5, LOW);

    delay(200);

    digitalWrite(CONTROLLINO_D4, HIGH);
    
    delay(tempo_acceso*1000);
    
  }

  else {
    
    digitalWrite(CONTROLLINO_D4, LOW);
    digitalWrite(CONTROLLINO_D3, LOW);
    digitalWrite(CONTROLLINO_D5, LOW);
    
  }

  delay(100);
  
}
