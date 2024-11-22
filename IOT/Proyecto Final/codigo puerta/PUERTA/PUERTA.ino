#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pines RFID
#define SS_PIN 10//SDA pin
#define RST_PIN 9//Reset Pin
#define MOSI_PIN 11//Mosi Pin
#define MISO_PIN 12// Miso Pin
#define SCK_PIN 13 //Sck/Scl Pin

// Pines de salida
#define LED_G 5      // LED verde
#define LED_R 4      // LED rojo
#define RELAY 2      // Relay para la cerradura
#define BUZZER 3     // Buzzer

// Constantes
#define ACCESS_DELAY 2000
#define DENIED_DELAY 1000

// Configuración del LCD (dirección I2C, columnas, filas)
LiquidCrystal_I2C lcd(0x27, 16, 2);


// Instancia RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Listas de UIDs permitidos
String tarjetasPermitidas[] = {
  "F2:91:76:33",
  "A3:EE:B2:12",
  "D0:85:48:32",
  "B9:80:F0:97"
};

String llaverosPermitidos[] = {
  "89:5A:D2:B9",
  "E9:7F:D4:B9",
  "73:57:26:F8",
  "23:C7:E6:F7"
};

void setup() {
  // Inicialización básica
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Inicialización de pines
  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Configuración inicial: cerradura cerrada
  noTone(BUZZER);
  digitalWrite(RELAY, HIGH); // Cerradura cerrada (ajustar según lógica)

  // Inicializar LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Sistema listo");

  Serial.println("Coloca tu tarjeta...");
}

void loop() {
  // Mensaje por defecto
  lcd.setCursor(0, 0);
  lcd.print("Acerque        ");
  lcd.setCursor(0, 1);
  lcd.print("identificador  ");
  
  // Detectar tarjeta
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Leer UID
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
    if (i < mfrc522.uid.size - 1) content.concat(":");
  }
  content.toUpperCase();
  Serial.println("UID leída: " + content);

  // Verificar acceso
  if (esAccesoPermitido(content)) {
    accesoAutorizado();
  } else {
    accesoDenegado();
  }
}

bool esAccesoPermitido(String uid) {
  for (String tarjeta : tarjetasPermitidas) {
    if (uid == tarjeta) return true;
  }
  for (String llavero : llaverosPermitidos) {
    if (uid == llavero) return true;
  }
  return false;
}

void accesoAutorizado() {
  Serial.println("Acceso autorizado");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acceso permitido");

  digitalWrite(RELAY, LOW);  // Activar relay
  digitalWrite(LED_G, HIGH);
  delay(ACCESS_DELAY);

  digitalWrite(RELAY, HIGH); // Desactivar relay
  digitalWrite(LED_G, LOW);
  lcd.clear();
}

void accesoDenegado() {
  Serial.println("Acceso denegado");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acceso denegado");

  digitalWrite(LED_R, HIGH);
  tone(BUZZER, 300);  // Generar tono
  delay(DENIED_DELAY);

  digitalWrite(LED_R, LOW);
  noTone(BUZZER);
  lcd.clear();
}
