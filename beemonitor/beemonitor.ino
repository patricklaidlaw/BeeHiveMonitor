// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306.h"


/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code,
please support Adafruit and open-source hardware by purchasing
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/


#define OLED_I2C_ADDRESS 0x3C
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {

    //MessageBoard m = new MessageBoard();

    Serial.begin(9600);
    Serial.println("Trying TinyMessage");

    // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS);  // initialize with the I2C addr  (for the 128x64)
    // init done
    display.clearDisplay();   // clears the screen and buffer
    display.display();

    // cloud functions: up to 4 per device.
    // function name: up to 12 characters
    Spark.function("SetReminder", SetReminder);
    Spark.function("SetWeather", SetWeather);
    Spark.function("SetSurf", SetSurf);
    Spark.function("Clear", Clear);
}


void loop() {
  Serial.println("Trying TinyMessage");

}

void showMsg(int n, String message) {

    int fontSize = (message.length() > 10 ? 1 : 2);

    display.setTextSize(fontSize);   // 1 = 8 pixel tall, 2 = 16 pixel tall...
    display.setTextColor(WHITE);
    display.setCursor(0, n*16);
    display.println(message);
    display.display();
}

void ClearMessage(int n) {
    display.fillRect(0, 16*n, 128, 16, 0);
    display.display();
}

int SetReminder(String message) {
    ClearMessage(0);
    if (message.length() > 0)
        showMsg(0, message);
}

int SetWeather(String message) {
    ClearMessage(2);
    if (message.length() > 0)
        showMsg(2, message);
}

int SetSurf(String message) {
    ClearMessage(3);
    if (message.length() > 0)
        showMsg(3, message);
}

int Clear(String message) {
    display.clearDisplay();
    display.display();
}
