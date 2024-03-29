/*
  Universal Reminder by Dominic Gerber
  see also: https://github.com/dogerber/universal_reminder

  Displays the current date and if a task has already been done or not. Tasks are triggered by times of the day.
  In between the device goes to sleep to save energy. 
*/

#include "ArduinoLowPower.h"    // for deep sleep, see https://www.arduino.cc/reference/en/libraries/arduino-low-power/
#include "Adafruit_ThinkInk.h"  // for the eInk screen
#include <Wire.h>
#include <RTClib.h>  // for RTC


// Definitions for Display
#define EPD_DC 10     // can be any pin, but required!
#define EPD_CS 9      // can be any pin, but required!
#define EPD_BUSY -1   // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 6     // can set to -1 to not use a pin (uses a lot of RAM!)
#define EPD_RESET -1  // can set to -1 and share with chip Reset (can't deep sleep)
#define COLOR1 EPD_BLACK
#define COLOR2 EPD_LIGHT
#define COLOR3 EPD_DARK
#define FONT_NAME FreeSans9pt7b
#define FONT_NAME2 FreeSansBold9pt7b
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

ThinkInk_290_Grayscale4_T5 display(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

RTC_PCF8523 rtc;  // real time clock object


// Pin used to trigger a wakeup (tilt sensor)
const int PIN_TILT_SENSOR = 5;

// variables
volatile bool woke_through_interrupt = false;
volatile int tasks_to_do = 0;
volatile int tasks_to_do_before = -1;
int time_to_wait_min = 0;

int times[] = { 500,};  // times where doing needs to be done, Represented as HHMM, in ascending order


// swissgerman char daysOfTheWeek[7][12] = { "Sunntig", "Maentig", "Zistig", "Mittwuch", "Dunnstig", "Fritig", "Samstig" };
char daysOfTheWeek[7][12] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// messages to display
char str_all_done[] = "[x] all tasks done";
char str_one_undone[] = "[ ] one task undone";
char str_multiple_undone[] = "[[  ]] multiple tasks undone";



void change_detected() {
  // called on interrupt through the tilt sensor
  if (!woke_through_interrupt && tasks_to_do !=0) {
    woke_through_interrupt = true;
    tasks_to_do = 0;
  }
}


// Function to find the closest time in the future from the given time, input in HHMM
int findClosestTime(int currentTime) {
  int closestTime = -1;  // return -1 for no times in the future
  int timeDifference = 2500;

  for (int i = 0; i < sizeof(times) / sizeof(times[0]); i++) {
    int tempDifference = times[i] - currentTime;
    if ((tempDifference < timeDifference) && (tempDifference > 0)) {
      closestTime = times[i];
      timeDifference = tempDifference;
    }
  }
  return closestTime;
}



void update_display() {
  // check if anything changed
  if (true) { //was: tasks_to_do != tasks_to_do_before
    woke_through_interrupt = true;  // prevents interruption (?)
    display.clearBuffer();          // clear screen

    // print date on display
    if (true) {
      DateTime now = rtc.now();
      display.setFont(&FONT_NAME);
      display.setCursor(50, 15);
      display.setTextSize(1);
      display.print(daysOfTheWeek[now.dayOfTheWeek()]);
      display.print(" - ");
      display.print(now.day());
      display.print(".");
      display.print(now.month());
      display.print(".");
      display.print(now.year());
      display.print(".");
      display.drawLine(0, 20, 300, 20, COLOR1);
    }

    // write how many tasks are to do
    display.setCursor(20, 68);
    display.setTextSize(1);

    if (tasks_to_do == 0) {
      display.setFont(&FONT_NAME);  // normal
      display.println(str_all_done);
    } else if (tasks_to_do == 1) {
      display.setFont(&FONT_NAME2);  // bold
      display.println(str_one_undone);
    } else {
      display.setFont(&FONT_NAME2);  // bold
      display.println(str_multiple_undone);
    }



    // add info for next wakeup
    if (true) {  //todo add tasks_to_do == 0 &&
      // determine next time
      DateTime now = rtc.now();
      DateTime next_task = now + TimeSpan(0, time_to_wait_min / 60, time_to_wait_min % 60, 0);

      // write to screen
      display.setCursor(30, 120);
      display.setFont(&FONT_NAME);
      display.print(" next task: ");
      if (now.day() != next_task.day()) {
        display.print("tomorrow, ");
      }
      display.print(next_task.hour());
      display.print(":");
      if (next_task.minute() < 10) { display.print("0"); }
      display.print(next_task.minute());

      // write when last updated
      if (false) {
        display.setCursor(30, 90);
        display.print("last done:");
        display.print(daysOfTheWeek[now.dayOfTheWeek()]);
        display.print(" - ");
        display.print(now.day());
        display.print(".");
        display.print(now.month());
        display.print(".");
        display.print(now.hour());
        display.print(":");
        display.print(now.minute());
      }

      // debug: output informations
      if (false) {
        display.setCursor(30, 90);
        display.setFont(&FONT_NAME);
        display.print("upd");
        display.print(now.hour());
        display.print(":");
        display.print(now.minute());
        display.print(", wait");
        display.print(time_to_wait_min);
        display.print(", ttd");
        display.print(tasks_to_do);
        display.print(", ttdb");
        display.print(tasks_to_do_before);
      }
    }




    // reset flags
    tasks_to_do_before = tasks_to_do;

    display.display();
  }

  // reset flag
  woke_through_interrupt = false;
}

void setup() {
  Serial.begin(115200);

  // set pin Modes
  pinMode(PIN_TILT_SENSOR, INPUT);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Attach a wakeup interrupt for tilt sensor
  LowPower.attachInterruptWakeup(digitalPinToInterrupt(PIN_TILT_SENSOR), change_detected, CHANGE);

  // start screen
  display.begin(THINKINK_GRAYSCALE4);
  display.clearBuffer();
  display.setTextSize(1);
  display.setTextColor(EPD_BLACK, EPD_WHITE);
  display.setFont(&FONT_NAME);

  // setup RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    display.setCursor(10, 64);
    display.print("Could not find RTC, lost power or missing?");
    display.display();
    while (1)
      ;
  }

  // display welcome message
  display.setCursor(10, 64);
  display.print("Universal Reminder v1.0");
  display.setCursor(20, 84);
  display.display();
  delay(500);

  // Infos to Serial
  Serial.println("");
  Serial.println("Started with code universal_reminder_code.ino by Dominic Gerber");
  Serial.println("see online www.github.com/dogerber/universal_reminder");
  Serial.println("");
}



void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  if (!woke_through_interrupt) {
    tasks_to_do++;
    if (tasks_to_do > 2) { tasks_to_do = 2; }  // cap to prevent refreshes
  } else {
    // loop started through interrupt, do no tadd a task
<<<<<<< HEAD
    woke_through_interrupt = false;  // reset flag
=======
    woke_through_interrupt = false; // reset flag
>>>>>>> 0b7f7f53a27ccd6c19c0a016620ad6031855e2a6
  }

  // get time from rtc
  DateTime now = rtc.now();

  // Get the current time
  int currentHour = now.hour();
  int currentMinute = now.minute();
  int currentTime = currentHour * 100 + currentMinute;

  // Find the closest time in the future
  int closestTime = findClosestTime(currentTime);

  // handle case where no more reminders for today
  if (closestTime < 0) {
    closestTime = 2400 + times[0];
  }

  // calculate time to wait for next scheduled task
  time_to_wait_min = (closestTime / 100 - currentTime / 100) * 60 + (closestTime % 100 - currentTime % 100);

  // update screen
  update_display();

  //delay(6000);  //for debugging
  digitalWrite(LED_BUILTIN, LOW);
  LowPower.deepSleep(time_to_wait_min * 60 * 1000);
}
