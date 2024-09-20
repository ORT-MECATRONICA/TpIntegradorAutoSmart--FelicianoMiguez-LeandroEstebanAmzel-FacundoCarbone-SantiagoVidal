#include <Wire.h>
#include <Adafruit_AHT10.h>

// Inicializamos el sensor AHT10
Adafruit_AHT10 aht;

void setup() {
  Serial.begin(115200);  // Iniciar la comunicación serie
  delay(1000);  // Esperar un momento para estabilizar la comunicación

  // Inicializar el sensor AHT10
  if (!aht.begin()) {
    Serial.println("No se pudo encontrar el sensor AHT10");
    while (1);  // Detener el programa si no encuentra el sensor
  }
  
  Serial.println("Sensor AHT10 inicializado correctamente");
}

void loop() {
  sensors_event_t humidity, temp;
  
  // Obtenemos las lecturas de temperatura y humedad
  aht.getEvent(&humidity, &temp);

  // Imprimir los valores en el Serial Monitor
  Serial.print("Temperatura: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  Serial.print("Humedad: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");

  delay(2000);  // Esperar 2 segundos antes de realizar otra lectura
}