#include <Arduino.h>

/*
  I, David Stender, 000828140 certify that this material is my original work.
  No other person's work has been used without due acknowledgement.
*/
 
// digital input pin definitions 
#define PIN_PIR D5 
#define PIN_BUTTON D6 

int countDownTimer;
 
 
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

  countDownTimer = 0;
} 
 
// ************************************************************* 
void loop() {
  bool bPIR;
  // read PIR sensor (true = Motion detected!).  As long as there 
  // is motion, this signal will be true.  About 2.5 seconds after  
  // motion stops, the PIR signal will become false. 
  bPIR = digitalRead(PIN_PIR);

  if(bPIR) {
    for(int i=0; i<10; i++){
      Serial.println(String(i+1));
      digitalWrite(LED_BUILTIN, false);
      delay(1000);
    }
  } else {
    digitalWrite(LED_BUILTIN, true);
  }
 
  
 
  // send the PIR signal directly to the LED 
  // but invert it because true = LED off!
  //digitalWrite(LED_BUILTIN, !bPIR);
}