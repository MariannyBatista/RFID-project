#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
#define LED_G 5 //define el pin del LED verde
#define LED_R 4 //define el pin del LED rojo
#define RELAY 3 //define el pin del relay
#define BUZZER 2 //define el pin del buzzer
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000
MFRC522 mfrc522(SS_PIN, RST_PIN); // Crea una instancia de MFRC522
void setup()
{
 Serial.begin(9600); // Inicia la comunicación serial
 SPI.begin(); // Inicia el bus SPI
 mfrc522.PCD_Init(); // Inicia MFRC522
 pinMode(LED_G, OUTPUT);
 pinMode(LED_R, OUTPUT);
 pinMode(RELAY, OUTPUT);
 pinMode(BUZZER, OUTPUT);
 noTone(BUZZER);
 digitalWrite(RELAY, HIGH);
 Serial.println("Coloca tu tarjeta en el lector...");
 Serial.println();
}
void loop()
{
 // Busca nuevas tarjetas
 if (!mfrc522.PICC_IsNewCardPresent())
 {
 return;
 }
 // Selecciona una de las tarjetas
 if (!mfrc522.PICC_ReadCardSerial())
 {
 return;
 }
 // Muestra el UID en el monitor serial
 Serial.print("UID de la tarjeta:");
 String content = "";
 byte letter;
 for (byte i = 0; i < mfrc522.uid.size; i++)
 {
 Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
 Serial.print(mfrc522.uid.uidByte[i], HEX);
 content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
 content.concat(String(mfrc522.uid.uidByte[i], HEX));
 }
 Serial.println();
 Serial.print("Mensaje : ");
 content.toUpperCase();

 // Cambia aquí el UID de la tarjeta(s) a la(s) que quieres dar acceso
 if (content.substring(1) == "XX XX XX XX")
 {
 Serial.println("Acceso autorizado");
 Serial.println();
 delay(500);
 digitalWrite(RELAY, LOW);
 digitalWrite(LED_G, HIGH);
 delay(ACCESS_DELAY);
 digitalWrite(RELAY, HIGH);
 digitalWrite(LED_G, LOW);
 }
 else
 {
 Serial.println("Acceso denegado");
 digitalWrite(LED_R, HIGH);
 tone(BUZZER, 300);
 delay(DENIED_DELAY);
 digitalWrite(LED_R, LOW);
 noTone(BUZZER);
 }
}