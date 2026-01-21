// Definimos el pin donde conectamos el punto medio
const int pinLDR = A0; 
int valorLDR = 0;

void setup() {
  // Iniciamos la comunicación serie para ver los datos en pantalla
  Serial.begin(9600);
}

void loop() {
  // Leemos el valor analógico (de 0 a 1023)
  valorLDR = analogRead(pinLDR);

  // Imprimimos el valor bruto
  Serial.print("Valor del sensor: ");
  Serial.print(valorLDR);

  // Una pequeña ayuda visual para interpretar el dato
  // Nota: El valor 500 es un ejemplo, ajústalo según la luz de tu habitación
  if (valorLDR < 200) {
    Serial.println(" -> Muy Oscuro (Noche)");
  } else if (valorLDR < 600) {
    Serial.println(" -> Luz tenue (Sombra/Interior)");
  } else {
    Serial.println(" -> Mucha Luz (Día/Linterna)");
  }

  // Esperamos un poco antes de la siguiente lectura
  delay(500); 
}