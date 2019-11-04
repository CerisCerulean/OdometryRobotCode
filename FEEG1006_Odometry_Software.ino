#include "Odometry_Manager.h"

#define ODOTRACK
// #define SQUARETEST
// #define SEMICIRCLETEST

#define DEFAULTSPEED 1

void setup()
{
	Wire.begin();			// Begin i2c wire

	OdometryController OdometryManager;

	OdometryManager.SetBuzzerPin(8);
	OdometryManager.SetServoPin(6);
	OdometryManager.SetLEDPin(5);

#ifdef ODOTRACK

	OdometryManager.Add_Move_Straight(34, DEFAULTSPEED);		// Start -> 1
	OdometryManager.Add_Move_Turn(90, DEFAULTSPEED);
	OdometryManager.Add_Point();
	OdometryManager.Add_Move_Straight(26, DEFAULTSPEED);		// 1 -> 2
	OdometryManager.Add_Move_Turn(-90, DEFAULTSPEED);
	OdometryManager.Add_Point();

	// Drop Sample @ 2
	OdometryManager.Add_DropIndicator(0);
	
	OdometryManager.Add_Move_Straight(50, DEFAULTSPEED);		// 2 -> 3
	OdometryManager.Add_Move_Turn(-90, DEFAULTSPEED);
	OdometryManager.Add_Point();
	OdometryManager.Add_Move_Circle(26, 90, DEFAULTSPEED, true);		// 3 -> 4
	OdometryManager.Add_Move_Turn(90, DEFAULTSPEED);
	OdometryManager.Add_Point();

	// Drop Sample @ 4
	OdometryManager.Add_DropIndicator(45);
	
	OdometryManager.Add_Move_Straight(66, DEFAULTSPEED);		// 4 -> 5
	OdometryManager.Add_Move_Turn(-90, DEFAULTSPEED);
	OdometryManager.Add_Point();
	OdometryManager.Add_Move_Straight(40, DEFAULTSPEED);		// 5 -> 6
	OdometryManager.Add_Move_Turn(-90, DEFAULTSPEED);
	OdometryManager.Add_Point();

	// Drop Sample @ 6
	OdometryManager.Add_DropIndicator(90);

	OdometryManager.Add_Move_Straight(40, DEFAULTSPEED);		// 6 -> 7
	OdometryManager.Add_Move_Turn(-90, DEFAULTSPEED);
	OdometryManager.Add_Point();
	OdometryManager.Add_Move_Straight(40, DEFAULTSPEED);		// 7 -> 8
	OdometryManager.Add_Move_Turn(-40, DEFAULTSPEED);
	OdometryManager.Add_Point();

	// Drop Sample @ 8
	OdometryManager.Add_DropIndicator(135);

	OdometryManager.Add_Move_Straight(62.2, DEFAULTSPEED);		// 8 -> 9
	OdometryManager.Add_Move_Turn(-140, DEFAULTSPEED);
	OdometryManager.Add_Point();
	OdometryManager.Add_Move_Straight(18, DEFAULTSPEED);		// 9 -> 10
	OdometryManager.Add_Move_Turn(90, DEFAULTSPEED);
	OdometryManager.Add_Point();

	// Drop Sample @ 10
	OdometryManager.Add_DropIndicator(180);

	OdometryManager.Add_Move_Circle(18, 270, DEFAULTSPEED, true);	// 10 -> 11
	OdometryManager.Add_Move_Turn(-142.6, DEFAULTSPEED);
	OdometryManager.Add_Point();
	OdometryManager.Add_Move_Straight(37.2, DEFAULTSPEED);		// 11 -> 12
	OdometryManager.Add_Point();
	OdometryManager.Add_Move_Straight(42.8, DEFAULTSPEED);		// 11 -> 13
	OdometryManager.Add_Point();


#endif // ODOTRACK

#ifdef SQUARETEST
	for (int i = 0; i <= 3; i++)
	{
		OdometryManager.Add_Move_Straight(20, 1);
		OdometryManager.Add_Move_Turn(90, 1);
	}
#endif //SQUARETEST

#ifdef SEMICIRCLETEST
	OdometryManager.Add_Move_Circle(20, 180, true, DEFAULTSPEED);
	OdometryManager.Add_Move_Turn(180, DEFAULTSPEED);
	OdometryManager.Add_Move_Circle(20, 180, false, DEFAULTSPEED);
	OdometryManager.Add_Move_Turn(180, DEFAULTSPEED);
#endif //SEMICIRCLETEST


	// Wait for a second before starting
	delay(1000);

	OdometryManager.ExecutePath();
}

void loop() {}