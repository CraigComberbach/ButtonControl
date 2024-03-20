/******************************************************************************
 * Version History:
 * v2.0.1	2024-03-20	Craig Comberbach
 * 	+Added Ceedling tests (using ceedling 0.32.0-9b8d8a9 pre-release and GCC 13.2.0)
 * 	+Using standard libraries instead of custom declarations (NULL vs NULL_POINTER definition)
 *	+Header file is compliant with self compilation if required
 * v2.0.0	2015-12-21	Craig Comberbach
 * 	Compiler: C30 v3.31	IDE: MPLABx 3.05	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 64 bit Home 
 * 	+Made a generic interface using function pointer so that any snippet of code can make use of this code
 * 	-Removed library dependency of Pins
 * v1.0.0	2015-12-21	Craig Comberbach
 * 	Compiler: C30 v3.31	IDE: MPLABx 3.05	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 64 bit Home 
 *	First version
******************************************************************************/
/*************    Header Files    ***************/
#include "ButtonControl.h"
#include <errno.h>

/************* Library Definition ***************/
/*************Semantic  Versioning***************/
/*************Library Dependencies***************/
/************Arbitrary Functionality*************/
/*************   Magic  Numbers   ***************/
#define ZEROED	0

/*************    Enumeration     ***************/
/***********  Structure Definitions  ************/
/***********State Machine Definitions************/
/*************  Global Variables  ***************/
/*************Interrupt Prototypes***************/
/*************Function  Prototypes***************/
/************* Device Definitions ***************/
/************* Module Definitions ***************/
/************* Other  Definitions ***************/

static struct Button_Object
{
	ErrorCode_t (*readButtonState)(ButtonObjects_t uniqueIdentifier);
	ButtonObjects_t buttonID;
	ButtonStatus_t status;
	unsigned int timer_mS;
	uint16_t pressedThreshold_mS;
	uint16_t longPressedThreshold_mS;
	void (*notificationFunction)(ButtonObjects_t, ButtonStatus_t);
	ButtonDefaultState_t DefaultState;
} selves[NUMBER_OF_BUTTONS]  = {[0 ... NUMBER_OF_BUTTONS - 1] =
	{
		.readButtonState = NULL,
		.notificationFunction = NULL,
		.buttonID = 0,
		.status = UNPRESSED,
		.timer_mS = ZEROED,
		.pressedThreshold_mS = 0,
		.longPressedThreshold_mS = 0,
		.DefaultState = NORMALLY_LOW,
	}};

void Buttons_Routine(unsigned long time_mS)
{
	int currentButton;

	//Update all buttons
	for(currentButton = 0; currentButton < NUMBER_OF_BUTTONS; currentButton++)
	{
		//Add time to button if pressed
		if(selves[currentButton].readButtonState(selves[currentButton].buttonID) != selves[currentButton].DefaultState)
			selves[currentButton].timer_mS += time_mS;
		else
		{
			selves[currentButton].timer_mS = 0;
			switch(selves[currentButton].status)
			{
				case UNPRESSED:
					//Nothing to do here
					break;
				case PRESSED:
					selves[currentButton].status = RELEASED;
					if(*selves[currentButton].notificationFunction != NO_NOTIFICATION)
						selves[currentButton].notificationFunction(currentButton, selves[currentButton].status);
					break;
				case LONG_PRESSED:
					selves[currentButton].status = RELEASED;
					if(*selves[currentButton].notificationFunction != NO_NOTIFICATION)
						selves[currentButton].notificationFunction(currentButton, selves[currentButton].status);
					break;
				case RELEASED:
					selves[currentButton].status = UNPRESSED;
					if(*selves[currentButton].notificationFunction != NO_NOTIFICATION)
						selves[currentButton].notificationFunction(currentButton, selves[currentButton].status);
					break;
				default:
					selves[currentButton].status = UNPRESSED;
					selves[currentButton].timer_mS = 0;
					break;
			}
			if((*selves[currentButton].notificationFunction != NO_NOTIFICATION) && (selves[currentButton].status != UNPRESSED))
			{
				selves[currentButton].status = UNPRESSED;
				selves[currentButton].notificationFunction(currentButton, selves[currentButton].status);
			}
		}

		//Check if any thresholds were met
		switch(selves[currentButton].status)
		{
			case UNPRESSED:
				if(selves[currentButton].timer_mS >= selves[currentButton].pressedThreshold_mS)
				{
					selves[currentButton].status = PRESSED;//Upgrade to next level!
					if(*selves[currentButton].notificationFunction != NO_NOTIFICATION)
						selves[currentButton].notificationFunction(currentButton, selves[currentButton].status);
				}
				break;
			case PRESSED:
				if(selves[currentButton].timer_mS >= selves[currentButton].longPressedThreshold_mS)
				{
					selves[currentButton].status = LONG_PRESSED;//Upgrade to next level!
					if(*selves[currentButton].notificationFunction != NO_NOTIFICATION)
						selves[currentButton].notificationFunction(currentButton, selves[currentButton].status);
				}
				break;
			case LONG_PRESSED:
				//Don't advance past this point or you risk looping around after ~65.5 seconds
				selves[currentButton].timer_mS = selves[currentButton].longPressedThreshold_mS;
				break;
			default:
				selves[currentButton].status = UNPRESSED;
				selves[currentButton].timer_mS = 0;
				break;
		}
	}
		
	return;
}

ErrorCode_t Initialize_Button(ErrorCode_t (*readButtonFunction)(ButtonObjects_t),
								uint8_t buttonToReference,
								ButtonObjects_t ButtonID,
								uint16_t thresholdForPress_mS,
								uint16_t thresholdForLongPress_mS,
								void (*notificationFunction)(ButtonObjects_t, ButtonStatus_t),
								ButtonDefaultState_t DefaultState)
{
	if(readButtonFunction == NULL)
	{
		return EINVAL;
	}
	if((ButtonID < 0) || (ButtonID >= NUMBER_OF_BUTTONS))
	{
		return ERANGE;
	}
	if((DefaultState != NORMALLY_HIGH) && (DefaultState != NORMALLY_LOW))
	{
		return ERANGE;
	}
	if(thresholdForLongPress_mS <= thresholdForPress_mS)
	{
		return EINVAL;
	}
	
	selves[ButtonID].readButtonState = readButtonFunction;
	selves[ButtonID].buttonID = buttonToReference;
	selves[ButtonID].status = UNPRESSED;
	selves[ButtonID].timer_mS = ZEROED;
	selves[ButtonID].pressedThreshold_mS = thresholdForPress_mS;
	selves[ButtonID].longPressedThreshold_mS = thresholdForLongPress_mS;
	selves[ButtonID].notificationFunction = notificationFunction;
	selves[ButtonID].DefaultState = DefaultState;
	
	return 0;
}
