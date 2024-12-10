//Miguez-Vidal-Carbone-Amzel
#include <Adafruit_AHTX0.h>
#include <Wire.h>
#include <hd44780.h>                        // Librería principal
#include <hd44780ioClass/hd44780_I2Cexp.h>  // Clase para pantallas I2C

// Pines
#define LDR 36
#define BUTTON_UP 12
#define BUTTON_DOWN 14
#define SENSORP 19
#define RELAY 5
// Estados del botón
#define NONE 0
#define UP 1
#define DOWN 2
#define BOTH 3
int estadoBoton = NONE;
bool BOT_UP = HIGH;
bool BOT_DOWN = HIGH;
bool estadoPuerta;
// LCD y sensor
Adafruit_AHTX0 aht;
hd44780_I2Cexp lcd;

// Estados de la máquina de estado
#define STATE_DISPLAY 0
#define STATE_ADJUST 1

int currentState = STATE_DISPLAY;  // Modo inicial: mostrar datos
int Umbral = 0;                    // Índice del umbral ajustado
int Umbrales[3] = { 25, 50, 75 };  // Umbrales: [Temp, Hum, Luz]

// Variables de sensor
float temp = 0.0;
float hum = 0.0;
int lumen = 0;

void setup() {
  Serial.begin(115200);

  // Inicializar LCD
  int status = lcd.begin(16, 2);
  if (status) {
    while (true) {}
  }

  // Inicializar sensor
  if (!aht.begin()) {
    while (1) {}
  }


  // Configuración de pines
  pinMode(LDR, INPUT);
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(SENSORP, INPUT_PULLUP); 
  pinMode(RELAY, OUTPUT);

  // Mensaje inicial
  lcd.setCursor(0, 0);
  lcd.print("Inicializando...");
  delay(1000);
}

void loop() {
  // Lectura de botones
  botLecture();

  // Máquina de estado
  switch (currentState) {
    case STATE_DISPLAY:
      displaySensorData();
      break;
    case STATE_ADJUST:
      adjustThresholds();
      break;
  }
  
  if(estadoPuerta == 1){
    digitalWrite(RELAY, HIGH);
  }
  else{
    digitalWrite(RELAY, LOW);
  }
}

void botLecture() {
  BOT_UP = digitalRead(BUTTON_UP);  // Invertir lógica por PULLUP
  BOT_DOWN = digitalRead(BUTTON_DOWN);

  switch (estadoBoton) {
    case NONE:
      if (!BOT_UP) {
        estadoBoton = UP;
        Serial.println("bot_up");
      } else if (!BOT_DOWN) {
        estadoBoton = DOWN;
        Serial.println("bot_down");
      }
      break;

    case UP:
      if (BOT_UP) {
        Umbrales[Umbral]++;
        estadoBoton = NONE;  // Reiniciar estado
        Serial.println("none");
      } else if (!BOT_DOWN) {
        estadoBoton = BOTH;
        Serial.println("both");
      }
      break;

    case DOWN:
      if (BOT_DOWN) {
        Umbrales[Umbral]--;
        estadoBoton = NONE;  // Reiniciar estado
        Serial.println("none");
      } else if (!BOT_UP) {
        estadoBoton = BOTH;
        Serial.println("both");
      }
      break;

    case BOTH:
      if (BOT_DOWN && BOT_UP) {
        handleScreenTransition();
        estadoBoton = NONE;
        Serial.println("none");
      }
      break;
  }
}

void handleScreenTransition() {
  if (currentState == STATE_DISPLAY) {
    currentState = STATE_ADJUST;
    Umbral = 0;
  } else if (currentState == STATE_ADJUST) {
    Umbral++;
    if (Umbral > 2) {
      currentState = STATE_DISPLAY;
    }
  }
}

void displaySensorData() {
  sensors_event_t humidity, temperature;
  aht.getEvent(&humidity, &temperature);

  temp = temperature.temperature;
  hum = humidity.relative_humidity;
  lumen = map(analogRead(LDR), 1000, 4095, 0, 100);
  estadoPuerta = digitalRead(SENSORP);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Tem:" + String(temp, 1));
  lcd.setCursor(0, 1);
  lcd.print("Hum:" + String(hum, 1) + "%");
  lcd.setCursor(9, 0);
  lcd.print("Lux:" + String(lumen));
  lcd.setCursor(10, 1);
  lcd.print("Door:" + String(estadoPuerta));
  delay(300);
}

void adjustThresholds() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ajustar:");
  lcd.setCursor(0, 1);

  switch (Umbral) {
    case 0:
      lcd.print("Temp:" + String(Umbrales[0]));
      break;
    case 1:
      lcd.print("Hum:" + String(Umbrales[1]));
      break;
    case 2:
      lcd.print("Lumenes:" + String(Umbrales[2]));
      break;
  }
  delay(300);
}