/**************************************************************************************************
Target Hardware:		PIC24F...
Code assumptions:		
Purpose:				
Notes:					

Version History:
vnext	?	Craig Comberbach
Compiler: XC16 v1.11	IDE: MPLABx 3.05	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 64 bit Home 

v2.0.0	2015-12-21	Craig Comberbach
Compiler: C30 v3.31	IDE: MPLABx 3.05	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 64 bit Home 
	+Made a generic interface using function pointer so that any snippet of code can make use of this code
	-Removed library dependency of Pins
v1.0.0	2015-12-21	Craig Comberbach
Compiler: C30 v3.31	IDE: MPLABx 3.05	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 64 bit Home 
	First version
**************************************************************************************************/
/*************    Header Files    ***************/
#include "ButtonControl.h"
#include <stddef.h>

/************* Library Definition ***************/
/*************Semantic  Versioning***************/
/*************Library Dependencies***************/
/************Arbitrary Functionality*************/
/*************   Magic  Numbers   ***************/
/*************    Enumeration     ***************/
/***********  Structure Definitions  ************/
/***********State Machine Definitions************/
/*************  Global Variables  ***************/
/*************Interrupt Prototypes***************/
/*************Function  Prototypes***************/
/************* Device Definitions ***************/
/************* Module Definitions ***************/
/************* Other  Definitions ***************/

struct BUTTONELABRA
{
	int (*readButtonState)(int uniqueIdentifier);
	int buttonID;
	enum BUTTON_STATUS status;
	unsigned int timer_mS;
	int pressedThreshold_mS;
	int longPressedThreshold_mS;
	void (*notificationFunction)(enum BUTTON_DEFINITIONS, enum BUTTON_STATUS);
	int defaultState;
} button[NUMBER_OF_BUTTONS];

void Buttons_Routine(unsigned long time_mS)
{
	int currentButton;

	//Update all buttons
	for(currentButton = 0; currentButton < NUMBER_OF_BUTTONS; currentButton++)
	{
		//Add time to button if pressed
		if(button[currentButton].readButtonState(button[currentButton].buttonID) != button[currentButton].defaultState)
			button[currentButton].timer_mS += time_mS;
		else
		{
			button[currentButton].timer_mS = 0;
			switch(button[currentButton].status)
			{
				case UNPRESSED:
					//Nothing to do here
					break;
				case PRESSED:
					button[currentButton].status = RELEASED;
					if(*button[currentButton].notificationFunction != NO_NOTIFICATION)
						button[currentButton].notificationFunction(currentButton, button[currentButton].status);
					break;
				case LONG_PRESSED:
					button[currentButton].status = RELEASED;
					if(*button[currentButton].notificationFunction != NO_NOTIFICATION)
						button[currentButton].notificationFunction(currentButton, button[currentButton].status);
					break;
				case RELEASED:
					button[currentButton].status = UNPRESSED;
					if(*button[currentButton].notificationFunction != NO_NOTIFICATION)
						button[currentButton].notificationFunction(currentButton, button[currentButton].status);
					break;
				default:
					button[currentButton].status = UNPRESSED;
					button[currentButton].timer_mS = 0;
					break;
			}
			if((*button[currentButton].notificationFunction != NO_NOTIFICATION) && (button[currentButton].status != UNPRESSED))
			{
				button[currentButton].status = UNPRESSED;
				button[currentButton].notificationFunction(currentButton, button[currentButton].status);
			}
		}

		//Check if any thresholds were met
		switch(button[currentButton].status)
		{
			case UNPRESSED:
				if(button[currentButton].timer_mS >= button[currentButton].pressedThreshold_mS)
				{
					button[currentButton].status = PRESSED;//Upgrade to next level!
					if(*button[currentButton].notificationFunction != NO_NOTIFICATION)
						button[currentButton].notificationFunction(currentButton, button[currentButton].status);
				}
				break;
			case PRESSED:
				if(button[currentButton].timer_mS >= button[currentButton].longPressedThreshold_mS)
				{
					button[currentButton].status = LONG_PRESSED;//Upgrade to next level!
					if(*button[currentButton].notificationFunction != NO_NOTIFICATION)
						button[currentButton].notificationFunction(currentButton, button[currentButton].status);
				}
				break;
			case LONG_PRESSED:
				//Don't advance past this point or you risk looping around after ~65.5 seconds
				button[currentButton].timer_mS = button[currentButton].longPressedThreshold_mS;
				break;
			default:
				button[currentButton].status = UNPRESSED;
				button[currentButton].timer_mS = 0;
				break;
		}
	}

		
	return;
}

void Initialize_Button(int (*readButtonFunction)(int), int buttonToReference, enum BUTTON_DEFINITIONS buttonValue, int thresholdForPress_mS, int thresholdForLongPress_mS, void (*notificationFunction)(enum BUTTON_DEFINITIONS, enum BUTTON_STATUS), int defaultState)
{
	if(readButtonFunction != NULL)
		button[buttonValue].readButtonState = readButtonFunction;
	else
	{/*TODO - Error Handling Code*/}
	button[buttonValue].buttonID = buttonToReference;
	button[buttonValue].status = UNPRESSED;
	button[buttonValue].timer_mS = 0;
	button[buttonValue].pressedThreshold_mS = thresholdForPress_mS;
	button[buttonValue].longPressedThreshold_mS = thresholdForLongPress_mS;
	button[buttonValue].notificationFunction = notificationFunction;
	button[buttonValue].defaultState = defaultState;
	
	return;
}
