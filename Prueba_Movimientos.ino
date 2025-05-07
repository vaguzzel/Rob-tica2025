/*
 * Laboratorio 1 - Parte 1
 * Control de movimiento básico para robot móvil con Arduino UNO y driver L298N
 * ICI 4150 - Robótica y Sistemas Autónomos
 * Versión autónoma: Ejecuta una secuencia de movimientos automáticamente sin PC
 */

// Definición de pines para el driver de motores

// Motor A (Izquierdo)
const int IN1 = 3;  // Control de dirección 1 del Motor A
const int IN2 = 2;  // Control de dirección 2 del Motor A
const int ENA = 5;  // Control de velocidad del Motor A (pin PWM)

// Motor B (Derecho)
const int IN3 = 5; // Control de dirección 1 del Motor B
const int IN4 = 4; // Control de dirección 2 del Motor B
const int ENB = 6;  // Control de velocidad del Motor B (pin PWM)

void setup() {
  // Configuración de todos los pines como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  // Habilitamos los motores a máxima velocidad
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  
  // Inicialmente el robot está detenido
  detener();
  
  // Esperar 2 segundos antes de iniciar la secuencia de prueba
  // para dar tiempo a colocar el robot en posición
  delay(2000);
  
  // Ejecutar la secuencia de prueba al iniciar
  ejecutarSecuenciaPrueba();
}

void loop() {
  // Ejecutar continuamente la secuencia de prueba
  // con un intervalo entre cada ejecución
  ejecutarSecuenciaPrueba();
  delay(3000); // Esperar 3 segundos entre cada secuencia completa
}

// Avanzar (ambos motores hacia adelante)
void avanzar() {
  // Motor A - Adelante
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  // Motor B - Adelante
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Retroceder (ambos motores hacia atrás)
void retroceder() {
  // Motor A - Atrás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Motor B - Atrás
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Girar a la derecha (Motor izquierdo adelante, motor derecho atrás)
void girarDerecha() {
  // Motor A (Izquierdo) - Adelante
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  
  // Motor B (Derecho) - Atrás
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// Girar a la izquierda (Motor izquierdo atrás, motor derecho adelante)
void girarIzquierda() {
  // Motor A (Izquierdo) - Atrás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  
  // Motor B (Derecho) - Adelante
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// Detener ambos motores
void detener() {
  // Motor A - Detener
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  
  // Motor B - Detener
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Ejecutar una secuencia de prueba de todos los movimientos
void ejecutarSecuenciaPrueba() {
  // Avanzar durante 0.5 segundos
  avanzar();
  delay(500);
  
  // Detener durante 0.5 segundos
  detener();
  delay(1000);
  
  // Retroceder durante 0.5 segundos
  retroceder();
  delay(500);
  
  // Detener durante 0.5 segundos
  detener();
  delay(1000);
  
  // Girar a la derecha durante 0.5 segundos
  girarDerecha();
  delay(500);
  
  // Detener durante 0.5 segundos
  detener();
  delay(1000);
  
  // Girar a la izquierda durante 0.5 segundos
  girarIzquierda();
  delay(500);
  
  // Finalmente detener
  detener();
  delay(1000);
}