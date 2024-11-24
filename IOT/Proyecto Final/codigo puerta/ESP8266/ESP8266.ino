#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

// Configuración WiFi
const char* ssid = "seol-llie";
const char* password = "732278426";

// Configuración CallMeBot
const char* phone = "+56923920355";
const char* apiKey = "2138685";
WiFiClient client;

// Definir los pines para la comunicación serial
#define RX 13  
#define TX 15

SoftwareSerial arduinoSerial(RX, TX); // Comunicación serial con el Arduino

void setup() {
  Serial.begin(115200); // Comunicación con el monitor serial
  arduinoSerial.begin(9600); // Comunicación con el Arduino

  // Conexión WiFi
  Serial.print("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nConectado a WiFi.");
}

void loop() {
  // Verificar si hay datos del Arduino
  if (arduinoSerial.available()) {
    String data = arduinoSerial.readStringUntil('\n'); // Leer datos del Arduino

    Serial.println("Recibido del Arduino: " + data);

    // Enviar mensaje a través de CallMeBot
    if (WiFi.status() == WL_CONNECTED) {
      enviarMensaje(data);
    } else {
      Serial.println("WiFi desconectado, intentando reconectar...");
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("\nConectado a WiFi.");
    }
  }
}

void enviarMensaje(String mensaje) {
  
  String url = "http://api.callmebot.com/whatsapp.php?phone=" + String(phone) + "&text=" + mensaje + "&apikey=" + String(apiKey);

  HTTPClient http;
  Serial.println("Enviando solicitud a CallMeBot...");
  http.begin(client, url); // Inicia la conexión HTTP

  int httpCode = http.GET(); // Enviar solicitud GET
  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("Respuesta del servidor:");
    Serial.println(payload);
  } else {
    Serial.printf("Error en HTTP GET: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end(); // Finaliza la conexión HTTP
}
