#define ASK_TIMEOUT 2000
#define ASK_MINWAIT 1000

#include "USB.h"
#include "USBHIDKeyboard.h"
USBHIDKeyboard Keyboard;

#include <WiFi.h>
WiFiClient localClient;

const char* ssid = "GL-lab";
const char* password = "ciao56ciao";

const uint port = 10001;
const char* ip = "192.168.10.10";

#define DEB 1

byte ask_rfid[] = { 0x02, 0x00, 0x09, 0xFF, 0xB0, 0x01, 0x00, 0x18, 0x43 };

#define NUM_RFID 10
byte uno[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x70 };
byte due[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x80 };
byte tre[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x8F };
byte quattro[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x60 };
byte cinque[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x50 };
byte sei[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x40 };
byte sette[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x30 };
byte otto[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x21 };
byte nove[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x11 };
byte dieci[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2D, 0x01 };

int trovati[1000];

byte* lista[] = { uno, due, tre, quattro, cinque, sei, sette, otto, nove, dieci };
String res[] = { "uno", "due", "tre", "quattro", "cinque", "sei", "sette", "otto", "nove", "dieci" };

bool sentASK = false;
bool reconnect = false;

unsigned long t_ASK = 0;
int lenASK = 0;
int nRFID = 0;


void setup() {
  //BREATHE
  delay(1000);

  //SERIAL
  Serial.begin(115200);
  delay(100);

  //WIFI
  WiFi.begin(ssid, password);
  delay(100);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  localClient.connect(ip, port);
  delay(500);

  //KEYBOARD
  Keyboard.begin();
  USB.begin();

  delay(1000);

  // INIT TROVATI
  init_trovati();
}

int S = 0;
bool P = true;
unsigned long T = 0;
bool E = false;

int game_number = 0;

void loop() {

  // SCHERMATA HOME
  if (S == 0) {
    if (P) {
      if (DEB) Serial.print(S);
      if (DEB) Serial.println(" - Schermata HOME");
      P = false;
      T = millis();

      display('H');
    }

    checkButtons();

    if (E) {
      E = false;
      P = true;
      S = game_number;
    }
  }

  // GIOCO A - CERCA STRUMENTI
  else if (S == 10) {
    if (P) {
      if (DEB) Serial.print(S);
      if (DEB) Serial.println(" - Gioco A");
      P = false;
      T = millis();

      display('A');
    }

    checkAntenna();

    if (E) {
      E = false;
      P = true;
      S = game_number;
    }
  }

  // GIOCO B
  else if (S == 20) {
    if (P) {
      if (DEB) Serial.print(S);
      if (DEB) Serial.println(" - Gioco B");
      P = false;
      T = millis();

      display('B');
    }

    checkAntenna();

    if (E) {
      E = false;
      P = true;
      S = game_number;
    }
  }

  // GIOCO C
  else if (S == 30) {
    if (P) {
      if (DEB) Serial.print(S);
      if (DEB) Serial.println(" - Gioco C");
      P = false;
      T = millis();

      display('C');
    }

    checkAntenna();

    if (E) {
      E = false;
      P = true;
      S = game_number;
    }
  }

  // GIOCO D
  else if (S == 40) {
    if (P) {
      if (DEB) Serial.print(S);
      if (DEB) Serial.println(" - Gioco D");
      P = false;
      T = millis();

      display('D');
    }

    checkAntenna();

    if (E) {
      E = false;
      P = true;
      S = game_number;
    }
  }

  // FINE
  else if (S == 100) {
    if (P) {
      if (DEB) Serial.print(S);
      if (DEB) Serial.println(" - Fine Gioco");
      P = false;
      T = millis();

      display('F');
    }

    checkAntenna();

    if (E) {
      E = false;
      P = true;
      S = game_number;
    }
  }


}

void checkButtons() {
}

void display(char c) {
  Keyboard.write(c);
}

void checkAntenna() {
  // SENT REQUEST
  if (!sentASK) {
    if ((millis() - t_ASK) > ASK_MINWAIT) {

      t_ASK = millis();

      if (localClient.connected()) {

        if (DEB) Serial.println('\n');
        if (DEB) Serial.println("Send ASK command");
        localClient.write(ask_rfid, 9);
        sentASK = true;

      } else {

        reconnect = true;
      }
    }
  }

  // RECONNECT IF NEEDED
  if (reconnect) {
    if (DEB) Serial.println("RE-connecting...");
    reconnect = false;
    localClient.connect(ip, port);
  }

  // READ RESPONSE
  if (sentASK) {
    if ((millis() - t_ASK) > 50) {
      if ((millis() - t_ASK) < ASK_TIMEOUT) {
        lenASK = localClient.available();
        if (lenASK > 0) {
          if (DEB) {
            Serial.print("Received: ");
            Serial.print(lenASK);
            Serial.println(" byte.");
          }

          // LEGGO BUFFER
          init_trovati();
          if (lenASK <= 8) {
            // NON CE NULLA
            nRFID = 0;
            for (uint8_t i = 0; i < lenASK; i++) {
              uint8_t c = localClient.read();
            }
          } else {  // CE QUALCOSA

            // ELIMINO I PRIMI CARATTERI E LEGGO nRFID
            for (uint8_t i = 0; i < 9; i++) {
              uint8_t c = localClient.read();
              if (i == 6) {
                nRFID = c;
              }
            }

            if (DEB) {
              Serial.print("Number of RFID read: ");
              Serial.println(nRFID);
            }

            // CICLO SU TUTTI
            for (uint8_t j = 0; j < nRFID; j++) {

              uint8_t tempRFID[8];

              for (uint8_t i = 0; i < 8; i++) {
                uint8_t c = localClient.read();
                tempRFID[i] = c;
              }

              localClient.read();
              localClient.read();

              if (DEB) PrintHex8(tempRFID, 8);
              trovati[j] = check_code(tempRFID);
            }
          }

          localClient.flush();

          // TROVATI
          if (DEB) {
            Serial.print("trovati: ");
            for (uint8_t i = 0; i < nRFID; i++) {
              if (trovati[i] > -1) {
                Serial.print(res[trovati[i]]);
                Serial.print(",");
              }
            }
            Serial.println();
          }

          sentASK = false;
        }
      } else {
        // TIMEOUT
        if (DEB) Serial.println("TimeOut");
        sentASK = false;
        reconnect = true;
      }
    }
  }
}

void PrintHex8(uint8_t data[], uint8_t length)  // prints 8-bit data in hex with leading zeroes
{
  char tmp[16];

  for (int i = 0; i < length; i++) {
    sprintf(tmp, "0x%.2X", data[i]);
    Serial.print(tmp);
    Serial.print(" ");
  }

  Serial.println();
}

int check_code(uint8_t a[]) {

  for (uint8_t j = 0; j < NUM_RFID; j++) {
    bool flag = false;

    for (uint8_t i = 0; i < 8; i++) {
      if (a[i] != lista[j][i]) {
        flag = true;
      }
    }

    if (!flag) {
      return j;
    }
  }

  return -1;
}

void init_trovati() {
  for (uint8_t i = 0; i < NUM_RFID; i++) {
    trovati[i] = -1;
  }
}
