

#include "FastLED.h"
#include <LEDMatrix.h>
#include <DFPlayer_Mini_Mp3.h>

#include <SoftwareSerial.h>

#include <FontMatrise.h>

#include <FontSmall3x5.h>
#include <LEDText.h>

#define BIKE_1_PADDLE 52
#define BIKE_2_PADDLE 7
#define BIKE_3_PADDLE 8


#define WALL_WIDTH 38
#define WALL_HEIGHT 19
#define MATRIX_TYPE HORIZONTAL_ZIGZAG_MATRIX

#define WALL_LED_PIN    3
#define COLOR_ORDER    GRB
#define CHIPSET        WS2811

#define BIKE_1_CHASER_PIN 16
#define BIKE_2_CHASER_PIN 17
#define BIKE_3_CHASER_PIN 18

#define NUM_LEDS_BIKE_1 60
#define NUM_LEDS_BIKE_2 60
#define NUM_LEDS_BIKE_3 60
CRGB bike_one_leds[NUM_LEDS_BIKE_1];
CRGB bike_two_leds[NUM_LEDS_BIKE_2];
CRGB bike_three_leds[NUM_LEDS_BIKE_3];

cLEDMatrix<-WALL_WIDTH,WALL_HEIGHT, MATRIX_TYPE> wall_leds;
cLEDText Slogans;
cLEDText BIKEONE;
cLEDText BIKETWO;
cLEDText BIKETHREE;
const unsigned char Sch_Slogans[]= {EFFECT_SCROLL_LEFT "  IMAGINE WITH YOUR MIND.      BELIEVE WITH YOUR HEART.   ACHIEVE WITH YOUR MIGHT."};
//const unsigned char Sch_Slogans[]= {EFFECT_SCROLL_LEFT "./0123456789"};

String BIKEONE_SCORE="";
String BIKETWO_SCORE="";
String BIKETHREE_SCORE="";

float BIKE_1_SCORE;
float BIKE_2_SCORE;
float BIKE_3_SCORE;
float TEAM_TOTAL_SCORE;

int BIKE_1_TURN;
int BIKE_2_TURN;
int BIKE_3_TURN;

//mp3 stuff
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);





void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  FastLED.addLeds<CHIPSET, WALL_LED_PIN, COLOR_ORDER>(wall_leds[0], wall_leds.Size());
  FastLED.addLeds<CHIPSET, BIKE_1_CHASER_PIN, COLOR_ORDER>(bike_one_leds, NUM_LEDS_BIKE_1);
  FastLED.addLeds<CHIPSET, BIKE_2_CHASER_PIN, COLOR_ORDER>(bike_two_leds, NUM_LEDS_BIKE_2);
  FastLED.addLeds<CHIPSET, BIKE_3_CHASER_PIN, COLOR_ORDER>(bike_three_leds, NUM_LEDS_BIKE_3);

  BIKE_1_SCORE =0;
  BIKE_2_SCORE =0;
  BIKE_3_SCORE =0;

  //set pin modes for reed switches on bikes
  pinMode(BIKE_1_PADDLE, INPUT);
  pinMode(BIKE_2_PADDLE, INPUT);
  pinMode(BIKE_3_PADDLE, INPUT);


  //set scrolling msg slogan
//FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(wall_leds[0], wall_leds.Size());
  FastLED.setBrightness(180);
  FastLED.clear(true);
  delay(500);
  Slogans.SetFont(SmallFont3x5Data);
  Slogans.Init(&wall_leds, wall_leds.Width(), Slogans.FontHeight() + 1, 0, 10);
  Slogans.SetText((unsigned char *)Sch_Slogans, sizeof(Sch_Slogans)-1);
//  Slogans.SetTextColrOptions(COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);
  float one_score=0;
  BIKEONE_SCORE=String(one_score,1);
  unsigned char buffer_one[4];
  BIKEONE_SCORE.toCharArray(buffer_one,4);
  BIKEONE.SetFont(SmallFont3x5Data);
  BIKEONE.SetBackgroundMode(BACKGND_LEAVE);
  BIKEONE.Init(&wall_leds,12,5,0,0);
  BIKEONE.SetText((unsigned char *)buffer_one,sizeof(buffer_one)-1);
  BIKEONE.SetScrollDirection(SCROLL_UP);
  
  BIKEONE.SetTextColrOptions( COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);

float two_score=0;
 BIKETWO_SCORE=String(two_score,1);
  unsigned char buffer_two[4];
  BIKETWO_SCORE.toCharArray(buffer_two,4);
  BIKETWO.SetFont(SmallFont3x5Data);
  BIKETWO.SetBackgroundMode(BACKGND_LEAVE);
  BIKETWO.Init(&wall_leds,12,5,0,14);
  BIKETWO.SetText((unsigned char *)buffer_two,sizeof(buffer_two)-1);
  BIKETWO.SetScrollDirection(SCROLL_UP);
  
  BIKETWO.SetTextColrOptions( COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);

float three_score=0;
 BIKETHREE_SCORE=String(three_score,1);
  unsigned char buffer_three[4];
  BIKETHREE_SCORE.toCharArray(buffer_three,4);
  BIKETHREE.SetFont(SmallFont3x5Data);
  BIKETHREE.SetBackgroundMode(BACKGND_LEAVE);
  BIKETHREE.Init(&wall_leds,12,5,0,26);
  BIKETHREE.SetText((unsigned char *)buffer_three,sizeof(buffer_three)-1);
  BIKETHREE.SetScrollDirection(SCROLL_UP);
  BIKETHREE.SetTextColrOptions( COLR_RGB | COLR_SINGLE, 0xff, 0x00, 0xff);
 //mp3 player stuff

  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(10);  //Set volume value. From 0 to 30
  myDFPlayer.play(1); 
}
void loop() {
  // put your main code here, to run repeatedly:
 //read and monitor each bike pin state, then convert to char for text display
  BIKE_1_TURN=digitalRead(BIKE_1_PADDLE);
  unsigned char bikeone_score[4];
  BIKE_2_TURN=digitalRead(BIKE_2_PADDLE);
  unsigned char biketwo_score[4];
  BIKE_2_TURN=digitalRead(BIKE_3_PADDLE);
  unsigned char bikethree_score[4];
 
  
  Serial.println(BIKE_1_TURN);
if (BIKE_1_TURN==HIGH)  
   {
    BIKE_1_SCORE+=0.0005;
    TEAM_TOTAL_SCORE+=0.0005;
    BIKEONE_SCORE=String(BIKE_1_SCORE,1);
        BIKEONE_SCORE.toCharArray(bikeone_score,4);
    Serial.println("Bike 1 score = "+String(BIKE_1_SCORE,1));

   BikeLights(bike_one_leds, NUM_LEDS_BIKE_1);
    }

    if (BIKE_2_PADDLE==HIGH) {
    BIKE_2_SCORE+=0.0005;
    TEAM_TOTAL_SCORE+=0.0005;
    BIKETWO_SCORE=String(BIKE_2_SCORE,1);
        BIKETWO_SCORE.toCharArray(biketwo_score,4);
    Serial.println("Bike 2 score = "+String(BIKE_2_SCORE,1));

    }

    if (BIKE_3_PADDLE==HIGH) {
    BIKE_3_SCORE+=0.0005;
    TEAM_TOTAL_SCORE+=0.0005;
    BIKETHREE_SCORE=String(BIKE_3_SCORE,1);
    BIKETHREE_SCORE.toCharArray(bikethree_score,4);
    Serial.println("Bike 3 score = "+String(BIKE_3_SCORE,1));

    }
// b_one= String(BIKE_1_SCORE);
//BIKEONE_SCORE[0]=b_one;
int BIKE_1_RECT_HEIGHT=round(BIKE_1_SCORE*4);
int BIKE_2_RECT_HEIGHT=round(BIKE_2_SCORE*4);
int BIKE_3_RECT_HEIGHT=round(BIKE_3_SCORE*4);


wall_leds.DrawFilledRectangle(1,0,12,BIKE_1_RECT_HEIGHT,CHSV(100, 100, 255));
wall_leds.DrawFilledRectangle(13,0,25,BIKE_2_RECT_HEIGHT,CHSV(50, 50, 255));
wall_leds.DrawFilledRectangle(26,0,38,BIKE_3_RECT_HEIGHT,CHSV(100, 200, 255));
     if (Slogans.UpdateText() == -1)

     
    Slogans.SetText((unsigned char *)Sch_Slogans, sizeof(Sch_Slogans) - 1);
     
  else 
    FastLED.show();
 ;
    if (BIKEONE.UpdateText() == -1)

     
    BIKEONE.SetText((unsigned char *)bikeone_score, sizeof(bikeone_score)-1);
     
  else 
    FastLED.show();

      if (BIKETWO.UpdateText() == -1)

     
    BIKETWO.SetText((unsigned char *)biketwo_score, sizeof(biketwo_score)-1);
     
  else 
    FastLED.show();

    if (BIKETHREE.UpdateText() == -1)

     
    BIKETHREE.SetText((unsigned char *)bikethree_score, sizeof(bikethree_score)-1);
     
  else 
    FastLED.show();

 
 ;
  delay(50);

}

void BikeLights(CRGB bikeno,int num_leds_strip) {
  for (int Led=0;Led<num_leds_strip;Led++) {
     bikeno[Led]=CRGB :: White;
     FastLED.show();
     delay(100);
     bikeno[Led]=CRGB::Black;
    }
  }

  void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}
