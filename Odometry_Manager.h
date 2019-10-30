// Odometry_Manager.h

#ifndef _ODOMETRY_MANAGER_h
#define _ODOMETRY_MANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>




class MD25
{
public:
	float GetBatteryVoltage();

	float GetMotorCurrent(int i_Index);

	void Transmit(const unsigned char i_Command, const unsigned char i_Value);
	void Transmit(const unsigned char i_Command);


private:

	int m_Mode{ 0 };
	unsigned char m_MD25Address{ 0 };
	unsigned char m_VoltageRegister{ 0 };
	unsigned char m_Motor1CurrentRegister{ 0 };
	unsigned char m_Motor2CurrentRegister{ 0 };
};



#endif

