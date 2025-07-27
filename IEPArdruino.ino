/*
Names: Rayhan, Yien
IEP Project Implementation

Functions used:
Rayhan: Menu function to call (1),(5) and (6) via Serial Monitor
Progress/Updates: working on it

Rayhan: Debug function to output all IO and sensor values
Progress/Updates: working on it

(1) Rayhan: UI to input a time to water plant, displays the time on the segment when it the time comes and then rings a buzzer
Progress/Updates: done with code comments

(2) Rayhan: Once temperature or humidity is above x deg cel. or  x RH, as detected by temp sensor/humidity sensor, prompt user using yellow LED to enter an input using IR receiver in order to reduce the brightness of the blue LED
Progress/Updates:

(3) Yien: Once temperature or humidity is above y deg cel., or y RH, display temperature, ring the buzzer and blink red LED continuously until temperature decreases(Tells the user to bring plant to cooler env. in serial monitor and also allows the user to off the red LED and buzzer using remote) (note that x < y)
Progress/Updates:

(4) Rayhan: Green LED indicates good, blue LED represents the LED thing on our plant prototype, red only happens when temp/RH is above y value)
Progress/Updates:

(5) Yien: When user clicks on button K1, it allows user to set the threshold for x and y using knob, and the x and y value that is being set will be displayed on the 7 segment and serial monitor.
Progress/Updates: Completed already

(6) Yien:After pressing button K1, it displays temp. and RH to user when a button is pressed. (This one already have in notes but can still use to farm more marks.)
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

void setup() { //Setup by Yien
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

void loop() { // Loop by Rayhan
	// Background tasks
    double xtemp = 36, yhumi = 60;
    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (digitalRead(BUTTONK1) == LOW)
    {
        //By Yi’en
        K1(t, h);
        disp.clearDisplay();
        ChangeValueTemp(xtemp);
        ChangeValueHumi(yhumi);
        disp.clearDisplay();
    }
	
	// Call Menu Function
}

//By Rayhan
void menu(){
	Serial.print("*** Rayhan and Yien's *** ");
	Serial.println("*** Arduino IEP Code ***");
	Serial.println("*** Menu ***");
	Serial.println("* 1. Plant watering timer *");
	Serial.println("* 2.  *");
	Serial.println("* 3.  *");
	Serial.println("* 4. Debug Mode *");
	Serial.println("* Enter your choice: ");
}

//By Rayhan
void timer() {
    Serial.println(("Enter timer in seconds:"));
    while (Serial.available() == 0) { //wait}
    // Get User input
    long seconds = Serial.parseInt();
    // Notify user of Countdown 
    Serial.print(("Starting countdown: "));
    Serial.print(seconds);
    Serial.println((" s"));
    // Countdown notification
    while (seconds > 0) {
        Serial.print(seconds);
        Serial.println(F("..."));
        delay(1000);
        seconds--;
    }
    // Notify user of countdown completion
    Serial.println(F("Time's up! Press Button K2 to stop buzzer."));
    while (digitalRead(BUTTONK2) == HIGH) {
        buz.playTone(NOTE_M3, 500);
        delay(500);
        buz.noTone();
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
void K2(float t, float h) {
    if (digitalRead(BUTTONK2) == LOW) {
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
        while (digitalRead(BUTTONK2) == LOW);
    }
}
//By Yien
void ChangeValueTemp(double xtemp) {
    Serial.println(F("Press button K1 to change threshold temperature, button K2 to break."));
    delay(500);
    while (true) {
        if (digitalRead(BUTTONK2) == LOW) break;
        if (digitalRead(BUTTONK1) == LOW) {
            delay(500);
            while (true) {
                Serial.println(F("Press button K1 to stop changing temperature, use knob to adjust temperature."));
                xtemp = analogRead(KNOB_PIN) / 10.24;
                Serial.print(F("Threshold value : "));
                Serial.println(xtemp, 1);
                delay(200);
                displayTemperature((int8_t)xtemp);
                if (digitalRead(BUTTONK1) == LOW) {
                    Serial.print(F("New threshold temperature is: "));
                    Serial.println(xtemp);
                    return;
                }
            }
        }
    }
}
//By Yien
void ChangeValueHumi(double yhumi) {
    Serial.println(F("Press button K2 to change threshold humidity, button K1 to break."));
    delay(500);
    while (true) {
        if (digitalRead(BUTTONK1) == LOW) break;
        if (digitalRead(BUTTONK2) == LOW) {
            delay(500);
            while (true) {
                Serial.println(F("Press button K1 to stop changing humidity, use knob to adjust humidity."));
                yhumi = analogRead(KNOB_PIN) / 10.24;
                Serial.print(F("Threshold value : "));
                Serial.println(yhumi, 1);
                delay(200);
                displayHumidity((int8_t)yhumi);
                if (digitalRead(BUTTONK1) == LOW) {
                    Serial.print(F("New threshold humidity is: "));
                    Serial.println(yhumi);
                    return;
                }
            }
        }
    }
}














