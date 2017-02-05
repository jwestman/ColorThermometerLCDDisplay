#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
/*
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD (E)nable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC/VDD pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/

/*
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
*/
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
#define PIN            7

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      3

// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ400);

const float milliVoltTempChange = 9.31; // at 5.0 volt

//TMP36 Pin Variable
int temperaturePin = 1; // Analog1

const byte blueToRed[318][3] = {
  {0,0,0}, {16,16,16}, {0,0,132}, {0,0,131}, {0,0,139}, {0,0,146}, {0,0,145}, {0,0,148}, {0,0,159}, {0,0,158}, {0,0,170}, {0,0,173}, {0,0,172}, {0,0,177}, {0,0,182}, {0,0,181}, 
  {0,0,184}, {0,0,195}, {0,0,196}, {0,0,195}, {0,0,205}, {0,0,209}, {0,0,208}, {0,0,214}, {0,0,223}, {0,0,222}, {0,0,223}, {0,0,235}, {0,0,237}, {0,0,236}, {0,0,245}, {0,0,250}, 
  {0,0,249}, {0,0,251}, {0,0,255}, {0,3,255}, {0,4,255}, {0,3,255}, {0,8,255}, {0,13,255}, {0,12,255}, {0,14,255}, {0,24,255}, {0,25,255}, {0,24,255}, {0,33,255}, {0,36,255}, 
  {0,35,255}, {0,41,255}, {0,49,255}, {0,48,255}, {0,50,255}, {0,60,255}, {0,61,255}, {0,60,255}, {0,69,255}, {0,73,255}, {0,72,255}, {0,76,255}, {0,84,255}, {0,83,255}, 
  {0,84,255}, {0,95,255}, {0,97,255}, {0,96,255}, {0,101,255}, {0,105,255}, {0,104,255}, {0,107,255}, {0,115,255}, {0,116,255}, {0,126,255}, {0,129,255}, {0,128,255}, 
  {0,134,255}, {0,141,255}, {0,140,255}, {0,142,255}, {0,152,255}, {0,153,255}, {0,152,255}, {0,161,255}, {0,165,255}, {0,164,255}, {0,169,255}, {0,177,255}, {0,176,255}, 
  {0,177,255}, {0,188,255}, {0,189,255}, {0,188,255}, {0,193,255}, {0,197,255}, {0,196,255}, {0,199,255}, {0,209,255}, {0,209,254}, {0,219,254}, {0,221,253}, {0,221,254}, 
  {0,220,254}, {0,226,249}, {2,232,243}, {2,232,244}, {1,231,244}, {3,234,242}, {11,244,235}, {12,245,235}, {11,244,235}, {19,252,227}, {22,255,224}, {22,255,225}, {21,255,225}, 
  {25,255,221}, {31,255,214}, {31,255,215}, {30,255,215}, {32,255,214}, {40,255,206}, {41,255,205}, {41,255,206}, {40,255,206}, {45,255,201}, {48,255,198}, {48,255,199}, {47,255,199}, 
  {50,255,196}, {57,255,189}, {57,255,190}, {56,255,190}, {57,255,189}, {66,255,180}, {67,255,179}, {67,255,180}, {66,255,180}, {72,255,174}, {77,255,169}, {77,255,170}, {76,255,170},
  {78,255,167}, {86,255,159}, {86,255,160}, {85,255,160}, {93,255,152}, {96,255,150}, {96,255,151}, {95,255,151}, {100,255,146}, {106,255,140}, {106,255,141}, {105,255,141},
  {107,255,139}, {114,255,131}, {115,255,131}, {114,255,131}, {122,255,124}, {125,255,121}, {125,255,122}, {124,255,122}, {127,255,119}, {131,255,114}, {131,255,115}, {130,255,115},
  {131,255,114}, {140,255,106}, {141,255,105}, {141,255,106}, {140,255,106}, {147,255,99}, {151,255,95}, {151,255,96}, {150,255,96}, {153,255,93}, {160,255,85}, {160,255,86}, 
  {160,255,85}, {168,255,78}, {170,255,76}, {170,255,77}, {169,255,77}, {175,255,71}, {180,255,66}, {180,255,67}, {179,255,67}, {181,255,65}, {189,255,57}, {190,255,57}, {189,255,57},
  {196,255,50}, {199,255,47}, {199,255,48}, {198,255,48}, {201,255,45}, {206,255,40}, {206,255,41}, {205,255,41}, {206,255,40}, {214,255,31}, {215,255,30}, {215,255,31}, {214,255,31}, 
  {221,255,24}, {225,255,21}, {225,255,22}, {224,255,22}, {227,255,19}, {235,255,11}, {235,255,12}, {234,255,12}, {235,254,11}, {242,248,3}, {244,247,1}, {244,248,2}, {243,248,2}, 
  {249,241,0}, {254,236,0}, {254,237,0}, {253,237,0}, {254,234,0}, {254,226,0}, {255,226,0}, {255,225,0}, {255,217,0}, {255,214,0}, {255,215,0}, {255,211,0}, {255,207,0}, {255,208,0}, 
  {255,206,0}, {255,196,0}, {255,195,0}, {255,196,0}, {255,187,0}, {255,184,0}, {255,185,0}, {255,181,0}, {255,173,0}, {255,174,0}, {255,173,0}, {255,163,0}, {255,162,0}, {255,163,0},
  {255,155,0}, {255,151,0}, {255,152,0}, {255,149,0}, {255,140,0}, {255,141,0}, {255,140,0}, {255,131,0}, {255,129,0}, {255,130,0}, {255,126,0}, {255,121,0}, {255,122,0}, {255,120,0},
  {255,111,0}, {255,102,0}, {255,99,0}, {255,100,0}, {255,95,0}, {255,88,0}, {255,89,0}, {255,87,0}, {255,78,0}, {255,77,0}, {255,78,0}, {255,71,0}, {255,66,0}, {255,67,0}, {255,63,0},
  {255,55,0}, {255,56,0}, {255,55,0}, {255,46,0}, {255,44,0}, {255,45,0}, {255,40,0}, {255,36,0}, {255,37,0}, {255,34,0}, {255,25,0}, {255,26,0}, {251,17,0}, {249,14,0}, {250,15,0}, 
  {244,10,0}, {236,3,0}, {237,4,0}, {235,3,0}, {223,0,0}, {213,0,0}, {208,0,0}, {209,0,0}, {204,0,0}, {195,0,0}, {196,0,0}, {195,0,0}, {183,0,0}, {181,0,0}, {182,0,0}, {176,0,0}, 
  {172,0,0}, {173,0,0}, {169,0,0}, {158,0,0}, {159,0,0}, {158,0,0}, {148,0,0}, {145,0,0}, {146,0,0}, {138,0,0}, {131,0,0}, {132,0,0}, {127,0,0}, {83,0,0}, {240,240,240}, {255,255,255}
};

void setup() {
  Serial.begin(9600);
  analogReference(INTERNAL);
  lcd.begin(16, 2);

  lcd.setCursor(0,0); //col row
  lcd.print("Init Colors");

  pixels.begin(); // Init NeoPixel library.
  
  showColors();  
}

void loop() {
  /*
  temperatureReading = analogRead(temperaturePin); // 0-1023
  Serial.println(temperatureReading);
  float voltage = (temperatureReading * aref_voltage) / 1024; // 0-3300mv
  float tempC = (voltage - 0.5) * 100; // convert from 10mv per degreeC with 500mv offset
  float tempF = (tempC * 9.0 / 5.0) + 32;
  Serial.println(String(tempF));
  delay(1000);
  */

  int temperatureReading = analogRead(temperaturePin); // 0-1023
  //Serial.println(temperatureReading);
  float tempC = temperatureReading / milliVoltTempChange;
  float tempF = tempC * 1.8 + 32;

  // 318  == 0-317  colors
  // 1024 == 0-1023 readings
  //  reading: 512 / 1024 = 0.5 * 318 == 159
  float fbrIdx = temperatureReading / 1024.0 * 318.0;
  int brIdx = (int)fbrIdx;

  // 40F to 90F
  if (tempF <= 40) {
    brIdx = 1;
  } else if (tempF >= 90) {
    brIdx = 317;
  } else {
    fbrIdx = tempF - 40.0;
    brIdx = (fbrIdx / 50.0) * 316; 
  }
  
  // bound to 0-317
  brIdx = min(brIdx, 317);
  brIdx = max(0, brIdx);
  
  for (byte pix=0; pix<3; pix++) {
    pixels.setPixelColor(pix, pixels.Color(blueToRed[brIdx][0], blueToRed[brIdx][1], blueToRed[brIdx][2]));
  }
  pixels.show();

  String fTempStr = String(tempF, 1) + " f"; //76.5 f
  String cTempStr = String(tempC, 1) + " c"; //24.7 c

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(fTempStr);
  lcd.setCursor(0, 1);
  lcd.print(cTempStr);
    
  delay(2000);
}

void showColors() {
  int numColors = sizeof(blueToRed) / sizeof(blueToRed[0]); // size of colors div size of first child
  
  for (int brIdx=0; brIdx<numColors; brIdx++) {
    lcd.setCursor(2,1); //col row
    lcd.print("Index= " + String(brIdx));

    for (byte pix=0; pix<3; pix++) {
      pixels.setPixelColor(pix, pixels.Color(blueToRed[brIdx][0], blueToRed[brIdx][1], blueToRed[brIdx][2]));
    }
    pixels.show();
    delay(20);
  }  
}

