#include "Math.h"
#include "Shooter.h"

using std::atan;
using std::cos;
using std::pow;
using std::sin;
using std::sqrt;
using std::tan;

/**
 * Find the perfect speed to shoot the ball into a basket.
 * 
 * \return the velocity (in ft/s).
 */
static double getShootVelocity(double distance, double elevation, double shootAngle, double basketRadius, double ballRadius)
{
	static double g = 32.0;
	double impactAngle = atan( tan(shootAngle) - (2 * elevation) / distance;
	double variance = basketRadius - (ballRadius / sin(degToRad(impactAngle)));
	double velocity = sqrt( (g * pow(distance + variance, 2.0)) / (2.0 * (distance + variance) * sin(shootAngle) * cos(shootAngle) - elevation * pow(cos(shootAngle) , 2.0) ) );
	return velocity;
}
