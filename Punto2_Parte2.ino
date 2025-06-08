
const int pinR = A0; // Pin componente Rojo
const int pinG = A1; // Pin componente Verde
const int pinB = A2; // Pin componente Azul

// umbrales para detección de colores 

//  ROJO
const int umbralRojo_R_MIN = 150; // min de R para considerarse rojo
const int umbralRojo_G_MAX = 80;  // max de G para - rojo
const int umbralRojo_B_MAX = 80;  // max de B para - rojo

//  VERDE
const int umbralVerde_G_MIN = 150; // min de G para considerarse verde
const int umbralVerde_R_MAX = 80;  // max de R para - verde
const int umbralVerde_B_MAX = 80;  // max de B para - verde

//  AZUL
const int umbralAzul_B_MIN = 150; // min de B para considerarse azul
const int umbralAzul_R_MAX = 80;  // max de R para - azul
const int umbralAzul_G_MAX = 80;  // max de G para - azul

void setup() {
  Serial.begin(9600); // iniciar comunicación serial

  Serial.println("--- Sensor RGB: Detección de Colores por Umbrales ---");
  Serial.println("Presenta objetos de colores al sensor y observa los valores R, G, B.");
  Serial.println("Ajusta los umbrales en el código según tus observaciones.");
  Serial.println("-----------------------------------------------------------");
}

void loop() {
  // Leer valores R, G, B del sensor
  int valorR = analogRead(pinR);
  int valorG = analogRead(pinG);
  int valorB = analogRead(pinB);

  //Mostrar los valores RGB crudos 
  Serial.print("R: ");
  Serial.print(valorR);
  Serial.print("  G: ");
  Serial.print(valorG);
  Serial.print("  B: ");
  Serial.print(valorB);

  //Aplicar umbralización para detectar el color

  String colorDetectado = "Desconocido";

  // ROJO
  if (valorR > umbralRojo_R_MIN && valorR > valorG && valorR > valorB &&
      valorG < umbralRojo_G_MAX &&
      valorB < umbralRojo_B_MAX) {
    colorDetectado = "ROJO";
  }
  // VERDE
  else if (valorG > umbralVerde_G_MIN && valorG > valorR && valorG > valorB &&
           valorR < umbralVerde_R_MAX &&
           valorB < umbralVerde_B_MAX) {
    colorDetectado = "VERDE";
  }
  // AZUL
  else if (valorB > umbralAzul_B_MIN && valorB > valorR && valorB > valorG &&
           valorR < umbralAzul_R_MAX &&
           valorG < umbralAzul_G_MAX) {
    colorDetectado = "AZUL";
  }
  
  Serial.print("  --> Color detectado: ");
  Serial.println(colorDetectado);

  delay(1000); 
}