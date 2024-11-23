#include <Arduino.h>

// Definición de parámetros para la señal senoidal
const int dacPin = 25; // Pin DAC1 (GPIO25)
const int numSamples = 50; // Número de muestras por ciclo de la senoide
const int amplitude = 127; // Amplitud de la senoide (0-255)
const int offset = 128; // Desplazamiento para que la senoide sea positiva
const float frequency = 50.0; // Frecuencia de la señal en Hz

// Arreglo para almacenar los valores de la senoide
uint8_t sineWave[numSamples];

void setup() {
  // Configuración de serial para el plotter
  Serial.begin(115200);

  // Configuración del DAC
  pinMode(dacPin, OUTPUT);

  // Generar la tabla de senoide
  for (int i = 0; i < numSamples; i++) {
    float angle = (2.0 * PI * i) / numSamples; // Ángulo en radianes
    sineWave[i] = (uint8_t)(amplitude * sin(angle) + offset); // Valor de la senoide
  }
}

void loop() {
  // Tiempo entre muestras para lograr la frecuencia deseada
  int delayMicros = (int)((1.0 / frequency) / numSamples * 1e6);

  // Salida de la senoide al DAC y envío al plotter
  for (int i = 0; i < numSamples; i++) {
    dacWrite(dacPin, sineWave[i]); // Escribe el valor en el DAC
    Serial.println(sineWave[i]);  // Enviar el valor al Plotter Serial
    delayMicroseconds(delayMicros); // Espera el tiempo necesario
  }
}
