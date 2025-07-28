/*
Names: Rayhan, Yien
IEP Project Implementation

Functions used:
Rayhan: Menu function to call (0.5),(1),(5) and (6) via Serial Monitor
Progress/Updates: done

(0.5) Rayhan: Debug function to output all IO and sensor values
Progress/Updates: started

(1) Rayhan: UI to input a time to water plant, displays the time on the segment when it the time comes and then rings a buzzer
Progress/Updates: done with code comments

(2) Yien: Once temperature or humidity is above y deg cel., or y RH, display temperature, ring the buzzer and blink red LED continuously until temperature decreases(Tells the user to bring plant to cooler env. in serial monitor and also allows the user to off the red LED and buzzer using remote) (note that x < y)
Progress/Updates: (doing rn)

(3) Rayhan: Green LED indicates good, blue LED represents the LED thing on our plant prototype, red only happens when temp/RH is above y value)
Progress/Updates: started

(4) Yien: When user clicks on button K1, it allows user to set the threshold for x and y using knob, and the x and y value that is being set will be displayed on the 7 segment and serial monitor.
Progress/Updates: Completed already

(5) Yien:After pressing button K1, it displays temp. and RH to user when a button is pressed. (This one already have in notes but can still use to farm more marks.)
Progress/Updates: Completed already
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
}

//function prototypes
void K1(float t, float h);
void displayTemperature(int8_t temperature);
void displayHumidity(int8_t h);
void timer();
void ChangeValueTemp(double xtemp);
void ChangeValueHumi(double yhumi);

//By Rayhan
void loop() {
    // Update global sensor readings
    t = dht.readTemperature();
    h = dht.readHumidity();

    // Show the menu only once until a task has finished
    if (!menuShown) {
        Serial.println("*** Rayhan and Yien's *** Arduino IEP Code ***");
        Serial.println("* * * Menu * * *");
        Serial.println("*  Plant watering timer (K2)*");
        Serial.println("*  Display temperature/humidity & change thresholds (K1)*");
        Serial.println("*  Debug Mode (type \"debug\" in Serial monitor)*");
        Serial.println("Press a button to select...");
        menuShown = true;
    }

    // Check buttons each time through loop()
    if (digitalRead(BUTTONK2) == LOW) {
        timer();
        menuShown = false;         // re‑show menu after function completes
    }
    else if (digitalRead(BUTTONK1) == LOW) {
        K1(t, h);                  // use updated global t and h
        ChangeValueTemp(xtemp);
        ChangeValueHumi(yhumi);
        menuShown = false;         // re‑show menu after finishing
    }

    // Optional: add a small delay to debounce the buttons
    delay(50);
}

//By Rayhan
void menu(){
	int choice = 0;
	// main output
	Serial.print("*** Rayhan and Yien's *** ");
	Serial.println("*** Arduino IEP Code ***");
	Serial.println("* * * Menu * * *");
	Serial.println("*  Plant watering timer (BK2)*");
	Serial.println("*  Display temperature and humidity, and also to change threshold values. (BK1)*");
	Serial.println("*  Debug Mode (Serial Only, enter debug to access)*");
	Serial.println("* Enter your choice: ");
	// determine which function to call
	if (digitalRead(BUTTONK2) == 0){timer();} // conditions to call timer()
	else if (digitalRead(BUTTONK1) == 0){
		//By Yi'en
    K1(t, h);
    disp.clearDisplay();
    ChangeValueTemp(xtemp);
    ChangeValueHumi(yhumi);
    disp.clearDisplay();
	} //
	
	
	
}

void debug(){
  // output humidity, temperature, potentiometer, button, LED values
  Serial.println("raw temp: ");
  Serial.println(t);
  Serial.println("raw humidity: ");
  Serial.println(h);
  Serial.println("Potentiometer value: ");
  Serial.println(analogRead(KNOB_PIN));
  if (digitalRead(BUTTONK1) == 0){Serial.println("BK1 pressed!");}
  if (digitalRead(BUTTONK1) == 0){Serial.println("BK1 pressed!");}
}

//By Rayhan
void timer() {
    Serial.println("Enter timer in seconds:");
    // Wait for the user to type something in the Serial Monitor
    while (Serial.available() == 0) {
        // just waiting
    }

    long seconds = Serial.parseInt();
    Serial.print("Starting countdown: ");
    Serial.print(seconds);
    Serial.println(" s");

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
        // playTone() blocks for the specified duration and then stops on its own
        buz.playTone(120, 500);
        delay(500);
        // Optionally ensure the buzzer is off
        buz.off();
        delay(500);
    }
    Serial.println(F("Buzzer stopped."));
}

//By Rayhan
void displayTemperature(int8_t temperature) {
    int8_t temp[4];
    temp[0] = temperature / 10;
    temp[1] = temperature % 10;
    temp[2] = 17; // space
    temp[3] = 12; // 'C'
    disp.display(temp);
}
//By Rayhan
void displayHumidity(int8_t h) {
    int8_t hume[4];
    hume[0] = h / 10;
    hume[1] = h % 10;
    hume[2] = 17; // space
    hume[3] = 18; // 'H'
    disp.display(hume);
}

//By Yien
void K1(float t, float h) { //Code to display temp. and humi
    if (digitalRead(BUTTONK1) == 0) {
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
        while (digitalRead(BUTTONK1) == 0);
    }
}
//By Yien
void ChangeValueTemp(double xtemp) { //Code to change threshold for temp
    Serial.println("Press button K1 to change threshold temperature, button K2 to break.");
    delay(500);
    while (true) {
        if (digitalRead(BUTTONK2) == 0) break;
        if (digitalRead(BUTTONK1) == 0) {
            delay(500);
            while (true) {
                Serial.println("Press button K1 to stop changing temperature, use knob to adjust temperature.");
                xtemp = analogRead(KNOB_PIN) / 10.24;
                Serial.print("Threshold value : ");
                Serial.println(xtemp, 1);
                delay(200);
                displayTemperature((int8_t)xtemp);
                if (digitalRead(BUTTONK1) == 0) {
                    Serial.print("New threshold temperature is: ");
                    Serial.println(xtemp);
                    return;
                }
            }
        }
    }
}
//By Yien
void ChangeValueHumi(double yhumi) { //Code to change threshold for humi
    Serial.println("Press button K2 to change threshold humidity, button K1 to break.");
    delay(500);
    while (true) {
        if (digitalRead(BUTTONK1) == 0) 
        {
            break;
        }
        if (digitalRead(BUTTONK2) == 0) {
            delay(500);
            while (true) {
                Serial.println("Press button K1 to stop changing humidity, use knob to adjust humidity.");
                yhumi = analogRead(KNOB_PIN) / 10.24;
                Serial.print(F("Threshold value : "));
                Serial.println(yhumi, 1);
                delay(200);
                displayHumidity(yhumi);
                if (digitalRead(BUTTONK1) == 0) {
                    Serial.print("New threshold humidity is: ");
                    Serial.println(yhumi);
                    return;
                }
            }
        }
    }
}




