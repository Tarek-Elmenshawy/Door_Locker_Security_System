/***********************************************************************/
/***********    Author     : Tarek Elmenshawy       		************/
/***********    File Name  : CONTROL_ECU_service.c			************/
/***********    Date       : 07-06-2022             		************/
/***********    Description: App. services source file    	************/
/***********************************************************************/

#undef F_CPU
#define F_CPU 8000000UL

#include "CONTROL_ECU_service.h"
#include "uart.h"
#include "timer.h"
#include "eeprom.h"
#include "dc_motor.h"
#include "buzzer.h"

#include <util/delay.h>

static volatile uint8 g_ticks = 0;

/***********************************************************************/
/***********				Functions Definitions			  **********/
/***********************************************************************/

static uint8 comparePassword(uint8 * a_receivedPassword)
{
	uint8 i, l_oldPassword;

	/* Read and compare each password byte from EEPROM */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		/* Read a byte in the corresponding address */
		EEPROM_readByte(PASSWORD_ADDRESS + i, &l_oldPassword);

		/* Return error if a byte doesn't match */
		if(l_oldPassword != a_receivedPassword[i])
			return 0;
	}

	return 1;									/* Return success if two password match */
}

static void receivePassword(uint8 * a_password)
{
	uint8 i;

	UART_sendByte(READY_TO_RECEIVE);			/* Send ready message to HMI-ECU */

	/* Receive password form HMI-ECU */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		/* Receive byte by byte */
		a_password[i] = UART_receiveByte();
	}

}

uint8 isThereSavedPassword(void)
{
	uint8 l_data;

	EEPROM_readByte(KEY_ADDRESS, &l_data);		/* Read the key flag from EEPROM */

	/* Check key value if it is set, then there is a saved password */
	if(DEFAULT_KEY == l_data)	return 1;
	else						return 0;
}

void savePassword(void)
{
	uint8 i;
	uint8 l_password[PASSWORD_SIZE];

	receivePassword(l_password);				/* Receive password */

	/* Save password in EEPROM */
	for(i = 0; i < PASSWORD_SIZE; i++)
	{
		/* Write a byte in the corresponding address */
		EEPROM_writeByte(PASSWORD_ADDRESS + i, l_password[i]);
		/* Wait for writing process completed */
		_delay_ms(10);
	}

	EEPROM_writeByte(KEY_ADDRESS, DEFAULT_KEY);	/* Write the key flag */
	UART_sendByte(SAVED_PASSWORD);				/* Send a message indicating password is saved */
}

void checkPassword(void)
{
	uint8 l_password[PASSWORD_SIZE];

	receivePassword(l_password);				/* Receive password */

	/* Compare received password with saved password*/
	if(comparePassword(l_password))
	{
		UART_sendByte(MATCH_PASSWORD);			/* Send matching message */
	}
	else
	{
		UART_sendByte(MISMATCH_PASSWORD);		/* Send mismatching message */
	}

}

void appCallBack(void)
{
	g_ticks++;									/* Increment ticks counter each timer interrupt */
}

void openDoor(void)
{

	DC_MOTOR_rotate(DC_MOTOR_CCW);				/* Rotate motor to open the door */
	g_ticks = 0;								/* Reset timer counter */
	UART_sendByte(DOOR_IS_OPENING);				/* Send message to HMI for opening indication */
	while(g_ticks < DOOR_OPENING_TIME);			/* Wait for opening time finished */
	DC_MOTOR_rotate(DC_MOTOR_STOP);				/* Stop motor */
	g_ticks = 0;								/* Reset timer counter */
	UART_sendByte(DOOR_IS_OPEN);				/* Send message to HMI for open indication */
	while(g_ticks < DOOR_HOLD_OPEN_TIME);		/* Wait for holding door */
	DC_MOTOR_rotate(DC_MOTOR_CW);				/* Rotate motor to close the door */
	g_ticks = 0;								/* Reset timer counter */
	UART_sendByte(DOOR_IS_CLOSING);				/* Send message to HMI for closing indication */
	while(g_ticks < DOOR_CLOSING_TIME);			/* Wait for closing time finished */
	DC_MOTOR_rotate(DC_MOTOR_STOP);				/* Stop motor */
	UART_sendByte(DOOR_IS_CLOSE);				/* Send message to HMI for close indication */
}

void danger(void)
{
	BUZZER_on();								/* Turn on buzzer */
	g_ticks = 0;								/* Reset timer counter */
	while(g_ticks < DANGER_TIME);				/* Wait for danger time finished */
	BUZZER_off();								/* Turn off buzzer */
	UART_sendByte(DANGER_TIME_OFF);				/* Send message to HMI for finishing danger time */
}
