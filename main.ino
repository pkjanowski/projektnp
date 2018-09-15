#include <SoftwareSerial.h>

#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = 9;
constexpr uint8_t SS_PIN = 10;

MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial mySerial(3, 2); // RX, TX  definicja pinów do programowego interfejsu szeregowego

void setup() {
  pinMode(7, OUTPUT); //ustawienie kierunków pinów wyjściowych
  pinMode(8, OUTPUT);
  Serial.begin(9600);   // inicjalizacja interfejsu szeregowego
  mySerial.begin(9600); // inicjalizacja interfejsu szeregowego programowego
  while (!Serial);
  SPI.begin();      // inicjalizacja magistrali SPI (komunikacja z czytnikiem kart)
  mfrc522.PCD_Init();   // inicjalizacja czytnika kart

  Serial.println("");
  Serial.println("Ready");
  Serial.println("");
}

void loop() {

  // przekazywanie danych z programowego interfejsu szeregowego do interfejsu szeregowego
  if ( mySerial.available() )   {
    Serial.write( mySerial.read() );
  }

  // przekazywanie danych z interfejsu szeregowego do programowego interfejsu szeregowego
  if ( Serial.available() )       {
    mySerial.write( Serial.read() );
  }
  
  //oczekiwanie na karte
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  //wybór karty
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  //przesyłanie ID karty przez interfejs szeregowy
  String UniqueID = "";
  for (byte i = 0; i < mfrc522.uid.size; i ++)
  {
    Serial.print (mfrc522.uid.uidByte [i] < 0x10 ? "0" : "");
    Serial.print (mfrc522.uid.uidByte [i], HEX);
    UniqueID.concat (String (mfrc522.uid.uidByte [i] < 0x10 ? "0" : ""));
    UniqueID.concat (String (mfrc522.uid.uidByte [i], HEX));
  }
  UniqueID.toUpperCase();

  //sprawdzenie czy zbliżona karta ma dostęp
  if (UniqueID == "E1F162A3") //zgoda na dostęp
  {
    digitalWrite(7, HIGH);
    delay(1000);
    digitalWrite(7, LOW);
  }
  else                        //blokada dostępu
  {
    digitalWrite(8, HIGH);
    delay(1000);
    digitalWrite(8, LOW);
  }
}

