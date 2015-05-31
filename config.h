/*
 * Configuration variables for everything in the program
 */
 
// Red crystal fading variables
#define RED_CRYSTAL_MAX_BRIGHTNESS 200	//0-255
#define RED_CRYSTAL_MIN_BRIGHTNESS 0
#define RED_CRYSTAL_MIN_TIME 5000  // minimum time to fade in milliseconds
#define RED_CRYSTAL_MAX_TIME 10000
#define RED_CRYSTAL_CHANGE_INCREMENT 3 // incremental change in brightness each time the crystal led updates

// Monocle random timing variables
#define MONOCLE_MIN_TIME 60000
#define MONOCLE_MAX_TIME 180000
#define MONOCLE_SERVO_MIN_SPEED 4
#define MONOCLE_SERVO_MAX_SPEED 40
#define MONOCLE_SERVO_STARTING_POSITION 1 // 1-8
#define MONOCLE_FLASH_MIN_TIME 10000
#define MONOCLE_FLASH_MAX_TIME 120000

// Bottle random timing variables
#define BOTTLE_MIN_TIME 600000
#define BOTTLE_MAX_TIME 900000

#define BOTTLE_DOOR_OPEN_TIME 10000
#define BOTTLE_DOOR_SERVO_OPEN_POS 121
#define BOTTLE_DOOR_SERVO_CLOSED_POS 77
#define BOTTLE_DOOR_SERVO_SPEED 2
#define BOTTLE_MOVE_TIME 7000
#define BOTTLE_SERVO_INSIDE_POS 56
#define BOTTLE_SERVO_OUTSIDE_POS 122
#define BOTTLE_SERVO_SPEED_MIN 4
#define BOTTLE_SERVO_SPEED_MAX 10