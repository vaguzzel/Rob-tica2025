
// Pines para HC-SR04 
const int trigPin = 9; 
const int echoPin = 10;

const float umbralDistanciaCm = 10.0; // umbral para detectar un obstáculo (en cm)

void setup() {
  Serial.begin(9600); // iniciar comunicación serial
  pinMode(trigPin, OUTPUT); // trigPin  salida
  pinMode(echoPin, INPUT);  // echoPin  entrada

  Serial.println("--- Sensor Ultrasónico: Detección de Obstáculos por Umbral ---");
  Serial.print("Umbral de detección: < ");
  Serial.print(umbralDistanciaCm);
  Serial.println(" cm");
  Serial.println("-------------------------------------------------------------");
}

void loop() {
  // medir la distancia usando el sensor.
  float distanciaMedida = medirDistanciaUltrasonico();

  // distancia no negativa después de la corrección
  if (distanciaMedida < 0) {
    distanciaMedida = 0;
  }

  // mostrar la distancia medida
  Serial.print("Distancia medida: ");
  Serial.print(distanciaMedida, 2); // Mostrar con 2 decimales
  Serial.print(" cm. ");

  // umbralización para detectar el obstáculo
  if (distanciaMedida > 0 && distanciaMedida < umbralDistanciaCm) {
    Serial.println("--> ¡Obstáculo DETECTADO!");
  } else if (distanciaMedida == 0 && pulseIn(echoPin, HIGH, 25000) == 0) {

    Serial.println("--> No se detecta obstáculo cercano (fuera de rango o sin eco).");
  }
  else {
    Serial.println("--> Sin obstáculo cercano.");
  }

  delay(500); 
}

// Función medir distancia
float medirDistanciaUltrasonico() {
  // trigPin en bajo al inicio
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Generar el pulso de trigger 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir la duración del pulso de echo
  
  long duracion = pulseIn(echoPin, HIGH, 25000); 

  // Calcular la distancia
  
  float distanciaCalculada = duracion * 0.034 / 2.0;
  
  if (duracion == 0) {
    return 0.0; 
  }
  
  return distanciaCalculada;
}