#include <Wire.h>
#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"

#define CLK 10
#define DIO 11
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7
#define BUTTONK2 9
#define BUTTONK1 8
#define KNOB_PIN A0


TM1637 disp(CLK,DIO);

DHT dht;

void setup() {
    disp.init();  
    dht.begin();
    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(BUTTONK2, INPUT_PULLUP);
    pinMode(BUTTONK1, INPUT_PULLUP);

    Serial.begin(9600); 
}

void loop() { 
  double xtemp = 36, yhumi = 60;
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if(digitalRead(BUTTONK1) == 0)
  {
    K1(t, h);
    disp.clearDisplay();
    ChangeValueTemp(xtemp);
    ChangeValueHumi(yhumi);
    disp.clearDisplay();
  }
}
/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */

void displayTemperature(int8_t temperature)
{
// Rayhan
  int8_t temp[4];
 
  temp[0] = temperature / 10;
  temp[1] = temperature % 10;
  temp[2] = 17; // index of 17 for space
  temp[3] = 12;           //index of 12 for 'C' symbol.
  disp.display(temp);
}


void displayHumidity(int8_t h)
{
// Rayhan
  int8_t hume[4];
  hume[0] = h/100;
  h %= 100;
  hume[0] = h / 10;
  hume[1] = h % 10;
  hume[2] = 17; // index of 17 for space
  hume[3] = 18;           //index of 18 for 'H' symbol.
  disp.display(hume);
}


void K2(float t, float h) //Done by Yi'en
{
  if (digitalRead(BUTTONK2) == 0)
  {
    delay(500);
    Serial.println("Button K2 is pressed.");
    Serial.print("The temperature is: ");
    Serial.println(t);
    displayTemperature((int8_t)t);
    delay(1000);
    Serial.print("The humidity is: ");
    Serial.println(h);
    displayHumidity((int8_t)h);
    delay(1000);
    disp.clearDisplay();
    while(digitalRead(BUTTONK2) == 0);
  }
}

void ChangeValueTemp(double xtemp) //Yi'en
{
  Serial.print("Press button K1 to change threshold temperature, button K2 to break.");
  Serial.println(" ");
  delay(500);
  while(1)
  {
    if(digitalRead(BUTTONK2) == 0)
    {
      break;
    }
    if(digitalRead(BUTTONK1) == 0)
    {
      delay(500);
      while(1)
      {
        Serial.print("Press button K1 to stop changing temperature, use knob to adjust temperature.");
        xtemp = analogRead(KNOB_PIN)/10.24;
        Serial.print("Threshold value : ");
        Serial.println(xtemp,1);
        delay(200);
        displayTemperature(xtemp);
        if(digitalRead(BUTTONK1) == 0)
        {
          Serial.print("New threshold temperature is: ");
          Serial.println(xtemp);
          return;
        }
      }
    }
  }
}


void ChangeValueHumi(double yhumi) //Yi'en
{
  Serial.print("Press button K2 to change threshold humidity, button K1 to break.");
  Serial.println(" ");
  delay(500);
  while(1)
  {
    if(digitalRead(BUTTONK1) == 0)
    {
      break;
    }
    if(digitalRead(BUTTONK2) == 0)
    {
      delay(500);
      while(1)
      {
        Serial.print("Press button K1 to stop changing humidity, use knob to adjust humidity.");
        yhumi = analogRead(KNOB_PIN)/10.24;
        Serial.print("Threshold value : ");
        Serial.println(yhumi,1);
        delay(200);
        displayHumidity(yhumi);
        if(digitalRead(BUTTONK1) == 0)
        {
          Serial.print("New threshold humidity is: ");
          Serial.println(yhumi);
          return;
        }
      }
    }
  }
}







