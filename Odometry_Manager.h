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

	int m_Mode{ 1 };

	const unsigned char m_MD25Address{ 0x58 };

	// Memory Registers in MD25 with default values from datasheet
	const unsigned char m_Speed1Reg{ 0x00 };
	const unsigned char m_Speed2Reg{ 0x01 };

	const unsigned char m_Enc1aReg{ 0x02 };
	const unsigned char m_Enc1bReg{ 0x03 };
	const unsigned char m_Enc1cReg{ 0x04 };
	const unsigned char m_Enc1dReg{ 0x05 };

	const unsigned char m_Enc2aReg{ 0x06 };
	const unsigned char m_Enc2bReg{ 0x07 };
	const unsigned char m_Enc2cReg{ 0x08 };
	const unsigned char m_Enc2dReg{ 0x09 };

	const unsigned char m_VoltageReg{ 0x0A };
	const unsigned char m_Motor1CurrentReg{ 0x0B };
	const unsigned char m_Motor2CurrentReg{ 0x0C };

	const unsigned char m_SoftwareReg{ 0x0D };
	
	const unsigned char m_AccelerationReg{ 0x0E };

	const unsigned char m_ModeReg{ 0x10 };

};



#endif

