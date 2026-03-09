#include <Servo.h>

// 1. LIBRERÍAS
#include <Wire.h>              // Librería para comunicación I2C
#include <LiquidCrystal_I2C.h> // Librería para la pantalla LCD I2C

// 2. DEFINICIONES DE PINES Y PARÁMETROS

// Sensor de Suelo Capacitivo
#define SOIL_PIN A3

// Parámetros de Calibración del Suelo (¡AJUSTA ESTOS VALORES!)
// Estos valores definen el rango de 0% (seco) a 100% (mojado) para tu sensor.
// DEBES calibrarlos para tu unidad específica.
const int AIR_VALUE = 548;   // Valor RAW de la lectura cuando el sensor está en el aire (SECO = 0%)
const int WATER_VALUE = 350; // Valor RAW de la lectura cuando el sensor está sumergido en agua (MOJADO = 100%)

// 3. INICIALIZACIÓN DE OBJETOS
LiquidCrystal_I2C lcd(0x27, 16, 2); // 0x27 es la dirección I2C común (puede ser 0x3F)


//LDRs
//1 3
//2 4
const int pinLDR1 = A0; //arriba izquierda
const int pinLDR2 = A1; //abajo izquierda
const int pinLDR3 = A2; //arriba derecha
//const int pinLDR4 = A3; //abajo derecha
const int motorLimit = 102;

int valorLDR1 = 0;
int valorLDR2 = 0;
int valorLDR3 = 0;
//int valorLDR4 = 0;

//SERVOS
const int pinServoX = 9;  //base - horizontal
const int pinServoY = 10;  //vertical

Servo servoX;
Servo servoY;


//parámetros de control
const int margen = 15;  //dif < 15 no se mueve
//const int mover = 1;  //cuantos grados se mueve

int posX = 90;  //posicion inicial servos
int posY = 90;


void setup() {
  Serial.begin(9600);

  servoX.attach(pinServoX);
  servoY.attach(pinServoY);

  // Inicia el bus I2C (usa pines A4 y A5 por defecto)
  Wire.begin(); 

  // Inicializa el LCD
  lcd.init();
  lcd.backlight(); 
  lcd.print("Humedad Suelo Init");
  
  delay(2000);
  lcd.clear();

}

void loop() {
  //Servos empiezan en 90 grados
  servoX.write(posX);
  servoY.write(posY);



  //Leemos el valor analógico (de 0 (oscuro) a 1023(luz))
  valorLDR1 = analogRead(pinLDR1);
  valorLDR2 = analogRead(pinLDR2);
  valorLDR3 = analogRead(pinLDR3);
  //valorLDR4 = analogRead(pinLDR4);

  //imprime valor LDRs
  Serial.print("\nLDRs ->");
  Serial.print(" 1: ");
  Serial.print(valorLDR1);
  Serial.print(" | 2: ");
  Serial.print(valorLDR2);
  Serial.print(" | 3: ");
  Serial.print(valorLDR3);
  //Serial.print(" | 4: ");
  //Serial.print(valorLDR4);
  delay(50);

  
  int difX = (valorLDR1 - valorLDR2);  //base; izquierda - derecha
  int difY = (valorLDR1 + valorLDR2)/2 - (valorLDR3);  //arriba - abajo


  if (abs(difX) > margen) {  //base
    if (difX > 0) {
      posX++;  //hay más luz a la izq
    }
    else {
      posX--;  //hay más luz a la dcha
    }
  }
  
  if (abs(difY) > margen) {
    if (difY > 0) {
      posY++;  //hay más luz arriba
    }
    else {
      posY--;  //hay más luz abajo
    }
  }

  //revisar ángulos de los servos
  if (posX > 180) posX = 180;
  if (posX < 0) posX = 0;

  if (posY > 160) posY = 160;
  if (posY < 0) posY = 0;

  //mover servos
  servoX.write(posX);
  servoY.write(posY);


  //imprime nuevos valores de los servos
  Serial.print(" || Motores Ángulos ->");
  Serial.print(" Motor base: ");
  Serial.print(posX);
  Serial.print(" | Motor arriba: ");
  Serial.print(posY);

    // Espera 1 segundo entre lecturas
  delay(300);

  // ==================================================
  // 1. LECTURA DEL SENSOR DE SUELO CAPACITIVO
  // ==================================================
  
  // Lectura analógica del pin A0 (0 a 1023)
  int soil_raw_value = analogRead(SOIL_PIN); 

  // Mapear el valor RAW a un porcentaje de humedad (0% = Seco, 100% = Mojado)
  // map(valor, de_min, de_max, a_min, a_max)
  int soil_percent = map(soil_raw_value, AIR_VALUE, WATER_VALUE, 0, 100);

  // Asegurar que el valor esté entre 0 y 100
  if (soil_percent < 0) soil_percent = 0;
  if (soil_percent > 100) soil_percent = 100;
  
  // ==================================================
  // 2. MOSTRAR DATOS EN EL LCD Y MONITOR SERIE
  // ==================================================

  // Fila 0: Etiqueta
  lcd.setCursor(0, 0);
  lcd.print("Humedad del Suelo");

  // Fila 1: Porcentaje y Valor RAW
  lcd.setCursor(0, 1);
  lcd.print(soil_percent);
  lcd.print("% (RAW: ");
  lcd.print(soil_raw_value);
  lcd.print(")"); 
  
  // Monitor Serie para depuración
  Serial.println(" ");
  Serial.print("RAW: ");
  Serial.print(soil_raw_value);
  Serial.print(" | Humedad: ");
  Serial.print(soil_percent);
  Serial.println("%");

}

