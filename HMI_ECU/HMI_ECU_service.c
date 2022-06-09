/***********************************************************************/
/***********    Author     : Tarek Elmenshawy       		************/
/***********    File Name  : HNI_ECU_service.c			************/
/***********    Date       : 07-06-2022             		************/
/***********    Description: App. services source file    	************/
/***********************************************************************/

#undef F_CPU
#define F_CPU 8000000UL

#include "HMI_ECU_service.h"
#include "uart.h"
#include "lcd.h"
#include "keypad.h"
#include <util/delay.h>

/***********************************************************************/
/***********				Functions Definitions			  **********/
/***********************************************************************/

static void sendPassword(uint8 * a_password)
{
	uint8 i;

	/* Receive a ready message from CONTROL-ECU */
	while(UART_receiveByte() != READY_TO_RECEIVE);

	/* Send password to CONTROL-ECU */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		/* Send byte by byte */
		UART_sendByte(a_password[i]);
	}

}

static uint8 checkPassword(uint8 * a_password)
{
	uint8 l_data, l_result;

	/* Send check password command to CONTROL-ECU */
	UART_sendByte(CHECK_PASSWORD);

	/* Send password to CONTROL-ECU */
	sendPassword(a_password);

	/* Receive the result from CONTROL-ECU */
	l_data = UART_receiveByte();

	switch(l_data)
	{
		case MATCH_PASSWORD:
			l_result = 1;
			break;

		case MISMATCH_PASSWORD:
			l_result = 0;
			break;
	}

	/* Return the checking result */
	return l_result;
}

void newPassword(void)
{
	uint8 i, l_passworSet = 1;
	uint8 l_password[PASSWORD_SIZE];

	/* Stuck in this loop to get a new confirmed password */
	while(l_passworSet)
	{
		/* Display a message to enter a new password */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Enter new pass. ");
		LCD_moveCursor(1, 0);

		/* Loop and get password digit by digit */
		for(i = 0; i < PASSWORD_SIZE; i++)
		{
			l_password[i] = KEYPAD_getPressedKey();
			LCD_displayCharacter('*');
			_delay_ms(300);
		}

		/* Wait for pressing enter button */
		while(KEYPAD_getPressedKey() != 13);
		_delay_ms(300);

		/* Display confirmation message */
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "Confirm pass.   ");
		LCD_moveCursor(1, 0);

		/* Password confirmation process*/
		l_passworSet = 0;
		for(i = 0; i < PASSWORD_SIZE; i++)
		{
			/* Check if password entered correctly */
			if(l_password[i] != KEYPAD_getPressedKey())
			{
				l_passworSet = 1;
			}
			LCD_displayCharacter('*');
			_delay_ms(300);
		}

		/* Wait for pressing enter button */
		while(KEYPAD_getPressedKey() != 13);
		_delay_ms(300);

		/* If password confirmation process fail, repeat the process */
		if(l_passworSet)
		{
			/* Display error message */
			LCD_displayStringRowColumn(0, 0, "Password is not ");
			LCD_displayStringRowColumn(1, 0, "   confirmed.   ");
			_delay_ms(1000);
		}
	}

	/* Send save password command to CONTROL-ECU */
	UART_sendByte(SAVE_PASSWORD);
	/* Send password to CONTROL-ECU */
	sendPassword(l_password);

	/* Wait for saving confirmation message */
	while(UART_receiveByte() != SAVED_PASSWORD);

	/* Display message of saving password */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Password Saved! ");
	_delay_ms(500);
}

uint8 getPassword(void)
{
	uint8 i;
	uint8 l_password[PASSWORD_SIZE];

	/* Display a message to enter a password */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Enter password: ");
	LCD_moveCursor(1, 0);

	/* Loop and get password digit by digit */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		l_password[i] = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		_delay_ms(300);
	}

	/* Wait for pressing enter button */
	while(KEYPAD_getPressedKey() != 13);
	_delay_ms(300);

	/* Check the password and return the result */
	return checkPassword(l_password);

}

void openDoor(void)
{
	/* Send open door command to CONTROL-ECU */
	UART_sendByte(OPEN_DOOR);

	/* Wait for the first response from CONTROL-ECU */
	while(UART_receiveByte() != DOOR_IS_OPENING);

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is opening ");

	/* Wait for the second response from CONTROL-ECU */
	while(UART_receiveByte() != DOOR_IS_OPEN);

	LCD_displayStringRowColumn(0, 0, "Door is unlocked");

	/* Wait for the third response from CONTROL-ECU */
	while(UART_receiveByte() != DOOR_IS_CLOSING);

	LCD_displayStringRowColumn(0, 0, "Door is closing ");

	/* Wait for the fourth response from CONTROL-ECU */
	while(UART_receiveByte() != DOOR_IS_CLOSE);

}

void danger(void)
{
	/* Send danger command to CONTROL-ECU */
	UART_sendByte(DANGER);

	/*Display an error message */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "     ERROR!     ");
	LCD_displayStringRowColumn(1, 0, "     THIEF!     ");

	/* Wait for a response from CONTROL-ECU */
	while(UART_receiveByte() != DANGER_TIME_OFF);
}
