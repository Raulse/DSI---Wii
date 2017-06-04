
#include "HIDWII.h"

using namespace CyberGarage;

HIDWII::HIDWII()
{
	//Inicializa wiimote
	wiimote = new Wiimote();
	//Caso de error
	if (wiimote->open() == false) {
		delete wiimote;
		printf("Could not find Wiimote !!");
		PostQuitMessage(0);
	}
}


HIDWII::~HIDWII()
{
	wiimote->close();
	delete wiimote;
}

bool HIDWII::LeeMando(){
	lastState = currentState;
	wiimote->read();

	//Botones
	currentState.buttonA = wiimote->IsAPressed();
	currentState.buttonB = wiimote->IsBPressed();
	currentState.button1= wiimote->IsOnePressed();
	currentState.button2 = wiimote->IsTwoPressed();

	currentState.buttonPlus = wiimote->IsPlusPressed();
	currentState.buttonMinus = wiimote->IsMinusPressed();

	currentState.buttonHome = wiimote->IsHomePressed();

	//Cursores
	currentState.buttonUp = wiimote->IsUpPressed();
	currentState.buttonDown = wiimote->IsDownPressed();
	currentState.buttonLeft = wiimote->IsLeftPressed();
	currentState.buttonRight = wiimote->IsRightPressed();

	//Joysticks
	currentState.XMotion = wiimote->getXMotion() - 0x80;
	currentState.YMotion = wiimote->getYMotion() - 0x80;
	currentState.ZMotion = wiimote->getZMotion() - 0x80;

	Sleep(10);
	return true;
}

void HIDWII::Mando2HID(){
	//Botones
	buttonA = currentState.buttonA && !lastState.buttonA;
	lastButtonA = lastState.buttonA && !currentState.buttonA;

	buttonB = currentState.buttonB && !lastState.buttonB;
	lastButtonB = lastState.buttonB&& !currentState.buttonB;

	buttonX = currentState.button1 && !lastState.button1;
	lastButtonX = lastState.button1&& !currentState.button1;

	buttonY = currentState.button2 && !lastState.button2;
	lastButtonY = lastState.button2&& !currentState.button2;

	buttonStart = currentState.buttonPlus && !lastState.buttonPlus;
	lastButtonStart = lastState.buttonPlus && !currentState.buttonPlus;

	buttonSelect = currentState.buttonMinus&& !lastState.buttonMinus;
	lastButtonSelect = lastState.buttonMinus&& !currentState.buttonMinus;

	//Cursores 
	buttonUp = currentState.buttonUp && !lastState.buttonUp;
	lastButtonUp = lastState.buttonUp&& !currentState.buttonUp;

	buttonDown = currentState.buttonDown && !lastState.buttonDown;
	lastButtonDown = lastState.buttonDown && !currentState.buttonDown;

	buttonLeft = currentState.buttonLeft && !lastState.buttonLeft;
	lastButtonLeft = lastState.buttonLeft&& !currentState.buttonLeft;

	buttonRight = currentState.buttonRight && !lastState.buttonRight;
	lastButtonRight = lastState.buttonRight && !currentState.buttonRight;

	buttonR3 = currentState.buttonHome && !lastState.buttonHome;
	lastButtonR3 = lastState.buttonHome && !currentState.buttonHome;

	//AQUI DEBEMOS TRANSFORMAR LOS MOTION EN THUMBLX Y THUMBLY
	thumbLX = currentState.XMotion;
	thumbLY = -currentState.YMotion;

	DeadZone();
	
}
void HIDWII::EscribeMando(){
	//NO HAY VIBRACION
}

void HIDWII::DeadZone()
{
	// Zero value if thumbsticks are within the dead zone 
	if ((thumbLX < 10 &&
		thumbLX > -10) &&
		(thumbLY < 10 &&
		thumbLY > -10))
	{
		thumbLX = 0;
		thumbLY = 0;
	}

	if ((thumbRX < 10 &&
		thumbRX > -10) &&
		(thumbRY < 10 &&
		thumbRY > -10))
	{
		thumbRX = 0;
		thumbRY = 0;
	}

}

