/*
 * HMI_ECU.c
 *
 *  Created on: Jun 7, 2022
 *      Author: El-NM
 */
#undef F_CPU
#define F_CPU 8000000UL

#include "std_types.h"
#include "lcd.h"
#include "keypad.h"
#include "HMI_ECU_service.h"
#include "uart.h"
#include <util/delay.h>

int main(void)
{
	uint8 l_data, l_error;
	UART_ConfigType UART_configuration =
	{
		UART_8BIT_DATA,
		UART_DISABLE_PARITY,
		UART_1STOP_BIT,
		9600
	};

	/* Initialize UART to communicate wit CONTROL-ECU */
	UART_init(&UART_configuration);
	/* Initialize LCD */
	LCD_init();

	/* Receive the first message to indicate system status */
	l_data = UART_receiveByte();

	/* If there is no password saved, get a new password */
	if(NEW_PASSWORD == l_data)
	{
		/* Perform getting password sequence */
		newPassword();
	}

	while(1)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "+: Open Door    ");
		LCD_displayStringRowColumn(1, 0, "-: Change Pass. ");

		/* Stuck in this loop till an option is selected */
		do
		{
			l_data = KEYPAD_getPressedKey();
		}
		while(!(l_data == '+' || l_data == '-'));

		_delay_ms(300);

		/* Ask for password to perform the required option */
		l_error = 0;
		while(l_error < 3)
		{
			/* If password is entered correctly, perform selected option */
			if(getPassword())
			{
				switch(l_data)
				{
					case '+':
						/* Perform open door sequence */
						openDoor();
						break;

					case '-':
						/* Perform getting password sequence */
						newPassword();
						break;
				}
				break;
			}
			else
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0, 0, "Wrong Password! ");
				l_error++;
				if(l_error < 3)_delay_ms(1000);

			}
		}

		/* if password is wrongly entered 3 times, warn the user */
		if(l_error == 3)
		{
			/*Perform danger sequence */
			danger();
		}


	}

	return 0;
}
