
//-----------------------------------------------------------------------------
// File: RumbleController.cpp
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#define STRICT
#include <windows.h>
#include <commdlg.h>
#include <XInput.h>
#include <basetsd.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )
#include "resource.h"
#include "HIDXBOX.h"

#include "HIDWII.h"

//-----------------------------------------------------------------------------
// Function-prototypes
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
VOID CALLBACK TimerCallBack();
void generaEventos(HID * Control);

void InputBloqueoVibracion(HID * Control);
void InputBotones(HID * Control, POINT pt);
void InputJoystick(HID * Control, POINT& pt);

//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
//-----------------------------------------------------------------------------
// Struct to hold xinput state
//-----------------------------------------------------------------------------

//WCHAR g_szMessage[4][1024] = {0};
HWND g_hWnd;

static int repeat = 1; // repeat key counter

HID *Control;

enum Mando {Xbox_t,Wii_t};

const Mando mando = Wii_t;
//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: Entry point for the application.  Since we use a simple dialog for 
//       user interaction we don't need to pump messages.
//-----------------------------------------------------------------------------

int APIENTRY wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, int )
{
    // Register the window class
    // Init state
   // ZeroMemory( g_Controllers, sizeof( CONTROLER_STATE ) * MAX_CONTROLLERS );
	
	switch (mando)
	{
	case Xbox_t:
		Control = new HIDXBOX();
		break;

	case Wii_t:
		Control = new HIDWII();
		break;
	}

	SetTimer(g_hWnd, 0, 10,(TIMERPROC) TimerCallBack);

	// Run the message loop.
	//Coge eventos del usuario y del SO
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))//El parametro de null es para indicar a que ventana, siguientes para indicar orden(teclado o raton)
	{
		TranslateMessage(&msg);//Traduce el input en caracteres
		DispatchMessage(&msg);//Cada vez que se le llama, causa indirectamente que que invoca a WindowProc
	}//Si mientras se esperan mensajes necesitas ejecutar algo, mirar documentacion(1)
    
	KillTimer(g_hWnd, 0);

    // Clean up 
    UnregisterClass( L"XInputSample", NULL );

    return 0;
}

VOID CALLBACK TimerCallBack(){
	Control->actualiza();
	generaEventos(Control);

}

//-----------------------------------------------------------------------------
void generaEventos(HID * Control)
{
	POINT pt; // cursor location
	GetCursorPos(&pt);

	InputBloqueoVibracion(Control);

	InputBotones(Control,pt);

	//ScreenToClient(g_hWnd, &pt);

	InputJoystick(Control,pt);

	//ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}


//-----------------------------------------------------------------------------

void InputBloqueoVibracion(HID * Control)
{
	//Si pulsamos un botón y no habiamos pulsado en el estado anterior

		//Si hay algo de vibración, para cambiar de estado y bloquear la vibración
		if (!(!Control->blockVibration && Control->rightVibration == 0 &&
			Control->leftVibration == 0))
			Control->blockVibration = !Control->blockVibration;
	
}

void InputBotones(HID * Control, POINT pt){

	//Click izquierdo
	if (Control->buttonA)
		mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, NULL);
	else if (Control->lastButtonA)
		mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, NULL);

	//Click derecho

	if (Control->buttonB)
		mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, NULL);
	else if (Control->lastButtonB)
		mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, NULL);

	//AV PAG
	if (Control->buttonY)
		keybd_event(VK_PRIOR, 0x21, KEYEVENTF_EXTENDEDKEY | 0, 0);

	else if (Control->lastButtonY){};

	//RE PAG
	if (Control->buttonX)
		keybd_event(VK_NEXT, 0x22, KEYEVENTF_EXTENDEDKEY | 0, 0);
	else if (Control->lastButtonX){};


	//ENTER
	if (Control->buttonStart)
		keybd_event(VK_RETURN, 0x0D, KEYEVENTF_EXTENDEDKEY | 0, 0);
	else if (Control->lastButtonStart){};

	//ESCAPE
	if (Control->buttonSelect)
		keybd_event(VK_ESCAPE, 0x1B, KEYEVENTF_EXTENDEDKEY | 0, 0);
	else if (Control->lastButtonSelect){};


	//CURSORES

	//DETECTO SI SE PULSA
	if (Control->buttonLeft)
		keybd_event(VK_LEFT, 0x25, KEYEVENTF_EXTENDEDKEY | 0, 0);
	//SI SE LIBERA
	else if (Control->lastButtonLeft)
		keybd_event(VK_LEFT, 0x25, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP | 0, 0);

	if (Control->buttonUp)
		keybd_event(VK_UP, 0x26, KEYEVENTF_EXTENDEDKEY | 0, 0);
	else if (Control->lastButtonUp)
		keybd_event(VK_UP, 0x26, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP | 0, 0);

	if (Control->buttonRight)
		keybd_event(VK_RIGHT, 0x27, KEYEVENTF_EXTENDEDKEY | 0, 0);
	else if (Control->lastButtonRight)
		keybd_event(VK_RIGHT, 0x27, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP | 0, 0);

	if (Control->buttonDown)
		keybd_event(VK_DOWN, 0x28, KEYEVENTF_EXTENDEDKEY | 0, 0);
	else if (Control->lastButtonDown)
		keybd_event(VK_DOWN, 0x28, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP | 0, 0);


	//RUEDA DEL RATON
	if (Control->leftTrigger > 50)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, -Control->leftTrigger /3, NULL);
	if (Control->rightTrigger > 50)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, Control->rightTrigger / 3, NULL);

	if (Control->buttonR3)
		PostQuitMessage(0);

}

void InputJoystick(HID * Control, POINT& pt)
{
	if (Control->thumbLY != 0)
		pt.y -= Control->thumbLY; /// 3276.7;
	if (Control->thumbLX != 0)
		pt.x += Control->thumbLX; /// 3276.7;

}


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Window message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_ACTIVATEAPP:
        {
            if( wParam == TRUE )
            {
                // App is now active, so re-enable XInput
                XInputEnable( true );
            }
            else
            {
                // App is now inactive, so disable XInput to prevent
                // user input from effecting application and to 
                // disable rumble. 
				hWnd = GetActiveWindow();//ESTO NO SE DONDE VA

				//XInputEnable( false );
            }


            break;
        }

		
		case WM_KEYDOWN:
		{
			if (wParam == 'D')
			{
				Control->DeadZoneOn = !Control->DeadZoneOn;
				break;
			}

			if (wParam != VK_LEFT && wParam != VK_RIGHT &&
				wParam != VK_UP && wParam != VK_DOWN)
				break;
			
			POINT pt;
			GetCursorPos(&pt);
			// Convert screen coordinates to client coordinates.
			ScreenToClient(hWnd, &pt);

			switch (wParam)
			{
				// Move the cursor to reflect which
				// arrow keys are pressed.
			case VK_LEFT: // left arrow
				pt.x -= repeat;
				break;
			case VK_RIGHT: // right arrow
				pt.x += repeat;
				break;
			case VK_UP: // up arrow
				pt.y -= repeat;
				break;
			case VK_DOWN: // down arrow
				pt.y += repeat;
				break;

			default:
				return 0;
			}


			repeat++; // Increment repeat count.
			// Keep the cursor in the client area.
			RECT rc; // client area coordinates
			GetClientRect(hWnd, &rc);
			if (pt.x >= rc.right)
			{
				pt.x = rc.right - 1;
			}
			else
			{
				if (pt.x < rc.left)
				{
					pt.x = rc.left;
				}
			}
			if (pt.y >= rc.bottom)
				pt.y = rc.bottom - 1;
			else
				if (pt.y < rc.top)
					pt.y = rc.top;
			// Convert client coordinates to screen coordinates.
			ClientToScreen(hWnd, &pt);
			SetCursorPos(pt.x, pt.y);
			return 0;
		}

		case WM_KEYUP:
			repeat = 1; // Clear repeat count.
			return 0;


        case WM_DESTROY:
        {
            PostQuitMessage( 0 );
            break;
        }
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}



