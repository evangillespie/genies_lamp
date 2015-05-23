
#include "config.h"
#include "pins.h"

#include <VarSpeedServo.h>


// Global Variables
bool g_lamp_status;


// Red Crystal Variables
int g_red_crystal_dt;
int g_red_crystal_getting_brighter;
int g_red_crystal_led_current_brightness;
unsigned long g_red_crystal_next_update_time;


// Monocle Variables
int g_monocle_position;
unsigned long g_monocle_last_reset_time;
long g_monocle_trigger_time;
int g_monocle_servo_state;
bool g_monocle_flash_state;
unsigned long g_monocle_flash_tigger_time;
VarSpeedServo monocle_servo;


// Bottle Variables
int g_bottle_position;
unsigned long g_bottle_last_reset_time;
long g_bottle_trigger_time;
VarSpeedServo bottle_door_servo;
VarSpeedServo bottle_servo;


void setup() {

  Serial.begin(9600);

  randomSeed(analogRead(1));
  
  pinMode(PIR_SENSOR_PIN, INPUT);
  
  // Big Window
  pinMode(BIG_WINDOW_LED_PIN, OUTPUT); 
  analogWrite(BIG_WINDOW_LED_PIN, 0);
   
  // Lamp
  pinMode(LAMP_LED_PIN, OUTPUT);
  digitalWrite(LAMP_LED_PIN, LOW);
  g_lamp_status = false; 
  
  // Red Crystal
  g_red_crystal_getting_brighter = 1;
  g_red_crystal_led_current_brightness = RED_CRYSTAL_MIN_BRIGHTNESS;
  g_red_crystal_dt = -1;
  g_red_crystal_next_update_time = 0;

  // Monocle
  g_monocle_position = 0;
  g_monocle_trigger_time = -1;
  g_monocle_last_reset_time = 0;
  pinMode(MONOCLE_FLASH_LED_PIN, OUTPUT);
  digitalWrite(MONOCLE_FLASH_LED_PIN, LOW);
  g_monocle_flash_state = false;
  g_monocle_flash_tigger_time = 0;
  monocle_servo.attach(MONOCLE_SERVO_PIN);
  monocle_move_servo_to_position_at_random_speed(MONOCLE_SERVO_STARTING_POSITION);

  // Bottle
  g_bottle_position = 0;
  g_bottle_trigger_time = -1;
  g_bottle_last_reset_time = 0;
  bottle_door_servo.write(BOTTLE_DOOR_SERVO_CLOSED_POS, BOTTLE_DOOR_SERVO_SPEED, false);

}


void loop() {
  if( is_PIR_on() == true){
    big_window_on();
    lamp_on();
    red_crystal_update();
  } else {
    off_sequence();
  }

  monocle_update();
  bottle_update();

  delay(100);
}


bool is_PIR_on() {
  /*
   * return true if the pir is on
   */
   // @TODO: get real value
   return true;  
}


void off_sequence() {
  /*
   * Shut everything down
   */
  // @TODO: everything  
}


void big_window_on() {
  /*
   * Ensure the big window led is on and update to pot value
   */
  analogWrite(BIG_WINDOW_LED_PIN, analogRead(BIG_WINDOW_POT_PIN) / 4);
}


void lamp_on() {
  /*
   * Turn the lamp led on
   */
  if (g_lamp_status == false){
    g_lamp_status = true;
    digitalWrite(LAMP_LED_PIN, HIGH);
  }
}


void red_crystal_update() {
  /*
   * update the red crystal in it's fading scheme
   */   

  if ( g_red_crystal_dt < 0) {
    long fading_time = random(RED_CRYSTAL_MIN_TIME, RED_CRYSTAL_MAX_TIME + 1);
    g_red_crystal_dt = (RED_CRYSTAL_CHANGE_INCREMENT * fading_time) / 
            (RED_CRYSTAL_MAX_BRIGHTNESS - RED_CRYSTAL_MIN_BRIGHTNESS);
    g_red_crystal_next_update_time = millis() + (unsigned long)g_red_crystal_dt;
  }

  if (millis() >= g_red_crystal_next_update_time) {
    
    g_red_crystal_led_current_brightness = g_red_crystal_led_current_brightness + 
            (RED_CRYSTAL_CHANGE_INCREMENT * g_red_crystal_getting_brighter);
    g_red_crystal_next_update_time = g_red_crystal_next_update_time + g_red_crystal_dt;
  
    if (g_red_crystal_led_current_brightness >= RED_CRYSTAL_MAX_BRIGHTNESS) {
      g_red_crystal_led_current_brightness = RED_CRYSTAL_MAX_BRIGHTNESS;
      g_red_crystal_getting_brighter = -1;
      g_red_crystal_dt = -1;
    }
    if (g_red_crystal_led_current_brightness <= RED_CRYSTAL_MIN_BRIGHTNESS) {
      g_red_crystal_led_current_brightness = RED_CRYSTAL_MIN_BRIGHTNESS;
      g_red_crystal_getting_brighter = 1;
      g_red_crystal_dt = -1;
    }

    analogWrite(RED_CRYSTAL_LED_PIN, g_red_crystal_led_current_brightness);
  }
}


void monocle_update() {
  /*
   *  decide what to do with the monocle and set any necessary outputs
   */
  if (g_monocle_trigger_time < 0) {
    g_monocle_trigger_time = random(MONOCLE_MIN_TIME, MONOCLE_MAX_TIME + 1);
  }
  if (millis() >= g_monocle_last_reset_time + g_monocle_trigger_time) {
    g_monocle_last_reset_time = millis();
    g_monocle_trigger_time = -1;

    if (is_PIR_on()) {
      monocle_trigger();
    }
  }

  if (is_PIR_on()){
    //update the camera flash
    if (g_monocle_servo_state == 7) {
      // flash only triggers at state 7
      if (g_monocle_flash_state == true){
        digitalWrite(MONOCLE_FLASH_LED_PIN, LOW);
        g_monocle_flash_state = false;
      } else {
        if (g_monocle_flash_tigger_time == 0){
          g_monocle_flash_tigger_time = millis() + (unsigned long)random(MONOCLE_FLASH_MIN_TIME, MONOCLE_FLASH_MAX_TIME + 1);
        }

        if (millis() >= g_monocle_flash_tigger_time){
          g_monocle_flash_state = true;
          digitalWrite(MONOCLE_FLASH_LED_PIN, HIGH);
          g_monocle_flash_tigger_time = 0;
        }
      }
    } else {
      digitalWrite(MONOCLE_FLASH_LED_PIN, LOW);
      g_monocle_flash_tigger_time = 0;
      g_monocle_flash_state = false;
    }
  }
}


void monocle_trigger() {
  /*
   * The random event has occured. make the monocle do stuff
   */
  int r_monocle_servo_state = g_monocle_servo_state;
  while (g_monocle_servo_state == r_monocle_servo_state) {
    r_monocle_servo_state = random(1, 9);
  }
  g_monocle_servo_state = r_monocle_servo_state;

  monocle_move_servo_to_position_at_random_speed(g_monocle_servo_state);
}


void monocle_move_servo_to_position_at_random_speed(int position_state) {
  /*
   *  Move the monocle servo to a given position_state at some random speed
   *
   *  @param position_state: integer representation of the position state
   */
  int speed = random(MONOCLE_SERVO_MIN_SPEED, MONOCLE_SERVO_MAX_SPEED + 1);
  switch(position_state) {
    case 1:
      monocle_servo.write(65, speed, false);
      break;
    case 2:
      monocle_servo.write(70, speed, false);
      break;
    case 3:
      monocle_servo.write(79, speed, false);
      break;
    case 4:
      monocle_servo.write(88, speed, false);
      break;
    case 5:
      monocle_servo.write(98, speed, false);
      break;
    case 6:
      monocle_servo.write(111, speed, false);
      break;
    case 7:
      monocle_servo.write(125, speed, false);
      break;
    case 8:
      monocle_servo.write(141, speed, false);
      break;
  }
}

void bottle_update() {
  /*
   *  Decide when to make the bottle actions happen
   */
  if (g_bottle_trigger_time < 0) {
    g_bottle_trigger_time = random(BOTTLE_MIN_TIME, BOTTLE_MAX_TIME+1);
  }

  if (millis() >= g_bottle_last_reset_time + g_bottle_trigger_time) {
    g_bottle_last_reset_time = millis();
    g_bottle_trigger_time = -1;
    if (is_PIR_on()){
      bottle_trigger();
    }
  }
}

void bottle_trigger() {
  /*
   *  Make the bottle do its actions
   */
  
  // open door
  bottle_door_servo.write(BOTTLE_DOOR_SERVO_OPEN_POS, BOTTLE_DOOR_SERVO_SPEED, false);
  

}
