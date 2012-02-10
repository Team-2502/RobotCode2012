#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Math.h"
#include "JoystickCallback.h"
#include "LSM303_I2C.h"
#define GET_FUNC(x) &Robot::x

class VisionSystem;

class ADXL345_PID : public ADXL345_I2C, PIDSource
{
public:
	ADXL345_PID(UINT8 moduleNumber, DataFormat_Range range=kRange_2G) : ADXL345_I2C(moduleNumber,range) { }
	
	double PIDGet() { return atan2(GetAccelerations().YAxis,GetAccelerations().XAxis)-1.5; } //86 degrees is "down"
};

/**
 * Our main robot class.
 */
class Robot : public SimpleRobot
{
public:
	/**
	 * Constructor.
	 */
	Robot();
	
	/**
	 * Destructor.
	 */
	~Robot();
	
	/**
	 * Method for operations during autonomous mode.
	 */
	void Autonomous();
	
	/**
	 * Method for teleop mode.
	 */
	void OperatorControl();
	
private:
	void toggleDiagnosticMode();
	void toggleAccel();
	void runWheels();
	void displayLSM();
	
	ADXL345_PID *accel;
	LSM303_I2C *lsm;
	VisionSystem *vs;
	Gyro *gyro;
	bool inDiagnosticMode;
	bool showAccel;
	
	PIDController *pid;
	
	JoystickWrapper *joystick1;
	JoystickWrapper *joystick2;
	JoystickCallback<Robot> *jc;
};

#endif // ROBOT_H
