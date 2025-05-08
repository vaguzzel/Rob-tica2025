#include <Wire.h>
#include <MPU9250_asukiaaa.h> 
#include <math.h> 

MPU9250_asukiaaa mpu;

// valores de bias de la última calibración
const float GYRO_BIAS_X = 0.278442;
const float GYRO_BIAS_Y = 0.276489;
const float GYRO_BIAS_Z = 1.627624;
const float ACCEL_BIAS_X = 0.064088;
const float ACCEL_BIAS_Y = -0.022818;
const float ACCEL_BIAS_Z = 0.053292; // desviación respecto a 1g

// variables para calcular ángulos
float pitch_deg = 0;
float roll_deg = 0;
float yaw_angle_deg = 0; // ángulo yaw acumulado

// variables para calcular dt (intervalo de tiempo)
unsigned long previous_time_micros = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Wire.begin();
  mpu.setWire(&Wire);

  mpu.beginAccel();
  mpu.beginGyro();

  Serial.println("MPU Inicializado. Usando valores de Bias predefinidos.");
  Serial.println("Iniciando lecturas de sensores y cálculo de ángulos...");
  delay(1000);

  // inicializamos tiempo para cálculo de dt
  previous_time_micros = micros();
}

void loop() {
  // calculamos dt (intervalo de tiempo desde la última lectura)
  unsigned long current_time_micros = micros();
  // dt en segundos = (tiempo_actual - tiempo_previo) / 1,000,000
  float dt = (current_time_micros - previous_time_micros) / 1000000.0f;
  previous_time_micros = current_time_micros; // actualizamos para la próxima iteración

  // leer los sensores
  bool accelOk = (mpu.accelUpdate() == 0);
  bool gyroOk = (mpu.gyroUpdate() == 0);

  if (accelOk && gyroOk) {
    // obtener lecturas crudas y aplicar corrección de bias 
    float ax_corr = mpu.accelX() - ACCEL_BIAS_X;
    float ay_corr = mpu.accelY() - ACCEL_BIAS_Y;
    float az_corr = mpu.accelZ() - ACCEL_BIAS_Z; // ya corregido respecto a 1g

    float gx_corr = mpu.gyroX() - GYRO_BIAS_X;
    float gy_corr = mpu.gyroY() - GYRO_BIAS_Y;
    float gz_corr = mpu.gyroZ() - GYRO_BIAS_Z; // yaw rate corregido

    // calcular Inclinación (Pitch y Roll) desde acelerómetro

    // roll - rotación sobre eje X
    // atan2 maneja cuadrantes correctamente. (resultado en radianes).
    float roll_rad = atan2(ay_corr, az_corr);
    roll_deg = roll_rad * (180.0 / M_PI); // convertir a grados

    // pitch- rotación sobre eje Y
    // usamos -ax_corr para la convención común 
    float pitch_rad = atan2(-ax_corr, sqrt(ay_corr * ay_corr + az_corr * az_corr));
    pitch_deg = pitch_rad * (180.0 / M_PI); // convertir a grados

    // calcular Ángulo de Giro (Yaw) integrando el Giroscopio
    // sumamos el giro ocurrido en el intervalo dt
    // yaw_angle_deg = yaw_angle_deg + (velocidad_giro_Z * tiempo_intervalo)
    yaw_angle_deg += gz_corr * dt;

 

    // ángulos calculados
    Serial.print("Roll: "); Serial.print(roll_deg, 1); Serial.print(" | ");
    Serial.print("Pitch: "); Serial.print(pitch_deg, 1); Serial.print(" | ");
    Serial.print("Yaw: "); Serial.println(yaw_angle_deg, 1); // yaw acumulado

  } else {
    Serial.println("Error al leer MPU!");
  }

  delay(500); // un pequeño delay 
}