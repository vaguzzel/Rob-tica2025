/*
 * Este código ejecuta una secuencia de movimientos con pausas intermedias.

 */


// Pines del Driver L298N
// izq
const int ENA = 6; 
const int IN1 = 3; 
const int IN2 = 2; 
// der
const int ENB = 11; 
const int IN3 = 5; 
const int IN4 = 4; 

// constantes Secuencia de Movimiento 
const unsigned long INITIAL_WAIT_MS = 10000; 
const unsigned long PAUSE_MS = 1000;        

// duraciones de cada FASE de movimiento
const unsigned long TIME_FASE_1 = 2000; 
const unsigned long TIME_FASE_2 = 3000; 
const unsigned long TIME_FASE_3 = 1000;  
const unsigned long TIME_FASE_4 = 2000;  

// calcular tiempos de finalización acumulados (incluyendo pausas)
const unsigned long END_FASE_1 = TIME_FASE_1;
const unsigned long END_PAUSE_1 = END_FASE_1 + PAUSE_MS;
const unsigned long END_FASE_2 = END_PAUSE_1 + TIME_FASE_2;
const unsigned long END_PAUSE_2 = END_FASE_2 + PAUSE_MS;
const unsigned long END_FASE_3 = END_PAUSE_2 + TIME_FASE_3;
const unsigned long END_PAUSE_3 = END_FASE_3 + PAUSE_MS;
const unsigned long END_FASE_4 = END_PAUSE_3 + TIME_FASE_4;


// PWM para cada fase
const int PWM_LENTO = 100;
const int PWM_RAPIDO = 200;
const int PWM_GIRO_L = 150;  
const int PWM_GIRO_R = 50;   
const int PWM_REVERSA = -100;  

//variables de tiempo
unsigned long movementStartTime = 0;  
String currentPhase = ""; 

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

// imprimir la fase solo cuando cambia
void printPhase(String phaseName) {
  if (currentPhase != phaseName) {
    Serial.println(phaseName);
    currentPhase = phaseName;
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // configurar pines de motor
  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);

  // detener motores al inicio
  setMotorSpeed('L', 0);
  setMotorSpeed('R', 0);

  Serial.println("======================================");
  Serial.println("Parte 2, Punto 4: Control Velocidad y Tiempo (CON PAUSAS)");
  Serial.println("Ejecutando secuencia de movimientos con pausas de 1 seg...");
  Serial.println("======================================");

  // espera inicial
  Serial.print("INFO: Preparado. Iniciando secuencia en ");
  Serial.print(INITIAL_WAIT_MS / 1000);
  Serial.println(" segundos...");
  delay(INITIAL_WAIT_MS);
  Serial.println("INFO: ¡Iniciando secuencia!");
  movementStartTime = millis(); // guardar tiempo de inicio de la secuencia
}

void loop() {
  // calcular el tiempo transcurrido desde el inicio de la secuencia
  unsigned long timeSinceMoveStart = millis() - movementStartTime;

  // determinar en qué fase de la secuencia estamos
  if (timeSinceMoveStart < END_FASE_1) {
    //avanzar lento
    printPhase("Fase 1: Avanzando Lento");
    setMotorSpeed('L', PWM_LENTO);
    setMotorSpeed('R', PWM_LENTO);
  } else if (timeSinceMoveStart < END_PAUSE_1) {
    //pausa 1
    printPhase("Pausa 1 (Motores Detenidos)");
    setMotorSpeed('L', 0);
    setMotorSpeed('R', 0);
  } else if (timeSinceMoveStart < END_FASE_2) {
    // avanzar Rápido
    printPhase("Fase 2: Avanzando Rapido");
    setMotorSpeed('L', PWM_RAPIDO);
    setMotorSpeed('R', PWM_RAPIDO);
  } else if (timeSinceMoveStart < END_PAUSE_2) {
    //  pausa 2
    printPhase("Pausa 2 (Motores Detenidos)");
    setMotorSpeed('L', 0);
    setMotorSpeed('R', 0);
  } else if (timeSinceMoveStart < END_FASE_3) {
    //girar Derecha Suave
    printPhase("Fase 3: Girando Derecha");
    setMotorSpeed('L', PWM_GIRO_L);  
    setMotorSpeed('R', PWM_GIRO_R);
  } else if (timeSinceMoveStart < END_PAUSE_3) {
    // pausa 3
    printPhase("Pausa 3 (Motores Detenidos)");
    setMotorSpeed('L', 0);
    setMotorSpeed('R', 0);
  } else if (timeSinceMoveStart < END_FASE_4) {
    //  Retroceder Lento 
    printPhase("Fase 4: Retrocediendo Lento");
    setMotorSpeed('L', PWM_REVERSA); 
    setMotorSpeed('R', PWM_REVERSA);  
  } else {
    // detener
    static bool stopped = false;
    if (!stopped) {
      printPhase("Secuencia Terminada: Motores Detenidos");
      setMotorSpeed('L', 0);
      setMotorSpeed('R', 0);
      stopped = true; // Detener solo una vez
    }
  }

  delay(20);
}
