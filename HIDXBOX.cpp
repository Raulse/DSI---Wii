#include "HIDXBOX.h"


HIDXBOX::HIDXBOX()
{
	DeadZoneOn = true;
	blockVibration = false;
	vibraIzquierda = false;
	vibraDerecha = false;
	timeL =timeR = 0;
}


HIDXBOX::~HIDXBOX()
{
}


bool HIDXBOX::LeeMando(){
	XBox.lastState = XBox.state;//Guardo el estado anterior
	XBox.dwResult = XInputGetState(0, &XBox.state);//Obtengo el estado actual
	if (XBox.dwResult == ERROR_SUCCESS)
		return true;
	else
		return false;

}

void HIDXBOX::EscribeMando(){
	if (!blockVibration)
		Vibracion();
}

void HIDXBOX::Vibracion()
{
	//L2
	// Map bLeftTrigger's 0-255 to wLeftMotorSpeed's 0-65535
	if (leftTrigger > 0)
		leftVibration = ((leftTrigger + 1) * 256) - 1;

	//Soltar boton
	else if (!vibraDerecha && vibraIzquierda && timeL < 1)
	{
		leftVibration = 30000;
		timeL += 0.08;
	}
	else
	{
		leftVibration = 0;
		vibraIzquierda = false;
	}

	//R2
	// Map bRightTrigger's 0-255 to wRightMotorSpeed's 0-65535
	if (rightTrigger > 0)
		rightVibration = ((rightTrigger + 1) * 256) - 1;

	//Pulsar boton
	else if (vibraDerecha && timeR < 1)
	{
		rightVibration = 30000;
		timeR += 0.08;
	}
	else
	{
		rightVibration = 0;
		vibraDerecha = false;
	}

	if (estado == Estado::VIBRACION)
	{
		leftVibration = 30000;
		rightVibration = 30000;

	}

	XBox.vibration.wLeftMotorSpeed = leftVibration;
	XBox.vibration.wRightMotorSpeed = rightVibration;

	XInputSetState(0, &XBox.vibration);
}


void HIDXBOX::Mando2HID(){
	WORD wLastButtons = XBox.lastState.Gamepad.wButtons;
	WORD wButtons = XBox.state.Gamepad.wButtons;

	//Joysticks
	//Joysticks
	thumbLX = XBox.state.Gamepad.sThumbLX;
	thumbLY = XBox.state.Gamepad.sThumbLY;
	thumbRX = XBox.state.Gamepad.sThumbRX;
	thumbRY = XBox.state.Gamepad.sThumbRY;

	if (DeadZoneOn)
		DeadZoneJoystick();

	//Botones
	buttonA = wButtons & XINPUT_GAMEPAD_A && !(wLastButtons & XINPUT_GAMEPAD_A);
	buttonB = wButtons & XINPUT_GAMEPAD_B && !(wLastButtons & XINPUT_GAMEPAD_B);
	buttonX = wButtons & XINPUT_GAMEPAD_X && !(wLastButtons & XINPUT_GAMEPAD_X);
	buttonY = wButtons & XINPUT_GAMEPAD_Y && !(wLastButtons & XINPUT_GAMEPAD_Y);
	if (buttonA || buttonB || buttonX || buttonY)
	{
		timeR = 0;
		vibraDerecha = true;
	}

	lastButtonA = wLastButtons & XINPUT_GAMEPAD_A && !(wButtons & XINPUT_GAMEPAD_A);
	lastButtonB = wLastButtons & XINPUT_GAMEPAD_B && !(wButtons & XINPUT_GAMEPAD_B);
	lastButtonX = wLastButtons & XINPUT_GAMEPAD_X && !(wButtons & XINPUT_GAMEPAD_X);
	lastButtonY = wLastButtons & XINPUT_GAMEPAD_Y && !(wButtons & XINPUT_GAMEPAD_Y);
	if (lastButtonA || lastButtonB || lastButtonX || lastButtonY)
	{
		timeL = 0;
		vibraIzquierda = true;
	}

	//Cursores
	buttonUp = wButtons & XINPUT_GAMEPAD_DPAD_UP && !(wLastButtons & XINPUT_GAMEPAD_DPAD_UP);
	buttonDown= wButtons & XINPUT_GAMEPAD_DPAD_DOWN && !(wLastButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	buttonLeft = wButtons & XINPUT_GAMEPAD_DPAD_LEFT && !(wLastButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	buttonRight = wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !(wLastButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

	lastButtonUp = wLastButtons & XINPUT_GAMEPAD_DPAD_UP && !(wButtons & XINPUT_GAMEPAD_DPAD_UP);
	lastButtonDown = wLastButtons & XINPUT_GAMEPAD_DPAD_DOWN && !(wButtons & XINPUT_GAMEPAD_DPAD_DOWN);
	lastButtonLeft = wLastButtons & XINPUT_GAMEPAD_DPAD_LEFT && !(wButtons & XINPUT_GAMEPAD_DPAD_LEFT);
	lastButtonRight = wLastButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !(wButtons & XINPUT_GAMEPAD_DPAD_RIGHT);

	//Start y options
	buttonStart = wButtons & XINPUT_GAMEPAD_START && !(wLastButtons & XINPUT_GAMEPAD_START);
	buttonSelect = wButtons & XINPUT_GAMEPAD_BACK && !(wLastButtons & XINPUT_GAMEPAD_BACK);

	lastButtonStart = wLastButtons & XINPUT_GAMEPAD_START && !(wButtons & XINPUT_GAMEPAD_START);
	lastButtonSelect = wLastButtons & XINPUT_GAMEPAD_BACK && !(wButtons & XINPUT_GAMEPAD_BACK);

	//L1 y R1
	buttonL1 = wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !(wLastButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
	buttonR1 = wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !(wLastButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);

	lastButtonL1 = wLastButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !(wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER);
	lastButtonR1 = wLastButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !(wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER);

	//L3 Y R3
	buttonL3 = wButtons & XINPUT_GAMEPAD_LEFT_THUMB && !(wLastButtons & XINPUT_GAMEPAD_LEFT_THUMB);
	buttonR3 = wButtons & XINPUT_GAMEPAD_RIGHT_THUMB && !(wLastButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

	lastButtonL3 = wLastButtons & XINPUT_GAMEPAD_LEFT_THUMB && !(wButtons & XINPUT_GAMEPAD_LEFT_THUMB);
	lastButtonR3 = wLastButtons & XINPUT_GAMEPAD_RIGHT_THUMB && !(wButtons & XINPUT_GAMEPAD_RIGHT_THUMB);

	//L2 Y R2
	leftTrigger = XBox.state.Gamepad.bLeftTrigger;
	rightTrigger = XBox.state.Gamepad.bRightTrigger;
	
}

void HIDXBOX::DeadZoneJoystick()
{
	// Zero value if thumbsticks are within the dead zone 
	if ((thumbLX < INPUT_DEADZONE &&
		thumbLX > -INPUT_DEADZONE) &&
		(thumbLY < INPUT_DEADZONE &&
		thumbLY > -INPUT_DEADZONE))
	{
		thumbLX = 0;
		thumbLY = 0;
	}

	if ((thumbRX < INPUT_DEADZONE &&
		thumbRX > -INPUT_DEADZONE) &&
		(thumbRY < INPUT_DEADZONE &&
		thumbRY > -INPUT_DEADZONE))
	{
		thumbRX = 0;
		thumbRY = 0;
	}

}


