#ifndef ROBOT_H
#define ROBOT_H

#include <WPILib.h>
#include "Math.h"
#include "JoystickCallback.h"
#include "LSM303_I2C.h"
#define GET_FUNC(x) &Robot::x

class VisionSystem;

class AccelPID_Wrapper : public PIDSource
{
public:
	AccelPID_Wrapper(ADXL345_I2C *member) { internal = member; }
	virtual ~AccelPID_Wrapper() { delete internal; }
	
	double PIDGet() { return atan2(internal->GetAccelerations().YAxis,internal->GetAccelerations().XAxis)-1.5; } //86 degrees is "down"
	
private:
	ADXL345_I2C *internal;
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
	void balanceRobotOff();
	void balanceRobotOn();
	void RampDown();
	void RampUp();
	void CollectorEject();
	void RampOff();
	
	enum ButtonMappings { BalanceRobot = 1 };
	
	AccelPID_Wrapper *accel;
	VisionSystem *vs;
	Gyro *gyro;
	
	PIDController *pid;
	
	JoystickCallback<Robot> *jc;
	JoystickWrapper *joystick1;
	JoystickWrapper *joystick2;
};

#endif // ROBOT_H
