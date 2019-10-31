// 
// 
// 

#include "Odometry_Manager.h"

float MD25Controller::GetBatteryVoltage()
{
	// GetBatteryVoltage() //
	// Function to get battery voltage from MD25 //

	Transmit(m_VoltageReg);								// Send batt volt command to MD25
	Wire.requestFrom(m_MD25Address, (uint8_t)1);		// Request data from MD25

	while (Wire.available() < 1);						// Wait for data
	
	
	float o_BatteryVolts = Wire.read();	
	o_BatteryVolts = o_BatteryVolts / 10;				// Divide by 10 to get real value (1.d.p)

	return o_BatteryVolts;
}

float MD25Controller::GetMotorCurrent(const int i_Index)
{
	// GetMotorCurrent() //
	// Function to get motor current from MD25 //

	// Transmit the correct register for the motor we want the current for
	if (i_Index == 1)
	{
		Transmit(m_Motor1CurrentReg);
	}
	else if (i_Index == 2)
	{
		Transmit(m_Motor2CurrentReg);
	}
	
	// NEEDS CHANGING TO PREVENT ERRORS WHEN MOTOR ISN'T 1 or 2


	Wire.requestFrom(m_MD25Address, (uint8_t)1);		// Request data from MD25

	while (Wire.available() < 1);						// Wait for data

	float o_MotorCurrent = Wire.read();
	o_MotorCurrent = o_MotorCurrent / 10;				// Divide by 10 to get real value (1.d.p)

	return o_MotorCurrent;
}

void MD25Controller::Transmit(const unsigned char i_Command, const unsigned char i_Value)
{
	Wire.beginTransmission(m_MD25Address);
	Wire.write(i_Command);
	Wire.write(i_Value);
	Wire.endTransmission();
}

void MD25Controller::Transmit(const unsigned char i_Command)
{
	Wire.beginTransmission(m_MD25Address);
	Wire.write(i_Command);
	Wire.endTransmission();
}

void MD25Controller::SetMotorSpeed(const uint8_t i_Motor, const uint8_t i_Speed)
{

}
