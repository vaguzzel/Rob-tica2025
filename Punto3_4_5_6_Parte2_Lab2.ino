#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <MPU9250_asukiaaa.h>
#include <math.h>

//pines y objetos
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_16X);
MPU9250_asukiaaa mpu;
const int ENA=6, IN1=3, IN2=2, ENB=11, IN3=5, IN4=4;
const int trigPin=8, echoPin=10;

//parámetros de control y navegación
const float Kp = 2.0;
float targetYaw = 0.0;
float currentYawAngle = 0.0;
const float gyroBiasZ = 1.627624;
const float umbralObstaculoCm = 15.0;
const float FACTOR_CALIBRACION_SONIDO = 1.03;
const int VEL_AVANCE = 100;
const int VEL_AVANCE_LENTO = 100;
const int VEL_GIRO = 100;

// variables globales
unsigned long tiempoPrevioYaw = 0;
enum EstadoRobot { NAVEGANDO, EJECUTANDO_MANIOBRA };
EstadoRobot estadoActual = NAVEGANDO;

//inicialización de componentes
void setup() {
  Serial.begin(115200);
  Wire.begin();

  // pines
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  setMotorSpeed('L', 0); setMotorSpeed('R', 0);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // sensores
  if (tcs.begin()) {
    Serial.println("Sensor de Color TCS34725 detectado.");
  } else {
    Serial.println("ERROR: No se encontró el sensor de color. Deteniendo.");
    while (1);
  }
  mpu.setWire(&Wire);
  mpu.beginGyro();
  Serial.println("IMU MPU9250 inicializado.");
  
  tiempoPrevioYaw = micros();
  Serial.println("=================================================");

  // delay de inicio 
  Serial.println("Robot listo. Colóquelo en posición.");
  Serial.println("Iniciando en 5 segundos...");
  delay(1000); Serial.println("4...");
  delay(1000); Serial.println("3...");
  delay(1000); Serial.println("2...");
  delay(1000); Serial.println("1...");
  delay(1000);
  Serial.println("¡Iniciando navegación reactiva!");
  Serial.println("=================================================");
  targetYaw = 0; // se asume que la dirección inicial es 0 grados
}

// loop principal
void loop() {
  actualizarYaw(); 

  if (estadoActual == NAVEGANDO) {
    float distancia = medirDistanciaUltrasonico();
    String color = leerSensorColor();

    Serial.print("Dist: "); Serial.print(distancia,0);
    Serial.print("cm | Yaw: "); Serial.print(currentYawAngle,1);
    Serial.print(" | Tgt: "); Serial.print(targetYaw,1);
    Serial.print(" | Color: "); Serial.print(color);
    
    if (distancia < umbralObstaculoCm) {
      Serial.println(" -> ACCIÓN: OBSTÁCULO -> DETENER");
      detener();
    } else {
      if (color == "Rojo") {
        estadoActual = EJECUTANDO_MANIOBRA;
        ejecutarManiobraRoja();
      } else if (color == "Azul") {
        estadoActual = EJECUTANDO_MANIOBRA;
        ejecutarManiobraAzul();
      } else if (color == "Verde") {
        Serial.println(" -> ACCIÓN: VERDE -> AVANZAR");

        mantenerRumbo(VEL_AVANCE);
      } else {
        Serial.println(" -> ACCIÓN: OTRO -> AVANZAR LENTO");

        mantenerRumbo(VEL_AVANCE_LENTO);
      }
    }
  }
  
  delay(50);
}

// maniobras, control, sensores, utlilidad y motores
void ejecutarManiobraRoja() {
  Serial.println(" -> INICIO MANIOBRA ROJA");
  detener();
  delay(200);
  girarGrados(90); // izq
  unsigned long tiempoInicioAvance = millis();
  while(millis() - tiempoInicioAvance < 750) { 
    mantenerRumbo(VEL_AVANCE_LENTO);
    actualizarYaw();
    delay(10);
  }
  detener();
  delay(200);
  Serial.println(" -> FIN MANIOBRA ROJA");
  estadoActual = NAVEGANDO;
}

void ejecutarManiobraAzul() {
  Serial.println(" -> INICIO MANIOBRA AZUL");
  detener();
  delay(200);
  girarGrados(-90); // der
  unsigned long tiempoInicioAvance = millis();
  while(millis() - tiempoInicioAvance < 750) {
    mantenerRumbo(VEL_AVANCE_LENTO);
    actualizarYaw();
    delay(10);
  }
  detener();
  delay(200);
  Serial.println(" -> FIN MANIOBRA AZUL");
  estadoActual = NAVEGANDO;
}

void detener() {
  setMotorSpeed('L', 0);
  setMotorSpeed('R', 0);
}

void mantenerRumbo(int velocidadBase) {
  float error = normalizarAngulo(targetYaw - currentYawAngle);
  float correccion = Kp * error;
  setMotorSpeed('L', velocidadBase + correccion);
  setMotorSpeed('R', velocidadBase - correccion);
}

void girarGrados(float grados) {
  targetYaw = normalizarAngulo(targetYaw + grados);
  unsigned long tiempoInicioGiro = millis();
  while(millis() - tiempoInicioGiro < 4000) {
    actualizarYaw();
    float error = normalizarAngulo(targetYaw - currentYawAngle);
    if (abs(error) < 2.0) break;
    float esfuerzoGiro = Kp * 1.5 * error;
    esfuerzoGiro = constrain(esfuerzoGiro, -VEL_GIRO, VEL_GIRO);
    setMotorSpeed('L', -esfuerzoGiro);
    setMotorSpeed('R', esfuerzoGiro);
    delay(10);
  }
  detener();
}

void actualizarYaw() { 
  unsigned long currentTime = micros();
  float dt = (currentTime - tiempoPrevioYaw) / 1000000.0f;
  tiempoPrevioYaw = currentTime;
  if (mpu.gyroUpdate() == 0) {
    float gyroZ_cal = mpu.gyroZ() - gyroBiasZ;
    currentYawAngle += gyroZ_cal * dt;
    currentYawAngle = normalizarAngulo(currentYawAngle);
  }
}
float medirDistanciaUltrasonico() { 
  digitalWrite(trigPin, LOW); delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duracion = pulseIn(echoPin, HIGH, 25000);
  float distanciaCalculada = (duracion * 0.034 / 2.0) * FACTOR_CALIBRACION_SONIDO;
  return (duracion == 0) ? 999.0 : distanciaCalculada;
}
String leerSensorColor() { 
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  if (c == 0) c = 1;
  float fr = (float)r / c; float fg = (float)g / c; float fb = (float)b / c;
  if (c < 150) return "Negro";
  else if (fr > 0.4 && fg < 0.3 && fb < 0.3) return "Rojo";
  else if (fr < 0.35 && fg > 0.35 && fb < 0.3) return "Verde";
  else if (fr < 0.3 && fg < 0.4 && fb > 0.3) return "Azul";
  else if (fr > 0.2 && fg > 0.2 && fb > 0.2 && c > 450) return "Blanco";
  else return "Desconocido";
}
float normalizarAngulo(float angulo) { 
  while (angulo > 180.0f) angulo -= 360.0f;
  while (angulo <= -180.0f) angulo += 360.0f;
  return angulo;
}
void setMotorSpeed(char motor, int speed_pwm) { 
  int pin_ena, pin_in1, pin_in2;
  if (motor == 'L' || motor == 'l') { pin_ena = ENA; pin_in1 = IN1; pin_in2 = IN2;
  } else if (motor == 'R' || motor == 'r') { pin_ena = ENB; pin_in1 = IN3; pin_in2 = IN4;
  } else { return; }
  speed_pwm = constrain(speed_pwm, -255, 255);
  if (speed_pwm > 0) { digitalWrite(pin_in1, HIGH); digitalWrite(pin_in2, LOW);
  } else if (speed_pwm < 0) { digitalWrite(pin_in1, LOW); digitalWrite(pin_in2, HIGH);
  } else { digitalWrite(pin_in1, LOW); digitalWrite(pin_in2, LOW); }
  analogWrite(pin_ena, abs(speed_pwm));
}