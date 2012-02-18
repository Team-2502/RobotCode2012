#include "WPILib.h"

#include "Display.h"
#include "DriveTrain.h"
#include "Math.h"
#include "Logger.h"
#include "Singleton.h"
#include <cmath>

DriveTrain::DriveTrain()
{
	Singleton<Logger>::GetInstance().Logf("DriveTrain() initializing.");

	this->left = new Jaguar(1, 1);
	this->right = new Jaguar(1, 2);

	this->left->SetSafetyEnabled(false);
	this->right->SetSafetyEnabled(false);

	this->roboDrive = 0;
	this->run = true;
	this->type = ArcadeDrive;
}

DriveTrain::~DriveTrain()
{
	Singleton<Logger>::GetInstance().Logf("~DriveTrain() stopping.");

	delete this->left;
	delete this->right;

	//delete this->roboDrive;
}

int round(float d, int pos = 1) {
	if (d - floor(d) >= .5) return (int)ceil(d);
	return (int)floor(d);
}

void DriveTrain::DriveArcade(double x, double y)
{
	left->Set(-y-x);
	right->Set(-y+x);
}

void DriveTrain::DriveTank(double leftChannel, double rightChannel)
{
	if (run) {
		left->Set(-leftChannel);
		right->Set(-rightChannel);
	}
}

DriveType DriveTrain::CurrentDrive()
{
	return this->type;
}

void DriveTrain::ChangeDrive(DriveType type)
{
	this->type = type;
}
