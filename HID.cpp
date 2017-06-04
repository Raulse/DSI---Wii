#include "HID.h"

void HID::actualiza(){
	timeState += 0.01;
	mandoConectado = LeeMando();
	if (mandoConectado)
	{
		Mando2HID();
		switch (estado)
		{
			case Estado::DEFAULT:
				if (buttonA)
				{
					estado = Estado::ONE;
					timeState = 0;
				}
				break;
			case Estado::ONE:
				if (timeState < 1)
				{
					if (buttonB)
					{
						estado = Estado::TWO;
						timeState = 0;
					}
				}
				else
					estado = Estado::DEFAULT;
				break;

			case Estado::TWO:
				if (timeState < 1)
				{
					if (buttonX)
					{
						estado = Estado::THREE;
						timeState = 0;
					}
				}
				else
					estado = Estado::DEFAULT;
				break;

			case Estado::THREE:
				if (timeState < 1)
				{
					if (buttonY)
					{
						estado = Estado::VIBRACION;
						timeState = 0;
					}
				}
				else
					estado = Estado::DEFAULT;
				break;
			case Estado::VIBRACION:
				if (timeState > 2)
					estado = Estado::DEFAULT;
				break;

		}

		//No está activado el bloqueo de vibración
		EscribeMando();
	}

}