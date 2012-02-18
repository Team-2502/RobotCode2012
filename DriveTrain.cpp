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

void DriveTrain::DriveArcade(double x, double y) {
	float exponentialL = 0;
	if (y+x < 0) {
		exponentialL = 0 - sqrt(0 - (y+x));
	} else if (y+x == 0) {
		exponentialL = 0;
	} else {
		exponentialL = sqrt(y+x);
	}
	float exponentialR = 0;
	if (y-x < 0) {
		exponentialR = 0 - sqrt(0 - (y-x));
	} else if (y-x == 0) {
		exponentialR = 0;
	} else {
		exponentialR = sqrt(y-x);
	}
	bool exponential = false;
	if (exponential == true) {
		if (run == true) {
			Singleton<Display>::GetInstance().PrintfLine(0, "%f", y+x);
			Singleton<Display>::GetInstance().PrintfLine(1, "%f", exponentialL);
			left->Set(-(exponentialL));
			right->Set(-(exponentialR));
		}
	} else {
		if (run == true) {
			left->Set(-(y+x));
			right->Set(-(y-x));
		}
	}
}

void DriveTrain::DriveTank(double leftChannel, double rightChannel)
{
	if (run == true) {
		left->Set(-leftChannel);
		right->Set(-rightChannel);
	}
}

DriveType DriveTrain::CurrentDrive() {
	return this->type;
}

void DriveTrain::ChangeDrive(DriveType type) {
	this->type = type;
}

/*
void DriveTrain::DriveField(double x, double y, double gyro)
{
	float angle = 0.00;
	float sens = .2;
	float sensitivity = 10;
	
	if (x <= sens && y <= sens && x >= -sens && y >= -sens) {
		return;
	}
	
	if (x >= 0 && y >= 0) { 				// Quad 1
		angle = ((x*45.) - (y*45.)) + 45.;
	} else 	if (x >= 0 && y <= 0) {				// Quad 2
		angle = 0 - (((x*45) + (y*45)) - 135);
	} else 	if (x <= 0 && y <= 0) {			// Quad 3
		angle = ((0-x)*45)-((0-y)*45) + 225;
	} else 	if (x <= 0 && y >= 0) {				// Quad 4
		angle = 0 - (((0-x)*45) + ((0-y)*45) - 315);
	} else {
		angle = 0;
	}
	/* Translate Gyro angle into human-readable form *
	if (gyro < 0) {
		gyro = 0 - gyro;
		gyro = 360 - gyro;
		gyro = round(gyro, 1) % 360;
	} else {
		gyro = round(gyro, 1) % 360;
	}
	float turn = 0.00;
	int sturn = 0; // Default - Still
	// Calculate Smallest Turn Angle
	turn = gyro - angle;
	if (turn < 0 && turn >= -180) {
		turn = 0-turn;
		sturn = 1;
	} else if (turn > 0 && turn <= 180) {
		turn = turn;
		sturn = 1;
	} else if (turn == 0) {
		turn = turn;
		sturn = 0;
	} else if (turn >= 180) {
		turn = turn - 180;
		sturn = -1;
	} else if (turn <= -180) {
		turn = turn + 180;
		turn = 0 - turn;
		turn = 180 - turn;
		sturn = -1;
	} else {
		// ?! Don't Turn !?
		turn = 0;
		sturn = 0;
	}
	// Override turn
	if (turn <= sensitivity && turn >= 0-sensitivity) {
		sturn = 0;
	}
	// Spin Motor Speeds according to sturn
	//dstation->PrintfLine(DriverStationLCD::kUser_Line4, "%i", sturn);
	//dstation->PrintfLine(DriverStationLCD::kUser_Line6, "%f", turn2);
	float r = ((x*x) + (y*y)) / (sqrt(2));
	//Singleton<Display>::GetInstance().PrintfLine(3, "%f", gyro);
	//Singleton<Display>::GetInstance().PrintfLine(4, "%i", sturn);
	//Singleton<Display>::GetInstance().PrintfLine(5, "%f", turn);
	if (run == true) {
		if (sturn == -1) {
			left->Set(r);
			right->Set(r);
		} else if (sturn == 1) {
			left->Set(-r);
			right->Set(-r);
		} else {
			left->Set(-r);
			right->Set(r);
		}
	}
}
*/