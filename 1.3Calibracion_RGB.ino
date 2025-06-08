#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(
TCS34725_INTEGRATIONTIME_600MS, TCS34725_GAIN_1X);

void setup() {
Serial.begin(9600);

if (tcs.begin()) {
  Serial.println("Sensor TCS34725 detectado.");
  } else {
  Serial.println("No se encontró el sensor.");
  while (1);
  }
}

String detectarColor(float fr, float fg, float fb, uint16_t c) {
  if (c < 225) {
    return "Negro";
    } else if (fr > 0.4 && fg < 0.3 && fb < 0.3) {
    return "Rojo";
    } else if (fr < 0.35 && fg > 0.35 && fb < 0.3) {
    return "Verde";
    } else if (fr < 0.3 && fg < 0.4 && fb > 0.3) {
    return "Azul";
    } else if (fr > 0.2 && fg > 0.2 && fb > 0.2 && c > 450) {
    return "Blanco";
    } else {
    return "Desconocido";
  }
}

void loop() {
uint16_t r, g, b, c;
tcs.getRawData(&r, &g, &b, &c);

if (c == 0) c = 1;

float fr = (float)r / c;
float fg = (float)g / c;
float fb = (float)b / c;

String color = detectarColor(fr, fg, fb, c);

Serial.print(" R: "); Serial.print(fr);
Serial.print(" G: "); Serial.print(fg);
Serial.print(" B: "); Serial.print(fb);
Serial.print(" C: "); Serial.print(c);
Serial.print(" -> Color: "); Serial.println(color);

delay(1000);
}
