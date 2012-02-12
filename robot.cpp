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
	Logger* logger = new Logger("/ni-rt/system/logs/robot.txt");
	//vs = new VisionSystem(new SquareFinder);
	//vs->start();
	Singleton<Display>::SetInstance(new Display);
	Singleton<Logger>::SetInstance(logger);
	Singleton<DriveTrain>::SetInstance(new DriveTrain);
	Singleton<Logger>::GetInstance().Logf("Starting the Robot class.");

	//accel = 0;//new ADXL345_PID(1);
	//lsm = new LSM303_I2C(1);

	//	pid = new PIDController(0.7,.1,.1,accel,&Singleton<DriveTrain>::GetInstance());

	//gyro = new Gyro(1);
	//gyro->Reset();
	
	joystick1 = new JoystickWrapper(1, Extreme3DPro);
	//joystick2 = new JoystickWrapper(2, Attack3);
	jc = new JoystickCallback<Robot>(joystick1,this);
	jc->SetUpCallback(9,GET_FUNC(toggleDiagnosticMode));

	inDiagnosticMode = false;
	showAccel = false;
	
	GetWatchdog().SetEnabled(false);
}

Robot::~Robot()
{
	Singleton<Logger>::GetInstance().Logf("Shutting down the Robot class.");

	//Destroy instances of singletons that we have used
	Singleton<Display>::DestroyInstance();
	Singleton<DriveTrain>::DestroyInstance();
	Singleton<Logger>::DestroyInstance();
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

	double accelX = 0;
	double accelY = 0;
	double accelZ = 0;
	time_t last_switch_drive = time(NULL);
	time_t last_switch_type = time(NULL);

	while(IsOperatorControl())
	{
		if(!inDiagnosticMode) {
			//			pid->SetSetpoint(0.0);

			float x, y;
			float z;
			joystick1->GetRawAxis(&x,&y);
			z = joystick1->GetRawRotation();
			//float y2;
			//joystick2->GetRawAxis(&x,&y2);
			// This will disable drive train
			// should not be included in final product
			/*
			if (joystick1->GetButton(7)) {
				if (last_switch_type > 0) {
					driveTrain.run = !driveTrain.run;
					last_switch_type = time(NULL);
				}
			}
			if (joystick1->GetButton(5)) {
				if (driveTrain.CurrentDrive() == TankDrive) {
					if (time(NULL) - last_switch_drive > 0) {
						driveTrain.ChangeDrive(ArcadeDrive);
						last_switch_drive = time(NULL);
					}
				} else {
					if (time(NULL) - last_switch_drive > 0) {
						driveTrain.ChangeDrive(TankDrive);
						last_switch_drive = time(NULL);
					}
				}
			}
			*/

			driveTrain.DriveArcade(-z, y);
			Singleton<Display>::GetInstance().Clear();
			Singleton<Display>::GetInstance().PrintfLine(0, "%f", z);
			Singleton<Display>::GetInstance().PrintfLine(1, "%f", y);
			//Singleton<Display>::GetInstance().PrintfLine(0, "JoyX: %1.5f", x);
			//Singleton<Display>::GetInstance().PrintfLine(1, "JoyZ: %1.5f", z);

			/*
			TargetReport tr = vs->getBestTarget();
			if(showAccel) {
				//accelX = accel->GetAcceleration(ADXL345_I2C::kAxis_X);
				//accelY = accel->GetAcceleration(ADXL345_I2C::kAxis_Y);
				//accelZ = accel->GetAcceleration(ADXL345_I2C::kAxis_Z);
			} else {
				accelX = lsm->GetAccelerations().XAxis;
				accelY = lsm->GetAccelerations().YAxis;
				accelZ = lsm->GetAccelerations().ZAxis;
			}

			Singleton<Display>::GetInstance().PrintfLine(2,"Vis: (%1.2f,%1.2f)",tr.normalized_x,tr.normalized_y);
			if(showAccel) {
				Singleton<Display>::GetInstance().PrintfLine(3, "X_a: %f", accelX);
				Singleton<Display>::GetInstance().PrintfLine(4, "Y_a: %f", accelY);
				//Singleton<Display>::GetInstance().PrintfLine(5, "Deg: %f", accel->PIDGet());
			} else {
				Singleton<Display>::GetInstance().PrintfLine(3, "X_m: %f", accelX);
				Singleton<Display>::GetInstance().PrintfLine(4, "Y_m: %f", accelY);
				Singleton<Display>::GetInstance().PrintfLine(5, "Z_m: %f", accelZ);
			}
			*/

			//float speed = 0.5*tr.normalized_x; //Scale to range. [-1,0,1] : [left,center,right]
			//driveTrain.DriveTank( speed, -speed );
			/*
			if (driveTrain.CurrentDrive() == TankDrive) {
				//Singleton<Display>::GetInstance().PrintfLine(0, "Currently: TankDrive");
				driveTrain.DriveTank( y, y2);
			} else {
				//Singleton<Display>::GetInstance().PrintfLine(0, "Currently: ArcadeDrive");
				driveTrain.DriveArcade(z, y);
			}*/
			Wait(0.01);
		} 
		else {
			/*
			Singleton<Display>::GetInstance().Clear();
			Singleton<Display>::GetInstance().PrintfLine(0,"[Diagnostic Mode]");
			Singleton<Display>::GetInstance().PrintfLine(1,"2) Show LSM303 outs");
			Singleton<Display>::GetInstance().PrintfLine(2,"3) Run Wheels [F/B/S]");
			Singleton<Display>::GetInstance().PrintfLine(3,"");
			Singleton<Display>::GetInstance().PrintfLine(4,"9) Exit");
			Singleton<Display>::GetInstance().PrintfLine(5,"");
			*/
		}
		
		jc->Update();
		Singleton<Display>::GetInstance().Update();
		Wait(0.01);
	}
	Singleton<Logger>::GetInstance().Logf("Stopping operator control.");
}

void Robot::toggleDiagnosticMode()
{
	jc->UnbindAll();
	if(inDiagnosticMode) { //Leaving
		jc->SetUpCallback(3,GET_FUNC(toggleAccel));
	} else { //Entering
		jc->SetHeldCallback(2,GET_FUNC(displayLSM));
		jc->SetUpCallback(3,GET_FUNC(runWheels));
	}
	jc->SetUpCallback(9,GET_FUNC(toggleDiagnosticMode));
	inDiagnosticMode = !inDiagnosticMode;
}

void Robot::runWheels()
{
	static int mode = 0;
	mode = (mode+1)%3;
	switch(mode) {
	case 1: //Forward
		Singleton<Display>::GetInstance().PrintfLine(5,"[STAT] Forward...");
		Singleton<DriveTrain>::GetInstance().DriveTank(-1.0,-1.0);
		break;
	case 2: //Backward
		Singleton<Display>::GetInstance().PrintfLine(5,"[STAT] Backward...");
		Singleton<DriveTrain>::GetInstance().DriveTank(1.0,1.0);
		break;
	default:
	case 0: //Stopped
		Singleton<Display>::GetInstance().PrintfLine(5,"");
		Singleton<DriveTrain>::GetInstance().DriveTank(0,0);
		break;
	}
}

void Robot::toggleAccel()
{
	showAccel = !showAccel;
}

void Robot::displayLSM()
{
	LSM303_I2C::AxesReport r = lsm->GetAccelerations();
	Singleton<Display>::GetInstance().PrintfLine(1,"LSM X: %f",r.XAxis);
	Singleton<Display>::GetInstance().PrintfLine(2,"LSM Y: %f",r.YAxis);
	Singleton<Display>::GetInstance().PrintfLine(3,"LSM Z: %f",r.ZAxis);
}

START_ROBOT_CLASS(Robot)
