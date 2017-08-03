// This #include statement was automatically added by the Particle IDE.
#include "Adafruit_SSD1306.h"
#include "HX711ADC.h"

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

void setup()   {

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
//-10.250 = 13.7
//-10450 = 13.4
//-10550 = 13.3
//-10650 = 13.2
//-10750 = 13.1
//-10
    scale.set_scale(-10350.f);                      // 2240.f this value is obtained by calibrating the scale with known weights; see the README for details
    //scale.set_scale();
    //  scale.tare();				        // reset the scale to 0

    Serial.println("After setting up the scale:");

//    display.print("read: \t\t");
//    display.println(scale.read());                 // print a raw reading from the ADC

//    display.print("read average: \t\t");
//    display.println(scale.read_average(20));       // print the average of 20 readings from the ADC

//    display.print("get value: \t\t");
//    display.println(scale.get_value(5));		// print the average of 5 readings from the ADC minus the tare weight, set with tare()

//    display.print("get units: \t\t");
//    display.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
  						// by the SCALE parameter set with set_scale

//    Serial.println("Readings:");

}


void loop() {
//String scaleaverage = scale.read_average(20);


  SetReminder("Running");
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

String Mydots;
int Mycounter = 1;

ClearMessage(3);
while(Mycounter <= 9){
  Mydots += "\t";
  showMsg(3,Mydots);
  Mycounter++;
  delay(75);
  }
/*  showMsg(3,"\t");
  delay(50);
  showMsg(3,"\t\t");
  delay(50);
  showMsg(3,"\t\t\t");
  delay(50);
  showMsg(3,"\t\t\t\t");
  delay(50);
  showMsg(3,"\t\t\t\t\t");
  delay(50);
  showMsg(3,"\t\t\t\t\t\t");
  delay(50);
  showMsg(3,"\t\t\t\t\t\t\t");
  delay(50);
  showMsg(3,"\t\t\t\t\t\t\t\t");
  delay(50);
  showMsg(3,"\t\t\t\t\t\t\t\t\t");
  */
scale.power_up();
  delay(1000);
/*
String Mydots;
int Mycounter = 1;
ClearMessage(3);
while(Mycounter <= 9){
  Mydots += "\t";
  showMsg(3,Mydots);
  Mycounter++;
  delay(500);
  }
*/
analogRead(A4);


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
