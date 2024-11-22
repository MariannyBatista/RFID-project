#include <SPI.h>
#include <MFRC522.h> // Biblioteca para RFID

#define SS_PIN 10 // SDA pin
#define RST_PIN 9 // Reset pin

MFRC522 mfrc522(SS_PIN, RST_PIN); // Crear instancia del lector RFID

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial
  SPI.begin();        // Inicializa el bus SPI
  mfrc522.PCD_Init(); // Inicializa el módulo RFID
  Serial.println("Esperando una tarjeta o llavero...");
}

void loop() { 
  // Verificar si hay una nueva tarjeta presente
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // Salir si no hay tarjetas presentes
  }

  // Intentar leer la tarjeta
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // Salir si no se puede leer la tarjeta
  }

  Serial.println("ID de la tarjeta en formato HEX:");

  // Mostrar el UID de la tarjeta en el monitor serial
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""); // Añadir un 0 si es necesario
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) Serial.print(":");
  }
  Serial.println();

  // Detener la comunicación con la tarjeta
  mfrc522.PICC_HaltA();

  // Pequeño retardo para evitar lecturas repetidas
  delay(500);
}
