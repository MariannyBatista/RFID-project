// Práctica activar un relé
const int relePin=9;
void setup()
{
pinMode(LED,OUTPUT);
}
void loop()
{
digitalWrite(relePin,HIGH);
}