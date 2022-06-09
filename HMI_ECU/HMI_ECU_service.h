/***********************************************************************/
/***********    Author     : Tarek Elmenshawy       		************/
/***********    File Name  : HMI_ECU_service.h			************/
/***********    Date       : 07-06-2022             		************/
/***********    Description: App. services header file    	************/
/***********************************************************************/

/***********************************************************************/
/***********	Guard of file will call one time in .c file    *********/
/***********************************************************************/

#ifndef HMI_ECU_SERVICE_H
#define HMI_ECU_SERVICE_H

#include "std_types.h"

/***********************************************************************/
/***********				Constants Definitions			  **********/
/***********************************************************************/

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

#define SAVE_PASSWORD			0x10
#define CHECK_PASSWORD			0x20
#define OPEN_DOOR				0x30
#define DANGER					0x40

/***********************************************************************/
/***********				Functions Prototypes			  **********/
/***********************************************************************/

void newPassword(void);
uint8 getPassword(void);
void openDoor(void);
void danger(void);

#endif /* HMI_ECU_SERVICE_H */
