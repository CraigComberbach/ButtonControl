#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

//Buttons Debounce Library
#define BUTTON_CONTROL_MAJOR 2
#define BUTTON_CONTROL_MINOR 0
#define BUTTON_CONTROL_PATCH 1

//Buttons
typedef enum
{
	BUTTON_,
	NUMBER_OF_BUTTON_OBJECTS
} Button_ObjectList_t;

typedef int_fast8_t ErrorCode_t;
#define SUCCESS 0

#endif CONFIG_H//CONFIG_H
