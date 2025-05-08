#include <Wire.h>
#include <MPU9250_asukiaaa.h> 

MPU9250_asukiaaa mpu;

// variables para almacenar los offsets (bias) calculados
float gyroBiasX = 0, gyroBiasY = 0, gyroBiasZ = 0;
float accelBiasX = 0, accelBiasY = 0, accelBiasZ = 0;

// variables temporales para sumar lecturas durante la calibración
float gyroSumX = 0, gyroSumY = 0, gyroSumZ = 0;
float accelSumX = 0, accelSumY = 0, accelSumZ = 0;

const int N = 1000; // aumentamos un poco el número de lecturas para mayor estabilidad


bool stopped = false;

void setup() {
  Serial.begin(115200);
  while (!Serial); // esperar a que el monitor serie esté listo 

  Wire.begin();
  mpu.setWire(&Wire);

  // inicializar Acelerómetro y Giroscopio
  // es buena práctica verificar si la inicialización fue exitosa
  mpu.beginAccel();
  mpu.beginGyro();

  // pequeña pausa para estabilizar
  delay(1000);

  // calibración del Giroscopio
  Serial.println("Iniciando Calibracion del Giroscopio...");
  Serial.println("--> POR FAVOR, MANTEN EL SENSOR COMPLETAMENTE INMOVIL <--");
  delay(2000); // dar tiempo para dejarlo quieto

  for (int i = 0; i < N; i++) {
    if (mpu.gyroUpdate() == 0) { // leer datos del giroscopio (0 = éxito)
      gyroSumX += mpu.gyroX();
      gyroSumY += mpu.gyroY();
      gyroSumZ += mpu.gyroZ();
    } else {
      Serial.println("Error leyendo giroscopio durante calibracion!");
      
    }
    if (i % 100 == 0) Serial.print("."); // indicador de progreso
    delay(5); // pequeña pausa entre lecturas
  }
  Serial.println();

  // Calcular bias del giroscopio (promedio)
  gyroBiasX = gyroSumX / N;
  gyroBiasY = gyroSumY / N;
  gyroBiasZ = gyroSumZ / N;

  Serial.println("Calibracion del Giroscopio Terminada.");
  Serial.print("Gyro Bias X: "); Serial.println(gyroBiasX, 6); // imprimir con decimales
  Serial.print("Gyro Bias Y: "); Serial.println(gyroBiasY, 6);
  Serial.print("Gyro Bias Z: "); Serial.println(gyroBiasZ, 6);
  Serial.println("---------------------------------------");
  delay(1000);

  // calibración del Acelerómetro (Método Simple - Sensor Plano)
  Serial.println("Iniciando Calibracion del Acelerometro...");
  Serial.println("--> POR FAVOR, COLOCA EL SENSOR PLANO (EJE Z APUNTANDO HACIA ARRIBA) Y ESTABLE <--");
  delay(3000); // dar tiempo al usuario para posicionarlo

  for (int i = 0; i < N; i++) {
    if (mpu.accelUpdate() == 0) { //leer datos del acelerómetro (0 = éxito)
      accelSumX += mpu.accelX();
      accelSumY += mpu.accelY();
      accelSumZ += mpu.accelZ(); //la librería devuelve valores en 'g'
    } else {
      Serial.println("Error leyendo acelerometro durante calibracion!");
    }
    if (i % 100 == 0) Serial.print("."); //indicador de progreso
    delay(5); //pequeña pausa entre lecturas
  }
  Serial.println();

  // calcular promedios crudos del acelerómetro
  float avgAccelX = accelSumX / N;
  float avgAccelY = accelSumY / N;
  float avgAccelZ = accelSumZ / N;

  // calcular bias del acelerómetro
  // asumimos que X e Y deben ser 0g, y Z debe ser +1g cuando está plano con Z hacia arriba.
  accelBiasX = avgAccelX;        // el bias es la desviación de 0g
  accelBiasY = avgAccelY;        // el bias es la desviación de 0g
  accelBiasZ = avgAccelZ - 1.0f; // el bias es la desviación de +1g

  Serial.println("Calibracion del Acelerometro Terminada.");
  Serial.print("Accel Bias X (desviacion de 0g): "); Serial.println(accelBiasX, 6);
  Serial.print("Accel Bias Y (desviacion de 0g): "); Serial.println(accelBiasY, 6);
  Serial.print("Accel Bias Z (desviacion de 1g): "); Serial.println(accelBiasZ, 6);
  Serial.println("=======================================");
  Serial.println("Calibracion Completa. Iniciando lecturas corregidas...");
  delay(1000);
}

void loop() {
  // resvisar si se envió un comando para detener
  if (Serial.available() > 0) { // ¿hay datos enviados desde el Monitor Serie?
    char command = Serial.read(); // lee el carácter enviado
    if (command == 's' || command == 'S') { 
      stopped = true; 
      Serial.println("====== Loop detenido por comando Serial ======");
    }
  }

  // ejecuta la lógica principal si no está detenido
  if (!stopped) {
    
    bool accelOk = (mpu.accelUpdate() == 0);
    bool gyroOk = (mpu.gyroUpdate() == 0);

    if (accelOk && gyroOk) {
        // obtener lecturas raw (ya en unidades físicas g y deg/s)
        float accelX_raw = mpu.accelX();
        float accelY_raw = mpu.accelY();
        float accelZ_raw = mpu.accelZ();
        float gyroX_raw = mpu.gyroX();
        float gyroY_raw = mpu.gyroY();
        float gyroZ_raw = mpu.gyroZ();

        // restando el bias calculado en setup
        float accelX_corr = accelX_raw - accelBiasX;
        float accelY_corr = accelY_raw - accelBiasY;
        float accelZ_corr = accelZ_raw - accelBiasZ; // restamos la desviación de 1g

        float gyroX_corr = gyroX_raw - gyroBiasX;
        float gyroY_corr = gyroY_raw - gyroBiasY;
        float gyroZ_corr = gyroZ_raw - gyroBiasZ;

        // imprimir valores 
        Serial.print("Acc(g): ");
        Serial.print(accelX_corr, 3); Serial.print(", ");
        Serial.print(accelY_corr, 3); Serial.print(", ");
        Serial.print(accelZ_corr, 3); Serial.print(" | ");
        Serial.print("Gyr(d/s): ");
        Serial.print(gyroX_corr, 3); Serial.print(", ");
        Serial.print(gyroY_corr, 3); Serial.print(", ");
        Serial.println(gyroZ_corr, 3);
        Serial.print("Acc(g): "); 
    } else {
      Serial.println("Error al leer MPU!");
    }
    
  } 

  delay(100); 
}