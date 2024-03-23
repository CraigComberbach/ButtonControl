#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define ENABLE_BUTTON_CONTROL_TEST_WRAPPERS

//Buttons Debounce Library
#define BUTTON_CONTROL_MAJOR 2
#define BUTTON_CONTROL_MINOR 0
#define BUTTON_CONTROL_PATCH 1

//Buttons
typedef enum
{
	BUTTON_TEST1,
	BUTTON_TEST2,
	NUMBER_OF_BUTTON_OBJECTS
} BTN_ObjectList_t;

typedef int_fast8_t ErrorCode_t;
#define SUCCESS 0

#endif//CONFIG_H
