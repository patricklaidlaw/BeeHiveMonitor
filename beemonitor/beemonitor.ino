// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306.h"
#include "HX711ADC.h"
#include "DS18.h"
#include <math.h>

/*********************************************************************
This Bee Monitor uses a Monochrome OLEDs based on SSD1306 drivers for Initial Testing.
You can pick it up from  ------> http://www.adafruit.com/category/63_98
You can also pick one up from -----> Amazon

This uses 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Load Cell: https://www.amazon.com/gp/product/B071ZYYJHJ/ref=oh_aui_detailpage_o01_s00?ie=UTF8&psc=1
Reliable HX711: https://www.amazon.com/gp/product/B01D1FECVI/ref=oh_aui_detailpage_o06_s00?ie=UTF8&psc=1
OLEDs: https://www.amazon.com/Diymall-Yellow-Serial-Arduino-Display/dp/B00O2LLT30/ref=sr_1_3?s=industrial&ie=UTF8&qid=1501700218&sr=1-3&keywords=oled+i2c
OLEDs: https://www.amazon.com/HiLetgo-Serial-128X64-Display-Color/dp/B06XRBTBTB/ref=sr_1_6?s=industrial&ie=UTF8&qid=1501700218&sr=1-6&keywords=oled+i2c

Photon Wireing Guide
D0 -->OLED SDA
D1 -->OLED SCL
D4 -->OLDED Reset not actually used
D2 -->DS18B20 Yellow wire
A0 -->HX711 SCK
A1 -->HX711 DT

VCC to 3.3v Pin on Photon
GND to the GND on Photon

We Utilize the HX711ADC Drivers by some dude i don't remember his name might be by Adafruit

*********************************************************************/
// HX711.DOUT	- pin #A1
// HX711.PD_SCK	- pin #A0

HX711ADC scale(A1, A0);		// parameter "gain" is ommited; the default value 128 is used by the library

#define OLED_I2C_ADDRESS 0x3C
#define OLED_RESET D4
Adafruit_SSD1306 display(OLED_RESET);


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

/********************************
Notes
********************************/

/********************************
OneWire & DS18B20 temp sesnor setup
********************************/
DS18 sensor(D2);



void setup()   {
/********************************
OneWire & DS18B20 temp sesnor setup
********************************/
    Time.zone(-7);
    //MessageBoard m = new MessageBoard();
    Serial.begin(38400);

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

    SetReminder("Testing");
    //**************
    //HX711 setup
    //**************
    delay(100);
    SetReminder("Setup HX711");
    showMsg(1," reading HX711");
    delay(150);
    showMsg(2," Reading Scale");

    delay(100);
    showMsg(3," calibrating");
    delay(100);

    display.println(scale.read());			// print a raw reading from the ADC

    display.print("read average: \t\t");
    display.println(scale.read_average(20));  	// print the average of 20 readings from the ADC

    display.print("get value: \t\t");
    display.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight (not set yet)

    display.print("get units: \t\t");
    display.println(scale.get_units(5), 1);	// print the average of 5 readings from the ADC minus tare weight (not set) divided
  						// by the SCALE parameter (not set yet)
//106741.044776119402985
//11149
//-10250 = 13.7
//-10350
//-10450 = 13.4
//-10550 = 13.3
//-10650 = 13.2
//-10750 = 13.1
//-10
    scale.set_scale(-10000.f);                      // 2240.f this value is obtained by calibrating the scale with known weights; see the README for details
    //scale.set_scale();
    //  scale.tare();				        // reset the scale to 0

}


void loop() {
//Temp Section
// Read the next available 1-Wire temperature sensor
if (sensor.read()) {
  // Do something cool with the temperature
  String displayTemp = String("Temp %.2f F ", sensor.fahrenheit());
  SetReminder("CurrentTemp");
  ClearMessage(1);
  showMsg(1, displayTemp);
  dotTimer(1000);
  Clear("all");
  display.printf("Temperature %.2f C %.2f F ", sensor.celsius(), sensor.fahrenheit());
  display.printf(displayTemp);
  Serial.printf("Temperature %.2f C %.2f F ", sensor.celsius(), sensor.fahrenheit());
  Particle.publish("temperature", String(sensor.fahrenheit()), PRIVATE);

  // Additional info useful while debugging
  printDebugInfo();

// If sensor.read() didn't return true you can try again later
// This next block helps debug what's wrong.
// It's not needed for the sensor to work properly
} else {
  // Once all sensors have been read you'll get searchDone() == true
  // Next time read() is called the first sensor is read again
  if (sensor.searchDone()) {
    Serial.println("No more addresses.");
    // Avoid excessive printing when no sensors are connected
    delay(250);

  // Something went wrong
  } else {
    printDebugInfo();
  }
}
Serial.println();

dotTimer(3500);

//String scaleaverage = scale.read_average(20);
  SetReminder("Current Weight");
  ClearMessage(1);
  ClearMessage(2);
  ClearMessage(3);
  ClearMessage(3);

//int AverageWeight = scale.read_average(10);
//String MyreadAverage = String(AverageWeight);
String MyreadAverage = String(scale.read_average(10));

showMsg(1, MyreadAverage);
display.println(scale.get_units(10), 1);

scale.power_down();
dotTimer(2500);
scale.power_up();
  delay(1000);

analogRead(A4);


}
void dotTimer(int myDelay){
String myDots;
int myCounter = 9;
myDelay = myDelay/myCounter;

ClearMessage(3);
while(myCounter > 0){
  myDots += "\t";
  showMsg(3,myDots);
  myCounter--;

  delay(myDelay);
  }
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

/****************
Hive Temp reading functions
*****************/
void printDebugInfo() {
  // If there's an electrical error on the 1-Wire bus you'll get a CRC error
  // Just ignore the temperature measurement and try again
  if (sensor.crcError()) {
    Serial.print("CRC Error ");
  }

  // Print the sensor type
  const char *type;
  switch(sensor.type()) {
    case WIRE_DS1820: type = "DS1820"; break;
    case WIRE_DS18B20: type = "DS18B20"; break;
    case WIRE_DS1822: type = "DS1822"; break;
    case WIRE_DS2438: type = "DS2438"; break;
    default: type = "UNKNOWN"; break;
  }
  Serial.print(type);

  // Print the ROM (sensor type and unique ID)
  uint8_t addr[8];
  sensor.addr(addr);
  Serial.printf(
    " ROM=%02X%02X%02X%02X%02X%02X%02X%02X",
    addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7]
  );

  // Print the raw sensor data
  uint8_t data[9];
  sensor.data(data);
  Serial.printf(
    " data=%02X%02X%02X%02X%02X%02X%02X%02X%02X",
    data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8]
  );
}
