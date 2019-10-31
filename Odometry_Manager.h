// Odometry_Manager.h

#ifndef _ODOMETRY_MANAGER_h
#define _ODOMETRY_MANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Wire.h>


#include <ArduinoSTL.h>

#define STRAIGHT 0
#define TURN 1
#define CIRCLE 2

//#include <Adafruit_LEDBackpack.h>

class ManeuverObject
{
public:
	uint8_t m_MoveType{ STRAIGHT };		// Maneuver Type
	// Straight = 0
	// Turn = 1
	// Circle = 2

	float m_SpeedModifier{ 1 };	// Modifier for speed (All)
	// 1 for 100% speed
	// 0.5 for 50% speed

	float m_Distance{ 0 };		// Distance of maneuver in cm (Straight)

	float m_Radius{ 0 };			// Radius of maneuver in cm (Circle)

	float m_Angle{ 0 };			// Angle of maneuver in degrees (Turn/Circle)

	bool m_Clockwise{ true };		// Direction of turn (Turn)
};

// Class to allow for control and management MD25
class MD25Controller
{
public:

	// Full Register Constructor for MD25Controller
	MD25Controller
	(
	uint8_t i_Speed1Reg = 0x00,
	uint8_t i_Speed2Reg = 0x01,

	uint8_t i_Enc1aReg = 0x02,
	uint8_t i_Enc1bReg = 0x03,
	uint8_t i_Enc1cReg = 0x04,
	uint8_t i_Enc1dReg = 0x05,

	uint8_t i_Enc2aReg = 0x06,
	uint8_t i_Enc2bReg = 0x07,
	uint8_t i_Enc2cReg = 0x08,
	uint8_t i_Enc2dReg = 0x09,

	uint8_t i_VoltageReg = 0x0A,
	uint8_t i_Motor1CurrentReg = 0x0B,
	uint8_t i_Motor2CurrentReg = 0x0C,

	uint8_t i_SoftwareReg = 0x0D,

	uint8_t i_AccelerationReg = 0x0E,

	uint8_t i_ModeReg = 0x10
	): 
		m_Speed1Reg(i_Speed1Reg),
		m_Speed2Reg(i_Speed2Reg),

		m_Enc1aReg(i_Enc1aReg),
		m_Enc1bReg(i_Enc1bReg),
		m_Enc1cReg(i_Enc1cReg),
		m_Enc1dReg(i_Enc1dReg),

		m_Enc2aReg(i_Enc2aReg),
		m_Enc2bReg(i_Enc2bReg),
		m_Enc2cReg(i_Enc2cReg),
		m_Enc2dReg(i_Enc2dReg),

		m_VoltageReg(i_VoltageReg),
		m_Motor1CurrentReg(i_Motor1CurrentReg),
		m_Motor2CurrentReg(i_Motor2CurrentReg),

		m_SoftwareReg(i_SoftwareReg),

		m_AccelerationReg(i_AccelerationReg),

		m_ModeReg(i_ModeReg)
	{}
	
	///////////////////////////
	// MD25 Member Variables //
	///////////////////////////
	float GetBatteryVoltage();

	float GetMotorCurrent(int i_Index);

	void SetMotorSpeed(const uint8_t i_Motor, const uint8_t i_Speed);

	void SetAccelerationValue(const uint8_t i_Acceleration);

	void SetSpeedRegulation(const bool i_EnableSpeedRegulation);

	void ResetEncoders();

	float GetEncoderValue(const uint8_t i_Motor);

	void SetMode(const uint8_t i_Mode);

private:

	// MD25 Controller Private Commands

	void Transmit(const unsigned char i_Command, const unsigned char i_Value);
	void Transmit(const unsigned char i_Command);


	///////////////////////////
	// MD25 Member Variables //
	///////////////////////////

	// MD25 Controller Variables

	int m_Mode{ 1 };

	uint8_t m_AccelerationValue{ 5 };

	// MD25 I2C Address
	const uint8_t m_MD25Address{ 0x58 };

	// MD25 Control Registers

	const uint8_t m_Speed1Reg{ 0x00 };
	const uint8_t m_Speed2Reg{ 0x01 };

	const uint8_t m_Enc1aReg{ 0x02 };
	const uint8_t m_Enc1bReg{ 0x03 };
	const uint8_t m_Enc1cReg{ 0x04 };
	const uint8_t m_Enc1dReg{ 0x05 };

	const uint8_t m_Enc2aReg{ 0x06 };
	const uint8_t m_Enc2bReg{ 0x07 };
	const uint8_t m_Enc2cReg{ 0x08 };
	const uint8_t m_Enc2dReg{ 0x09 };

	const uint8_t m_VoltageReg{ 0x0A };
	const uint8_t m_Motor1CurrentReg{ 0x0B };
	const uint8_t m_Motor2CurrentReg{ 0x0C };

	const uint8_t m_SoftwareReg{ 0x0D };

	const uint8_t m_AccelerationReg{ 0x0E };

	const uint8_t m_ModeReg{ 0x10 };


	// MD25 Special Command Registers

	const uint8_t m_EncoderZeroReg{ 0x20 };
	const uint8_t m_DisableSpeedRegulationReg{ 0x30 };
	const uint8_t m_EnableSpeedRegulationReg{ 0x31 };
	const uint8_t m_DisableMotorTimeoutReg{ 0x32 };
	const uint8_t m_EnableMotorTimeoutReg{ 0x33 };

};


// Class to manage high level functions of the robot
class OdometryController
{
public:


	void Add_Move_Straight(float i_Distance, float i_SpeedModifier);
	void Add_Move_Turn(float i_Angle, float i_SpeedModifier);
	void Add_Move_Circle(float i_Radius, float i_Angle, float i_SpeedModifier);

	void ExecutePath();
	 

private:

	void Move_Straight(ManeuverObject);
	void Move_Turn(ManeuverObject);
	void Move_Circle(ManeuverObject);

	std::vector<ManeuverObject> m_ManeuverVector;

	// MD25 Controller Object member variable
	MD25Controller* m_MD25 = new MD25Controller();

};



#endif
