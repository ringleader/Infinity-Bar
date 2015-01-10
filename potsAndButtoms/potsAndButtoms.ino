#include <Adafruit_NeoPixel.h>
#include <math.h>

#define PIN 6
#define REDPIN 4
#define GRNPIN 3
#define BLUPIN 5
#define BUTTON_PIN 3
#define PIXEL_COUNT 256

uint8_t red, grn, blu, showType = 0;
bool oldState = HIGH;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  
  red = analogRead(REDPIN)/4;
  grn = analogRead(GRNPIN)/4;
  blu = analogRead(BLUPIN)/4;
  
  //if(analogRead(REDPIN) > 0 || analogRead(BLUPIN) > 0 || analogRead(GRNPIN) > 0){
    // Read and display RGB values 
    colorSet(strip.Color(grn, red, blu));
  ///*
  //}else{    
    bool newState = digitalRead(BUTTON_PIN);
    // Check if state changed from high to low (button press).
    if (newState == HIGH && oldState == LOW) {
      // Short delay to debounce button.
      delay(20);
      // Check if button is still low after debounce.
      newState = digitalRead(BUTTON_PIN);
      if (newState == HIGH) {
        showType++;
        if (showType > 9)
          showType=0;
          startShow(showType);
        }
    }
  //}
  //*/
}


void startShow(int i) {
  switch(i){
    case 0: nightRide(25, 2);
    break;
    case 1: ignite(45, strip.Color(random(255),random(255),random(255)));
    break;
    case 2: breathe(1, strip.Color(random(255),random(255),random(255)), 1);
    break;
    case 3: rainbow(20);
    break;
    case 4: circus(20,1);
    break;
    case 5: colorWipe(strip.Color(random(255),random(255),random(255)), 25);
    break;
    case 6: theaterChase(strip.Color(random(255),random(255),random(255)), 50); 
    break;
    case 7: colorFadeAndReverse(20, strip.Color(random(255),random(255),random(255)));
    break;
    case 8: rainbowCycle(20);
    break;
    case 9: theaterChaseRainbow(50);
    break;
  }
}

void nightRide(uint8_t wait, uint8_t cycles) {
  int pos = 75, dir = 1, pos2 = 50;
  for(int k=0;k <=105*cycles;k++){ 
    int j;
 
    // Draw 5 pixels centered on pos. setPixelColor() will clip any
    // pixels off the ends of the strip, we don't need to watch for that.
    strip.setPixelColor(pos - 2, 0x100000); // Dark red
    strip.setPixelColor(pos - 1, 0x800000); // Medium red
    strip.setPixelColor(pos , 0xFF3000); // Center pixel is brightest
    strip.setPixelColor(pos + 1, 0x800000); // Medium red
    strip.setPixelColor(pos + 2, 0x100000); // Dark red
     
     
    
    strip.setPixelColor(pos2 - 2, 0x100000); // Dark red
    strip.setPixelColor(pos2 - 1, 0x800000); // Medium red
    strip.setPixelColor(pos2 , 0xFF3000); // Center pixel is brightest
    strip.setPixelColor(pos2 + 1, 0x800000); // Medium red
    strip.setPixelColor(pos2 + 2, 0x100000); // Dark red
    
    strip.show();
    delay(30);
     
    // Rather than being sneaky and erasing just the tail pixel,
    // it's easier to erase it all and draw a new one next time.
    for(j=-2; j<= 2; j++) {strip.setPixelColor(pos+j, 0);strip.setPixelColor(pos2+j, 0);}
     
    // Bounce off ends of strip
    pos += dir;
    pos2-= dir;
    
    if(pos == 128&&dir==1) {
      pos2 = 255;
    }else if(pos == 128){
      pos2 =-1;
    }

    if(pos < 75) {
      pos = 76;
      dir = -dir;
    } else if(pos >= 181) {
      pos = 181 - 2;
      dir = -dir;
    }
  }
}    


void ignite(uint8_t wait, uint32_t color){
  int rightTop=65,
  rightBottom=64,
  leftTop=192,
  leftBottom=193;
  strip.setBrightness(255);
  for (int i=0; i < 64 ; i++) {
    strip.setPixelColor(leftTop, color);
    strip.setPixelColor(leftBottom, color);
    strip.setPixelColor(rightTop, color);
    strip.setPixelColor(rightBottom, color);
    rightBottom--;
    leftBottom++;
    leftTop--;
    rightTop++;    
    strip.show();
    delay(wait);
    
  }
}

// input wait, color, and number of cycles and board will fade intensity up and down 
void breathe(uint8_t wait, uint32_t c, uint8_t cycles){
  for(int k=0;k<=cycles;k++){
    for(int j=220; j>25; j= j-5) {
        strip.setBrightness(j);
    for (int i = strip.numPixels(); i >= 0; i++) {
      strip.setPixelColor(i, c);    //turn every third pixel on
    }
   
  strip.show();
  delay(wait);
  }
  for(int j=25; j<220; j=j+5) {
        strip.setBrightness(j);
    for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);    //turn every third pixel on
    }
   
  strip.show();
  delay(wait);
  }
 }
}
void colorFadeAndReverse(uint8_t wait, uint32_t c){
  for(int j=25; j<220; j=j+3) {
        strip.setBrightness(j);
    for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);    //turn every third pixel on
    }
   
  strip.show();
  delay(wait);
  }
  for(int j=255; j>0; j= j-2) {
        strip.setBrightness(j);
    for (int i = strip.numPixels(); i >= 0; i++) {
      strip.setPixelColor(i, c);    //turn every third pixel on
    }
   
  strip.show();
  delay(wait);
  }
}
void colorFade(uint8_t wait, uint32_t c){
  for(int j=0; j<255; j++) {
        strip.setBrightness(j);
    for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);    //turn every third pixel on
    }
   
  strip.show();
  delay(wait);
  }
}

void circus(uint8_t wait, uint8_t cycles){
  int highPix = strip.numPixels();
  int totalPix = strip.numPixels();
  for(int j=0; j<=cycles; j++){ 
  strip.setBrightness(255);
  for (int i=0; i < totalPix ; i++) {
    strip.setPixelColor(i, random(255),random(255),random(255));    //turn every third pixel on
    if(i==totalPix/2){
      i == 0;
    }
    strip.setPixelColor(highPix, random(255),random(255),random(255));
    highPix--;
    strip.show();
    delay(wait);
    if(highPix == 0){
       highPix = strip.numPixels();
    }
    }
  }
}
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void colorSet(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
  }
  strip.show();
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

