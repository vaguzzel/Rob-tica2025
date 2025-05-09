#include <Wire.h>
#include <MPU9250_asukiaaa.h> 
#include <math.h>            

//constantes del Robot 
const float WHEEL_DISTANCE = 0.13;       // Distancia entre ruedas en METROS 
const unsigned long INITIAL_DELAY_MS = 8000; 
const unsigned long MOVEMENT_DURATION_MS = 10000; 
const int BASE_SPEED_PWM = 150;           

// constantes del Controlador Proporcional 

const float Kp = 2.0; //valor inicial  

// Pines del Driver L298N
// Motor Izquierdo
const int ENA = 6; 
const int IN1 = 3; 
const int IN2 = 2; 
// Motor Derecho
const int ENB = 11; 
const int IN3 = 5; 
const int IN4 = 4; 

// variables de Tiempo 
unsigned long last_time_us_control;   
unsigned long movementStartTime = 0;  

// variables imu
MPU9250_asukiaaa mpu; // Objeto para el sensor MPU

//valores bias del imu
const float gyroBiasX = 0.278442;
const float gyroBiasY = 0.276489;
const float gyroBiasZ = 1.627624;


float currentYawAngle = 0.0;   // Ángulo de giro actual estimado (grados)
float targetYawAngle = 0.0;    // Ángulo deseado

// controlar el motor

void setMotorSpeed(char motor, int speed_pwm) {
  int pin_ena, pin_in1, pin_in2;
  if (motor == 'L' || motor == 'l') {
    pin_ena = ENA; pin_in1 = IN1; pin_in2 = IN2;
  } else if (motor == 'R' || motor == 'r') {
    pin_ena = ENB; pin_in1 = IN3; pin_in2 = IN4;
  } else { return; }
  speed_pwm = constrain(speed_pwm, -255, 255);
  if (speed_pwm > 0) { digitalWrite(pin_in1, HIGH); digitalWrite(pin_in2, LOW);
  } else if (speed_pwm < 0) { digitalWrite(pin_in1, LOW); digitalWrite(pin_in2, HIGH);
  } else { digitalWrite(pin_in1, LOW); digitalWrite(pin_in2, LOW); }
  analogWrite(pin_ena, abs(speed_pwm));
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // configurar pines de motor
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setMotorSpeed('L', 0); setMotorSpeed('R', 0); // motores detenidos al inicio

  // inicializar I2C y MPU
  Wire.begin();
  mpu.setWire(&Wire);
  //giroscopio
  mpu.beginGyro();

  Serial.println("==============================================");
  Serial.println("Control de Rumbo Recto con IMU (Control P)");
  Serial.print("Kp (Ganancia Proporcional): "); Serial.println(Kp);
  Serial.println("Intentando mantener Yaw = 0 grados...");
  Serial.println("==============================================");

  // Inicializar tiempo para dt del control
  last_time_us_control = micros();

  Serial.print("INFO: Preparado. Iniciando movimiento en ");
  Serial.print(INITIAL_DELAY_MS / 1000);
  Serial.println(" segundos...");
  delay(INITIAL_DELAY_MS); // Pausa inicial
  Serial.println("INFO: ¡Iniciando movimiento con correccion!");
  movementStartTime = millis(); // Guardar tiempo de inicio del movimiento
}

void loop() {
  // calcular dt para el ciclo de control
  unsigned long current_time_us_control = micros();
  float dt_control = (current_time_us_control - last_time_us_control) / 1000000.0; // dt en segundos
  last_time_us_control = current_time_us_control;
  if (dt_control <= 0.0 || dt_control > 0.5) { // dt inválido o demasiado grande
      return; // Saltar este ciclo
  }

  // leer y procesar datos giroscopio
  float gyroZ_raw;
  if (mpu.gyroUpdate() == 0) { // 0 significa éxito
    gyroZ_raw = mpu.gyroZ(); // en grados/segundo
  } else {
    Serial.println("Error leyendo giroscopio!");
    gyroZ_raw = 0; // Usar 0 si hay error
  }

  // Aplicar calibración (restar bias)
  float gyroZ_cal = gyroZ_raw - gyroBiasZ;

  // calcular yaw
  // Integrar la velocidad angular calibrada
  currentYawAngle += gyroZ_cal * dt_control; // Actualización en grados


  // calcular error de orientación
  // Error = Ángulo Deseado - Ángulo Actual
  float errorYaw = targetYawAngle - currentYawAngle;

  // calcular señal de corrección
  float correction = Kp * errorYaw;

  // calcular PWM para cada motor
  int pwmLeft = BASE_SPEED_PWM + (int)correction;
  int pwmRight = BASE_SPEED_PWM - (int)correction; 

  // Limitar los PWM resultantes al rango válido (-255 a 255)
  pwmLeft = constrain(pwmLeft, 0, 255); 
  pwmRight = constrain(pwmRight, 0, 255);

  //  Duración del Movimiento 
  unsigned long timeSinceMoveStart = millis() - movementStartTime;
  static bool motors_stopped = false;

  if (timeSinceMoveStart < MOVEMENT_DURATION_MS) {
    // Aplicar PWM calculado a los motores
    setMotorSpeed('L', pwmLeft);
    setMotorSpeed('R', pwmRight);
    motors_stopped = false;
  } else {
    // Detener después del tiempo
    if (!motors_stopped) {
      setMotorSpeed('L', 0);
      setMotorSpeed('R', 0);
      Serial.println("INFO: Movimiento terminado. Motores detenidos.");
      motors_stopped = true;
    }
    // Detener motores por seguridad si ya pasó el tiempo
    setMotorSpeed('L', 0);
    setMotorSpeed('R', 0);
  }

  //  Imprimir Datos para Monitoreo y Tuning  
  static unsigned long last_print_time = 0;
  if (millis() - last_print_time > 100) {  
    Serial.print(millis() / 1000.0, 1); Serial.print("s | ");
    Serial.print("Yaw: "); Serial.print(currentYawAngle, 1); Serial.print(" | ");
    Serial.print("Err: "); Serial.print(errorYaw, 1); Serial.print(" | ");
    Serial.print("Corr: "); Serial.print(correction, 1); Serial.print(" | ");
    Serial.print("PwmL: "); Serial.print(pwmLeft); Serial.print(" | ");
    Serial.print("PwmR: "); Serial.println(pwmRight);
    last_print_time = millis();
  }
}
