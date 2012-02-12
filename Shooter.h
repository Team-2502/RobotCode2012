#ifndef SHOOTER_H
#define SHOOTER_H

#include <WPILib.h>

class Shooter
{
public:
	Shooter();
	~Shooter();
	
	void SetTurret(double direction);
	void Shoot(double speed);
	void ShootBasket(double distance, int level);
	void Update();
	
private:
	Jaguar* bottomJag;
	Jaguar* topJag;
};

#endif // SHOOTER_H
