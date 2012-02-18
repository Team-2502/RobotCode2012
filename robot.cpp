#include "Display.h"
#include "DriveTrain.h"
#include "JoystickWrapper.h"
#include "Logger.h"
#include "Robot.h"
#include "Singleton.h"
#include "squarefinder.h"
#include <time.h>

Robot::Robot()
{
	GetWatchdog().SetEnabled(false);

	Logger* logger = new Logger("/ni-rt/system/logs/robot.txt");
	vs = new VisionSystem(new SquareFinder);
	vs->start();
	Singleton<Display>::SetInstance(new Display);
	Singleton<Logger>::SetInstance(logger);
	Singleton<DriveTrain>::SetInstance(new DriveTrain);
	Singleton<Logger>::GetInstance().Logf("Starting the Robot class.");

	accel = new AccelPID_Wrapper(new ADXL345_I2C(1)); //Takes ownership of ADXL345

	pid = new PIDController(0.1,.01,0.0,accel,&Singleton<DriveTrain>::GetInstance()); //\todo Tune these! No D.
	pid->Disable();

	gyro = new Gyro(1);
	gyro->Reset();

	joystick1 = new JoystickWrapper(1, Extreme3DPro);
	//joystick2 = new JoystickWrapper(2, Attack3);
	jc = new JoystickCallback<Robot>(joystick1,this);
	jc->SetDownCallback(BalanceRobot,GET_FUNC(balanceRobotOn));
	jc->SetUpCallback(BalanceRobot,GET_FUNC(balanceRobotOff));
}
//Robot used to be 5200B processor.
Robot::~Robot()
{
	Singleton<Logger>::GetInstance().Logf("Shutting down the Robot class.");

	//Destroy instances of singletons that we have used
	Singleton<Display>::DestroyInstance();
	Singleton<DriveTrain>::DestroyInstance();
	Singleton<Logger>::DestroyInstance();
	
	//delete pid;
	//delete accel;
}

void Robot::Autonomous()
{
	Singleton<Logger>::GetInstance().Logf("Starting Autonomous Mode.");
	while ( IsAutonomous() )
	{
		///\todo Implement this!
		Wait(0.01);
	}
	Singleton<Logger>::GetInstance().Logf("Stopping Autonomous Mode.");
}

void Robot::OperatorControl()
{
	DriveTrain& driveTrain = Singleton<DriveTrain>::GetInstance();
	Singleton<Logger>::GetInstance().Logf("Starting operator control.");

	while(IsOperatorControl())
	{
		float x, y, rot;
		joystick1->GetRawAxis(&x,&y);
		rot = joystick1->GetRawRotation();

		driveTrain.DriveArcade(-rot*.7, y);
		Singleton<Display>::GetInstance().Clear();
		
		Singleton<Display>::GetInstance().PrintfLine(0, "X: %f", x);
		Singleton<Display>::GetInstance().PrintfLine(1, "Y: %f", y);
		Singleton<Display>::GetInstance().PrintfLine(2, "R: %f", rot);
		
		//jc->Update();
		Singleton<Display>::GetInstance().Update();
		
		Wait(0.01);
	}
	Singleton<Logger>::GetInstance().Logf("Stopping operator control.");
}

void Robot::balanceRobotOff()
{
	pid->Disable();
}

void Robot::balanceRobotOn()
{
	pid->Enable();
	pid->SetSetpoint(0.0);
}

START_ROBOT_CLASS(Robot)
