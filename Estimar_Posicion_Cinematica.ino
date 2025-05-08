#include <math.h> 

//constantes
const float WHEEL_DISTANCE = 0.14; // distancia entre ruedas en METROS
const float MAX_MOTOR_SPEED_MPS = 0.6567; // velocidad máxima estimada en METROS/SEGUNDO
const unsigned long INITIAL_DELAY_MS = 15000;
unsigned long movementStartTime = 0; // guarda cuándo empieza el movimiento
// 

// variables de Estado (Odometría)
float robot_x = 0.0; 
float robot_y = 0.0; 
float robot_theta = 0.0; //orientación estimada (radianes)

// variables de Tiempo
unsigned long last_time_us; // tiempo de la última actualización (microsegundos)

// Pines del Driver L298N

// Motor Izquierdo
const int ENA = 6; // Pin PWM A (velocidad) - Conectado a D6~
const int IN1 = 3; // Pin Dirección 1 - Conectado a D3
const int IN2 = 2; // Pin Dirección 2 - Conectado a D2
// Motor Derecho
const int ENB = 11; // Pin PWM B (velocidad) - Conectado a D11~
const int IN3 = 5; // Pin Dirección 3 - Conectado a D5
const int IN4 = 4; // Pin Dirección 4 - Conectado a D4

// variables para la velocidad actual de los motores
// guardan el último comando enviado a cada motor (-255 a 255)
int current_pwm_L = 0;
int current_pwm_R = 0;

// Función para controlar un motor
// speed_pwm: -255 (reversa max) a 255 (adelante max)
void setMotorSpeed(char motor, int speed_pwm) {
  int pin_ena, pin_in1, pin_in2;

  // Asignar pines según el motor
  if (motor == 'L' || motor == 'l') {
    pin_ena = ENA;
    pin_in1 = IN1;
    pin_in2 = IN2;
    current_pwm_L = speed_pwm; // Guardar PWM actual
  } else if (motor == 'R' || motor == 'r') {
    pin_ena = ENB;
    pin_in1 = IN3;
    pin_in2 = IN4;
    current_pwm_R = speed_pwm; // Guardar PWM actual
  } else {
    return; 
  }

  // Limitar velocidad PWM entre -255 y 255
  speed_pwm = constrain(speed_pwm, -255, 255);

  // Establecer Dirección en pines IN
  if (speed_pwm > 0) { // Adelante
    digitalWrite(pin_in1, HIGH);
    digitalWrite(pin_in2, LOW);
  } else if (speed_pwm < 0) { // Reversa
    digitalWrite(pin_in1, LOW);
    digitalWrite(pin_in2, HIGH);
  } else { // Detener (Frenado Bajo)
    digitalWrite(pin_in1, LOW);
    digitalWrite(pin_in2, LOW);
  }

  // Establecer Velocidad (PWM) en pines ENA/ENB
  // abs() obtiene el valor absoluto porque PWM siempre es positivo (0-255)
  analogWrite(pin_ena, abs(speed_pwm));
}


void setup() {
  Serial.begin(115200);
  while (!Serial); // Espera a que el Monitor Serie se conecte
  Serial.println("==============================================");
  Serial.println("Estimador de Odometria Diferencial Iniciado");
  Serial.println("Usando:");
  Serial.print("  Distancia Ruedas (L): "); Serial.print(WHEEL_DISTANCE); Serial.println(" m");
  Serial.print("  Velocidad Max Estimada: "); Serial.print(MAX_MOTOR_SPEED_MPS); Serial.println(" m/s");
  Serial.println("ADVERTENCIA: La estimacion basada solo en PWM acumulara error rapidamente.");
  Serial.println("==============================================");


  // configurar pines de motor como salida
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // detener motores al inicio por seguridad
  setMotorSpeed('L', 0);
  setMotorSpeed('R', 0);

  // inicializar tiempo para cálculo de dt
  last_time_us = micros();

  Serial.println("Estado inicial: X=0.00 m, Y=0.00 m, Theta=0.0 deg");
  Serial.println("Formato Salida: t(s) | X(m) | Y(m) | Th(deg) | vL(m/s) | vR(m/s)");
  Serial.println("----------------------------------------------");
  
  Serial.print("INFO: Preparado. Iniciando movimiento en ");
  Serial.print(INITIAL_DELAY_MS / 1000); // Muestra segundos
  Serial.println(" segundos...");
  delay(INITIAL_DELAY_MS);
  Serial.println("INFO: ¡Iniciando movimiento!");
  movementStartTime = millis();
}

void loop() {
  // calcular Delta de Tiempo (dt) en segundos 
  unsigned long current_time_us = micros();
  float dt;
  // Manejo de posible overflow de micros() y dt muy pequeño o negativo
   if (current_time_us < last_time_us) { // Overflow 
       dt = (4294967295UL - last_time_us + current_time_us) / 1000000.0; // Corrección overflow
   } else {
       dt = (current_time_us - last_time_us) / 1000000.0; // dt normal
   }
   last_time_us = current_time_us; // Actualizar para el próximo ciclo

   unsigned long timeSinceMoveStart = millis() - movementStartTime;

   // ignorar si dt es inválido o demasiado pequeño para la odometría
   if (dt <= 0.00001) {
       if (timeSinceMoveStart < 3000) {
           setMotorSpeed('L', 150); // Adelante
           setMotorSpeed('R', 150);
       } else if (timeSinceMoveStart < 5000) { // Girar
            setMotorSpeed('L', 150);
            setMotorSpeed('R', -150); // Rueda derecha en reversa para giro sobre el centro
       }
       else {
           setMotorSpeed('L', 0); // Detener
           setMotorSpeed('R', 0);
       }
       return; // Salir del loop actual sin actualizar odometría
   }

  // obtener PWM actual
  //valores actualizados la última vez que se llamó a setMotorSpeed()

  //estimar Velocidades Lineales de las Ruedas (vL, vR)
  // PWM actual y la velocidad máxima estimada.
  float v_L = (current_pwm_L / 255.0) * MAX_MOTOR_SPEED_MPS;
  float v_R = (current_pwm_R / 255.0) * MAX_MOTOR_SPEED_MPS;

  //calcular Velocidades del Robot (V lineal, omega angular)
  float V = (v_R + v_L) / 2.0;
  float omega = (v_R - v_L) / WHEEL_DISTANCE; // Radianes/segundo

  // calcular Cambio en Orientación (delta_theta) 
  float delta_theta = omega * dt; // Radianes

  // calcular Ángulo Promedio del intervalo
  float theta_avg = robot_theta + delta_theta / 2.0;

  // calcular Cambio en Posición (delta_x, delta_y) 
  float delta_x = V * cos(theta_avg) * dt; // Metros
  float delta_y = V * sin(theta_avg) * dt; // Metros

  //actualizar Estado del Robot (Posición y Orientación)
  robot_x += delta_x;
  robot_y += delta_y;
  robot_theta += delta_theta;

  //normalizar Ángulo 
  if (robot_theta > M_PI) {         // M_PI es número PI
    robot_theta -= 2.0 * M_PI;      // Restar una vuelta completa (2*PI radianes)
  } else if (robot_theta <= -M_PI) {
    robot_theta += 2.0 * M_PI;      // Sumar una vuelta completa
  }

  //imprimir Estado Estimado (periódicamente)
  static unsigned long last_print_time = 0;
  // Imprime el estado cada 100 milisegundos (10 veces por segundo)
  if (millis() - last_print_time > 100) {
      Serial.print(millis() / 1000.0, 1); Serial.print(" | ");   // Tiempo en segundos
      Serial.print(robot_x, 2); Serial.print(" | ");             // Posición X en metros
      Serial.print(robot_y, 2); Serial.print(" | ");             // Posición Y en metros
      Serial.print(robot_theta * 180.0 / M_PI, 1); Serial.print(" | "); // Ángulo Theta en grados
      Serial.print(v_L, 2); Serial.print(" | ");                 // Velocidad estimada Izq m/s
      Serial.println(v_R, 2);                                    // Velocidad estimada Der m/s
      last_print_time = millis(); // Actualizar el tiempo de la última impresión
  }


  // LÓGICA DE CONTROL DEL ROBOT
  // Ejemplo: Avanzar 3 segundos, girar 2 segundos, detenerse.
  if (timeSinceMoveStart  < 3000) {
      // Avanzar recto con PWM 150
      setMotorSpeed('L', 150);
      setMotorSpeed('R', 150);
  } else if (timeSinceMoveStart < 5000) { // Desde el segundo 3 hasta el 5
      // Girar sobre el centro hacia la derecha (R atrás, L adelante)
       setMotorSpeed('L', 150);
       setMotorSpeed('R', -150); // Nota el negativo para reversa
  }
  else {
      // Detenerse después de 5 segundos
      setMotorSpeed('L', 0);
      setMotorSpeed('R', 0);
  }
}