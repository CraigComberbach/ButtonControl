#ifndef BUTTON_CONTROL_H
#define	BUTTON_CONTROL_H
#include "Config.h"

/***********Add to config file header************/
/*
	//Buttons Debounce Library
	#define BUTTON_CONTROL_MAJOR	2
	#define BUTTON_CONTROL_MINOR	0
	#define BUTTON_CONTROL_PATCH	0

//Buttons
enum BUTTON_DEFINITIONS
{
	BUTTON_,
	NUMBER_OF_BUTTONS
}; 
*/

/***************Add to config file***************/
/*
#ifndef BUTTON_CONTROL_LIBRARY
	#error "You need to include the Button Control library for this code to compile"
#endif
 */

/*************Semantic  Versioning***************/
#define BUTTON_CONTROL_HAL

/*************Library Dependencies***************/
/*************   Magic  Numbers   ***************/
#define NO_NOTIFICATION	(void*)0
#define NORMALLY_HIGH	1
#define NORMALLY_LOW	0

/*************    Enumeration     ***************/
enum BUTTON_STATUS
{
	UNPRESSED,
	PRESSED,
	LONG_PRESSED,
	RELEASED
};

/***********      Data Structures    ************/
/*************Function  Prototypes***************/
/**
 * Periodically called, takes care of the debouncing, state changes, and events that occur at a state change
 * @param time_mS How many mS have lapsed since the last time this function was called
 */
void Buttons_Routine(unsigned long time_mS);

/**
 * Initialize an individual button to behave according to a set of runtime parameters
 * @param readButtonFunction A pointer to a function that will return the current value of a button
 * @param buttonReference The unique identifier that that the read function will require for direct access to the buttons current value
 * @param pinValue The pin to read, use the PIN_DEFINITIONS found in config.h
 * @param buttonValue The name of the button, setup in config.h
 * @param thresholdForPress_mS How many mS before a press becomes official
 * @param thresholdForLongPress_mS How many mS before a long press becomes official
 * @param notificationFunction A pointer to a function to be called when a buttons state changes
 * @param defaultState The resting level of a button
 */
void Initialize_Button(int (*readButtonFunction)(int), int buttonReference, enum BUTTON_DEFINITIONS buttonValue, int thresholdForPress_mS, int thresholdForLongPress_mS, void (*notificationFunction)(enum BUTTON_DEFINITIONS, enum BUTTON_STATUS), int defaultState);

#endif
