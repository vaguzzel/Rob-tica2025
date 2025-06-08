// Pines HC-SR04
const int trigPin = 9; 
const int echoPin = 10;

// para medición más estable
const int numLecturasParaPromedio = 15; 

void setup() {
  Serial.begin(9600); // Iniciar comunicación serial
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);  

  Serial.println("--- Inicio de Calibración del Sensor Ultrasónico ---");
  Serial.println("Sigue los pasos de calibración manual.");
  Serial.println("Coloca un objeto a una distancia REAL conocida y anota la 'Distancia promedio medida'.");
  Serial.println("----------------------------------------------------");
}

void loop() {
  float sumaDeMediciones = 0;

  // Realizar mediciones y sumarlas
  for (int i = 0; i < numLecturasParaPromedio; i++) {
    sumaDeMediciones += tomarUnaMedicionDirecta();
    delay(50); 
  }

  // Calcular el promedio de las mediciones
  float distanciaPromedioMedida = sumaDeMediciones / numLecturasParaPromedio;

  
  Serial.print("Distancia promedio medida: ");
  Serial.print(distanciaPromedioMedida, 2); 
  Serial.println(" cm");

  // Esperar 
  delay(3000); 
}

float tomarUnaMedicionDirecta() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Generar el pulso de trigger 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir la duración del pulso de echo 
  long duracion = pulseIn(echoPin, HIGH);

  // Calcular la distancia
  float distanciaCalculada = duracion * 0.034 / 2.0;
  
  return distanciaCalculada;
}