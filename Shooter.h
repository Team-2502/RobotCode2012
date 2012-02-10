#ifndef SHOOTER_H
#define SHOOTER_H

class Shooter
{
public:
	Shooter();
	~Shooter();
	
	void SetTurret(double direction);
	void Shoot(double distance, double spin = 0.0);
	void Update();
};

#endif // SHOOTER_H
