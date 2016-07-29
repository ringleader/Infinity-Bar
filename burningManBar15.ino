/*******************************
 * burning man bar code      *
 ******************************/
 
 // V1.3
 
 /* CUSTOMIZABLE PARAMETERS */
   // Neopixel LED Strip
   #define NumPixels 362                    // number of pixels in strip (255 MAX)
   #define DataOut 6   // pin number (most are valid)
   #define POT A0
   #define BG 1  //unsure right now
   
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
   #define MAX_OPTIONS 55 //The maximum number of NeoPixel options available besides 99
   
/*************************************************************************************************/
/***||||||||||||||||||||||||| No Editable Parameters Below This Line ||||||||||||||||||||||||||***/
/*************************************************************************************************/
#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NumPixels, DataOut, NEO_GRB + NEO_KHZ800);


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
uint32_t cOff = strip.Color(0, 0, 0);

//background color
uint32_t currentBg = random(256);
uint32_t nextBg = currentBg;

unsigned long previoustime;
unsigned long currentMillis;

char Pulseflag = 1;

void setup() {
  
  // NeoPixel Initialization
  strip.begin();
  strip.show();    // All pixels OFF
  
  // Random Number Generator Initialization
  randomSeed(analogRead(FloatingPin));    // Use Floating ADC Pin
  
  
}

void loop() {
  uint32_t randomColor = strip.Color(random(255),random(255),random(255));
  uint32_t shutDown = strip.Color(0,0,0);
  
  
  strip.setBrightness(255);

  if(analogRead(POT) >= 0 && analogRead(POT) < 100){  
    sparkler(20);                  
  }else if(analogRead(POT) >= 100 && analogRead(POT) < 200){   
    colorWipe(randomColor, delayVal); // randome color startup
  }else if(analogRead(POT) >= 200 && analogRead(POT) < 300){    
    rainbow(20);
  }else if(analogRead(POT) >= 300 && analogRead(POT) < 400){    
    theaterChaseRainbow(40);
  }else if(analogRead(POT) >= 400 && analogRead(POT) < 500){
    theaterChase(randomColor, 50); // Random Theater
  }else if(analogRead(POT) >= 500 && analogRead(POT) < 600){
    ripple();
  }else if(analogRead(POT) >= 600 && analogRead(POT) < 700){
    theaterChase(randomColor, 50); // Random Theater
  }else if(analogRead(POT) >= 700 && analogRead(POT) < 800){
    RandomPixelFade();
  }else if(analogRead(POT) >= 700 && analogRead(POT) < 800){
    SetRandomPixelRGB();
    KillRandomPixel();
  }else{
    for(int i=0;i < 1000;i++){
      ripple();
    }
    for(int i=0;i < 25;i++){
      theaterChaseRainbow(40);
    }
    theaterChaseRainbow(40);
    for(int i=0;i < 1000;i++){
      rainbow(20);
    }
    for(int i=0;i < 10;i++){
      colorWipe(strip.Color(random(255),random(255),random(255)), delayVal);
    }
    
    for(int i=0;i < 10;i++){
    theaterChase(strip.Color(random(255),random(255),random(255)), 50); // Random Theater
  }
  }
         
}


void waver(int color1, int color2, int cycles, int quickness) {
  int t = 0;
  for(int j = 0; j <strip.numPixels()*cycles;j++){
    for (float i = 0; i < strip.numPixels(); i ++)
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
    strip.setBrightness(255);
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
void ReadMSGEQ7(int Spectrum[]) {
    // Band 0 = Lowest Frequencies.
    byte band;
    
    for(band = 0; band < 7; band++) {
      Spectrum[band] = (analogRead(spectrumAnalogL) + analogRead(spectrumAnalogR) + analogRead(spectrumAnalogL) + analogRead(spectrumAnalogR) + analogRead(spectrumAnalogL) + analogRead(spectrumAnalogR)) / 5; // Read several times and take the average
      digitalWrite(spectrumStrobe,HIGH);
      digitalWrite(spectrumStrobe,LOW);     
    }
}

void Spectrum() {
  // Spectrum analyzer read values will be kept here.
  static int Spectrum[7];
  static char color = 1;  // Spectrum
  
  // Get current Spectrum data
  ReadMSGEQ7(Spectrum);
  
  // Display
  char barLength = 0;
  char i = 0;
  char j = 0;
  char currentStartPt = 0;
  static int SpectrumMAP[7];  // Spectrum values, mapped to desired brightness

  // Strobe Color Rotation Set
  if (Spectrum[0] > 250) {
    color += 1;
    
    if (color > 3) {
      color = 1;
    }
  }
  
  // Repeated Spectrum Strip
  for (int j = 0; j < strip.numPixels(); j += 8) {
    switch (color) {
      case 1:
        strip.setPixelColor((j + 7), 0, 0, 255);
      break;
    
      case 2:
        strip.setPixelColor((j + 7), 0, 255, 0);
      break;
    
      case 3:
        strip.setPixelColor((j + 7), 255, 0, 0);
      break; 
    }
   
    for (int i = 0; i < 7; i++) {
      SpectrumMAP[i] = map(Spectrum[i], 0, 255, 0, maxBrightness);
      strip.setPixelColor((i + j), SpectrumMAP[i], SpectrumMAP[i], SpectrumMAP[i]);
    }
  
  }
  
  pixelSets += strip.numPixels();  // Entire strip has been updated.
  strip.show();
 
}  // END Spectrum


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

  char mid = strip.numPixels();
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
    for(currentdown = 0 ,currentup = 0; currentup < (strip.numPixels()+50); currentdown--, currentup++)
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
  

