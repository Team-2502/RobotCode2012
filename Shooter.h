#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>
#include "SharpIR.h"

enum TurretDirection
{
	TURRET_OFF,
	TURRET_LEFT,
	TURRET_RIGHT,
};

class Shooter
{
public:
	Shooter();
	~Shooter();
	
	void MoveTurret(TurretDirection direction);
	void SetTurret(double direction);
	void Shoot(double speed);
	void ShootBasket(double distance, int level);
	void Update();
	
private:
	double turretDirection;
	Jaguar* 		bottomJag;
	Jaguar* 		topJag;
	PIDController*	bottomPID;
	PIDController*	topPID;
	Encoder*		bottomEncoder;
	Encoder*		topEncoder;
	Encoder*		turretEncoder;
	Victor* 		turretVictor;
	SharpIR*		turretIR;
};

#endif // SHOOTER_H
