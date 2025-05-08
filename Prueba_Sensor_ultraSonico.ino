/*
 * Laboratorio 1 - Parte 1
 * Verificación del funcionamiento del sensor ultrasónico HC-SR04
 * ICI 4150 - Robótica y Sistemas Autónomos
 */

// pines para el sensor ultrasónico HC-SR04
const int TRIG_PIN = 9;  // Pin de Trigger
const int ECHO_PIN = 10;  // Pin de Echo

// variables cálculo de distancia
long duracion;
float distancia;
float distancia_cm;

void setup() {
  // inicialización monitor serie
  Serial.begin(9600);
  Serial.println("Inicializando sensor ultrasónico HC-SR04...");
  
  // configuración pines del sensor ultrasónico
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // al principio el trigger está en estado bajo
  digitalWrite(TRIG_PIN, LOW);
  
  Serial.println("Sensor ultrasónico inicializado.");
  Serial.println("Mostrando mediciones de distancia cada 500ms:");
}

void loop() {
  // medimos distancia con el sensor ultrasónico
  medirDistancia();
  
  // mostramos la distancia medida en el monitor serie
  Serial.print("Distancia: ");
  Serial.print(distancia_cm);
  Serial.println(" cm");
  
  // esperar antes de la siguiente medición
  delay(500);
}

void medirDistancia() {
  // Enviar un pulso de 10μs por el pin Trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Medir el tiempo que tarda en recibir el eco
  duracion = pulseIn(ECHO_PIN, HIGH);
  
  /* Calculamos la distancia
   * Velocidad del sonido = 343 m/s = 0.0343 cm/μs
   * Teniendo en cuenta que la distancia es la mitad del recorrido (ida y vuelta)
   * distancia = (tiempo × velocidad) / 2
   * distancia = (duracion × 0.0343) / 2
   */
  distancia_cm = (duracion * 0.0343) / 2;
}