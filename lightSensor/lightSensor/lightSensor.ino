//LDRs
//1 3
//2 4
const int pinLDR1 = A0; //arriba izquierda
const int pinLDR2 = A1; //abajo izquierda
const int pinLDR3 = A2; //arriba derecha
//const int pinLDR4 = A3; //abajo derecha

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
const int mover = 1;  //cuantos grados se mueve

int posX = 90;  //posicion inicial servos
int posY = 90;

void setup() {
  // Iniciamos la comunicación serie para ver los datos en pantalla
  Serial.begin(9600);

  servoX.attach(pinServoX);
  servoY.attach(pinServoY);

}

void loop() {
  //Servos empiezan en 90 grados
  servoX.write(posX);
  servoY.write(posY);

  // Leemos el valor analógico (de 0 a 1023)
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


}
