#include <Wire.h>

#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"
#define CLK 10//CLK of the TM1637 IC connect to D10 of OPEN-SMART UNO R3
#define DIO 11//DIO of the TM1637 IC connect to D11 of OPEN-SMART UNO R3
TM1637 disp(CLK,DIO);

DHT dht;

void setup() {
	disp.init();  
	dht.begin();
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  displayTemperature((int8_t)t);//
  delay(3000);
  displayHumidity((int8_t)h);//
  delay(2000);
}
/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */

void displayTemperature(int8_t temperature)
{
  int8_t temp[4];
  temp[0] = temperature/100;
	temperature %= 100;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
}

void displayHumidity(int8_t humi)
{
  int8_t temp[4];
  temp[0] = humi/100;
  humi %= 100;
  temp[1] = humi / 10;
  temp[2] = humi % 10;
  temp[3] = 18;	          //index of 'H' for celsius degree symbol.
  disp.display(temp);
}