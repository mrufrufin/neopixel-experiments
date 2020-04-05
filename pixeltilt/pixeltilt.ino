#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 2
#define NUMPIXELS 60

float curIntensity[NUMPIXELS] = {};


//idea is to add velocity each frame and round each time to an LED index
float curLoc = 0.0;
float fadeMult = 0.9;
float curVel = 0.0;
int curColor[3] = {255,255,255};
float loThresh = 0.005 ; //lo threshold, just count as 0

int dir = 1;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
   Serial.begin(9600);
  pixels.begin();
  pixels.show();
  
  for(int i = 0; i < NUMPIXELS; i++)
      curIntensity[i] = 0.0;

  for(int i = 0; i <3; i++)
      curColor[i] = (int)random(150,255);

  int roundloc = roundToInt(curLoc);
   curIntensity[roundloc] = 1.0;
}

void loop()
{
  int showpix = 0; //flag to update state

  //updatePosition();
  movePosition();
  showpix = updateIntensities();
  if(showpix > 0)
    {
      updateColors();
      pixels.show();
    };

}
/*
void serialEvent()
{
  while(Serial.available())
    {
      int inbyte = Serial.read();
      //Serial.println(inbyte);
      curVel = byteToVel(inbyte);
    };
}
*/
//want to map byte 0-255 to -0.5 to 0.5 velocity
float byteToVel(int vel)
{
  float in_lo = 0.0;
  float in_hi = 255.0;
  float out_lo = -0.05;
  float out_hi = 0.05;
  float invel = vel > in_hi ? in_hi : (vel < in_lo ? in_lo : (float)vel);

  float returnVal = ((invel - in_lo) * (out_hi - out_lo)/(in_hi-in_lo)) + out_lo;
  //Serial.println(returnVal);
  return returnVal;
}

int intensityToGamma(int cur)
{
  float in_lo = 0.0;
  float in_hi = 1.0;
  float out_lo = 0;
  float out_hi = 255;
  float incur = cur > in_hi ? in_hi : (cur < in_lo ? in_lo : (float)cur);

  float returnVal = ((incur - in_lo) * (out_hi - out_lo)/(in_hi-in_lo)) + out_lo;
  return (int)returnVal;
}

int roundToInt(float ipt)
{
  return (int)(ipt + 0.5);
}

void updatePosition()
{
  curLoc = curLoc + curVel;
}

//return 0 if no update needed, else return 1

int updateIntensities()
{
  int roundloc = roundToInt(curLoc);
  int updateNeeded = 0;
  roundloc = roundloc < 0 ? 0 : (roundloc >= NUMPIXELS ? NUMPIXELS - 1 : roundloc);

  for(int i = 0; i < NUMPIXELS; i++)
    {
      float cur = curIntensity[i];
      if(i == roundloc) cur = 1.0;
      else if(cur < loThresh) cur = 0.0;
      else cur = cur * fadeMult;

      if(cur != curIntensity[i]) updateNeeded = 1;
      curIntensity[i] = cur;
    };
  return updateNeeded;
}

void updateColors(void)
{
  for(int i =0; i < NUMPIXELS; i++)
    {
      
      int cur[3] = {0,0,0};
      
      float intensityMult = curIntensity[i];
      
      for(int j = 0; j < 3; j++)
        {
          cur[j] = (int)(curColor[j]*intensityMult);
        };
  

  
      //int curGamma = intensityToGamma(curIntensity[i]);
      /*
      if(curGamma > 0)
      {
        Serial.print(i);
        Serial.print(" ");
        Serial.println(curGamma);
      };
      */
      pixels.setPixelColor(i, pixels.Color(cur[0],cur[1],cur[2]));
    };
}

void movePosition()
{
  float vel = 0.1;
  float loc = curLoc + (vel * dir);
  if(loc < 0)
  {
    dir = 1;
    loc = 0;
  } 
  else if(loc >= NUMPIXELS)
  {
    loc = NUMPIXELS-1;
    dir = -1;
  };

  curLoc = loc;
}


