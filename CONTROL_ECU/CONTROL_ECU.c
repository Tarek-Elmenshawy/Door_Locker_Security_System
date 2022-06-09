/***********************************************************************/
/***********    Author     : Tarek Elmenshawy       		************/
/***********    File Name  : CONTROL_ECU.c					************/
/***********    Date       : 07-06-2022             		************/
/***********    Description: Control ECU application    	************/
/***********************************************************************/

#undef F_CPU
#define F_CPU 8000000UL

#include "std_types.h"
#include "CONTROL_ECU_service.h"

#include "timer.h"
#include "twi.h"
#include "uart.h"

#include "dc_motor.h"
#include "buzzer.h"
#include "eeprom.h"

#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
	uint8 l_request;
	TWI_ConfigType TWI_configuration =
	{
		0x01,
		400000
	};
	UART_ConfigType UART_configuration =
	{
		UART_8BIT_DATA,
		UART_DISABLE_PARITY,
		UART_1STOP_BIT,
		9600
	};
	TIMER_ConfigType TIMER_configuration =
	{
			TIMER1_ID,
			TIMER_CTC_MODE,
			TIMER1_CLK_FCPU_BY_1024,
			0,
			7811
	};

	sei();										/* Enable global interrupt */
	TIMER_setCallBack(TIMER1_ID, appCallBack);	/* Set timer call back */
	TIMER_init(&TIMER_configuration);			/* Initialize timer1 */
	TWI_init(&TWI_configuration);				/* Initialize I2C */
	UART_init(&UART_configuration);				/* Initialize UART */
	BUZZER_init();								/* Configure buzzer pin */
	DC_MOTOR_init();							/* Configure motor pins */



	/* Check if this time is the first time */
	if(isThereSavedPassword())
	{
		/* Send message to HMI indicating that there is a saved password */
		UART_sendByte(SAVED_PASSWORD);
	}
	else
	{
		/* Send message to HMI indicating that there isn't a saved password */
		UART_sendByte(NEW_PASSWORD);
	}

	while(1)
	{
		/* Get a request from HMI-ECU */
		l_request = UART_receiveByte();

		/* Return a response based on the required request */
		switch(l_request)
		{
			case SAVE_PASSWORD:
				savePassword();
				break;

			case CHECK_PASSWORD:
				checkPassword();
				break;

			case OPEN_DOOR:
				openDoor();
				break;

			case DANGER:
				danger();
				break;
		}
	}
	return 0;
}


