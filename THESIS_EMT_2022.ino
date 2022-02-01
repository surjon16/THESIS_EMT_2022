#include "LiquidCrystal.h"
#include "pitches.h"

#define RELAY 8
#define BUTTON_01 9
#define BUTTON_02 10
#define BUZZER 11

LiquidCrystal LCD(7, 6, 5, 4, 3, 2);

int set_time = 0;
long blink_rate = 0;
boolean blink_on = true;

void setup() {

  Serial.begin(9600);
  
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON_01, INPUT);
  pinMode(BUTTON_02, INPUT);

  LCD.begin(16, 2);
  blink_rate = millis();
  turn_off_light();

}

void loop() {

  if(digitalRead(BUTTON_02) == HIGH) {
    if(set_time >= 60) set_time = 0;
    set_time += 5;

    LCD.clear();
    
    LCD.setCursor(0,0);
    LCD.print("UV-C: OFF");
    
    LCD.setCursor(0,1);
    LCD.print("Set Timer: ");    
    LCD.print(set_time);
    LCD.print("s");
    
    while(digitalRead(BUTTON_02) == HIGH) {}
    delay(100);
  }

  if(digitalRead(BUTTON_01) == HIGH) {
    while(digitalRead(BUTTON_01) == HIGH) {}
    delay(100);
    if(set_time == 0) {       
      LCD.clear();
      LCD.setCursor(0,0);
      LCD.print("Invalid Time.");
    }
    else run_timer();    
    delay(2000);  
  }

  if((millis() - blink_rate) >= 500) {
  
    LCD.clear();
    
    LCD.setCursor(0,0);
    LCD.print("UV-C: OFF");
    
    LCD.setCursor(0,1);
    LCD.print("Set Timer: ");
    
    if(blink_on) {
      LCD.print(set_time);
      LCD.print("s");
      blink_on = false;
    }
    else blink_on = true;
   
    blink_rate = millis();
  }

}

void run_timer() {
  
  int current_time = 0;
  int total_time = set_time;
  set_time = 0;

  turn_on_light();
  buzz_start();

  LCD.clear();
  
  LCD.setCursor(0,0);
  LCD.print("UV-C: ON");
  
  LCD.setCursor(0,1);
  LCD.print("Starting.");

  delay(2000);

  for(current_time = total_time; current_time > 0; current_time--) {
    
    LCD.clear();
    
    LCD.setCursor(0,0);
    LCD.print("UV-C: ON");
    
    LCD.setCursor(0,1);
    LCD.print("Time Left: ");    
    LCD.print(current_time);
    LCD.print("s");

    buzz_click();
    
  }

  turn_off_light();

  LCD.clear();
  
  LCD.setCursor(0,0);
  LCD.print("UV-C: OFF");
  
  LCD.setCursor(0,1);
  LCD.print("Time Left: ");
  LCD.print("DONE.");
  
  buzz_stop();

}

void turn_on_light() {
  digitalWrite(RELAY, LOW);
}

void turn_off_light() {
  digitalWrite(RELAY, HIGH);
}

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
    Serial.println(millis());
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
