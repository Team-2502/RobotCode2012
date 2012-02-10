#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>

class DriverStationLCD;

/**
 * Wraps around the DriverStationLCD class provided by WPILib with some useful features.
 */
class Display
{
public:
	/**
	 * Constructor.
	 */
	Display();
	
	/**
	 * Destructor.
	 */
	~Display();
	
	/**
	 * Clear the display.
	 */
	void Clear();
	
	/**
	 * Print a formatted string.
	 * 
	 * \param format the format string.
	 */
	void Printf(const char *format, ...);
	
	/**
	 * Print a formatted string to a specific line of the display.
	 *
	 * \param line the line number (can be 0-5).
	 * \param format the format string.
	 */
	void PrintfLine(int line, const char *format, ...);
	
	/**
	 * Redraw the display. This is required after every call to a print or clear function.
	 */
	void Update();
	
private:
	DriverStationLCD *dstation;
	std::string lines[6];
};

#endif
