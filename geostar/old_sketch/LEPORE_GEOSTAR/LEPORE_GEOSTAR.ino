#include <WiFi.h>

WiFiClient localClient;

const char* ssid = "GL-lab";
const char* password = "ciao56ciao";

const uint port = 10001;
const char* ip = "192.168.10.10";


void setup() {
  Serial.begin(115200);

  Serial.println("Connect Wlan");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());
  delay(500);
  localClient.connect(ip, port);
  delay(500);
}

byte ask_rfid[] = { 0x02, 0x00, 0x09, 0xFF, 0xB0, 0x01, 0x00, 0x18, 0x43 };

void loop() {
  sendRequest();
  delay(1000);
}

void sendRequest() {

  if (localClient.connected()) {
    localClient.write(ask_rfid, 9);  // send data
  }

  while (!localClient.available()) {
    delay(10);
  }

  while (localClient.available()) {
    word c = localClient.read();
    Serial.print(c, HEX);
    Serial.print(" ");
  }
  Serial.println();
}