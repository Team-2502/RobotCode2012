#include <WPILib.h>
#include <cstdarg>
#include "Display.h"

using std::string;

Display::Display() {
	dstation = DriverStationLCD::GetInstance();
}

Display::~Display() {
	
}

void Display::Printf(const char *format, ...) {
	va_list args;
	va_start(args, format);
	char buf[256];
	vsnprintf(buf, 255, format, args);
	va_end(args);
	lines[0] = buf;
}

void Display::PrintfLine(int line, const char *format, ...) {
	if (line < 0 || line > 5)
		return;
	va_list args;
	va_start(args, format);
	char buf[256];
	vsnprintf(buf, 255, format, args);
	va_end(args);
	lines[line] = buf;
}

void Display::Update() {
	dstation->PrintfLine(DriverStationLCD::kUser_Line1, lines[0].c_str());
	dstation->PrintfLine(DriverStationLCD::kUser_Line2, lines[1].c_str());
	dstation->PrintfLine(DriverStationLCD::kUser_Line3, lines[2].c_str());
	dstation->PrintfLine(DriverStationLCD::kUser_Line4, lines[3].c_str());
	dstation->PrintfLine(DriverStationLCD::kUser_Line5, lines[4].c_str());
	dstation->PrintfLine(DriverStationLCD::kUser_Line6, lines[5].c_str());
	dstation->UpdateLCD();
}

void Display::Clear() {
	dstation->PrintfLine(DriverStationLCD::kUser_Line1, "");
	dstation->PrintfLine(DriverStationLCD::kUser_Line2, "");
	dstation->PrintfLine(DriverStationLCD::kUser_Line3, "");
	dstation->PrintfLine(DriverStationLCD::kUser_Line4, "");
	dstation->PrintfLine(DriverStationLCD::kUser_Line5, "");
	dstation->PrintfLine(DriverStationLCD::kUser_Line6, "");
	//dstation->UpdateLCD();
}
