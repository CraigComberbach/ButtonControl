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
	BTN_AUTOMATICALLY_INITIALIZED,
	BTN_MANUALLY_INITIALIZED,
	NUMBER_OF_BUTTON_OBJECTS
} BTN_ObjectList_t;

typedef int_fast8_t ErrorCode_t;
#define SUCCESS 0

#endif//CONFIG_H
