#pragma once
#include "HID.h"
#include <XInput.h>

//Para que solo detecte el joystick si se mueve un bastante
#define INPUT_DEADZONE  ( 0.24f * FLOAT(0x7FFF) / 3276.7 )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

struct CONTROLER_STATE
{
	XINPUT_STATE lastState;
	XINPUT_STATE state;
	DWORD dwResult;
	XINPUT_VIBRATION vibration;
};

class HIDXBOX: public HID
{
public:
	HIDXBOX();
	~HIDXBOX();

private:
	virtual bool LeeMando();
	virtual void EscribeMando();
	virtual void Mando2HID();

	void Vibracion();
	void DeadZoneJoystick();

	CONTROLER_STATE XBox;
	bool vibraIzquierda, vibraDerecha;

	float timeL,timeR;
};

