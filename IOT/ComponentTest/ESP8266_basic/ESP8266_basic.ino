
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Configuración WiFi
const char* ssid = "seol-llie";
const char* password = "732278426";

// URL completa de CallMeBot
const char* url = "http://api.callmebot.com/whatsapp.php?phone=+56923920355&text=Hola%20desde%20ESP8266%21&apikey=2138685";

WiFiClient client; // Objeto WiFiClient

void setup() {
  Serial.begin(115200);

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
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    Serial.println("Enviando solicitud...");
    http.begin(client, url); // Cambiado: ahora requiere el objeto WiFiClient como primer parámetro
    int httpCode = http.GET(); // Enviar solicitud GET

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Respuesta del servidor:");
      Serial.println(payload);
    } else {
      Serial.printf("Error en HTTP GET: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end(); // Finaliza la conexión HTTP
  } else {
    Serial.println("WiFi desconectado, intentando reconectar...");
  }

  delay(60000); // Esperar 1 minuto antes de enviar otro mensaje
}
