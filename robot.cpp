#include "Collector.h"
#include "Display.h"
#include "DriveTrain.h"
#include "JoystickWrapper.h"
#include "Logger.h"
#include "Robot.h"
#include "Singleton.h"
#include "squarefinder.h"
#include <time.h>
#include "SharpIR.h"

Robot::Robot()
{
	GetWatchdog().SetEnabled(false);

	Logger* logger = new Logger("/ni-rt/system/logs/robot.txt");
	vision = new Vision(new SquareFinder);
	vision->start();
	Singleton<Display>::SetInstance(new Display);
	Singleton<Logger>::SetInstance(logger);
	Singleton<DriveTrain>::SetInstance(new DriveTrain);
	Singleton<Collector>::SetInstance( new Collector );
	Singleton<Collector>::GetInstance().Start();

	Singleton<Logger>::GetInstance().Logf("Starting the Robot class.");

	//balanceAccelerometer = new AccelPID_Wrapper(new ADXL345_I2C(1)); //Takes ownership of ADXL345

	//balancePID = new PIDController(0.1,.01,0.0,balanceAccelerometer,&Singleton<DriveTrain>::GetInstance()); //\todo Tune these! No D.
	//balancePID->Disable();

	//gyro = new Gyro(1);
	//gyro->Reset();

	joystick1 = new JoystickWrapper(1, Extreme3DPro);
	//joystick2 = new JoystickWrapper(2, Attack3);
	joystickCallbackHandler = new JoystickCallback<Robot>(joystick1,this);
	joystickCallbackHandler->SetHeldCallback(RAMP_DOWN_BUTTON, GET_FUNC(RampDown));
	joystickCallbackHandler->SetHeldCallback(RAMP_UP_BUTTON, GET_FUNC(RampUp));
	joystickCallbackHandler->SetHeldCallback(EJECT_BALLS_BUTTON, GET_FUNC(CollectorEject));
	joystickCallbackHandler->SetUpCallback(RAMP_DOWN_BUTTON, GET_FUNC(RampOff));
	joystickCallbackHandler->SetUpCallback(RAMP_UP_BUTTON, GET_FUNC(RampOff));
	//joystickCallbackHandler->SetDownCallback(BalanceRobot,GET_FUNC(BalanceRobotOn));
	//joystickCallbackHandler->SetUpCallback(BalanceRobot,GET_FUNC(BalanceRobotOff));
}
//Robot used to be 5200B processor.
Robot::~Robot()
{
	Singleton<Logger>::GetInstance().Logf("Shutting down the Robot class.");

	//Destroy instances of singletons that we have used
	Singleton<Display>::DestroyInstance();
	Singleton<DriveTrain>::DestroyInstance();
	Singleton<Logger>::DestroyInstance();
	
	//delete balancePID;
	//delete balanceAccelerometer;
}

void Robot::RampDown()
{
	Singleton<Display>::GetInstance().PrintfLine(5, "Ramp Going Down");
	//Singleton<Collector>::GetInstance().ManipulateRamp(DOWN);
}

void Robot::RampUp()
{
	Singleton<Display>::GetInstance().PrintfLine(5, "Ramp Going Up");
	//Singleton<Collector>::GetInstance().ManipulateRamp(UP);
}

void Robot::RampOff()
{
	Singleton<Display>::GetInstance().PrintfLine(5, "Ramp Turning Off");
	//Singleton<Collector>::GetInstance().ManipulateRamp(RAMP_OFF);
}

void Robot::CollectorEject()
{
	Singleton<Display>::GetInstance().PrintfLine(5, "Ejecting Ball");
	//Singleton<Collector>::GetInstance().Eject();
}

void Robot::Autonomous()
{
	Singleton<Logger>::GetInstance().Logf("Starting Autonomous Mode.");
	Singleton<Collector>::GetInstance().SetBallCount( 2 );  // preloaded with 2 balls in autonomous

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
//	SharpIR *irTest = new SharpIR( 1, 5, COLLECTOR_FRONT_SIGNAL_VOLTAGE);
	
	while(IsOperatorControl())
	{
		float x, y, rot;
		joystick1->GetAxis(&x,&y);
		rot = -joystick1->GetRawRotation();

		driveTrain.DriveArcade(rot, y);
		Singleton<Display>::GetInstance().Clear();
		
		
		Singleton<Display>::GetInstance().PrintfLine(0, "X: %f", x);
		Singleton<Display>::GetInstance().PrintfLine(1, "Y: %f", y);
		Singleton<Display>::GetInstance().PrintfLine(2, "R: %f", rot);
//		Singleton<Display>::GetInstance().PrintfLine(3, "Ball: %i", irTest->Get());
//		Singleton<Display>::GetInstance().PrintfLine(4, "Ball: %f", irTest->GetVoltage());
		
		joystickCallbackHandler->Update();
		Singleton<Display>::GetInstance().Update();
		
		Wait(0.01);
	}
	Singleton<Logger>::GetInstance().Logf("Stopping operator control.");
}

void Robot::BalanceRobotOff()
{
	balancePID->Disable();
}

void Robot::BalanceRobotOn()
{
	balancePID->Enable();
	balancePID->SetSetpoint(0.0);
}

START_ROBOT_CLASS(Robot)
