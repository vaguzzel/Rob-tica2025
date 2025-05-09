#include <Wire.h>
#include <MPU9250_asukiaaa.h> 
#include <math.h>             

// Parámetros del robot
const float WHEEL_DISTANCE = 0.13;      
const float MAX_MOTOR_SPEED_MPS = 0.6567;  
const unsigned long INITIAL_DELAY_MS = 15000; 
const unsigned long MOVEMENT_DURATION_MS = 5000; 
const int STRAIGHT_PWM = 150;             

// Pines del Driver L298N
//izq
const int ENA = 6; 
const int IN1 = 3; 
const int IN2 = 2; 
//der
const int ENB = 11; 
const int IN3 = 5; 
const int IN4 = 4; 

//variables de Tiempo
unsigned long last_time_us_imu;    
unsigned long movementStartTime = 0; 

// IMU
MPU9250_asukiaaa mpu; // Objeto para el sensor MPU

// VALORES DE BIAS DEL IMU 
const float gyroBiasX = 0.278442;
const float gyroBiasY = 0.276489;
const float gyroBiasZ = 1.627624;

const float accelBiasX = 0.064088;   
const float accelBiasY = -0.022818;  
const float accelBiasZ = 0.053292;   

float currentYawAngle = 0.0;    
float currentPitchAngle = 0.0;  
float currentRollAngle = 0.0;  

// controlar motor 
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
  mpu.beginAccel(); // Inicializa el acelerómetro
  mpu.beginGyro();  // Inicializa el giroscopio

  Serial.println("==============================================");
  Serial.println("Prueba: Movimiento Recto con Registro IMU (Bias Actualizados)");
  Serial.println("Valores de Bias del IMU aplicados:");
  Serial.print("  Gyro (X,Y,Z): "); Serial.print(gyroBiasX); Serial.print(", "); Serial.print(gyroBiasY); Serial.print(", "); Serial.println(gyroBiasZ);
  Serial.print("  Accel (X,Y,Z): "); Serial.print(accelBiasX); Serial.print(", "); Serial.print(accelBiasY); Serial.print(", "); Serial.println(accelBiasZ);
  Serial.println("Orientacion asumida del MPU: Montado plano, X-MPU=Adelante, Y-MPU=Izquierda, Z-MPU=Arriba");
  Serial.println("==============================================");

  // Inicializar tiempo para dt del IMU
  last_time_us_imu = micros();

  Serial.print("INFO: Preparado. Iniciando movimiento en ");
  Serial.print(INITIAL_DELAY_MS / 1000);
  Serial.println(" segundos...");
  delay(INITIAL_DELAY_MS); // Pausa inicial
  Serial.println("INFO: ¡Iniciando movimiento recto!");
  movementStartTime = millis(); // Guardar tiempo de inicio del movimiento
}

void loop() {
  // calcular (dt) para el IMU
  unsigned long current_time_us_imu = micros();
  float dt_imu = (current_time_us_imu - last_time_us_imu) / 1000000.0; // dt en segundos
  last_time_us_imu = current_time_us_imu;
  if (dt_imu <= 0.00001 || dt_imu > 0.5) { // Si dt es muy pequeño o muy grande, saltar
      // Control de motores
      unsigned long timeSinceMoveStart = millis() - movementStartTime;
      if (timeSinceMoveStart < MOVEMENT_DURATION_MS) {
          setMotorSpeed('L', STRAIGHT_PWM);
          setMotorSpeed('R', STRAIGHT_PWM);
      } else {
          // Detener solo una vez
          static bool motors_stopped_imu_check = false;
          if (!motors_stopped_imu_check) {
            setMotorSpeed('L', 0);
            setMotorSpeed('R', 0);
            motors_stopped_imu_check = true;
          }
      }
      return;
  }

  // Leer y Procesar Datos del IMU 
  float accelX_raw, accelY_raw, accelZ_raw;
  float gyroX_raw, gyroY_raw, gyroZ_raw;

  // Leer datos crudos
  if (mpu.accelUpdate() == 0) { // 0 significa éxito
    accelX_raw = mpu.accelX(); 
    accelY_raw = mpu.accelY(); 
    accelZ_raw = mpu.accelZ(); 
  } else {
    Serial.println("Error leyendo acelerometro!");
    accelX_raw = 0; accelY_raw = 0; accelZ_raw = 0; // Evitar usar datos viejos o basura
  }

  if (mpu.gyroUpdate() == 0) { 
    gyroX_raw = mpu.gyroX(); // En grados/segundo
    gyroY_raw = mpu.gyroY(); 
    gyroZ_raw = mpu.gyroZ(); 
  } else {
    Serial.println("Error leyendo giroscopio!");
    gyroX_raw = 0; gyroY_raw = 0; gyroZ_raw = 0; // Evitar usar datos viejos o basura
  }

  // Aplicar calibración (restar bias)
  float accelX_cal = accelX_raw - accelBiasX;
  float accelY_cal = accelY_raw - accelBiasY;
  // accelZ_cal = accelZ_raw - accelBiasZ es la lectura donde el valor ideal plano es 1.0
  float accelZ_cal = accelZ_raw - accelBiasZ;


  float gyroX_cal = gyroX_raw - gyroBiasX;
  float gyroY_cal = gyroY_raw - gyroBiasY;
  float gyroZ_cal = gyroZ_raw - gyroBiasZ; //  usaremos  este para el Yaw

  // Calcular Ángulo de Giro (Yaw) 
  currentYawAngle += gyroZ_cal * dt_imu; 

  // Normalizar Yaw entre -180 y +180 grados
  if (currentYawAngle > 180.0) {
    currentYawAngle -= 360.0;
  } else if (currentYawAngle <= -180.0) {
    currentYawAngle += 360.0;
  }

  //calcular Ángulos de Inclinación (Pitch y Roll) desde el Acelerómetro 
  // Estas fórmulas asumen que el MPU está montado plano

  // Evitar división por cero o resultados extraños si el vector de aceleración es muy pequeño
  float accel_magnitude_sq = accelX_cal * accelX_cal + accelY_cal * accelY_cal + accelZ_cal * accelZ_cal;
  if (accel_magnitude_sq > 0.01) { // Solo calcular si la magnitud es razonable (ej, > 0.1g)
    // Roll: ángulo alrededor del eje X del robot (inclinación lateral)
    currentRollAngle = atan2(accelY_cal, accelZ_cal) * 180.0 / M_PI;
    // Pitch: ángulo alrededor del eje Y del robot (inclinación frontal/trasera)
    
    currentPitchAngle = atan2(-accelX_cal, accelZ_cal) * 180.0 / M_PI;
  }


  // control del robot 
  unsigned long timeSinceMoveStart = millis() - movementStartTime;
  static bool motors_stopped_main_logic = false; // Para detener solo una vez

  if (timeSinceMoveStart < MOVEMENT_DURATION_MS) {
    // Mover recto
    setMotorSpeed('L', STRAIGHT_PWM);
    setMotorSpeed('R', STRAIGHT_PWM);
    motors_stopped_main_logic = false; // rearmar si el tiempo vuelve a ser menor
  } else {
    // Detener después del tiempo
    if (!motors_stopped_main_logic) {
      setMotorSpeed('L', 0);
      setMotorSpeed('R', 0);
      Serial.println("INFO: Movimiento terminado. Motores detenidos.");
      motors_stopped_main_logic = true;
    }
  }

  // imprimir Datos  
  static unsigned long last_print_time = 0;
  if (millis() - last_print_time > 100) { // Imprimir cada 100 ms
    Serial.print(millis() / 1000.0, 1); Serial.print("s | ");
    Serial.print("Yaw: "); Serial.print(currentYawAngle, 1); Serial.print("deg | ");
    Serial.print("Pitch: "); Serial.print(currentPitchAngle, 1); Serial.print("deg | ");
    Serial.print("Roll: "); Serial.print(currentRollAngle, 1); Serial.print("deg");
    //  imprimir datos raw o calibrados del IMU para depuración
    Serial.print(" | Gz_c: "); Serial.print(gyroZ_cal, 2);
    Serial.print(" | Ax_c: "); Serial.print(accelX_cal, 2);
    Serial.print(" | Ay_c: "); Serial.print(accelY_cal, 2);
    Serial.print(" | Az_c: "); Serial.print(accelZ_cal, 2); 
    Serial.println();
    last_print_time = millis();
  }
}
