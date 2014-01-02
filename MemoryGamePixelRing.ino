#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define LED 13

#define PIN 6

#define buttonYellow 5
#define buttonBlue   4
#define buttonGreen  3
#define buttonRed    2

#define cYellow 0x0C0C00
#define cRed    0x0C0000
#define cGreen  0x000C00
#define cBlue   0x00000C
#define cWhite  0x0C0C0C
#define cBlack  0x000000
#define cPurple 0x0C000C

#define numPixels 16

// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
 Adafruit_NeoPixel strip = Adafruit_NeoPixel(numPixels, PIN, NEO_GRB + NEO_KHZ800);

//Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(5, 8, 6,
//  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
//  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
//  NEO_GRB            + NEO_KHZ800);

unsigned long wacht = 1000;
unsigned int sequence = 4;
unsigned int steps = 0;
#define maxsequence 50
unsigned int score = 1;
unsigned int array[maxsequence];

void setup() {
  pinMode(LED, OUTPUT);
  
  pinMode(buttonRed,   INPUT_PULLUP);
  pinMode(buttonGreen, INPUT_PULLUP);
  pinMode(buttonBlue,  INPUT_PULLUP);
  pinMode(buttonYellow,INPUT_PULLUP);
  randomSeed(analogRead(0));
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
  rood();
  delay(1000);
  groen();
  delay(1000);
  blauw();
  delay(1000);
  geel();
  delay(1000);
  
  Serial.begin(9600);
}

void loop() {
  Serial.println("-------------------------------");
  if ( sequence > maxsequence) {
    einde();
  }
  wit();

  for( int rndmx = 0; rndmx < sequence; rndmx++) {
    array[rndmx] = random(1,5);
    Serial.print(array[rndmx]);
    Serial.print(" ");
  }
  Serial.println();
  countDown(1,cWhite);
  uit();
  for( int showx = 0; showx < sequence; showx++) {
    switch (array[showx]) {
      case 0:
        uit();
        break;
      case 1:
        rood();
        break;
      case 2:
        groen();
        break;
      case 3:
        blauw();
        break;
      case 4:
        geel();
        break;
    }
    delay(wacht);
    uit();
    delay(250);
  }
  uit();
  int arrayCounter = 0;
  unsigned int bt = 0;
  unsigned int bt_old = 0;

    Serial.println(steps);
    Serial.println((wacht/numPixels)*(4-steps));

  for( int loopx = numPixels; loopx > 0 ; loopx--) {
//    Serial.print("loopx: ");
//    Serial.println(loopx);
    long time = millis() + ((wacht/numPixels)*(5-steps));
    strip.setPixelColor(loopx,cPurple);
    strip.show();
    digitalWrite(LED,LOW);
    while ( time > millis() )
    {
      digitalWrite(LED,HIGH);
      bt = button();
      if ( (bt_old != 0) && (bt == 0) ) {
        bt_old = 0;
      }
      if ( (bt_old == 0) && (bt != 0) ) {
        bt_old = bt;
        if ( array[arrayCounter] == bt) {
          Serial.println("ok");
          arrayCounter++;
        } else {
          Serial.println("error");
          error();
          loopx=0;
        } 
        if ( arrayCounter == sequence ) {
          loopx = 0;
        }
      }
    }
    digitalWrite(LED,LOW);
  }
  Serial.println("EINDE");
  if ( arrayCounter != sequence) {
    loose();
  } else {
    win();
    steps++;
    if( steps > 4) {
      steps = 0;
      sequence++;
    }
    wacht = wacht - (wacht / 100);  
  }
  delay(1000);
//  while( readButton(buttonRed) == HIGH ) {
//    delay(50);
//  }
}

int button() {
  boolean rlb;
  boolean rb;
  boolean glb;
  boolean gb;
  boolean blb;
  boolean bb;
  boolean ylb;
  boolean yb;
  rlb = digitalRead(buttonRed);
  glb = digitalRead(buttonGreen);
  blb = digitalRead(buttonBlue);
  ylb = digitalRead(buttonYellow);
  delay(50);
  rb = digitalRead(buttonRed);
  gb = digitalRead(buttonGreen);
  bb = digitalRead(buttonBlue);
  yb = digitalRead(buttonYellow);
  if ( ( rb == LOW) && (rlb == LOW) ) {
    return(1);
  }
  if ( ( gb == LOW) && (glb == LOW) ) {
    return(2);
  }
  if ( ( bb == LOW) && (blb == LOW) ) {
    return(3);
  }
  if ( ( yb == LOW) && (ylb == LOW) ) {
    return(4);
  }
  return 0;
}

int readButton(int button) {
  boolean lb;
  boolean b;
  lb = digitalRead(button);
  delay(50);
  b = digitalRead(button);
  if ( ( b == LOW) && (lb == LOW) ) {
    return(LOW);
  } else {
    return(HIGH);
  }
}

void segmentKleur(int segment, unsigned long kleur){
  segment--;
  for( int pxl = segment * 4; pxl < (segment * 4) + 4; pxl++) {
    strip.setPixelColor(pxl,kleur);
  }
  strip.show();
}
void rood(){
  segmentKleur(1,cRed);
}
void groen(){
  segmentKleur(3,cGreen);
}
void blauw(){
  segmentKleur(4,cBlue);
}
void geel(){
  segmentKleur(2,cYellow);
}
void fullCircle(unsigned long kleur, unsigned int np = numPixels) {
  for( int x = 0; x < np; x++) {
    strip.setPixelColor(x,kleur);
  }
  strip.show();
}

void countDown(unsigned int time, unsigned long kleur) {
  fullCircle(kleur);
  for ( int pxl = numPixels; pxl > 0; pxl--) {
    delay((time*1000)/numPixels);
    strip.setPixelColor(pxl,cBlack);
    strip.show();
  }
}
void uit(){
  fullCircle(cBlack);
}
void wit(){
  fullCircle(cWhite);
}
void error(){
  fullCircle(cRed);
}
void winlose(unsigned long kleur){
  fullCircle(kleur);
  delay(750);
  uit();
  fullCircle(kleur,score);
  delay(1000);
}
void win(){
  score++;
  winlose(cGreen);
}
void loose(){
  winlose(cRed);
}
void einde() {
  uit();
  for ( int pxl = numPixels; pxl > 0; pxl--) {
    strip.setPixelColor(pxl,cPurple);
    strip.show();
    delay((1000)/numPixels);
    strip.setPixelColor(pxl,cBlack);
    strip.show();
    delay((1000)/numPixels);
  }
}
