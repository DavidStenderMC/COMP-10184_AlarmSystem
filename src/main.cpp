// COMP-10184 – Mohawk College 
// AlarmSystem
//  
// This program uses a PIR sensor to detect motion to strigger an alarm like system. When initially run the
// alarm is in an enabled state and if the PIR sensor detects motion it will trigger the alarms countdown. Once,
// the countdown is active the user has 10 sedonds to push the button and disable the alarm. If countdown is not
// stopped the LED will stay permanently active. If the countdown is stopped the alarm becomes disabled and the
// user must push the button again to enable it.
// 
// @author  David Stender
// @id   000828140
// 
// I created this work and I have not shared it with anyone else.

/*
  I, David Stender, 000828140 certify that this material is my original work.
  No other person's work has been used without due acknowledgement.
*/

#include <Arduino.h>
 
// digital input pin for the PIR sensor
#define PIN_PIR D5 

// digital input pin for the button
#define PIN_BUTTON D6

// The value of the LED when it is on
#define LED_ON false

// The value of the LED when it is off
#define LED_OFF true

// The value of the button when it is pressed
#define BUTTON_PUSHED_STATE 0

// The value of the aalarm system state when it is disabled
#define ALARM_DISABLED  0

// The value of the alarm system state when it is enabled
#define ALARM_ENABLE    1

// The value of the alarm system state when it is counting down
#define ALARM_COUNTDOWN 2

// The value of the alarm system state when it is active
#define ALARM_ACTIVE    3

// The number of times the LED should blink during the ALARM_COUNTDOWN state
#define ALARM_COUNTDOWN_TIME 40

// The delay in milliseconds to wait after the button is pressed
#define BUTTON_DELAY 200

// The delay in milliseconds to wait after turning on the LED (for blinking the alarm during its countdown)
#define LED_ON_TIME_DELAY 50

// The delay in milliseconds to wait after turning off the LED (for blinking the alarm during its countdown)
#define LED_OFF_TIME_DELAY 200

// Holds the current state of the alarm system
int iAlarmState;
// Holds the current value of the alarm countdown timer
int alarmCountdownTimer;
// The state of the PIR sensor on the last loop
bool lastPIRState;
// The state of the PIR sensor on this loop
bool currentPIRState; 

// *********************************************************** 
// Reads the input for both the PIR sensor and the button. The input of the button will be used
// differently depending on the current state of the alarm system.
// 
void collectInputs(){
  // The PIR sensor will hold its output for about 2.5 seconds after it is detects motion.
  // The state is held here to help make the alarm system more stable after it detects motion. 
  lastPIRState = currentPIRState;
  currentPIRState = digitalRead(PIN_PIR);
  
  int buttonState = digitalRead(PIN_BUTTON);

  if(buttonState == BUTTON_PUSHED_STATE && (iAlarmState != ALARM_ACTIVE && iAlarmState != ALARM_ENABLE)){
    if(iAlarmState == ALARM_COUNTDOWN) {
      iAlarmState = ALARM_DISABLED;
    } else if(iAlarmState == ALARM_DISABLED){
      iAlarmState = ALARM_ENABLE;
      alarmCountdownTimer = ALARM_COUNTDOWN_TIME;
    }
    digitalWrite(LED_BUILTIN, LED_OFF);
    delay(BUTTON_DELAY);
  }
}

// *********************************************************** 
// Helps make the system more responsive then when using a normal delay. Wakes the device up
// every millisecond to check if any input has happened.
// 
// @param delayTime The time in milliseconds to delay the system
// 
void collectInputsWithDelay(int delayTime){
  for(int i=0; i<delayTime; i++) {
    collectInputs();
    delay(1);
  }
}

// *********************************************************** 
// Checks the current state of the alarm system and will perform different actions based on it.
// If the alarm sate is ALARM_ENABLE then the system will respond to motion events. If the
// alarm state is ALARM_COUNTDOWN then the system will blink the LED 4 times a second for 10
// seconds. If the alarm state is ALARM_ACTIVE then system is locked down until it is reset.
// If the alarm state is ALARM_DISABLED then the system will not respond to an motion events
// and must be enabled by pressing the button. 
// 
void checkAlarmState(){
  if(iAlarmState == ALARM_ENABLE && (lastPIRState == LOW && currentPIRState == HIGH)) {
    iAlarmState = ALARM_COUNTDOWN;
  } else if(iAlarmState == ALARM_COUNTDOWN){
    // Blinks the LED 4 times a second
    // The function collectInputsWithDelay() helps make the button
    // feel more responsive
    alarmCountdownTimer--;
    digitalWrite(LED_BUILTIN, LED_ON);
    collectInputsWithDelay(LED_ON_TIME_DELAY);
    digitalWrite(LED_BUILTIN, LED_OFF);
    collectInputsWithDelay(LED_OFF_TIME_DELAY);
    if(alarmCountdownTimer <= 0){
      iAlarmState = ALARM_ACTIVE;
    }    
  } else if(iAlarmState == ALARM_ACTIVE) {
    digitalWrite(LED_BUILTIN, LED_ON);
  } else if(iAlarmState == ALARM_DISABLED) {
    digitalWrite(LED_BUILTIN, LED_OFF);
  }
}
 
 
// ************************************************************* 
void setup() { 
  // configure the USB serial monitor 
  Serial.begin(115200); 
 
  // configure the LED output 
  pinMode(LED_BUILTIN, OUTPUT); 
  // PIR sensor is an INPUT 
  pinMode(PIN_PIR, INPUT); 
  // Button is an INPUT 
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  // Initialized the alarm variables
  lastPIRState = 0;
  currentPIRState = 0;
  iAlarmState = ALARM_ENABLE;
  alarmCountdownTimer = ALARM_COUNTDOWN_TIME;
  digitalWrite(LED_BUILTIN, LED_OFF);
} 
 
// ************************************************************* 
void loop() {
  collectInputs();
  checkAlarmState();
}