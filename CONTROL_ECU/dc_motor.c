/***********************************************************************/
/***********    Author     : Tarek Elmenshawy       		************/
/***********    File Name  : dc_motor.c			     		************/
/***********	Version	   : V0.1							************/
/***********    Date       : 24-05-2022             		************/
/***********    Description: DC Motor driver source file    ************/
/***********************************************************************/

#include "dc_motor.h"
#include "gpio.h"

/***********************************************************************/
/***********				Functions Definitions			  **********/
/***********************************************************************/

/*
 * Description
 * - The Function responsible for setup the direction for the two motor 
 *   pins through the GPIO driver.
 * - Stop the DC-Motor at the beginning through the GPIO driver.
 */
void DC_MOTOR_init(void)
{
	/* Configure motor pins as output pins */
	GPIO_setupPinDirection(DC_INT1_PORT, DC_INT1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DC_INT2_PORT, DC_INT2_PIN, PIN_OUTPUT);
	
	/* Set direction motor pins low to stop motor at the beginning */
	GPIO_writePin(DC_INT1_PORT, DC_INT1_PIN, 0);
	GPIO_writePin(DC_INT1_PORT, DC_INT2_PIN, 0);

}

/*
 * Description:
 * - The function responsible for rotate the DC Motor CW/ or A-CW or 
 *   stop the motor based on the state input state value.
 */
void DC_MOTOR_rotate(uint8 a_state)
{	
	/* Select motor direction as required */
	switch(a_state)
	{
		case DC_MOTOR_STOP:
			GPIO_writePin(DC_INT1_PORT, DC_INT1_PIN, 0);
			GPIO_writePin(DC_INT1_PORT, DC_INT2_PIN, 0);
			break;
		
		case DC_MOTOR_CW:
			GPIO_writePin(DC_INT1_PORT, DC_INT1_PIN, 1);
			GPIO_writePin(DC_INT1_PORT, DC_INT2_PIN, 0);
			break;
		
		case DC_MOTOR_CCW:
			GPIO_writePin(DC_INT1_PORT, DC_INT1_PIN, 0);
			GPIO_writePin(DC_INT1_PORT, DC_INT2_PIN, 1);
			break;
	}
}

