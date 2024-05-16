#define ASK_TIMEOUT 2000
#define ASK_MINWAIT 1000
#define TIMEOUT_RFID 60000

#define REL_PIN_UP 9
#define REL_PIN_DOWN 8

#define R_LED 14
#define G_LED 15
#define B_LED 16

#include <WiFi.h>
WiFiClient localClient;
WiFiUDP Udp;

// WIFI
int status = WL_IDLE_STATUS;
const char* ssid = "GL-lab";
const char* password = "ciao56ciao";
unsigned long tReconnect = 0;

// RFID TCP
const uint port = 10001;
const char* ip = "192.168.10.10";
IPAddress localIP = IPAddress(192, 168, 10, 20);
IPAddress gateway = IPAddress(192, 168, 10, 1);
IPAddress subnet = IPAddress(255, 255, 255, 0);

// BS UDP
const uint UDPport = 5000;  //57862
char receiveBuffer[4];
const char* bs_ip = "192.168.10.255";
char iUDP[2];
char oUDP[2];

#define DEB 1

byte ask_rfid[] = { 0x02, 0x00, 0x09, 0xFF, 0xB0, 0x01, 0x00, 0x18, 0x43 };

#define NUM_RFID 15
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
byte undici[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2B, 0x42 };
byte dodici[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2B, 0x52 };
byte tredici[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2B, 0x62 };
byte quattordici[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2B, 0x72 };
byte quindici[] = { 0xE0, 0x04, 0x01, 0x53, 0x06, 0xA2, 0x2B, 0x82 };

int trovati[100];

byte* lista[] = { uno, due, tre, quattro, cinque, sei, sette, otto, nove, dieci, undici, dodici, tredici, quattordici, quindici };
//String res[] = { "uno", "due", "tre", "quattro", "cinque", "sei", "sette", "otto", "nove", "dieci", "undici", "dodici", "tredici", "quattordici", "quindici" };

bool sentASK = false;
bool reconnect = false;

unsigned long t_ASK = 0;
int lenASK = 0;
int nRFID = 0;


void setup() {
  //BREATHE
  delay(1000);

  //LED
  pinMode(R_LED, OUTPUT);
  pinMode(G_LED, OUTPUT);
  pinMode(B_LED, OUTPUT);

  //RELAY
  pinMode(REL_PIN_UP, OUTPUT);
  pinMode(REL_PIN_DOWN, OUTPUT);
  digitalWrite(REL_PIN_UP, HIGH);
  digitalWrite(REL_PIN_DOWN, HIGH);

  //SERIAL
  Serial.begin(115200);
  delay(100);

  //WIFI
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    if (DEB) Serial.print(".");
  }

  if (DEB) Serial.println("CONNECTED:");
  if (DEB) Serial.println(WiFi.localIP());

  localClient.connect(ip, port);
  delay(500);

  //UDP
  Udp.begin(UDPport);
  delay(500);

  // INIT TROVATI
  init_trovati();
}

int S = 0;
bool P = true;
unsigned long T = 0;
bool E = false;

int missione = 0;
int domanda = 0;
bool wrong_answer = 0;

int missione_1[] = { 2, 1, 10, 7, 11, 6, 3, 0, 0, 0, 0, 0 };
int missione_2[] = { 2, 1, 10, 9, 3, 6, 5, 0, 0, 0, 0, 0 };
int missione_3[] = { 2, 1, 10, 9, 4, 5, 14, 6, 0, 0, 0, 0 };
int missione_4[] = { 15, 1, 10, 9, 6, 4, 5, 8, 12, 13, 14, 0 };

int* strumenti[] = { missione_1, missione_2, missione_3, missione_4 };
int strumento = 0;

bool checking_risposta = false;

unsigned long rT = 0, gT = 0, bT = 0;
int rI = 0, gI = 0, bI = 0;
bool rF = 0, gF = 0, bF = 0;
int rL = 0, gL = 0, bL = 0;
bool rS = false, gS = false, bS = false;

void loop() {

  checkReconnectWiFi();

  checkRGBled();

  // READ
  if (readUDP()) {
    if (DEB) Serial.print("Ricevuto: ");
    if (DEB) Serial.print(iUDP[0]);
    if (DEB) Serial.println(iUDP[1]);

    // RESET
    if (iUDP[0] == 'X') {
      if (iUDP[1] == 'X') {
        reset_all();
      }
    }

    // INIZIO MISSIONE
    else if ((iUDP[0] == 'A') || (iUDP[0] == 'B') || (iUDP[0] == 'C') || (iUDP[0] == 'D')) {
      domanda = 0;
      if (iUDP[0] == 'A') {
        missione = 1;
      } else if (iUDP[0] == 'B') {
        missione = 2;
      } else if (iUDP[0] == 'C') {
        missione = 3;
      } else if (iUDP[0] == 'D') {
        missione = 4;
      }

      if (DEB) Serial.print("Missione: ");
      if (DEB) Serial.println(missione);
    }

    // SELEZIONO LA DOMANDA
    else if (iUDP[0] == 'Q') {
      if (iUDP[1] == '?') {
        if (wrong_answer) {
          wrong_answer = false;
        } else {
          domanda++;
        }
      }
      if (DEB) Serial.print("Domanda: ");
      if (DEB) Serial.println(domanda);

      strumento = strumenti[missione - 1][domanda - 1];

      delay(100);  // A SA MAI

      // MANDO LA DOMANDA
      oUDP[0] = 'Q';
      oUDP[1] = c_itoa(strumento);
      writeUDP(oUDP);
      if (strumento > 0) {
        // RISPOSTA
        checking_risposta = true;
        init_trovati();
        T = millis();
        while (checking_risposta) {
          if ((millis() - T) > TIMEOUT_RFID) {
            reset_all();
          }
          checkAntenna();

          if ((trovati[0] >= 0) && (trovati[0] <= 14)) {
            if (DEB) Serial.println(trovati[0]);
            if ((trovati[0] + 1) == strumento) {
              oUDP[0] = 'R';
              oUDP[1] = c_itoa(strumento);
            } else {
              oUDP[0] = 'R';
              oUDP[1] = '0';
              wrong_answer = true;
            }
            checking_risposta = false;
          }
        }
        delay(500);
        writeUDP(oUDP);
      } else {
        // FINE - AZIONAMENTO MOTORI DOPO VIDEO
      }
    }
  }
}

void blink_blue(int d) {
  bI = 0;
  bT = millis();
  bF = true;
  bL = d;
}

void blink_red(int d) {
  rI = 0;
  rT = millis();
  rF = true;
  rL = d;
}

void blink_green(int d) {
  gI = 0;
  gT = millis();
  gF = true;
  gL = d;
}

void checkRGBled() {
  if (rF) {
    if ((millis() - rT) > 100) {
      if (rI < rL * 2) {
        rT = millis();
        digitalWrite(R_LED, rS);
        rS = !rS;
        rI++;
      } else {
        rF = false;
      }
    }
  } else {
    digitalWrite(R_LED, HIGH);
  }

  if (gF) {
    if ((millis() - gT) > 100) {
      if (gI < gL * 2) {
        gT = millis();
        digitalWrite(G_LED, gS);
        gS = !gS;
        gI++;
      } else {
        gF = false;
      }
    }
  } else {
    digitalWrite(G_LED, HIGH);
  }

  if (bF) {
    if ((millis() - bT) > 100) {
      if (bI < bL * 2) {
        bT = millis();
        digitalWrite(B_LED, bS);
        bS = !bS;
        bI++;
      } else {
        bF = false;
      }
    }
  } else {
    digitalWrite(B_LED, HIGH);
  }
}

void reset_all() {
  checking_risposta = false;
  missione = 0;
  domanda = 0;
  wrong_answer = false;
}

void checkReconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    if ((millis() - tReconnect) > 10000) {
      if (DEB) Serial.println("NOT CONNECTED. RECONNECTING...");
      blink_red(5);
      tReconnect = millis();
      WiFi.reconnect();
    }
  } else {
    if ((millis() - tReconnect) > 4444) {
      blink_red(1);
      tReconnect = millis();
    }
  }
}

int c_atoi(char a) {
  switch (a) {
    case '1':
      return 1;
    case '2':
      return 2;
    case '3':
      return 3;
    case '4':
      return 4;
    case '5':
      return 5;
    case '6':
      return 6;
    case '7':
      return 7;
    case '8':
      return 8;
    case '9':
      return 9;
    case 'a':
      return 10;
    case 'b':
      return 11;
    case 'c':
      return 12;
    case 'd':
      return 13;
    case 'e':
      return 14;
    case 'f':
      return 15;
    default:
      return 0;
  }
}

char c_itoa(int i) {
  switch (i) {
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    case 10:
      return 'a';
    case 11:
      return 'b';
    case 12:
      return 'c';
    case 13:
      return 'd';
    case 14:
      return 'e';
    case 15:
      return 'f';
    default:
      return '0';
  }
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
  } else {

    // READ RESPONSE
    while (sentASK) {
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
                  Serial.print(trovati[i]);
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
  for (uint8_t i = 0; i < 100; i++) {
    trovati[i] = -1;
  }
}

bool readUDP() {
  receiveBuffer[0] = '0';
  receiveBuffer[1] = '0';

  bool read = false;

  if (Udp.parsePacket()) {
    blink_green(1);
    Udp.read(receiveBuffer, 2);
    iUDP[0] = receiveBuffer[0];
    iUDP[1] = receiveBuffer[1];
    read = true;
  }

  return read;
}

void writeUDP(char msg[]) {

  for (uint8_t i = 0; i < 2; i++) {
    blink_blue(1);

    Udp.beginPacket(bs_ip, UDPport);
    Udp.write(msg[0]);
    Udp.write(msg[1]);
    Udp.endPacket();

    if (DEB) Serial.print("Sent: ");
    if (DEB) Serial.print(msg[0]);
    if (DEB) Serial.println(msg[1]);
    delay(100);
  }
}