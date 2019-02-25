#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "PID_lib.h"
#include "MAX6675_lib.h"
#include "Triac_lib.h"

#define PIN_INPUT 0
#define PIN_OUTPUT 3
#define UPDATE_PERIOD 300

void draw();

U8G2_SH1106_128X64_NONAME_F_4W_HW_SPI roasterDisplay(U8G2_R2, /* cs=*/ 10, /* dc=*/ 9, /* reset=*/ 8);
MAX6675 tc1(7);
MAX6675 tc2(6);
double exampleTemp = 25.2;

// double Setpoint, Input, Output;
// double Kp=2, Ki=5, Kd=1;
// PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, 0, 128, Direct);

void setup()
{
  // Input = analogRead(PIN_INPUT);
  // Setpoint = 100;

  // myPID.start();
  roasterDisplay.begin();
}

void loop()
{
  // Input = analogRead(PIN_INPUT);
  // myPID.compute();
  // analogWrite(PIN_OUTPUT, Output);
  draw();
  delay(UPDATE_PERIOD);
}

void draw()
{
  static double temp1, temp2, avg;

  roasterDisplay.clearBuffer();
  roasterDisplay.setFont(u8g_font_unifont);

  temp1 = tc1.readCelsius();
  temp2 = tc2.readCelsius();
  avg = (temp1 + temp2) / 2.0;

  roasterDisplay.drawStr(0, 10, "Temp1:      C");
  roasterDisplay.setCursor(53, 10);
  roasterDisplay.print(temp1, 2);

  roasterDisplay.drawStr(0, 30, "Temp2:      C");
  roasterDisplay.setCursor(53, 30);
  roasterDisplay.print(temp2, 2);

  roasterDisplay.drawStr(0, 50, "Avg:        C");
  roasterDisplay.setCursor(53, 50);
  roasterDisplay.print(avg, 2);

  roasterDisplay.sendBuffer();
}
