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
	Transmit(m_AccelerationReg, m_AccelerationValue);

	if (i_Motor == 1)
	{
		Transmit(m_Speed1Reg, i_Speed);
	}
	else if (i_Motor == 2)
	{
		Transmit(m_Speed2Reg, i_Speed);
	}
}

void MD25Controller::SetAccelerationValue(const uint8_t i_Acceleration)
{
	// Sets the internal value of the acceleration to a given int

	// The acceleration value cannot be less than 1
	// The acceleration value cannot be more than 10

	if (i_Acceleration <= 1)
	{
		m_AccelerationValue = 1;
	}
	else if (i_Acceleration >= 10)
	{
		m_AccelerationValue = 10;
	}
	else
	{
		m_AccelerationValue = i_Acceleration;
	}
}

void MD25Controller::SetSpeedRegulation(const bool i_EnableSpeedRegulation)
{
	if (i_EnableSpeedRegulation == false)
	{
		Transmit(m_DisableSpeedRegulationReg);
	}
	else
	{
		Transmit(m_EnableSpeedRegulationReg);
	}
}

void MD25Controller::ResetEncoders()
{
	Transmit(m_EncoderZeroReg);
}

float MD25Controller::GetEncoderValue(const uint8_t i_Motor)
{
	if (i_Motor != 1 || i_Motor != 2)
	{
		// We're trying to access a motor that doesn't exist
		// Do nothing
	}
	else
	{
		// We're trying to read an existing motor
		if (i_Motor == 1)
		{
			// We're trying to read motor 1
			Transmit(m_Enc1aReg);
		}
		else
		{
			// We're trying to read motor 2
			Transmit(m_Enc2aReg);
		}

		Wire.requestFrom(m_MD25Address, (uint8_t)4 );		// Request data from MD25
		while (Wire.available() < 4);						// Wait for data
		
		uint32_t o_EncoderValue = Wire.read();

		for (int i = 0; i <= 2; i++)						// Loop though 3 times to get each byte of data from MD25
		{
			o_EncoderValue <<= 8;
			o_EncoderValue += Wire.read();
		}

		return o_EncoderValue;
	}


}

void MD25Controller::SetMode(const uint8_t i_Mode)
{
	if (i_Mode >= 0 || i_Mode <= 3)
	{
		// We are trying to send a valid mode

		Transmit(m_ModeReg, i_Mode);
		m_Mode = i_Mode;
	}
	else
	{
		// We are trying to send and invalid mode

		// Do nothing
	}
}
