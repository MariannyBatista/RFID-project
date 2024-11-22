#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Configuración del LCD (dirección I2C, columnas, filas)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Asegúrate de que 0x27 es la dirección correcta

void setup() {
  // Inicializar la pantalla LCD
  lcd.init();
  lcd.backlight(); // Encender luz de fondo
  lcd.setCursor(0, 0); // Posiciona el cursor en la primera línea, primer carácter
  lcd.print("Hola, Mundo!"); // Escribe un mensaje en la primera línea
  lcd.setCursor(0, 1); // Posiciona el cursor en la segunda línea, primer carácter
  lcd.print("LCD I2C Listo!"); // Escribe un mensaje en la segunda línea
}

void loop() {
  // No necesitas agregar código aquí para esta prueba básica
}
