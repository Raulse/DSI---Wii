#pragma once
#include "HID.h"
//WII
#include <stdio.h>
#include "Wiimote.h"

struct WiiState{
	bool buttonA, buttonB, button1, button2;

	bool buttonPlus, buttonMinus;

	//Cursores
	bool buttonUp, buttonDown, buttonLeft, buttonRight;

	bool buttonHome;

	//boton = wiimote->IsHomePressed()
	float XMotion, YMotion, ZMotion;
};

class HIDWII : public HID
{
public:
	HIDWII();
	~HIDWII();

private:
	virtual bool LeeMando();
	virtual void EscribeMando();
	virtual void Mando2HID();
	void DeadZone();
	CyberGarage::Wiimote *wiimote;

	WiiState lastState;
	WiiState currentState;
};

