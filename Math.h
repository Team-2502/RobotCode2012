/**
 * \file Math.h
 * \brief Contains useful math functions and constants.
 */
#ifndef MATH_H
#define MATH_H

#include <cmath>

static const double PI = 4.0 * std::atan(1.0); /**< Pi. */

/**
 * Convert degrees to radians.
 * 
 * \param degrees the angle (in degrees).
 * \return the angle (in radians).
 */

/*double degToRad(double degrees)
{
	return degrees * PI / 180.0;
}*/

/**
 * Convert radians to degrees.
 * 
 * \param radians the angle (in radians).
 * \return the angle (in degrees).
 */
/*double radToDeg(double radians)
{
	return radians * 180.0 / PI;
}*/

/**
 * Rotate a coordinate by an angle.
 * 
 * \param x the x coordinate.
 * \param y the y coordinate.
 * \param angle the angle to rotate the coordinate by (in degrees).
 * \param rx the rotated x coordinate.
 * \param ry the rotated y coordinate.
 */
/*void rotate(double x, double y, double angle, double* rx, double* ry)
{
	angle = degToRad(angle);
	*rx = x * std::cos(angle) - y * std::sin(angle);
	*ry = x * std::sin(angle) + y * std::cos(angle);
}*/

#endif // MATH_H
