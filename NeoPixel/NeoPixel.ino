#include <Adafruit_NeoPixel.h>

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);

uint32_t OFF = strip.Color(0,0,0);
uint32_t RED = strip.Color(255, 0, 0);
uint32_t CURRENT_COLOR = strip.Color(255,255,255);

float potMax = 1000.0;
float potMin = 20.0;

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show();

  pinMode(A0, INPUT);

  pinMode(13, INPUT);
  digitalWrite(13, LOW);
  pinMode(12, INPUT);
  digitalWrite(12, LOW);
  pinMode(11, INPUT);
  digitalWrite(11, LOW);
  pinMode(10, INPUT);
  digitalWrite(10, LOW);
}

boolean alternateChecker;
int checkerDelay;
int pingCount = 0;
boolean pingUp = true;
boolean pingDown = false;
int pingDelay;

void loop() {
  
  // analog input from proto bit
  float protoBitReading = analogRead(A0);
  int maxRead = 1000;
  int minRead = 20;
  float protoBit = (protoBitReading > maxRead) ? maxRead : protoBitReading;
  protoBit = (protoBit < minRead) ? minRead : protoBit;
  protoBit = (int) (((protoBit - minRead) / (maxRead - minRead)) * 255.0);
  
  // modes from the 4-pole switch
  boolean ping = true;//(digitalRead(13) == HIGH) ? true : false;  
  boolean color = false;//(digitalRead(12) == HIGH ) ? true : false;
  boolean checker = false;//(digitalRead(11) == HIGH) ? true : false;
  boolean brightness = false;//(digitalRead(10) == HIGH) ? true : false;

  if (ping) {
    Serial.println("Ping:");
       
    int pingRemainder = pingCount % strip.numPixels();
    int numToTurnOn = 0;
    if (pingUp) {
      numToTurnOn = pingRemainder;
    } else if (pingDown) {
      numToTurnOn = strip.numPixels() - pingRemainder;
    }
    
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      if (i == numToTurnOn) {
        strip.setPixelColor(i, CURRENT_COLOR);
      } else {
        strip.setPixelColor(i, OFF);
      }
    }
    
    int maxPingDelay = 1000 / strip.numPixels();
    int minPingDelay = 50 / strip.numPixels();
    
    pingDelay = ((protoBit / 255.0) * (maxPingDelay - minPingDelay)) + (minPingDelay / strip.numPixels());
    delay(pingDelay);
    strip.show();
    pingCount++;

    if (pingRemainder == 7) {
      pingUp = !pingUp;
      pingDown = !pingDown;
    }

  } else if (color) {
    
    Serial.println("Color Spectrum:");
    
    byte WheelPos = protoBit;
    int r = 0;
    int g = 0;
    int b = 0;
    
    if(WheelPos < 85) {
      r = WheelPos * 3;
      g = 255 - WheelPos * 3;
      b = 0;
    } else if(WheelPos < 170) {
      WheelPos -= 85;
      r = 255 - WheelPos * 3;
      g = 0;
      b = WheelPos * 3;
    } else {
     WheelPos -= 170;
     r = 0;
     g = WheelPos * 3;
     b = 255 - WheelPos * 3;
    }
    CURRENT_COLOR = strip.Color(r,g,b);
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, CURRENT_COLOR);
    }
    strip.show();
  
  // adjust brightness
  } else if (checker) {

    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      int modResult = alternateChecker ? 1 : 0;
      if (i % 2 == modResult) {
        strip.setPixelColor(i, CURRENT_COLOR);        
      } else {
        strip.setPixelColor(i, OFF);
      }
    }
   
    int delayMax = 700;
    int delayMin = 50;
    alternateChecker = !alternateChecker;
    checkerDelay = ((protoBit / 255.0) * (delayMax - delayMin)) + delayMin;
    delay(checkerDelay);
    Serial.println(checkerDelay);
    strip.show();

  } else if (brightness) {
    Serial.println("Brightness:");
    
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor((strip.numPixels() - i), CURRENT_COLOR);
    }
    strip.setBrightness(protoBit);
    strip.show();
    
    // animated --> ping back and fourth at a specific frequency  
//    protoBit = (protoBit / 255.0) * 5.0;
//
//    // bar graph
//    for (uint16_t i = 0; i < strip.numPixels(); i++) {
//      float seg = 5.0 / 8.0;
//      if (protoBit < seg * i) {
//        strip.setPixelColor((strip.numPixels() - i), CURRENT_COLOR);
//      } else {
//        strip.setPixelColor((strip.numPixels() - i), OFF);
//     
//      }
//      delay(10);
//      strip.show();
//    }

  } else {
    Serial.println("Error -- check wiring.");
  }
  
}
