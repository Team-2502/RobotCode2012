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

//Wiring constants
const unsigned DIGITAL_SIDECAR_SLOT = 1;       //Jaguar
const unsigned DRIVE_LEFT_CHANNEL = 1;         //Jaguar
const unsigned DRIVE_RIGHT_CHANNEL = 2;        //Jaguar
const unsigned SHOOTER_BOTTOM_JAG_CHANNEL = 3; //Jaguar
const unsigned SHOOTER_TOP_JAG_CHANNEL = 4;    //Jaguar
const unsigned TURRET_CHANNEL = 5;             //Victor
const unsigned COLLECTOR_IN_CHANNEL = 6;       //Victor
const unsigned COLLECTOR_UP_CHANNEL = 7;       //Victor

const unsigned RAMP_LEFT_SPIKE = 1;
const unsigned RAMP_RIGHT_SPIKE = 2;

const unsigned IR_BOTTOM_DIO            = 1;
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

#endif // CONSTANTS_H 
