#include <Arduino.h>

//RFID-RC522 DEPS
#include <MFRC522.h> 
#include <SPI.h>

//HTTP Requests Deps
#include <WiFi.h>
#include <HTTPClient.h>

//Variáveis
#define SS_PIN  5  // ESP32 pin GPIO5 
#define RST_PIN 27 // ESP32 pin GPIO27
#define ledgreen 16
#define ledred 17

//Wifi PASSWD and SSID
const char* ssid = "Bocaberta";
const char* password = "total123**";

String serverName = "http://192.168.1.106:1880/update-sensor";

#define espmode  1 // 2 = SHOW CARD HEX | 1 = NORMAL MODE

byte authorizedUID[4] = {0xA6, 0x73, 0x0B, 0xF8};
MFRC522 rfid(SS_PIN, RST_PIN);

//const int ledred = 26;
//const int ledgreen = 27;

void funtest(){
  printf("test");
}
void tagreader(){
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
      if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
        MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
        Serial.print("RFID/NFC Tag Type: ");
        Serial.println(rfid.PICC_GetTypeName(piccType));

        // print UID in Serial Monitor in the hex format
        Serial.print("UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();

        rfid.PICC_HaltA(); // halt PICC
        rfid.PCD_StopCrypto1(); // stop encryption on PCD
      }
  }
}

void ledrefuse(){
  digitalWrite(ledred, HIGH);
  delay(100);
  digitalWrite(ledred, LOW);
  delay(100);
  digitalWrite(ledred, HIGH);
  delay(100);
  digitalWrite(ledred, LOW);
  delay(300);
  digitalWrite(ledred, HIGH);
  delay(100);
  digitalWrite(ledred, LOW);
  delay(100);
  digitalWrite(ledred, HIGH);
  delay(100);
  digitalWrite(ledred, LOW);
}

void ledaccept(){
  digitalWrite(ledgreen, HIGH);
  delay(100);
  digitalWrite(ledgreen, LOW);
  delay(100);
  digitalWrite(ledgreen, HIGH);
  delay(100);
  digitalWrite(ledgreen, LOW);
  delay(300);
  digitalWrite(ledgreen, HIGH);
  delay(100);
  digitalWrite(ledgreen, LOW);
  delay(100);
  digitalWrite(ledgreen, HIGH);
  delay(100);
  digitalWrite(ledgreen, LOW);

}

void setup() {

  // LED CONFIG
  pinMode(ledred, OUTPUT);
  pinMode(ledgreen, OUTPUT); 

  // put your setup code here, to run once:
  Serial.begin(115200);

  SPI.begin(); //INICIALIZA O BARRAMENTO SPI
  rfid.PCD_Init(); //INICIALIZA MFRC522
  Serial.println("\nTap an RFID/NFC tag on the RFID-RC522 reader");
}

void loop() {
  if (espmode == 2){
    tagreader();
  }
  else{
    if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

      if (rfid.uid.uidByte[0] == authorizedUID[0] &&
          rfid.uid.uidByte[1] == authorizedUID[1] &&
          rfid.uid.uidByte[2] == authorizedUID[2] &&
          rfid.uid.uidByte[3] == authorizedUID[3] ) {
        Serial.println("Authorized Tag");
        ledaccept();
      }
      else
      {
        Serial.print("Unauthorized Tag with UID:");
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();
        ledrefuse();
      }

      rfid.PICC_HaltA(); // halt PICC
      rfid.PCD_StopCrypto1(); // stop encryption on PCD
    }
  }
  }
}


