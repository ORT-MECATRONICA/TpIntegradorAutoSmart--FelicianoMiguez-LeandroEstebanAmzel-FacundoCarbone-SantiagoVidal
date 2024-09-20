int ldrPin = 32;  // Pin donde está conectado el LDR

void setup() {
  // Iniciar la comunicación serie para mostrar los datos en el monitor
  Serial.begin(115200);
  
  // Configurar el pin del LDR como entrada
  pinMode(ldrPin, INPUT);
}

void loop() {
  // Leer el valor analógico del LDR
  int ldrValue = analogRead(ldrPin);

  // Imprimir el valor leído en el Serial Monitor
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Esperar 1 segundo antes de hacer otra lectura
  delay(1000);
}
