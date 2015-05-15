#include "config.h"
#include "pins.h"

// Global Variables
bool g_lamp_status;

// Red Crystal Variables
int g_red_crystal_fading_time;
unsigned long g_red_crystal_last_update_time;
bool g_red_crystal_getting_brighter;
int g_red_crystal_led_current_brightness;

// Monocly Variables
int g_monocle_position;
unsigned long g_monocle_last_reset_time;
int g_monocle_trigger_time;

void setup() {
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
  g_red_crystal_fading_time = 0;
  g_red_crystal_last_update_time = 0;
  g_red_crystal_getting_brighter = true;
  g_red_crystal_led_current_brightness = 0;

  // Monocle
  g_monocle_position = 0;
  g_monocle_last_reset_time = 0;
};

void loop() {
  if( is_PIR_on() == true){
    big_window_on();
    lamp_on();
    red_crystal_update();
  } else {
    off_sequence();
  }

  monocle_update();
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
  analogWrite(BIG_WINDOW_LED_PIN, analogRead(BIG_WINDOW_POT_PIN));
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
  if(g_red_crystal_fading_time == 0){
     g_red_crystal_fading_time = random(RED_CRYSTAL_MIN_TIME, RED_CRYSTAL_MAX_TIME);  //fading time is in milliseconds
     g_red_crystal_last_update_time = millis();
  }
  float slope;
  unsigned long time_difference;
  int change;
  
  slope = RED_CRYSTAL_MAX_BRIGHTNESS / g_red_crystal_fading_time;
  time_difference = millis() - g_red_crystal_last_update_time;
  change = (int)(slope * time_difference);
  
  if (g_red_crystal_getting_brighter){
    if (g_red_crystal_led_current_brightness + change > RED_CRYSTAL_MAX_BRIGHTNESS) {
      g_red_crystal_led_current_brightness = RED_CRYSTAL_MAX_BRIGHTNESS;
      g_red_crystal_getting_brighter = false;
      g_red_crystal_fading_time = 0;
    } else {
      g_red_crystal_led_current_brightness += change;
    }
  } else {
    if (g_red_crystal_led_current_brightness - change > 0) {
      g_red_crystal_led_current_brightness = 0;
      g_red_crystal_getting_brighter = true;
      g_red_crystal_fading_time = 0;
    } else {
      g_red_crystal_led_current_brightness -= change;
    }
  }
  
  analogWrite(g_red_crystal_led_current_brightness, RED_CRYSTAL_LED_PIN);
}

void monocle_update() {
  /*
   *  decide what to do with the monocle and set any necessary outputs
   */
  if (g_monocle_trigger_time == -1) {
    g_monocle_trigger_time = random(MONOCLE_MIN_TIME, MONOCLE_MAX_TIME);
  }
  if (millis() >= g_monocle_last_reset_time + g_monocle_trigger_time) {
    g_monocle_last_reset_time = millis();
    g_monocle_trigger_time = -1;
    monocle_trigger();
  }
}

void monocle_trigger() {
  /*
   * The random eventhas occured. make the monocle do stuff
   */
  // @TODO: everything
  
}
