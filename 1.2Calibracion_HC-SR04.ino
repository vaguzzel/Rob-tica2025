// Pines para el sensor HC-SR04
const int trigPin = 9; 
const int echoPin = 10;

// Número de lecturas a promediar para obtener una medición más estable
const int numLecturasParaPromedio = 15; 

void setup() {
  Serial.begin(9600); // Iniciar comunicación serial
  pinMode(trigPin, OUTPUT); // Configurar trigPin como salida
  pinMode(echoPin, INPUT);  // Configurar echoPin como entrada

  Serial.println("--- Inicio de Calibración del Sensor Ultrasónico ---");
  Serial.println("Sigue los pasos de calibración manual.");
  Serial.println("Coloca un objeto a una distancia REAL conocida y anota la 'Distancia promedio medida'.");
  Serial.println("----------------------------------------------------");
}

void loop() {
  float sumaDeMediciones = 0;

  // Realizar varias mediciones y sumarlas
  for (int i = 0; i < numLecturasParaPromedio; i++) {
    sumaDeMediciones += tomarUnaMedicionDirecta();
    delay(50); // Pequeña pausa entre cada una de las mediciones individuales
               // esto ayuda a estabilizar las lecturas y evitar ecos residuales.
  }

  // Calcular el promedio de las mediciones
  float distanciaPromedioMedida = sumaDeMediciones / numLecturasParaPromedio;

  // Mostrar la distancia promedio medida por el sensor
  Serial.print("Distancia promedio medida: ");
  Serial.print(distanciaPromedioMedida, 2); // Mostrar con 2 decimales
  Serial.println(" cm");

  // Esperar un tiempo antes de realizar el siguiente conjunto de mediciones.
  // Esto te da tiempo para cambiar la posición del objeto a la siguiente distancia real
  // y anotar los valores.
  delay(3000); 
}

// Función para tomar una única medición de distancia
float tomarUnaMedicionDirecta() {
  // Asegurar que el trigPin esté en bajo al inicio
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Generar el pulso de trigger (10 microsegundos)
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir la duración del pulso de echo (en microsegundos)
  // pulseIn() espera a que el pin se ponga HIGH, mide el tiempo que permanece HIGH, 
  // y luego espera a que se ponga LOW.
  long duracion = pulseIn(echoPin, HIGH);

  // Calcular la distancia
  // La velocidad del sonido es aproximadamente 0.034 cm/microsegundo.
  // La distancia es (duración * velocidad del sonido) / 2, porque el sonido va y vuelve.
  float distanciaCalculada = duracion * 0.034 / 2.0;
  
  return distanciaCalculada;
}