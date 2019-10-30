// 
// 
// 

#include "Odometry_Manager.h"

float MD25::GetBatteryVoltage()
{
	// GetBatteryVoltage() //
	// Function to get battery voltage from MD25 //

	Transmit(m_VoltageRegister);			// Send batt volt command to MD25
	Wire.requestFrom(m_MD25Address, 1);		// Request data from MD25

	while (Wire.available() < 1);			// Wait for data
	
	
	float o_BatteryVolts = Wire.read();	
	o_BatteryVolts = o_BatteryVolts / 10;

	return o_BatteryVolts;
}

float MD25::GetMotorCurrent(const int i_Index)
{
	// GetMotorCurrent() //
	// Function to get motor current from MD25 //

	// Transmit the correct register for the motor we want the current for
	if (i_Index == 1)
	{
		Transmit(m_Motor1CurrentRegister);
	}
	else if (i_Index == 2)
	{
		Transmit(m_Motor2CurrentRegister);
	}
	
	Wire.requestFrom(m_MD25Address, 1);		// Request data from MD25

	while (Wire.available() < 1);			// Wait for data

	float o_MotorCurrent = Wire.read();
	o_MotorCurrent = o_MotorCurrent / 10;

	return o_MotorCurrent;
}

void MD25::Transmit(const unsigned char i_Command, const unsigned char i_Value)
{
	Wire.beginTransmission(m_MD25Address);
	Wire.write(i_Command);
	Wire.write(i_Value);
	Wire.endTransmission();
}

void MD25::Transmit(const unsigned char i_Command)
{
	Wire.beginTransmission(m_MD25Address);
	Wire.write(i_Command);
	Wire.endTransmission();
}
