#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

#define PIN 4
#define SOFT_RX 3
#define SOFT_TX 2
#define COLOR_EEPROM_ADDR 0 // Адрес в EEPROM для хранения значений RGB
#define BRIGHTNESS_EEPROM_ADDR 1 // Адрес в EEPROM для хранения значения яркости

#define NUMPIXELS 33

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial mySerial(SOFT_RX, SOFT_TX);

struct RGB {
    int r;
    int g;
    int b;
};

void setup() {
    Serial.begin(9600);
    while (!Serial) {
        ; // ждем подключения последовательного порта. Необходимо только для нативного USB-порта
    }
    mySerial.begin(115200);
    pixels.begin();
    
    // Считываем значение цвета из EEPROM
    int def_color = EEPROM.read(COLOR_EEPROM_ADDR);
    
    pixels.setBrightness(25);

    RGB color = getColor(def_color);
    lightUpPixels(color);
}

void loop() {
    while (mySerial.available()) {
        int inByte = mySerial.read();
        Serial.write(inByte);
        RGB color = getColor(inByte);
        lightUpPixels(color);
        EEPROM.write(COLOR_EEPROM_ADDR, inByte); // Сохраняем значение в EEPROM
    }
}

RGB getColor(int inByte) {
    RGB color;
    switch (inByte) {
        // Красный
        case '1':
            color.r = 255;
            color.g = 0;
            color.b = 0;
            break;
        // Оранжевый
        case '2':
            color.r = 255;
            color.g = 85;
            color.b = 0;
            break;
        // Желтый
        case '3':
            color.r = 255;
            color.g = 125;
            color.b = 0;
            break;
        // Зеленый
        case '4':
            color.r = 0;
            color.g = 255;
            color.b = 0;
            break;
        // Голубой
        case '5':
            color.r = 0;
            color.g = 255;
            color.b = 255;
            break;
        // Синий
        case '6':
            color.r = 0;
            color.g = 0;
            color.b = 255;
            break;
        // Фиолетовый
        case '7':
            color.r = 138;
            color.g = 43;
            color.b = 226;
            break;
        // Белый
        case '8':
            color.r = 255;
            color.g = 255;
            color.b = 255;
            break;
        // Черный
        case '9':
            color.r = 0;
            color.g = 0;
            color.b = 0;
            break;
        default:
            color.r = 255;
            color.g = 255;
            color.b = 255;
            break;
    }
    return color;
}

void lightUpPixels(RGB color) {
    int middle = NUMPIXELS / 2;
    for (int i = 0, j = NUMPIXELS - 1; i < middle; i++, j--) {
        pixels.setPixelColor(i, pixels.Color(color.r, color.g, color.b));
        pixels.setPixelColor(j, pixels.Color(color.r, color.g, color.b));
        delay(50);
        pixels.show();
    }
    // Если NUMPIXELS нечетное число, то зажигаем средний светодиод
    if (NUMPIXELS % 2 == 1) {
        pixels.setPixelColor(middle, pixels.Color(color.r, color.g, color.b));
        delay(50);
        pixels.show();
    }
}
