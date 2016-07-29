/*******************************
 * CrossFire Neopixels  + Greg *
 * me@zachcross.com            *
 * greg@pohodich.com           *
 ******************************/
 
 // V1.3
 
 /* CUSTOMIZABLE PARAMETERS */
 
 
 
 
#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define NUMSTARS 121 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    5
#define CLOCKPIN   3
Adafruit_DotStar strip = Adafruit_DotStar(
  NUMSTARS, DATAPIN, CLOCKPIN, DOTSTAR_GRB);
  
  
  
   // Neopixel LED Strip
   #define NumPixels 50                    // number of pixels in strip (255 MAX)
   #define DataOut 4   // pin number (most are valid)
   #define POT A0
   #define BG 1  //unsure right now
   #define MIDDLE_ADDED_INTENSITY 30
   
   #define PixelInfo NEO_GRB + NEO_KHZ800   // pixel type flags, add together as needed:
                                                //   NEO_RGB     Pixels are wired for RGB bitstream
                                                //   NEO_GRB     Pixels are wired for GRB bitstream
                                                //   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
                                                //   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
                                                
// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel. Avoid connecting
// on a live circuit...if you must, connect GND first.
                                                
   // Power Interlock (Jumper 5V supply pin (Vin) to specified ADC pin.)
   #define PowerInterlock A5              // ADC Pin
   #define PowerThreshold 950             // 0 - 1024
   #define PowerIndicatorBrightness 25    // 0 - 255
   
   // Show Selection / Menu Customization
   #define DefaultMenuSelection 0
   #define SelectionChangeWipeDelay 3
   
   // Random Number Generator Seed (This pin should be left floating.)
   #define FloatingPin A3
   
   // MSQGEQ7 - Pin Numbers Hard Wired on Spectrum Shield
   #define spectrumReset 5
   #define spectrumStrobe 4
   #define spectrumAnalogL A0    // Left Channel
   #define spectrumAnalogR A1    // Right Channel
   
   // Spectrum Function Settings
   #define maxBrightness  15
   #define MAX_OPTIONS 23 //The maximum number of NeoPixel options available besides 99
   
/*************************************************************************************************/
/***||||||||||||||||||||||||| No Editable Parameters Below This Line ||||||||||||||||||||||||||***/
/*************************************************************************************************/


// Global Variables
char menuSelection = DefaultMenuSelection;
unsigned long pixelSets = 0;

int color;
int center = 0;
int step = -1;
int maxSteps = 16;
float fadeRate = 1;
int diff;
int delayVal = 20;
char ver[] = {1.30};
int britemode = 0;
uint32_t cOff = strip.Color(0, 0, 0);
int lux  = 0;

//background color
uint32_t currentBg = random(256);
uint32_t nextBg = currentBg;

unsigned long previoustime;
unsigned long currentMillis;

char Pulseflag = 1;

void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
    clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
  #endif

  // NeoPixel Initialization
  strip.begin();
  strip.show();    // All pixels OFF
  
  // Random Number Generator Initialization
  randomSeed(analogRead(FloatingPin));    // Use Floating ADC Pin
  
  
}
void loop() {
  uint32_t randomColor = strip.Color(random(255),random(255),random(255));
  uint32_t shutDown = strip.Color(0,0,0);
  
  if(britemode){
    strip.setBrightness(255);
  }else{
    strip.setBrightness(20);
  }  
  

  if(analogRead(POT) < 20){
    EndToEndWipe(0); // shut down
    britemode = 0;
  }else if(analogRead(POT) >= 20 && analogRead(POT) < 70){  
    sparkler(20);
  }else if(analogRead(POT) >= 70 && analogRead(POT) < 110){  
    knightRider(3, 32, 8, randomColor, analogRead(POT)); // Cycles, Speed, Width, RGB Color
  }else if(analogRead(POT) >= 110 && analogRead(POT) < 150){
    builder(10, analogRead(POT));
  }else if(analogRead(POT) >= 150 && analogRead(POT) < 200){
    heartBeat();
  }else if(analogRead(POT) >= 200 && analogRead(POT) < 300){
    pulseWave(randomColor, 15, 0.80, 2, 20);
  }else if(analogRead(POT) >= 300 && analogRead(POT) < 350){   
    colorWipe(randomColor, delayVal); // randome color startup
  }else if(analogRead(POT) >= 350 && analogRead(POT) < 400){    
    rainbowStripper(20);
  }else if(analogRead(POT) >= 400 && analogRead(POT) < 450){    
    rainbowCycle(20);
  }else if(analogRead(POT) >= 450 && analogRead(POT) < 500){
    theaterChaseRainbow(40);
  }else if(analogRead(POT) >= 500 && analogRead(POT) < 600){
    Pulse(random(255), random(255),random(255),random(255),random(255),random(255));
  }else if(analogRead(POT) >= 600 && analogRead(POT) < 650){
    ripple();
  }else if(analogRead(POT) >= 650 && analogRead(POT) < 700){
    waver(random(255),random(255),5,10, analogRead(POT));
  }else if(analogRead(POT) >= 700 && analogRead(POT) < 800){
    theaterChase(randomColor, 50); // Random Theater
  }else if(analogRead(POT) >= 800 && analogRead(POT) < 850){
    RandomPixelFade();
  }else if(analogRead(POT) >= 850 && analogRead(POT) < 900){
    RandomWalk(50,30,analogRead(POT));
  }else if(analogRead(POT) >= 900 && analogRead(POT) < 950) {
    SetRandomPixelRGB();
    KillRandomPixel();
  }else{
    // go white  
    if (analogRead(POT) >= 950 && analogRead(POT) <  1000){
      strip.setBrightness(50);
    }else{
      strip.setBrightness(255);
      britemode = 1;
    }
    for(int i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(255,255,255));
    }
    strip.show();
 }
 
         
}

void knightRider(uint16_t cycles, uint16_t speed, uint8_t width, uint32_t color, int interrupt) {
  uint32_t old_val[strip.numPixels()]; // up to 256 lights!
  // Larson time baby!
  for(int i = 0; i < cycles; i++){
    if(interrupt-10 > analogRead(POT) ||  interrupt+10 < analogRead(POT)){
      break; 
    }
    for (int count = 1; count<strip.numPixels(); count++) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for(int x = count; x>0; x--) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        strip.setPixelColor(x-1, old_val[x-1]); 
      }
      strip.show();
      delay(speed);
    }
    for (int count = strip.numPixels()-1; count>=0; count--) {
      strip.setPixelColor(count, color);
      old_val[count] = color;
      for(int x = count; x<=strip.numPixels();x++) {
        old_val[x-1] = dimColor(old_val[x-1], width);
        strip.setPixelColor(x+1, old_val[x+1]);
      }
      strip.show();
      delay(speed);
    }
  }
}
uint32_t dimColor(uint32_t color, uint8_t width) {
   return (((color&0xFF0000)/width)&0xFF0000) + (((color&0x00FF00)/width)&0x00FF00) + (((color&0x0000FF)/width)&0x0000FF);
}


uint8_t permute_color(uint8_t start_color, uint8_t max_change) {
  uint8_t rand;

  rand = random(1, max_change);

  //always wander away from overflow/underflow
  if(rand > start_color) {
    return start_color + rand;  
  } else if(255 - rand > start_color) {
    return start_color - rand;
  }
  if(random(1, 2) == 2) {
    return start_color - rand;
  } 
  return start_color + rand;
}
uint8_t calculate_tween(uint8_t start_color, uint8_t end_color, uint8_t this_step, uint8_t tween_constant) {
  float tweenfactor;
  float difference;
  uint8_t change;

  if(this_step == 0) return start_color;
  tweenfactor = ((float) this_step / (float) tween_constant);

  if(end_color > start_color) {
    difference = (float) end_color - (float) start_color;
    change = (uint8_t) (start_color +  difference*tweenfactor);
  } else if(start_color > end_color) {
    difference = (float) start_color - (float) end_color;
    change = (uint8_t) (start_color - difference*tweenfactor);
  } 

  return change;
}
void RandomWalk(uint8_t max_change, uint8_t tween, int interrupt) {
  uint16_t i, j;
  
  uint8_t nextred[strip.numPixels()];
  uint8_t nextgreen[strip.numPixels()];
  uint8_t nextblue[strip.numPixels()];
  uint8_t lastred[strip.numPixels()];
  uint8_t lastgreen[strip.numPixels()];
  uint8_t lastblue[strip.numPixels()];
        
        
  //set colors separately from brightness
  for(j=0; ; j++) {
           if(interrupt-10 > analogRead(POT) ||  interrupt+10 < analogRead(POT)){
              break; 
            }

    if ((j % 5) == 0) {


        
    }
    if(britemode){  
                  lux = 255;
                  strip.setBrightness(lux);
                }else{
                  lux = 101;
                  strip.setBrightness(lux);
                }

    if((j % tween) == 0) {
      //choose a new color for each pixel
      for(i=0; i<strip.numPixels(); i++) { 
        if(j == 0 && lastred[i] == 0 && lastgreen[i] == 0 && lastblue[i] == 0) {
          //Serial.print("Randomizing initial value for pixel "); Serial.println(i);
          lastred[i] = random(0, 255);
          lastgreen[i] = random(0, 255);
          lastblue[i] = random(0, 255);
        } else {
          //Serial.print("Shifting old value into previous array for pixel "); Serial.println(i);
          lastred[i] = nextred[i];
          lastgreen[i] = nextgreen[i];
          lastblue[i] = nextblue[i];
        }
              
                                if(interrupt-10 > analogRead(POT) ||  interrupt+10 < analogRead(POT)){
                                  break; 
                                }


        nextred[i] = permute_color(lastred[i], max_change);
        nextgreen[i] = permute_color(lastgreen[i], max_change);
        nextblue[i] = permute_color(lastblue[i], max_change);

                                /* lights 7, 12 and 16 have power problems.  attempting to keep them illuminated and looking good */
                                if(i == 7 && lux > 200) nextred[i] = 0;
                                if(i == 12 && lux > 64) {
                                  nextblue[i] = 0;
                                  nextred[i] = 0;
                                  if(lux > 100) nextgreen[i] = 0;
                                }
                                if(i == 16 && lux > 64) {
                                  nextgreen[i] = 0;
                                  nextred[i] = 0;
                                  if(lux > 100) nextblue[i] = 0;
                                } 
                                
                                /* middle lights (3, 6, 7, 10) need more intensity to overcome the fact that they have more batting */
                                if (i == 3 || i == 6 || /* i == 7 ||*/ i == 10) {
                                   if(nextred[i] < 200) nextred[i] += MIDDLE_ADDED_INTENSITY;
                                   if(nextgreen[i] < 200) nextred[i] += MIDDLE_ADDED_INTENSITY;
                                   if(nextblue[i] < 200) nextred[i] += MIDDLE_ADDED_INTENSITY;
                                }
      } 
    }
    for(i=0; i < strip.numPixels(); i++) { 
      uint8_t redtween;
      uint8_t greentween;
      uint8_t bluetween;

      //take tween cycles to go between lastred and nextred, etc
      redtween = calculate_tween(lastred[i], nextred[i], j%tween, tween);
      greentween = calculate_tween(lastgreen[i], nextgreen[i], j%tween, tween);
      bluetween = calculate_tween(lastblue[i], nextblue[i], j%tween, tween);

      strip.setPixelColor(i, redtween, greentween, bluetween);
    }
    strip.show();
    delay(0);
  }
}


void heartBeat() {  
  // all pixels show the same color:
  int redo =random(255);
  int greeno = random(255);
  int blueo = random (255);
      for (int i = 0 ; i < 50 ; i++){
      strip.setPixelColor(i, redo, greeno, blueo);
      }
      
      strip.show();
      delay (20);
      
   int x = 3;
   for (int ii = 1 ; ii <252 ; ii = ii = ii + x){
     strip.setBrightness(ii);
     strip.show();              
     delay(5);
    }
    
    x = 3;
   for (int ii = 252 ; ii > 3 ; ii = ii - x){
     strip.setBrightness(ii);
     strip.show();              
     delay(3);
     }
   delay(10);
   
   x = 6;
  for (int ii = 1 ; ii <255 ; ii = ii = ii + x){
     strip.setBrightness(ii);
     strip.show();              
     delay(2);  
     }
   x = 6;
   for (int ii = 255 ; ii > 1 ; ii = ii - x){
     strip.setBrightness(ii);
     strip.show();              
     delay(3);
   }
  delay (50);   
}


void waver(int color1, int color2, int cycles, int quickness, int interrupt) {
  int t = 0;
  for(int j = 0; j <strip.numPixels()*cycles;j++){
    if(interrupt-10 > analogRead(POT) ||  interrupt+10 < analogRead(POT)){
      break; 
    }
    for (float i = 0; i < 120; i ++)
      strip.setPixelColor(i, strip.Color(color1, color2, 64*sin((i-2*t)/8) + 64));
    strip.show();
  
    t ++;
    delay(quickness); 
  }
}
void rainbowStripper(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


void sparkler(uint8_t wait){
  int highPix = strip.numPixels();
  uint32_t randomColor = strip.Color(random(255),random(255),random(255));
  int spark = random(strip.numPixels());
  int spark1 = random(strip.numPixels());
  int spark2 = random(strip.numPixels());
  int spark3 = random(strip.numPixels());
  int spark4 = random(strip.numPixels());
  int spark5 = random(strip.numPixels());
  int spark6 = random(strip.numPixels());
  int spark7 = random(strip.numPixels());
  int spark8 = random(strip.numPixels());
  int spark9 = random(strip.numPixels());
    
    for (int i=0; i < 255; i++) {
      strip.setBrightness(i);      
      strip.setPixelColor(spark, randomColor);
      strip.setPixelColor(spark1, randomColor); 
      strip.setPixelColor(spark2, randomColor);
      strip.setPixelColor(spark3, randomColor);
      strip.setPixelColor(spark4, randomColor);
      strip.setPixelColor(spark5, randomColor);
      strip.setPixelColor(spark6, randomColor);
      strip.setPixelColor(spark7, randomColor);
      strip.setPixelColor(spark8, randomColor);
      strip.setPixelColor(spark9, randomColor);
        delay(3);  
        strip.show();
     }
     for (int i=255; i > 0; i--) {
        
        strip.setBrightness(i);    
        
        delay(2);  
        strip.show();
     }
}

uint32_t builder(uint8_t wait, int interrupt){
  int highPix = strip.numPixels();
  uint32_t randomColor = strip.Color(random(255),random(255),random(255));
    for (int j=0; j < strip.numPixels(); j++) {
      for (int i=0; i < strip.numPixels()- j; i++) {
        strip.setPixelColor(i-1,0,0,0);
        strip.setPixelColor(i, randomColor);
        
        if(interrupt-10 > analogRead(POT) ||  interrupt+10 < analogRead(POT)){
          break; 
        }
        delay(wait);  
        strip.show();
        
     }
   }
   
   
}

void EndToEndWipe(int updateDelay) {
   int j = strip.numPixels();
   // Account for even / odd number of pixels.
   int i = !(j % 2);
   
   for(i; i <= (strip.numPixels() + 1); i += 2) {
     strip.setPixelColor(i, 0, 0, 0);
     strip.show();  // Show each time for a quick wipe effect.
     delay(updateDelay);
     
     strip.setPixelColor(j, 0, 0, 0);
     strip.show();  // Show each time for a quick wipe effect.
     delay(updateDelay);
     
     pixelSets += 2;
     j -= 2;
   }
}


void KillRandomPixel() {
   strip.setPixelColor(random(0, strip.numPixels()), 0, 0, 0);

  // Push Update to Strip
  strip.show();
  pixelSets++;
}

void SetRandomPixelRGB() {
  // Variable Declarations
  byte rgbAssignment; 
  
  // Determine R-G-B Color Value Assignments
  rgbAssignment = random(1, 4);
  
  switch (rgbAssignment) {
    case 1:
      // Red
      strip.setPixelColor(random(0, strip.numPixels()), 255, 0, 20);
    break;
   
    case 2:
     // Green
     strip.setPixelColor(random(0, strip.numPixels()), 190, 88, 0);
     
    break;
   
    case 3:
     // Blue
     strip.setPixelColor(random(0, strip.numPixels()), 255, 255, 255);
     
    break; 
  }
    
  // Push Update to Strip
  strip.show();
  pixelSets++;
}

void SetRandomPixel() {
  // Variable Declarations
  byte rgbAssignment; 
  
  // Determine R-G-B Color Value Assignments
  rgbAssignment = random(1, 4);
  
  switch (rgbAssignment) {
    case 1:
      // Red-Green Mix
      strip.setPixelColor(random(0, strip.numPixels()), random(0, 255), random(0, 255), 0);
    break;
   
    case 2:
     // Green-Blue Mix
     strip.setPixelColor(random(0, strip.numPixels()), 0, random(0, 255), random(0, 255));
     
    break;
   
    case 3:
     // Blue-Red Mix
     strip.setPixelColor(random(0, strip.numPixels()), random(0, 255), 0, random(0, 255));
     
    break; 
  }
  
  // Push Update to Strip
  strip.show();
  pixelSets++;
  
}

void Pulse(uint8_t R_Bright, uint8_t G_Bright, uint8_t B_Bright, uint8_t R_Pulse, uint8_t G_Pulse, uint8_t B_Pulse){

  char mid = strip.numPixels()/2;
  uint8_t saveR, saveG, saveB;
  int x;
  int currentup; 
  char currentdown;
  char decrement = 10;
  
  saveR = R_Pulse;
  saveG = G_Pulse;
  saveB = B_Pulse;
  
  currentMillis = millis();
  
  if ((currentMillis - previoustime) >= 2000 | Pulseflag)
  {
    Pulseflag = 0;
    previoustime = currentMillis;

    strip.setPixelColor((strip.numPixels()/2), R_Pulse, G_Pulse, B_Pulse);
    strip.show();
    
    // This for loop goes to 264 since so that the tail of the white pulse ends entirely off of the strip
    for(currentdown = strip.numPixels()/2 ,currentup = strip.numPixels()/2; currentup < (strip.numPixels()+24); currentdown--, currentup++)
    {
      // Restore the pulse values for the loop
        R_Pulse = saveR;
        G_Pulse = saveG;
        B_Pulse = saveB;
        
        // Start at center, full brightness
        strip.setPixelColor(currentup, R_Pulse, G_Pulse, B_Pulse);
        strip.setPixelColor(currentdown, R_Pulse, G_Pulse, B_Pulse);
        strip.show();
      
        // This goes from mid to +25 past end of strip
        currentdown--;
        currentup++;
        
       if(R_Pulse)
         R_Pulse -= decrement;
         
       if(G_Pulse)
         G_Pulse -= decrement;
        
       if(B_Pulse)
         B_Pulse -= decrement;
  
        strip.setPixelColor(currentdown,  R_Pulse, G_Pulse, B_Pulse);
        strip.setPixelColor(currentup,  R_Pulse, G_Pulse, B_Pulse);
      
      
        for(int i = 0; i < MAX_OPTIONS; i++){
        
          if(currentdown < mid - i)
          {
            if(i >= 19){
              if(R_Bright > 0){
                if(G_Pulse)
                  G_Pulse -= decrement;
                if(B_Pulse)
                  B_Pulse -= decrement;
                strip.setPixelColor(currentdown + i+1, R_Bright, G_Pulse, B_Pulse); 
                strip.setPixelColor(currentup - i-1, R_Bright, G_Pulse, B_Pulse); 
                
              }
              else if(G_Bright > 0){
                if(R_Pulse)
                  R_Pulse -= decrement;
                if(B_Pulse)
                  B_Pulse -= decrement;
                strip.setPixelColor(currentdown + i+1, R_Pulse, G_Bright, B_Pulse); 
                strip.setPixelColor(currentup - i-1, R_Pulse, G_Bright, B_Pulse); 
                
              }
              else if(B_Bright > 0){
                if(R_Pulse)
                  R_Pulse -= decrement;
                if(G_Pulse)
                  G_Pulse -= decrement;
                strip.setPixelColor(currentdown + i+1, R_Pulse, G_Pulse, B_Bright); 
                strip.setPixelColor(currentup - i-1, R_Pulse, G_Pulse, B_Bright); 
                
              }
          
              
            }
            else{
             if(R_Pulse)
               R_Pulse -= decrement;
               
             if(G_Pulse)
               G_Pulse -= decrement;
              
             if(B_Pulse)
               B_Pulse -= decrement;
              strip.setPixelColor(currentdown + i+1, R_Pulse, G_Pulse, B_Pulse); 
              strip.setPixelColor(currentup - i-1, R_Pulse, G_Pulse, B_Pulse);
            }
          }
        }
        
        
        strip.show();
        delay(12);
     
    }
    
  }

  RandomPixelFade();
  
}

void RandomPixelFade(void)
{
  
  unsigned char pixel1 = random(0, 100);
  unsigned char pixel2 = random(100, 180);
  unsigned char pixel3 = random(180, 240);
  
  long previousMillis1 = 0;
  long previousMillis2 = 0;
  long previousMillis3 = 0;
  
  int randInterval1 = random (0, 500);
  int randInterval2 =  random (0, 500);
  int randInterval3 =  random (0, 500);
  
  // Part of the randomization process
  // Each of the 3 random pixels are checked against a random variable
  // if a is less than this random variable, then the pixel is turned on
  // This way they dont always turn on at the same time, adds some nice randomness
  
  if(random(0, 1000) < random(0, 1000))
  {
    strip.setPixelColor(pixel1, 255, 255, 255);
    strip.show();
  }
  
  if(random(0, 1000) > random(0,1000))
  {
    strip.setPixelColor(pixel2, 255, 255, 255);
    strip.show();
  }
  
  if(random(0, 1000) > random(0,1000))
  {
    strip.setPixelColor(pixel3, 255, 255, 255);
    strip.show();
  }
  
  if (millis() - previousMillis1 >= randInterval1) 
  {
    
    randInterval1 = random (0, 500);
    
    // Pixel is going to be set white, then slowly fade back to blue
    for(int x = 255; x > 0; x-=5)
    {
      
      if( x <= 50)
      {
        strip.setPixelColor(pixel1, x, x, 50);
        strip.show();
      }
      else
      {
        strip.setPixelColor(pixel1, x, x, x);
        strip.show();
      }
      delay(1);
    }
    pixel1 = random(0,100);

  }
  
  if (millis() - previousMillis2  >= randInterval2) 
  {
    randInterval2 = random (0, 500);
   
    // Pixel is going to be set white, then slowly fade back to blue
    for(int x = 255; x > 0; x-=5)
    {
      
      if( x <= 50)
      {
        strip.setPixelColor(pixel2, x, x, 50);
        strip.show();
      }
      else
      {
        strip.setPixelColor(pixel2, x, x, x);
        strip.show();
      }
      delay(1);
    }
    pixel2 = random(100,180);

  }
  
  if (millis() - previousMillis3  >= randInterval3) 
  {
    randInterval3 = random (0, 500);  

    for(int x = 255; x > 0; x-=5)
    {
      
      if( x <= 50)
      {
        strip.setPixelColor(pixel3, x, x, 50);
        strip.show();
      }
      else
      {
        strip.setPixelColor(pixel3 , x, x, x);
        strip.show();
      }
      delay(1);
    }
    pixel3 = random(180,240);

  }

}

void rainbow(uint8_t wait) {
  static uint16_t i, j;
 
 if(j < 256){
   
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
  
    j++;
    delay(wait);
  }
  else
    j = 0;
  
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  static uint16_t i, j;

  if(j < (256*5)){
    
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    
    delay(wait);
    j++;
  }
  else
    j = 0;
  
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
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
  static int j;
  if(j < 256){     // cycle all 256 colors in the wheel
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
    j++;
  }
  else
    j = 0;
}

void ripple() {
  if (BG){
    if (currentBg == nextBg) {
      nextBg = random(256);
    }
    else if (nextBg > currentBg) {
      currentBg++;
    } else {
      currentBg--;
    }
    for(uint16_t l = 0; l < NumPixels; l++) {
      strip.setPixelColor(l, Wheel2(currentBg, 0.1));
    }
  } 
  else {
      for(uint16_t l = 0; l < NumPixels; l++) {
        strip.setPixelColor(l, 0, 0, 0);
      }
  }
    
  if (step == -1) {
      center = random(NumPixels);
      color = random(256);
      step = 0;
  }
    
    if (step == 0) {
      strip.setPixelColor(center, Wheel2(color, 1));
      step ++;
    }
    else {
      if (step < maxSteps) {
        strip.setPixelColor(wrap(center + step), Wheel2(color, pow(fadeRate, step)));
        strip.setPixelColor(wrap(center - step), Wheel2(color, pow(fadeRate, step)));
        if (step > 3) {
          strip.setPixelColor(wrap(center + step - 3), Wheel2(color, pow(fadeRate, step - 2)));
          strip.setPixelColor(wrap(center - step + 3), Wheel2(color, pow(fadeRate, step - 2)));
        }
        step ++;
      }
      else {
        step = -1;
      }
    }
    strip.show();
    delay(45);
}


void pulseWave(uint32_t c, uint8_t tailLength, double fadePercent, uint8_t repeatCount, uint8_t wait){
  for( uint8_t r=0; r<repeatCount; r++){  // how many times to cycle
    for( uint8_t i=0; i<strip.numPixels(); i++){ // cycle through the strip
      animationOff(); // turn off animation pixels
      strip.setPixelColor(i, c); // set head color
      for( uint8_t z=1; z<tailLength; z++){
        if(i-z >= 0){
          strip.setPixelColor(i-z, strip.Color(  getPixelColorComponent(i-z+1,1) * fadePercent,
                                                 getPixelColorComponent(i-z+1,2) * fadePercent,
                                                 getPixelColorComponent(i-z+1,3) * fadePercent));
        }
      }
      strip.show();
      delay(wait);
    }
  }
}

// turn off the animation pixels
void animationOff(){
  for( uint8_t x=0; x<strip.numPixels(); x++){
    strip.setPixelColor(x, cOff);
  }
  strip.show();
}
 
// Get a color value by segment (R=1, G=2, B=3) 
uint8_t getPixelColorComponent(uint8_t pixelNum, uint8_t segment){
  uint8_t segmentColorValue;
  uint32_t c = strip.getPixelColor(pixelNum);
  switch( segment ){
    case 1:
      segmentColorValue = ( c >> 16) & 255;
      break;
    case 2:
      segmentColorValue = ( c >> 8) & 255;
      break;
    case 3:
      segmentColorValue = c & 255;
      break;
  }
  return segmentColorValue;
}

 
int wrap(int step) {
  
  if(step < 0)
    return NumPixels + step;
    
  if(step > NumPixels - 1) 
    return step - NumPixels;
  
  return step;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel2(byte WheelPos, float opacity)
{
    if(WheelPos < 85) {
        return strip.Color((WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity, 0);
    }
    else if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color((255 - WheelPos * 3) * opacity, 0, (WheelPos * 3) * opacity);
    }
    else {
        WheelPos -= 170;
        return strip.Color(0, (WheelPos * 3) * opacity, (255 - WheelPos * 3) * opacity);
    }
}
  

