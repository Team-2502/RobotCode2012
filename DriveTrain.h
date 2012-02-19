#ifndef DRIVETRAIN_H
#define DRIVETRAIN_H

#include <WPILib.h>

class Jaguar;
class RobotDrive;

enum DriveType {
	TankDrive, 
	ArcadeDrive
};

/**
 * The drivetrain of our Robot. Can be driven arcade-style with a single joystick or tank-drive with two joysticks.
 */
class DriveTrain : public PIDOutput
{
public:
	DriveTrain();
	virtual ~DriveTrain();
	
	void PIDWrite(float output) { DriveTank(output,output); }
	
	//void DriveField(double x, double y, double gyro); -- Removed because we're not using it
	void DriveArcade(double x, double y);
	
	/**
	 * Drive tank-style with two channels.
	 * 
	 * \param leftChannel the left wheel channel.
	 * \param rightChannel the right wheel channel.
	 */
	void DriveTank(double leftChannel, double rightChannel);
	
	bool run; // Mainly for quick-shutoff
	DriveType type;
	DriveType CurrentDrive();
	void ChangeDrive(DriveType type);
	
private:
	Jaguar* left;
	Jaguar* right;
	RobotDrive* roboDrive;
};

#endif // DRIVETRAIN_H
