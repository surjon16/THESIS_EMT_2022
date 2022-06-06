#include "LiquidCrystal.h"
#include "pitches.h"

// Global Constants with PIN values.
#define RELAY 8
#define BUTTON_01 9
#define BUTTON_02 10
#define BUZZER 11


// Initialize the object for the LiquidCrystal.
// This will be used to update the LCD.
LiquidCrystal LCD(7, 6, 5, 4, 3, 2);

// Global Variables for the blinking feature.
int set_time = 0;
long blink_rate = 0;
boolean blink_on = true;

// Global Array for the timer options.
String timer[4] = {
  "2mins",
  "3mins",
  "5mins",
  "8mins",
};

// This function contains all initializations
void setup() {

  // For testing purposes on the serial monitor,
  // we set the baudrate to 9600
  //  Serial.begin(9600);
  
  // Set the RELAY pin to OUTPUT since
  // we need to control the RELAY
  pinMode(RELAY, OUTPUT);
  
  // Set the BUTTON pins to INPUT since
  // we need to read the signal from the
  // buttons.
  pinMode(BUTTON_01, INPUT);
  pinMode(BUTTON_02, INPUT);

  // Use the LCD object from line #12
  // Set the LCD based on its hardware dimension / matrix size
  LCD.begin(16, 2);
  
  // The blink_rate variable gets the
  // current milliseconds to measure
  // the blinking.
  blink_rate = millis();
  
  // We initially, turn off the UVC lights
  turn_off_light();

  // Buzz the buzzer when the setup is done.
  buzz_start();

}

void loop() {
 
  // While the loop() function is running,
  // it while infinitely check if the BUTTONS
  // are pressed. HIGH - when button is pressed down.
  
  if(digitalRead(BUTTON_02) == HIGH) {
    // This condition will catch the SET TIMER button.
    // Every time this condition goes TRUE, it will
    // set the TIMER accordingly.

    // The set_time variable is used as a pointer
    // to the index value of the timer array.
    // Pushing the SET TIMER button will update the 
    // value of the set_time variable from 0 to 3 only.
    // If the value reaches 3, it resets to 0.
    if(set_time >= 3) set_time = 0;
    else set_time++;

    // We should the screen based on how many times
    // the SET TIMER button was pressed.
    // NOTE:
    // 0 - 2mins
    // 1 - 3mins
    // 2 - 5mins
    // 3 - 8mins
    updateScreen(
      "UV-C: OFF",
      String("Set Timer: " + timer[set_time])
    );

    // Exit this condition only after the button was released.
    while(digitalRead(BUTTON_02) == HIGH) {}
    delay(100);
  }

  // This button basically starts the timer and runs the UVC-Light.
  if(digitalRead(BUTTON_01) == HIGH) {    
    
    // Wait until the button is released.
    while(digitalRead(BUTTON_01) == HIGH) {}
    delay(100);

    // After the button was released, run the timer.
    run_timer();    
    delay(2000);  
  }

  // This condition is infinitely checked within the loop.
  // Its sole purpose is to blink the area of the LCD to
  // emphasize the timer text.
  if((millis() - blink_rate) >= 500) {

    // This is the expected text of the LCD when in standby-mode or
    // while the timer is being set.
    String cursor_1 = "UV-C: OFF";
    String cursor_2 = "Set Timer: ";

    // These conditional statements will set/unset the value on cursor 2
    // so that in 500 miliseconds, the LCD will look like it's blinking.
    if(blink_on) {
      cursor_2 = String(cursor_2 + timer[set_time]);
      blink_on = false;
    }
    else blink_on = true;

    // Update the LCD Screen based on the cursor values.
    updateScreen(
      cursor_1,
      cursor_2
    );    

    // The blink_rate variable here will reset to the current millisecond.
    blink_rate = millis();
  }

}

// This function contains the whole timer process.
void run_timer() {

  // Set the values for the current_time and total_time.
  int current_time = 0;
  int total_time = 0;

  // Compute the actual time based on the set_time value
  // that was set during the loop.
  // NOTE:
  // 0 - 2mins
  // 1 - 3mins
  // 2 - 5mins
  // 3 - 8mins
  if(set_time == 0)       total_time = 60*2; // 2 minutes.
  else if(set_time == 1)  total_time = 60*3; // 3 minutes.
  else if(set_time == 2)  total_time = 60*5; // 5 minutes.
  else if(set_time == 3)  total_time = 60*8; // 8 minutes.

  // Set the set_time value back to its default value.
  set_time = 0;

  // Use this function to turn on the UVC-Lights
  turn_on_light();
  // Buzz the buzzer to notify the user.
  buzz_start();

  // Update the screen also to notify the user.
  updateScreen(
    "UV-C: ON",
    "Starting"
  );

  // This delay is the buffer time to turn on all 4 UVC-Lights.
  delay(2000);

  // Loop through the value of the total_time that was calculated in the
  // previous lines.
  for(current_time = total_time; current_time > 0; current_time--) {

    // Update the screen with the current_time value
    // to notify the user.
    updateScreen(
      "UV-C: ON",
      String("Time Left: " + parse_timer(current_time))
    );

    // Buzz the buzzer every second to notify
    // user of the running timer.
    buzz_click();
    
  }

  // After looping through the total_time, turn off the UVC-Light
  turn_off_light();

  // Update the LCD to notify the user that the timer is finished.
  updateScreen(
    "UV-C: OFF",
    "Time Left: DONE."
  );

  // Buzz the buzzer to notify the user audibly.
  buzz_stop();

}

// This function is used to turn the RELAY on.
// The RELAY will control the UVC-Lights Power.
void turn_on_light() {
  digitalWrite(RELAY, LOW);
}

// This function is used to turn the RELAY off.
void turn_off_light() {
  digitalWrite(RELAY, HIGH);
}

// ======================================================================================
// The functions below are used to buzz the BUZZER
// On specific notes with specific speed to make the buzz start, 
// click and stop to be audibly identifiable on the user's end.
void buzz_start() {
    
  int melody[] = { NOTE_G7, NOTE_A7, 0 };
  int noteDurations[] = { 32, 4, 4 };
  int songLength = sizeof(melody)/sizeof(melody[0]);

  for (int thisNote = 0; thisNote < songLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
  }
  
}

void buzz_click() {
    tone(BUZZER, NOTE_G7, 32);
    delay(968);
//    Serial.println(millis());
    noTone(BUZZER);
}

void buzz_stop() {  
  
  int melody[] = { NOTE_G7, NOTE_A7, 0, NOTE_G7, NOTE_A7, 0, NOTE_G7, NOTE_A7 };
  int noteDurations[] = { 32, 8, 32, 32, 8, 32, 32, 8 };
  int songLength = sizeof(melody)/sizeof(melody[0]);

  for (int thisNote = 0; thisNote < songLength; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(BUZZER);
  }
  
}

// ======================================================================================
// This function below is used to update the LCD.
// Its other useful purpose is to lessen the lines
// of code within the source code.
void updateScreen(String cursor_1, String cursor_2) {
/*
 * This will update the LCD screen
 * Accepts Strings to display on first row and second row.
*/
  LCD.clear();  
  LCD.setCursor(0,0);
  LCD.print(cursor_1);
  LCD.setCursor(0,1);
  LCD.print(cursor_2);
}

// The parse_timer function is a useful code
// to convert the time from Integer to String.
// This is useful when you are trying to display
// the current time into the LCD.
String parse_timer(int current_time) {
/*
 * This will parse seconds into minutes
 * for a high-level display.
*/

  int minutes = current_time/60;
  int seconds = current_time - (minutes*60);

  String _minutes = String("0" + String(minutes));
  String _seconds = String(seconds);

  if(seconds < 10) _seconds = String("0" + _seconds);

  return String(_minutes + ":" + _seconds);

}
