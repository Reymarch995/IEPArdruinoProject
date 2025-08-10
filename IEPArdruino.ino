/*
Names: Rayhan, Yien
IEP Project Implementation
Github: https://github.com/Reymarch995/IEPArdruinoProject/blob/main/IEPArdruino.ino


Functions used:
Rayhan: Menu function to call (0.5),(1),(5) and (6) via Serial Monitor
Progress/Updates: done


Rayhan overall progress: done, awaiting run
(0.5) Rayhan: Debug function to output all IO and sensor values
Progress/Updates: done with code comments, awaiting code run


(1) Rayhan: UI to input a time to water plant, displays the time on the segment when it the time comes and then rings a buzzer
Progress/Updates: done with code comments, success!


(3) Rayhan: Green LED indicates good, blue LED represents the LED thing on our plant prototype, red only happens when temp/RH is above y value)
Progress/Updates: done with code comments, awaiting code run


Yi'en overall progress:
(2) Yien: Once temperature or humidity is above y deg cel., or y RH, display temperature, ring the buzzer and blink red LED continuously until temperature decreases(Tells the user to bring plant to cooler env. in serial monitor and also allows the user to off the red LED and buzzer using remote) (note that x < y)
Progress/Updates: Done


(4) Yien: When user clicks on button K1, it allows user to set the threshold for x and y using knob, and the x and y value that is being set will be displayed on the 7 segment and serial monitor.
Progress/Updates: Completed already


(5) Yien:After pressing button K1, it displays temp. and RH to user when a button is pressed. (This one already have in notes but can still use to farm more marks.)
Progress/Updates: Completed already
*/
/* Comments to each other
R 28/7/2025 ~ Yup ive done all left for IEP, do do a code run pls tq! :))
Y 28/7/25 - I also finish already, I help you run tomorrow
*/


// Inits and definitions by Rayhan
#include <Wire.h>
#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"
#include "RichShieldPassiveBuzzer.h"


#define CLK 10
#define DIO 11
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7
#define BUTTONK2 9
#define BUTTONK1 8
#define KNOB_PIN A0
#define PassiveBuzzerPin 3








TM1637 disp(CLK, DIO);
DHT dht;
PassiveBuzzer buz(PassiveBuzzerPin);
double xtemp = 36, yhumi = 60;
float h = 0.0;
float t = 0.0;
bool menuShown = false;




void debug(void);








//Setup by Yien
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
    digitalWrite(LED_BLUE,HIGH);
}
//By Rayhan
void loop() {
    t = dht.readTemperature();
    h = dht.readHumidity();
    /* Show menu once */
    if (!menuShown) {
        menu();
        menuShown = true;
    }
        String cmd = Serial.readStringUntil('\n');  // read up to newline
        if (cmd == "debug") {
            debug();
            menuShown = false;      // show the main menu again afterwards
        }
    /* ---------- existing button handling ---------- */
    if (digitalRead(BUTTONK2) == LOW) {
        timer();
        menuShown = false;
    }
    else if (digitalRead(BUTTONK1) == LOW) {
        K1(t, h);
        disp.clearDisplay();       // use updated global t and h
        xtemp = ChangeValueTemp(xtemp);
        yhumi = ChangeValueHumi(yhumi);
        disp.clearDisplay();
        h = dht.readHumidity();
        t = dht.readTemperature();
        menuShown = false;         // re‑show menu after finishing
    }


    while (t > xtemp || h > yhumi) {    // alarm loop
        buzzercondition();
        if (digitalRead(BUTTONK1) == LOW) {
            digitalWrite(LED_RED, LOW);
            Serial.println(F("Alarm turned off."));
            break;
        }
        h = dht.readHumidity();
        t = dht.readTemperature();
    }
    delay(2000);
}












//By Rayhan
void menu(){
  int choice = 0;
  // main output
  Serial.print(F("*** Rayhan and Yien's *** "));
  Serial.println(F("*** Arduino IEP Code ***"));
  Serial.println(F("* * * Menu * * *"));
  Serial.println(F("*  Plant watering timer (BK2)*"));
  Serial.println(F("*  Display temperature and humidity, and also to change threshold values. (BK1)*"));
  Serial.println(F("*  Debug Mode (Serial Only, enter 'debug' to access)*"));
  Serial.println(F("* Enter your choice: "));
 
    // FIX: Check for debug command first
    if (Serial.available() > 0) {
        String input = Serial.readString();
        input.trim();
        input.toLowerCase();
        if (input == "debug") {
            debug();
            menuShown = false;
        }
    }




  // determine which function to call
  if (digitalRead(BUTTONK2) == 0){timer();} // conditions to call timer()
  else if (digitalRead(BUTTONK1) == 0){ // conditions to call Yien's set of functions
    //By Yi'en
    K1(t, h);
    disp.clearDisplay();
    xtemp = ChangeValueTemp(xtemp);
    yhumi = ChangeValueHumi(yhumi);
    disp.clearDisplay();
  //
  }
 
}








void debug() {
    // output humidity, temperature, potentiometer, button, LED values
    Serial.println();
    Serial.println(F("Entered debug mode!"));
    delay(1000);
    Serial.println(F("Enter 'exit' to exit debug mode..."));
    delay(1000);
   
    while (true) { // FIX: Proper loop structure
        if (Serial.available() > 0) {
            String input = Serial.readString();
            input.trim();
            input.toLowerCase();
            if (input == "exit") {
                Serial.println(F("Exiting debug mode..."));
                break;
            }
        }
       
        Serial.println(F("*************"));
        Serial.print(F("raw temp: ")); //output raw temp values
        Serial.println(t);
        Serial.print(F("raw humidity: ")); //output raw humidity values
        Serial.println(h);
        Serial.print(F("Potentiometer value: ")); //output raw potentiometer values
        Serial.println(analogRead(KNOB_PIN));
        if (digitalRead(BUTTONK1) == 0) {
            Serial.println(F("BK1 pressed!"));
        }
        if (digitalRead(BUTTONK2) == 0) {
            Serial.println(F("BK2 pressed!"));
        }
        Serial.println(F("*************"));
        delay(2000);
    }
}








//By Rayhan
void timer() {
    Serial.println(F("Enter timer in seconds:"));
    // Wait for the user to type something in the Serial Monitor
    while (Serial.available() == 0) {
        // just waiting
    }








    long seconds = Serial.parseInt();
    Serial.print("Starting countdown: ");
    Serial.print(seconds);
    Serial.println(F(" s"));








    // Countdown loop
    while (seconds > 0) {
        Serial.print(seconds);
        Serial.println(F("..."));
        delay(1000);
        seconds--;
    }








    // Notify user and buzz until Button K2 is pressed
    Serial.println(F("Time's up! Press Button K2 to stop buzzer."));
    while (digitalRead(BUTTONK2) == HIGH) {
        // play high frequency intermittent tone to attract attention
        buz.playTone(250, 500);
        delay(500);
        //ensure the buzzer is off
        buz.off();
        delay(500);
    }
    Serial.println(F("Buzzer stopped."));
}








//By Rayhan
void displayTemperature(int8_t temperature) {
    int8_t temp[4]; //array to display temp values in RichShield LCD
    temp[0] = temperature / 10;
    temp[1] = temperature % 10;
    temp[2] = 17; // space
    temp[3] = 12; // 'C'
    disp.display(temp);
}
//By Rayhan
void displayHumidity(int8_t h) {
    int8_t hume[4]; //array to display humidity values in RichShield LCD
    hume[0] = h / 10;
    hume[1] = h % 10;
    hume[2] = 17; // space
    hume[3] = 18; // 'H'
    disp.display(hume);
}








void plantStatus(){ // Green if healthy (within Range) Red if unhealthy, outside of range, Blue LED is always ON
  if((xtemp < 30 && xtemp > 25) && (yhumi < 70 && yhumi > 45)){digitalWrite(LED_GREEN,HIGH);} //indicate Plant is healthy and within optimal range!
 
}








//By Yien
void K1(float t, float h) //Yi'en
{
  if (isnan(t) || isnan(h))
  {
    disp.display(0, 14);
  }
  disp.clearDisplay();
  delay(500);
  Serial.println(F("Button K1 is pressed."));
  Serial.print("The temperature is: ");
  Serial.println(t);
  Serial.print("C");
  Serial.println(F(" "));
  displayTemperature((int8_t)t);
  delay(1000);
  Serial.print(F("The humidity is: "));
  Serial.println(h);
  Serial.print("H");
  Serial.println(F(" "));
  displayHumidity((int8_t)h);
  delay(1000);
  disp.clearDisplay();
  return;
}








//By Yien
float ChangeValueTemp(float xtemp) //Yi'en
{
  Serial.print("Press button K1 to change threshold temperature, button K2 to break.");
  Serial.println(F(" "));
  delay(500);
  while(1)
  {
    if(digitalRead(BUTTONK2) == 0)
    {
      return xtemp;
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
          return xtemp;
        }
      }
    }
  }
}
















//By Yien
float ChangeValueHumi(float yhumi) //Yi'en
{
  Serial.print("Press button K2 to change threshold humidity, button K1 to break.");
  Serial.println(F(" "));
  delay(500);
  while(1)
  {
    if(digitalRead(BUTTONK1) == 0)
    {
      return yhumi;
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
          return yhumi;
        }
      }
    }
  }
}
















void PlayBuzzer() //By Yi'en
{
  const int sirenMelody[] = {
262, // C4
  294, // D4
  330, // E4
  349, // F4
  392, // G4
  349, // F4
  330, // E4
  294, // D4
  262
}; //By Yi'en, array for musical notes for alarm
  const int toneDuration = 100;
  for(int i = 0; i < sizeof(sirenMelody)/sizeof(int); i++)
  {
    buz.playTone(sirenMelody[i], toneDuration);
    delay(50);
  }
  delay(300);
}








void buzzercondition() //By Yi'en
{
  float h, t;
  h = dht.readHumidity();
  t = dht.readTemperature();
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);
  PlayBuzzer();
  // Prioritize combined alert message
  if ((t > xtemp) && (h > yhumi)) {
    Serial.println(F("Please bring plant to a less humid and cooler environment. Press and hold button K1 to stop alarm."));
  }
  else if (h > yhumi) {
    Serial.println(F("Please bring plant to a less humid environment. Press and hold button K1 to stop alarm."));
  }
  else if (t > xtemp) {
    Serial.println(F("Please bring plant to a cooler environment. Press and hold button K1 to stop alarm."));
  }
  return;
}








// Made with <3 by Rayhan & Yien















































































