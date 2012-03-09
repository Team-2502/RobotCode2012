#include "Constants.h"
#include "Logger.h"
#include "Math.h"
#include "Shooter.h"
#include "Singleton.h"
#include "Collector.h"
#include "Display.h"

using namespace std;

/**
 * Find the perfect speed to shoot the ball into a basket.
 * 
 * \note all units are in FEET.
 * \param distance the distance to the target.
 * \param elevation the height of the target relative to the shooter.
 * \param shootAngle the angle that the ball is being shot at.
 * \param basketRadius the radius of the basket.
 * \param ballRadius the radius of the ball.
 * \return the velocity that the ball should be shot at (in ft/s).
 */
static double GetShootVelocity(double distance, double elevation, double shootAngle, double basketRadius, double ballRadius)
{
	static double rootG = sqrt(32.0);
	static double sinAngle = sin(2 * shootAngle);
	static double cosAngle = cos(2 * shootAngle);
	static double tanAngle = tan(shootAngle);
	//double impactAngle = atan( tan(shootAngle) - (2 * elevation) / distance);
	double impactAngle = atan( tanAngle - 2 * elevation / distance );
	double variance = basketRadius - (ballRadius / sin(impactAngle));
	//double velocity = sqrt( (g * pow(distance + variance, 2.0)) / (2.0 * (distance + variance) * sin(shootAngle) * cos(shootAngle) - elevation * pow(cos(shootAngle) , 2.0) ) );
	return rootG * fabs(distance + variance)/ sqrt(fabs(cosAngle*elevation+elevation-sinAngle*distance-variance*sinAngle));
}

Shooter::Shooter() :
		turretDirection(0.0)
{
	Singleton<Logger>::GetInstance().Logf("Shooter: Starting up...");
	//Setup Jaguars
	topJag = new Jaguar(DIGITAL_SIDECAR_SLOT, SHOOTER_TOP_JAG_CHANNEL);
	bottomJag = new Jaguar(DIGITAL_SIDECAR_SLOT, SHOOTER_BOTTOM_JAG_CHANNEL);
	turretVictor = new Victor(TURRET_CHANNEL);
//	turretIR = new SharpIR( 1, IR_TURRET_CHANNEL, TURRET_SIGNAL_VOLTAGE );
//	turretPosition = 0;
	topEncoder = new Encoder(SHOOTER_TOP_ENCODER_A, SHOOTER_TOP_ENCODER_B);
	bottomEncoder = new Encoder(SHOOTER_BOTTOM_ENCODER_A, SHOOTER_BOTTOM_ENCODER_B);
	turretEncoder = new Encoder(TURRET_ENCODER_A, TURRET_ENCODER_B);
	topPID = new PIDController(0.1, 0.01, 0.0, topEncoder, topJag);
	bottomPID = new PIDController(0.1, 0.01, 0.0, bottomEncoder, bottomJag);
	
	double pulseDistance = (TURRET_WHEEL_DIAMETER / TURRET_LAZY_SUSAN_DIAMETER) * 360.0 / (double)TURRET_ENCODER_PULSES;
	turretEncoder->SetDistancePerPulse(pulseDistance);
	turretEncoder->Start(); // This should be running at all times
}

Shooter::~Shooter()
{
	Singleton<Logger>::GetInstance().Logf("Shooter: Shutting down...");
	
	turretEncoder->Stop();
	
	delete topJag;
	delete bottomJag;
	delete turretVictor;
	delete turretIR;
	delete topEncoder;
	delete bottomEncoder;
	delete turretEncoder;
	delete topPID;
	delete bottomPID;
}

void Shooter::Shoot(double speed)
{
	Collector& collector = Singleton<Collector>::GetInstance();
	Singleton<Logger>::GetInstance().Logf("Shooting a ball with speed [%f ft/s] and spin [%f Hz]", speed, 0.0);
	
	///\todo Check inversion.
	//this->topJag->Set(-1.0 * speed / MAX_SHOOTER_SPEED);
	//this->bottomJag->Set(speed / MAX_SHOOTER_SPEED);
	//topEncoder->Reset();
	//topEncoder->Start();
	//bottomEncoder->Reset();
	//bottomEncoder->Start();
	//topPID->SetSetpoint(-1.0 * speed / MAX_SHOOTER_SPEED);
	//bottomPID->SetSetpoint(-0.75 * speed / MAX_SHOOTER_SPEED);
	
	//topPID->Enable();
	//bottomPID->Enable();
	topJag->Set(-1.0 * speed / MAX_SHOOTER_SPEED);
	bottomJag->Set(-0.75 * speed / MAX_SHOOTER_SPEED);
	Wait(2.5);
	collector.Shoot();
	bottomJag->Set(0.0);
	topJag->Set(0.0);
	
	//topPID->Disable();
	//bottomPID->Disable();
	//topEncoder->Stop();
	//bottomEncoder->Stop();
}

void Shooter::ShootBasket(double distance, int level)
{
	Singleton<Logger>::GetInstance().Logf("Shooting a basket at distance [%f ft] at level [%d]", distance, level);
	
	//Calculate the ideal velocity
	double basketElevation;
	switch( level )
	{
		case 1:
			basketElevation = BASKET_1_ELEVATION;
			break;
			
		case 2:
			basketElevation = BASKET_2_ELEVATION;
			break;
			
		case 3:
			basketElevation = BASKET_3_ELEVATION;
			break;
			
		default:
			return;
	}
	double elevation = SHOOTER_ELEVATION - basketElevation;
	double velocity = GetShootVelocity(distance, elevation, SHOOTER_ANGLE, BASKET_RADIUS, BALL_RADIUS);
	
	Shoot(velocity);
}

void Shooter::MoveTurret(TurretDirection direction)
{
	switch( direction )
	{
		case TURRET_OFF:
			SetTurret(0.0);
			break;
			
		case TURRET_LEFT:
			SetTurret(-0.25);
			break;
			
		case TURRET_RIGHT:
			SetTurret(0.25);
			break;
	}
}

void Shooter::SetTurret(double direction)
{
	turretDirection = direction;
	this->turretVictor->Set( -1.0 * direction );
	Singleton<Display>::GetInstance().PrintfLine(5, "T:%f", direction);
}

void Shooter::Update()
{
	double rotation = turretEncoder->GetDistance();
	if( (rotation < -90 && turretDirection < 0)  || (rotation > 90 && turretDirection > 0) )
		SetTurret(0);
}
