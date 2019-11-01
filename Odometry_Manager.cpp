#include "Odometry_Manager.h"

#define MOTOR1DIR 1
#define MOTOR2DIR -1

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

void MD25Controller::Transmit(const char i_Command, const char i_Value)
{
	Wire.beginTransmission(m_MD25Address);
	Wire.write(i_Command);
	Wire.write(i_Value);
	Wire.endTransmission();
}

void MD25Controller::Transmit(const char i_Command)
{
	Wire.beginTransmission(m_MD25Address);
	Wire.write(i_Command);
	Wire.endTransmission();
}

void MD25Controller::SetMotorSpeed(const uint8_t i_Motor, const int8_t i_Speed)
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
		
		int32_t encoderValue = Wire.read();

		for (int i = 0; i <= 2; i++)						// Loop though 3 times to get each byte of data from MD25
		{
			encoderValue <<= 8;
			encoderValue += Wire.read();
		}

		float o_EncoderDistance = encoderValue * 0.09;

		return o_EncoderDistance;
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

void OdometryController::Add_Move_Straight(float i_Distance, float i_SpeedModifier)
{
	// Create a ManeuverObject
	ManeuverObject straightMoveObject;

	// Setup ManeuverObject
	straightMoveObject.m_MoveType = STRAIGHT;
	straightMoveObject.m_Distance = i_Distance;
	straightMoveObject.m_SpeedModifier = i_SpeedModifier;

	// Add ManeuverObject to vector
	m_ManeuverVector.push_back(straightMoveObject);
}

void OdometryController::Add_Move_Turn(float i_Angle, float i_SpeedModifier)
{
	// Create a ManeuverObject
	ManeuverObject turnMoveObject;

	// Setup ManeuverObject
	turnMoveObject.m_MoveType = TURN;
	turnMoveObject.m_Angle = i_Angle;
	turnMoveObject.m_SpeedModifier = i_SpeedModifier;

	// Add ManeuverObject to vector
	m_ManeuverVector.push_back(turnMoveObject);
}

void OdometryController::Add_Move_Circle(float i_Radius, float i_Angle, bool i_Clockwise, float i_SpeedModifier)
{
	// Create a ManeuverObject
	ManeuverObject circleMoveObject;

	// Setup ManeuverObject
	circleMoveObject.m_MoveType = CIRCLE;
	circleMoveObject.m_Radius = i_Radius;
	circleMoveObject.m_Angle = i_Angle;
	circleMoveObject.m_Clockwise = i_Clockwise;
	circleMoveObject.m_SpeedModifier = i_SpeedModifier;

	// Add ManeuverObject to vector
	m_ManeuverVector.push_back(circleMoveObject);
}

void OdometryController::ExecutePath()
{
	// Itterate though the maneuver vector and execute each move
	for (ManeuverObject currentMove : m_ManeuverVector)
	{
		if (currentMove.m_MoveType == STRAIGHT)
		{
			Move_Straight(currentMove);
		}
		else if (currentMove.m_MoveType == TURN)
		{
			Move_Turn(currentMove);
		}
		else if (currentMove.m_MoveType == CIRCLE)
		{
			Move_Circle(currentMove);
		}

	}
}

void OdometryController::clearPath()
{
	m_ManeuverVector.clear();
}

void OdometryController::Move_Straight(ManeuverObject i_ManeuverObject)
{
	// Do straight line move
	
	// Set MD25 Mode to 1
	m_MD25->SetMode(1);
	
	m_MD25->ResetEncoders();

	m_MD25->SetAccelerationValue(5);

	// Set motor speeds
	m_MD25->SetMotorSpeed(1, i_ManeuverObject.m_SpeedModifier * 127 * MOTOR1DIR);
	m_MD25->SetMotorSpeed(2, i_ManeuverObject.m_SpeedModifier * 127 * MOTOR2DIR);

	float enc1 = m_MD25->GetEncoderValue(1);
	float enc2 = m_MD25->GetEncoderValue(2);

	float distanceToTarget1 = i_ManeuverObject.m_Distance - enc1 * MOTOR1DIR;
	float distanceToTarget2 = i_ManeuverObject.m_Distance - enc2 * MOTOR2DIR;

	bool withinSlowDistance = false;

	while ((distanceToTarget1 >= 0) && (distanceToTarget2 >= 0))
	{
		// We should continue moving
		
		// Check to see if we are within X units

		if (withinSlowDistance == false)
		{
			if ((distanceToTarget1 <= m_SlowDistance) || (distanceToTarget2 <= m_SlowDistance))
			{
				withinSlowDistance = true;
				// We are within the slow distance

				// Switch to a slower value
				m_MD25->SetAccelerationValue(5);

				m_MD25->SetMotorSpeed(1, i_ManeuverObject.m_SpeedModifier * 127 * m_SlowModifier * MOTOR1DIR);
				m_MD25->SetMotorSpeed(2, i_ManeuverObject.m_SpeedModifier * 127 * m_SlowModifier * -MOTOR2DIR);

			}
		}

		// Update encoder values
		enc1 = m_MD25->GetEncoderValue(1);
		enc2 = m_MD25->GetEncoderValue(2);

		distanceToTarget1 = i_ManeuverObject.m_Distance - enc1 * MOTOR1DIR;
		distanceToTarget2 = i_ManeuverObject.m_Distance - enc2 * MOTOR2DIR;

	}

	// We are at the target!
	Move_Stop();

}

void OdometryController::Move_Turn(ManeuverObject i_ManeuverObject)
{
	// Do turn move

	float wheelTravelDistance = (i_ManeuverObject.m_Angle / 360) * 3.141 * m_WheelbaseDistance;

	// Set MD25 Mode to 1
	m_MD25->SetMode(1);

	m_MD25->ResetEncoders();

	m_MD25->SetAccelerationValue(5);

	m_MD25->SetMotorSpeed(1, i_ManeuverObject.m_SpeedModifier * 127 * MOTOR1DIR);
	m_MD25->SetMotorSpeed(2, i_ManeuverObject.m_SpeedModifier * 127 * -MOTOR2DIR);

	float enc1 = m_MD25->GetEncoderValue(1);
	float enc2 = m_MD25->GetEncoderValue(2);

	float distanceToTarget1 = wheelTravelDistance - enc1 * MOTOR1DIR;
	float distanceToTarget2 = wheelTravelDistance - enc2 * -MOTOR2DIR;

	bool withinSlowDistance = false;

	while ((distanceToTarget1 >= 0) && (distanceToTarget2 >= 0))
	{
		// We should continue moving


		// Check to see if we are within X units

		if (withinSlowDistance == false)
		{
			if ((distanceToTarget1 <= m_SlowDistance) || (distanceToTarget2 <= m_SlowDistance))
			{
				withinSlowDistance = true;
				// We are within the slow distance

				// Switch to a slower value
				m_MD25->SetAccelerationValue(5);

				m_MD25->SetMotorSpeed(1, i_ManeuverObject.m_SpeedModifier * 127 * m_SlowModifier * MOTOR1DIR);
				m_MD25->SetMotorSpeed(2, i_ManeuverObject.m_SpeedModifier * 127 * m_SlowModifier * -MOTOR2DIR);
			}
		}

		// Update encoder values
		enc1 = m_MD25->GetEncoderValue(1);
		enc2 = m_MD25->GetEncoderValue(2);

		distanceToTarget1 = i_ManeuverObject.m_Distance - enc1 * MOTOR1DIR;
		distanceToTarget2 = i_ManeuverObject.m_Distance - enc2 * -MOTOR2DIR;

	}

	// We are at the target!
	Move_Stop();


}

void OdometryController::Move_Circle(ManeuverObject i_ManeuverObject)
{
	// Do circle move
	if (i_ManeuverObject.m_Radius > (m_WheelbaseDistance / 2))
	{
		float outsideTurnDistance = 2 * 3.141f * (i_ManeuverObject.m_Angle / 360) * (i_ManeuverObject.m_Radius + (m_WheelbaseDistance / 2));
		float insideTurnDistance = 2 * 3.141f * (i_ManeuverObject.m_Angle / 360) * (i_ManeuverObject.m_Radius - (m_WheelbaseDistance / 2));

		float motorSpeedRatio = insideTurnDistance / outsideTurnDistance;

		float outsideTurnSpeed = i_ManeuverObject.m_SpeedModifier * 127;
		float insideTurnSpeed = outsideTurnSpeed * motorSpeedRatio;

		// Default outside motor to 1
		uint8_t outsideMotor = 1;

		// Default inside motor to 2
		uint8_t insideMotor = 2;

		int8_t outsideMotorDirection = MOTOR1DIR;
		int8_t insideMotorDirection = MOTOR2DIR;

		if (i_ManeuverObject.m_Clockwise == false)
		{
			// We're turning anti-clockwise instead
			// Reverse motor defaults

			outsideMotor = 2;
			insideMotor = 1;
			outsideMotorDirection = MOTOR2DIR;
			insideMotorDirection = MOTOR1DIR;
		}

		m_MD25->SetMode(1);

		m_MD25->ResetEncoders();

		// Accelerate as fast as possible to get to set speed
		m_MD25->SetAccelerationValue(10);

		m_MD25->SetMotorSpeed(1, i_ManeuverObject.m_SpeedModifier * 127 * MOTOR1DIR);
		m_MD25->SetMotorSpeed(2, i_ManeuverObject.m_SpeedModifier * 127 * MOTOR2DIR);

		float encOutside = m_MD25->GetEncoderValue(outsideMotor);
		float encInside = m_MD25->GetEncoderValue(insideMotor);

		float distanceToTargetOutside = outsideTurnDistance - (encOutside * outsideMotorDirection);
		float distanceToTargetInside = insideTurnDistance - (encInside * insideMotorDirection);
		
		float outsidePercentageRemaining = 0;
		float insidePercentageRemaining = 0;

		while ((distanceToTargetInside >= 0) && (distanceToTargetOutside >= 0))
		{
			// We should continue moving

			encOutside = m_MD25->GetEncoderValue(outsideMotor);
			encInside = m_MD25->GetEncoderValue(insideMotor);

			distanceToTargetOutside = outsideTurnDistance - (encOutside * outsideMotorDirection);
			distanceToTargetInside = insideTurnDistance - (encInside * insideMotorDirection);
		}
		Move_Stop();
	}
}

void OdometryController::Move_Stop()
{
	m_MD25->SetMode(1);

	// Set acceleration value to max to slow down as quck as possible
	m_MD25->SetAccelerationValue(10);

	m_MD25->SetMotorSpeed(1, 0);
	m_MD25->SetMotorSpeed(2, 0);
}


