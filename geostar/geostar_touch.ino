void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  //calibrazione touch
  
}

int pS[] = {A0,A1,A2,A3,A6};
int tS[5];

void loop() {

  for (uint8_t i=0;i<5;i++) {
    tS[i] = touchRead(pS[i]);
  }

  for (uint8_t i=0;i<5;i++) {
    Serial.print(tS[i]);
    Serial.print(",");
  }
  Serial.println();

  delay(1000);
}
