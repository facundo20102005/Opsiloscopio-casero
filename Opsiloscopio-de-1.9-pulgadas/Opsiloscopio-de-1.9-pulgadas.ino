#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// Configuración de los pines para el ST7789
#define TFT_CS     15  // Pin CS
#define TFT_DC     2   // Pin DC
#define TFT_RST    4   // Pin RST
#define TFT_SCLK   18  // Pin del reloj SPI
#define TFT_MOSI   23  // Pin de datos SPI
#define TFT_BLK    32  // Pin para retroiluminación

// Pin para entrada de señal senoidal
#define SIGNAL_PIN 34  // GPIO para leer la señal (ADC1_CH6)

// Inicialización de la pantalla TFT
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Definición de colores
#define BLACK   0x0000
#define BLUE    0x001F
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF

// Resolución de la pantalla
#define SCREEN_WIDTH  320  // Ancho según rotación
#define SCREEN_HEIGHT 170  // Altura según rotación

// Parámetros del osciloscopio
#define AMPLITUDE     150   // Ajusta la amplitud máxima de la señal
#define UPDATE_DELAY  1    // Velocidad de actualización (ms)

// Buffer para almacenar la señal
int signalBuffer[SCREEN_WIDTH];
int x = 0;            // Índice actual en la pantalla
int prevY = SCREEN_HEIGHT / 2;  // Posición Y anterior

void setup() {
  // Inicialización de la pantalla
  tft.init(170, 320);   // Tamaño físico de la pantalla
  tft.setRotation(1);   // Orientación horizontal (ajustada para 1.9'')

  // Configurar el pin de entrada como analógico
  analogReadResolution(12);  // Resolución ADC de 12 bits
  analogSetAttenuation(ADC_11db);  // Rango de 0-3.3V
  
  // Dibujar marco y ejes
  tft.fillScreen(BLACK);
  drawFrame();
  drawGrid();
}

void loop() {
  // Leer la señal analógica del pin
  int signalValue = analogRead(SIGNAL_PIN);

  // Mapear el valor para ajustarlo a la altura de la pantalla con mayor amplitud
  int y = map(signalValue, 0, 4095, SCREEN_HEIGHT - AMPLITUDE, AMPLITUDE);

  // Limpiar el área completa cuando se reinicie
  if (x == 0) {
    tft.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK);
    drawFrame();
    drawGrid();
  }

  // Dibujar la nueva línea de la señal
  tft.drawLine(x == 0 ? SCREEN_WIDTH - 1 : x - 1, prevY, x, y, CYAN);

  // Guardar el valor en el buffer
  signalBuffer[x] = y;
  prevY = y;

  // Avanzar al siguiente píxel
  x++;
  if (x >= SCREEN_WIDTH) {
    x = 0;  // Reiniciar al inicio
  }

  delay(UPDATE_DELAY);
}

// Función para dibujar el marco
void drawFrame() {
  tft.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RED); // Borde rojo
  tft.setTextColor(WHITE);
  tft.setCursor(5, 5);
  tft.setTextSize(1);
  tft.println("Osciloscopio - ESP32");
}

// Función para dibujar la grilla
void drawGrid() {
  tft.setTextColor(GREEN);

  // Dibujar líneas verticales de la grilla
  for (int i = 0; i < SCREEN_WIDTH; i += 20) {
    tft.drawFastVLine(i, 0, SCREEN_HEIGHT, BLUE);  // Líneas verticales
  }

  // Dibujar líneas horizontales de la grilla
  for (int i = 0; i < SCREEN_HEIGHT; i += 20) {
    tft.drawFastHLine(0, i, SCREEN_WIDTH, BLUE);  // Líneas horizontales
  }
}
