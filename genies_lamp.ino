#include "config.h"
#include "pins.h"

// Global Variables
bool g_lamp_status;

// Red Crystal Variables
int g_red_crystal_dt;
int g_red_crystal_getting_brighter;
int g_red_crystal_led_current_brightness;
unsigned long g_red_crystal_next_update_time;

// Monocly Variables
int g_monocle_position;
unsigned long g_monocle_last_reset_time;
long g_monocle_trigger_time;

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
  g_red_crystal_dt = 0;
  g_red_crystal_next_update_time = 0;

  // Monocle
  g_monocle_position = 0;
  g_monocle_trigger_time = -1;
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

  if ( g_red_crystal_dt == 0) {
    long fading_time = random(RED_CRYSTAL_MIN_TIME, RED_CRYSTAL_MAX_TIME);
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
      g_red_crystal_dt = 0;
    }
    if (g_red_crystal_led_current_brightness <= RED_CRYSTAL_MIN_BRIGHTNESS) {
      g_red_crystal_led_current_brightness = RED_CRYSTAL_MIN_BRIGHTNESS;
      g_red_crystal_getting_brighter = 1;
      g_red_crystal_dt = 0;
    }

    analogWrite(RED_CRYSTAL_LED_PIN, g_red_crystal_led_current_brightness);
  }
}

void monocle_update() {
  /*
   *  decide what to do with the monocle and set any necessary outputs
   */
  if (g_monocle_trigger_time < 0) {
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
