#ifndef CONSTANTS_H
#define CONSTANTS_H

//Physical constants (in U.S. units, not metric)
const double BALL_RADIUS = 8.0 / 12.0;
const double BASKET_RADIUS = 18.0 / 12.0;
const double BASKET_1_ELEVATION = 3.0;
const double BASKET_2_ELEVATION = 5.0;
const double BASKET_3_ELEVATION = 8.0;
const double SHOOTER_ELEVATION = 4.0;
const double SHOOTER_ANGLE = 45.0;
const double MAX_SHOOTER_SPEED = 25.0;
const double BASKET_BACKBOARD_HEIGHT = 22.0 / 12.0;
const double CAMERA_FOCAL_LENGTH = 100.0; ///@todo this needs to be determined experimentally

//Wiring constants
const unsigned DIGITAL_SIDECAR_SLOT = 1;       //Jaguar
const unsigned DRIVE_LEFT_CHANNEL = 1;         //Jaguar
const unsigned DRIVE_RIGHT_CHANNEL = 2;        //Jaguar
const unsigned SHOOTER_BOTTOM_JAG_CHANNEL = 3; //Jaguar
const unsigned SHOOTER_TOP_JAG_CHANNEL = 4;    //Jaguar
const unsigned TURRET_CHANNEL = 5;             //Victor
const unsigned COLLECTOR_GRABBER_CHANNEL = 6;  //Victor
const unsigned COLLECTOR_LIFTER_CHANNEL = 7;   //Victor


const unsigned RAMP_LEFT_SPIKE = 1;
const unsigned RAMP_RIGHT_SPIKE = 2;

const unsigned IR_FRONT_DIO             = 1;
const unsigned IR_MIDDLE_DIO            = 2;
const unsigned IR_TOP_DIO               = 3;
const unsigned DRIVE_LEFT_ENCODER_A     = 4;
const unsigned DRIVE_LEFT_ENCODER_B     = 5;
const unsigned DRIVE_RIGHT_ENCODER_A    = 6;
const unsigned DRIVE_RIGHT_ENCODER_B    = 7;
const unsigned SHOOTER_BOTTOM_ENCODER_A = 8;
const unsigned SHOOTER_BOTTOM_ENCODER_B = 9;
const unsigned SHOOTER_TOP_ENCODER_A    = 10;
const unsigned SHOOTER_TOP_ENCODER_B    = 11;
const unsigned TURRET_ENCODER_A         = 12;
const unsigned TURRET_TOP_ENCODER_B     = 13;

const unsigned BALL_VISIBLE						= 1;
const unsigned BALL_NOT_VISIBLE					= 0;
const unsigned MAX_BALLS						= 2;
const double   COLLECTOR_RUNFAST				= 0.5;
const double   COLLECTOR_RUNSLOW				= 0.2;
const double   COLLECTOR_RUNFAST_REVERSE		= -0.5;
const double   COLLECTOR_RUNSLOW_REVERSE		= -0.2;
const double   COLLECTOR_STOP					= 0.0;
const double   COLLECTOR_FRONT_SIGNAL_VOLTAGE 	= 0.4;
const double   COLLECTOR_MIDDLE_SIGNAL_VOLTAGE 	= 2.5;
const double   COLLECTOR_TOP_SIGNAL_VOLTAGE 	= 2.5;

#endif // CONSTANTS_H 
