#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H
/**********Include Headers************/
#include <stddef.h>
#include <stdint.h>
#include "Config.h"

/**********Add to config.h************/
/*
	//Buttons Debounce Library
	#define BUTTON_CONTROL_MAJOR	2
	#define BUTTON_CONTROL_MINOR	0
	#define BUTTON_CONTROL_PATCH	1

//Buttons
enum ButtonObjects_t
{
	BUTTON_,
	NUMBER_OF_BUTTON_OBJECTS
};
*/

/**********Add to config.c************/
/*
#ifndef BUTTON_CONTROL_LIBRARY
	#error "You need to include the Button Control library for this code to compile"
#endif
*/

/********Semantic Versioning**********/
#define BUTTON_CONTROL_HAL

/***********Magic Numbers*************/
#define NO_NOTIFICATION NULL

/************Enumeration**************/
typedef enum
{
	NORMALLY_LOW,
	NORMALLY_HIGH,
} ButtonDefaultState_t;

typedef enum
{
	UNPRESSED,
	PRESSED,
	LONG_PRESSED,
	RELEASED
} ButtonState_t;

/*********Object Definition***********/
typedef struct Button_Object Button_Object_t;

/****Module Function Prototypes*******/
ErrorCode_t Button_Aquire_Object(Button_Object_t **self, Button_ObjectList_t ObjectID);
ErrorCode_t Button_Return_Object(Button_Object_t **self);
ErrorCode_t Button_Reset_Object(Button_Object_t *self);

ErrorCode_t Button_Current_State(Button_Object_t *self, ButtonState_t *State);

/**
 * Periodically called, takes care of the debouncing, state changes, and events that occur at a state change
 * @param time_mS How many mS have lapsed since the last time this function was called
 */
void Buttons_Routine(uint32_t time_mS);
//void Buttons_Routine_Deprecated(uint32_t time_mS);

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
ErrorCode_t Initialize_Button(ErrorCode_t (*readButtonFunction)(Button_ObjectList_t),
							  uint8_t buttonToReference,
							  Button_ObjectList_t ButtonID,
							  uint16_t thresholdForPress_mS,
							  uint16_t thresholdForLongPress_mS,
							  void (*notificationFunction)(Button_ObjectList_t, ButtonState_t),
							  ButtonDefaultState_t DefaultState);

#endif
