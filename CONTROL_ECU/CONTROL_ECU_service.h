/***********************************************************************/
/***********    Author     : Tarek Elmenshawy       		************/
/***********    File Name  : CONTROL_ECU_service.h			************/
/***********    Date       : 07-06-2022             		************/
/***********    Description: App. services header file    	************/
/***********************************************************************/

/***********************************************************************/
/***********	Guard of file will call one time in .c file    *********/
/***********************************************************************/

#ifndef CONTROL_ECU_SERVICE_H
#define CONTROL_ECU_SERVICE_H

#include "std_types.h"

/***********************************************************************/
/***********				Constants Definitions			  **********/
/***********************************************************************/
#define KEY_ADDRESS				0x00F2
#define DEFAULT_KEY				0xF0

#define PASSWORD_ADDRESS		0x0000
#define PASSWORD_SIZE			5

#define NEW_PASSWORD			0xA1
#define SAVED_PASSWORD			0xB1
#define DOOR_IS_OPENING			0xC1
#define DOOR_IS_OPEN			0xD1
#define DOOR_IS_CLOSING			0xE1
#define DOOR_IS_CLOSE			0xF1
#define MATCH_PASSWORD			0xA2
#define MISMATCH_PASSWORD		0xB2
#define DANGER_TIME_OFF			0xC2
#define READY_TO_RECEIVE		0xD2

#define DOOR_OPENING_TIME		15
#define DOOR_CLOSING_TIME		15
#define DOOR_HOLD_OPEN_TIME		3
#define DANGER_TIME				60

#define SAVE_PASSWORD			0x10
#define CHECK_PASSWORD			0x20
#define OPEN_DOOR				0x30
#define DANGER					0x40

/***********************************************************************/
/***********				Functions Prototypes			  **********/
/***********************************************************************/

uint8 isThereSavedPassword(void);
void appCallBack(void);
void savePassword(void);
void checkPassword(void);
void openDoor(void);
void danger(void);

#endif /* CONTROL_ECU_SERVICE_H */
