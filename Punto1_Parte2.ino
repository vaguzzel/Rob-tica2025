// --- Configuración del Sensor Ultrasónico ---
// Pines para el sensor HC-SR04 (ajusta si usas otros)
const int trigPin = 9; 
const int echoPin = 10;

// --- Umbral de Detección de Obstáculos ---
const float umbralDistanciaCm = 10.0; // Umbral para detectar un obstáculo (en cm)

// --- Opcional: Factor o Corrección de Calibración ---
// Si determinaste un factor o una corrección en el paso de calibración,
// puedes definirlo aquí. Por ejemplo:
// const float correccionOffset = -0.5; // Si el sensor mide 0.5cm de más consistentemente
// const float factorEscala = 0.98;     // Si el sensor mide un 2% de más

void setup() {
  Serial.begin(9600); // Iniciar comunicación serial
  pinMode(trigPin, OUTPUT); // Configurar trigPin como salida
  pinMode(echoPin, INPUT);  // Configurar echoPin como entrada

  Serial.println("--- Sensor Ultrasónico: Detección de Obstáculos por Umbral ---");
  Serial.print("Umbral de detección: < ");
  Serial.print(umbralDistanciaCm);
  Serial.println(" cm");
  Serial.println("-------------------------------------------------------------");
}

void loop() {
  // 1. Medir la distancia usando el sensor ultrasónico
  float distanciaMedida = medirDistanciaUltrasonico();

  // (Opcional) Aplicar corrección de calibración si la tienes
  // Ejemplo con offset:
  // distanciaMedida += correccionOffset; 
  // Ejemplo con factor de escala:
  // distanciaMedida *= factorEscala;

  // Asegurarse de que la distancia no sea negativa después de la corrección
  if (distanciaMedida < 0) {
    distanciaMedida = 0;
  }

  // 2. Mostrar la distancia medida (útil para depuración)
  Serial.print("Distancia medida: ");
  Serial.print(distanciaMedida, 2); // Mostrar con 2 decimales
  Serial.print(" cm. ");

  // 3. Aplicar umbralización para detectar el obstáculo
  if (distanciaMedida > 0 && distanciaMedida < umbralDistanciaCm) {
    // Si la distancia es mayor que 0 (lectura válida) Y menor que el umbral
    Serial.println("--> ¡Obstáculo DETECTADO!");
  } else if (distanciaMedida == 0 && pulseIn(echoPin, HIGH, 25000) == 0) {
    // A veces pulseIn() puede dar 0 si no hay eco o si el objeto está muy lejos (timeout)
    // El timeout por defecto de pulseIn es 1 segundo. Podemos usar un timeout más corto para objetos lejanos.
    // 25000 microsegundos es aprox para 400-430cm. Si es 0, puede ser objeto muy lejano o fuera de rango.
    Serial.println("--> No se detecta obstáculo cercano (fuera de rango o sin eco).");
  }
  else {
    Serial.println("--> Sin obstáculo cercano.");
  }

  delay(500); // Esperar medio segundo antes de la siguiente medición
}

// --- Función para Medir Distancia con el Sensor Ultrasónico ---
float medirDistanciaUltrasonico() {
  // Asegurar que el trigPin esté en bajo al inicio
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Generar el pulso de trigger (10 microsegundos)
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Medir la duración del pulso de echo (en microsegundos)
  // El tercer parámetro opcional en pulseIn es el timeout en microsegundos.
  // Un timeout más corto puede ayudar a que el programa no se bloquee tanto si no hay eco.
  // 23529 microsegundos es aproximadamente 400cm (0.034 cm/us * 23529 us / 2 = ~400cm)
  long duracion = pulseIn(echoPin, HIGH, 25000); // Timeout de 25ms

  // Calcular la distancia
  // Velocidad del sonido = 0.034 cm/microsegundo
  // Distancia = (duración * velocidad del sonido) / 2 (porque el sonido va y vuelve)
  float distanciaCalculada = duracion * 0.034 / 2.0;
  
  // Si la duración fue 0 (timeout o sin eco), la distancia será 0.
  // Podrías considerar si quieres devolver un valor especial para "fuera de rango"
  // pero para la umbralización simple, 0 funcionará bien para "no cerca".
  if (duracion == 0) {
    return 0.0; // O un valor grande que indique "fuera de rango", ej: 999.0
  }
  
  return distanciaCalculada;
}