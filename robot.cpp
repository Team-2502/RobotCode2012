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
#include "Shooter.h"

Robot::Robot()
{
	GetWatchdog().SetEnabled(false); ///\todo enable this before the competition

	Logger* logger = new Logger("/ni-rt/system/logs/robot.txt");
	vision = new Vision(new SquareFinder);
	vision->start();
	Singleton<Display>::SetInstance(new Display);
	Singleton<Logger>::SetInstance(logger);
	Singleton<DriveTrain>::SetInstance(new DriveTrain);
	Singleton<Collector>::SetInstance( new Collector );
	Singleton<Collector>::GetInstance().Start();
	Singleton<Shooter>::SetInstance( new Shooter );

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
	joystickCallbackHandler->SetTriggerDownCallback(GET_FUNC(ShootBasket));
	joystickCallbackHandler->SetHeldCallback(TURRET_BUTTON, GET_FUNC(MoveTurret));
	joystickCallbackHandler->SetUpCallback(TURRET_BUTTON, GET_FUNC(TurretOff));
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
	Singleton<Collector>::GetInstance().ManipulateRamp(DOWN);
}

void Robot::RampUp()
{
	Singleton<Display>::GetInstance().PrintfLine(5, "Ramp Going Up");
	Singleton<Collector>::GetInstance().ManipulateRamp(UP);
}

void Robot::RampOff()
{
	Singleton<Display>::GetInstance().PrintfLine(5, "Ramp Turning Off");
	Singleton<Collector>::GetInstance().ManipulateRamp(RAMP_OFF);
}

void Robot::CollectorEject()
{
	Singleton<Display>::GetInstance().PrintfLine(5, "Ejecting Ball");
	Singleton<Collector>::GetInstance().Eject();
}

void Robot::Autonomous()
{
	Singleton<Logger>::GetInstance().Logf("Starting Autonomous Mode.");
	Singleton<Collector>::GetInstance().SetBallCount( 2 );  // preloaded with 2 balls in autonomous

	while ( IsAutonomous() )
	{
		ShootBasket();
		Wait(0.01);
	}
	Singleton<Logger>::GetInstance().Logf("Stopping Autonomous Mode.");
}

void Robot::OperatorControl()
{
	DriveTrain& driveTrain = Singleton<DriveTrain>::GetInstance();
	Singleton<Logger>::GetInstance().Logf("Starting operator control.");
	int count = 0;
	
	__UNUSED(count)
	__UNUSED(driveTrain)
	
	while(IsOperatorControl())
	{
		float x, y, rot;
		joystick1->GetAxis(&x,&y);
		rot = -joystick1->GetRawRotation();

		driveTrain.DriveArcade(rot, tanFilter(y));
		Singleton<Display>::GetInstance().Clear();
	
		//Singleton<Display>::GetInstance().PrintfLine(0, "X: %f", x);
		//Singleton<Display>::GetInstance().PrintfLine(1, "Y: %f", y);
		//Singleton<Display>::GetInstance().PrintfLine(2, "R: %f", rot);
/*		
		if( count++ % 5 == 0 || true )
		{
			TargetReport report = vision->GetBestTarget(); 
			if(report.distance != 0 )
				Singleton<Shooter>::GetInstance().SetTurret(-0.21 * (report.normalized_x < 0 ? -1 : 1));
		}
		else
		{
			Singleton<Shooter>::GetInstance().SetTurret(0);
		}
*/
		joystickCallbackHandler->Update();
		Singleton<Display>::GetInstance().Update();
		Singleton<Shooter>::GetInstance().Update();
		
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

void Robot::ShootBasket()
{
	double 		offset		= 0.0;
	double 		distance	= 0.0;
	int 		targetLevel = 0;
	int 		failCount	= 0;
	Shooter& 	shooter 	= Singleton<Shooter>::GetInstance();
	Collector&  collector   = Singleton<Collector>::GetInstance();
	
	__UNUSED(offset);
	__UNUSED(distance);
	__UNUSED(targetLevel);
	__UNUSED(failCount);
	
	// disable the drive
	Singleton<DriveTrain>::GetInstance().DriveArcade(0.0, 0.0);
	/*  TEMPORARY COMMENT OUT FOR TESTING PREPARE TO SHOOT
	// Attempt to find target
	while(true)
	{
		vision->FindTarget(offset, distance, targetLevel);
		// try 10 times to find a target
		if( targetLevel == 0 )
		{
			failCount++;
			if( failCount > 10 )
			{
				Singleton<Display>::GetInstance().PrintfLine(6, "NO TARGET FOUND!!!");
				break;
			}
			Wait(.05);
			continue;
		}
		else
		{
			failCount = 0;
		}
		// target found, turn turret to center it
		shooter.SetTurret( offset * TURRET_SPEED );
		if( fabs(offset) < .01 ) //todo figure out best value
		{
			shooter.SetTurret( 0 );
			break;
		}
	}
	*/
	// We're pointing at the target, shoot
	collector.PrepareToShoot();

//	shooter.ShootBasket( distance, targetLevel );
	shooter.Shoot((joystick1->GetThrottle() + 1.0) / 2.0 * 25.0);
	
}

void Robot::MoveTurret()
{
	float rot = joystick1->GetRawRotation() / -2.0;
	Singleton<Shooter>::GetInstance().SetTurret(rot);
	Singleton<Display>::GetInstance().PrintfLine(5, "Turret:%f", rot);
	Singleton<DriveTrain>::GetInstance().DriveArcade(0.0,0.0);
}

void Robot::TurretOff()
{
	Singleton<Shooter>::GetInstance().SetTurret(0);
	Singleton<Display>::GetInstance().PrintfLine(5, "Turret Off");
}

START_ROBOT_CLASS(Robot)
