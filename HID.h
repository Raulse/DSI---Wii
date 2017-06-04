#pragma once
#include <windows.h>



class HID
{
protected:
	enum Estado{ DEFAULT, ONE, TWO, THREE, VIBRACION };

public:
	virtual void actualiza(); //Actualiza Mando2HID Y HID2Mando

	//Entrada
	bool DeadZoneOn, blockVibration;

	//Joysticks
	float thumbLX, thumbLY, thumbRX, thumbRY;
	
	//Botones
	bool buttonA, buttonB, buttonX, buttonY;
	bool lastButtonA, lastButtonB, lastButtonX, lastButtonY;

	//Cursores
	bool buttonUp, buttonDown, buttonLeft, buttonRight;
	bool lastButtonUp, lastButtonDown, lastButtonLeft, lastButtonRight;

	//Start y options
	bool buttonStart, buttonSelect;
	bool lastButtonStart, lastButtonSelect;

	//L1 y R1
	bool buttonL1, buttonR1;
	bool lastButtonL1, lastButtonR1;

	//L3 Y R3
	bool buttonL3, buttonR3;
	bool lastButtonL3, lastButtonR3;

	//Gatillos
	float leftTrigger, rightTrigger;

	//Salida
	float leftVibration, rightVibration;//Valores de vibración

	Estado estado = Estado::DEFAULT;
	float timeState = 0;

protected:
	virtual bool LeeMando() = 0;//Lee el estado del mando
	virtual void EscribeMando() = 0;//Escribe feedback en mando
	virtual void Mando2HID() = 0; //Vuelca el estado del mando al HID

	bool mandoConectado;
};

