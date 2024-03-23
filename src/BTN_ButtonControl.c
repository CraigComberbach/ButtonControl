/******************************************************************************
 * Version History:
 * v2.1.0	2024-03-23	Craig Comberbach
 * 	+Added Ceedling tests (using ceedling 0.32.0-9b8d8a9 pre-release and GCC 13.2.0)
 * 	+Using standard libraries instead of custom declarations (NULL vs NULL_POINTER definition)
 *	+Header file is compliant with self compilation if required
 *	Refactored to operate as an obscured object
 * v2.0.0	2015-12-21	Craig Comberbach
 * 	Compiler: C30 v3.31	IDE: MPLABx 3.05	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 64 bit Home
 * 	+Made a generic interface using function pointer so that any snippet of code can make use of this code
 * 	-Removed library dependency of Pins
 * v1.0.0	2015-12-21	Craig Comberbach
 * 	Compiler: C30 v3.31	IDE: MPLABx 3.05	Tool: ICD3	Computer: Intel Core2 Quad CPU 2.40 GHz, 5 GB RAM, Windows 10 64 bit Home
 *	First version
 ******************************************************************************/
/************Header Files*************/
#include "BTN_ButtonControl.h"
#include <errno.h>
#include <stdbool.h>

/********Semantic Versioning**********/
/***********Magic Numbers*************/
#define ZEROED 0

/************Enumerations*************/
/**********Type Definitions***********/
/*************Structures**************/
/**********Global Variables***********/
/*********Object Definition***********/
static struct Button_Object
{
	ReadButton_ptr_t ReadButtonFunction;
	Notification_ptr_t NotificationFunction;
	ButtonState_t State;
	uint32_t Timer_mS;
	uint16_t PressedThreshold_mS;
	uint16_t LongPressedThreshold_mS;
	ButtonDefaultState_t DefaultState;

	bool IsInitialized;
} selves[NUMBER_OF_BUTTON_OBJECTS] = {
	[0 ... NUMBER_OF_BUTTON_OBJECTS - 1] =
		{
											  .ReadButtonFunction = NULL,
											  .NotificationFunction = NULL,
											  .State = UNPRESSED,
											  .Timer_mS = ZEROED,
											  .PressedThreshold_mS = 0,
											  .LongPressedThreshold_mS = 0,
											  .DefaultState = NORMALLY_LOW,
											  .IsInitialized = false,
											  }
};

/*****Local Function Prototypes*******/
static void Update_State_Machine(BTN_Object_t *self, uint32_t time_mS);
static void Transition_Away_From_Unpressed(BTN_Object_t *self, uint32_t time_ms);

/*********Main Body Of Code***********/
void BTN_Routine(uint32_t time_mS)
{
	BTN_ObjectList_t currentButton;

	for(currentButton = 0; currentButton < NUMBER_OF_BUTTON_OBJECTS; currentButton++)
	{
		Update_State_Machine(&selves[currentButton], time_mS);
	}

	return;
}

static void Transition_Away_From_Unpressed(BTN_Object_t *self, uint32_t time_ms)
{
	return;
}

static void Update_State_Machine(BTN_Object_t *self, uint32_t time_mS)
{
	bool StateMachineChanged = false;
	ButtonState_t Was = self->State;

	switch(self->State)
	{
		case UNPRESSED:
			Transition_Away_From_Unpressed(self, time_mS);
			break;
		case PRESSED:
			break;
		case LONG_PRESSED:
			break;
		case RELEASED:
			break;
		default:
			break;
	}

	//if((Was != self->State) && (self->NotificationFunction != NO_NOTIFICATION))
	//{
	//	self->NotificationFunction(99, self->State);
	//}

	return;
}

//void Buttons_Routine_Deprecated(uint32_t time_mS)
//{
//	BTN_ObjectList_t currentButton;

//	//Update all buttons
//	for(currentButton = 0; currentButton < NUMBER_OF_BUTTON_OBJECTS; currentButton++)
//	{
//		//Add time to button if pressed
//		if(selves[currentButton].ReadButtonFunction(selves[currentButton].ID) != selves[currentButton].DefaultState)
//		{
//			selves[currentButton].Timer_mS += time_mS;
//		}
//		else
//		{
//			selves[currentButton].Timer_mS = 0;
//			switch(selves[currentButton].State)
//			{
//				case UNPRESSED:
//					//Nothing to do here
//					break;
//				case PRESSED:
//					selves[currentButton].State = RELEASED;
//					if(*selves[currentButton].NotificationFunction != NO_NOTIFICATION)
//					{
//						selves[currentButton].NotificationFunction(currentButton, selves[currentButton].State);
//					}
//					break;
//				case LONG_PRESSED:
//					selves[currentButton].State = RELEASED;
//					if(*selves[currentButton].NotificationFunction != NO_NOTIFICATION)
//					{
//						selves[currentButton].NotificationFunction(currentButton, selves[currentButton].State);
//					}
//					break;
//				case RELEASED:
//					selves[currentButton].State = UNPRESSED;
//					if(*selves[currentButton].NotificationFunction != NO_NOTIFICATION)
//					{
//						selves[currentButton].NotificationFunction(currentButton, selves[currentButton].State);
//					}
//					break;
//				default:
//					selves[currentButton].State = UNPRESSED;
//					selves[currentButton].Timer_mS = 0;
//					break;
//			}
//			if((*selves[currentButton].NotificationFunction != NO_NOTIFICATION) && (selves[currentButton].State != UNPRESSED))
//			{
//				selves[currentButton].State = UNPRESSED;
//				selves[currentButton].NotificationFunction(currentButton, selves[currentButton].State);
//			}
//		}

//		//Check if any thresholds were met
//		switch(selves[currentButton].State)
//		{
//			case UNPRESSED:
//				if(selves[currentButton].Timer_mS >= selves[currentButton].PressedThreshold_mS)
//				{
//					selves[currentButton].State = PRESSED;//Upgrade to next level!
//					if(*selves[currentButton].NotificationFunction != NO_NOTIFICATION)
//					{
//						selves[currentButton].NotificationFunction(currentButton, selves[currentButton].State);
//					}
//				}
//				break;
//			case PRESSED:
//				if(selves[currentButton].Timer_mS >= selves[currentButton].LongPressedThreshold_mS)
//				{
//					selves[currentButton].State = LONG_PRESSED;//Upgrade to next level!
//					if(*selves[currentButton].NotificationFunction != NO_NOTIFICATION)
//					{
//						selves[currentButton].NotificationFunction(currentButton, selves[currentButton].State);
//					}
//				}
//				break;
//			case LONG_PRESSED:
//				//Don't advance past this point or you risk looping around after ~65.5 seconds
//				selves[currentButton].Timer_mS = selves[currentButton].LongPressedThreshold_mS;
//				break;
//			default:
//				selves[currentButton].State = UNPRESSED;
//				selves[currentButton].Timer_mS = 0;
//				break;
//		}
//	}

//	return;
//}

ErrorCode_t BTN_Initialize(BTN_Object_t *self,
						   ReadButton_ptr_t readButtonFunction,
						   Notification_ptr_t NotificationFunction,
						   uint8_t buttonToReference,
						   uint16_t thresholdForPress_mS,
						   uint16_t thresholdForLongPress_mS,
						   ButtonDefaultState_t DefaultState)
{
	if(self == NULL)
	{
		return EINVAL;
	}
	if(readButtonFunction == NULL)
	{
		return EINVAL;
	}
	if((DefaultState != NORMALLY_HIGH) && (DefaultState != NORMALLY_LOW))
	{
		return ERANGE;
	}
	if(thresholdForLongPress_mS <= thresholdForPress_mS)
	{
		return EINVAL;
	}

	self->ReadButtonFunction = readButtonFunction;
	self->State = UNPRESSED;
	self->Timer_mS = ZEROED;
	self->PressedThreshold_mS = thresholdForPress_mS;
	self->LongPressedThreshold_mS = thresholdForLongPress_mS;
	self->NotificationFunction = NotificationFunction;
	self->DefaultState = DefaultState;
	self->IsInitialized = true;

	return 0;
}

ErrorCode_t BTN_Aquire_Object(BTN_Object_t **self, BTN_ObjectList_t ObjectID)
{
	if((ObjectID < 0) || (ObjectID >= NUMBER_OF_BUTTON_OBJECTS))
	{
		return ERANGE;
	}
	if(*self != NULL)
	{
		return EINVAL;
	}

	*self = &selves[ObjectID];

	return SUCCESS;
}

ErrorCode_t BTN_Return_Object(BTN_Object_t **self)
{
	if(*self == NULL)
	{
		return EPERM;
	}

	BTN_Reset_Object(*self);
	*self = NULL;

	return SUCCESS;
}

ErrorCode_t BTN_Reset_Object(BTN_Object_t *self)
{
	if(self == NULL)
	{
		return EPERM;
	}

	self->ReadButtonFunction = NULL;
	self->NotificationFunction = NULL;
	self->State = UNPRESSED;
	self->Timer_mS = ZEROED;
	self->PressedThreshold_mS = 0;
	self->LongPressedThreshold_mS = 0;
	self->DefaultState = NORMALLY_HIGH;
	self->IsInitialized = false;

	return SUCCESS;
}

ErrorCode_t BTN_Current_State(BTN_Object_t *self, ButtonState_t *State)
{
	if(self == NULL)
	{
		return EPERM;
	}

	*State = self->State;

	return SUCCESS;
}

#ifdef ENABLE_BUTTON_CONTROL_TEST_WRAPPERS
ButtonState_t TestGetBTN_Selves_State(BTN_ObjectList_t ID);
uint16_t TestGetBTN_Selves_Timer_mS(BTN_ObjectList_t ID);
uint16_t TestGetBTN_Selves_PressedThreshold_mS(BTN_ObjectList_t ID);
uint16_t TestGetBTN_Selves_LongPressedThreshold_mS(BTN_ObjectList_t ID);
ButtonDefaultState_t TestGetBTN_Selves_DefaultState(BTN_ObjectList_t ID);
bool TestGetBTN_Selves_IsInitialized(BTN_ObjectList_t ID);
ReadButton_ptr_t TestGetBTN_Selves_ReadButtonFunction(BTN_ObjectList_t ID);
Notification_ptr_t TestGetBTN_Selves_NotificationFunction(BTN_ObjectList_t ID);

void TestSetBTN_Selves_ReadButtonFunction(BTN_ObjectList_t ID, ErrorCode_t (*NewValue)(BTN_ObjectList_t ID));
void TestSetBTN_Selves_NotificationFunction(BTN_ObjectList_t ID, void (*NewValue)(BTN_ObjectList_t, ButtonState_t));
void TestSetBTN_Selves_State(BTN_ObjectList_t ID, ButtonState_t NewValue);
void TestSetBTN_Selves_Timer_mS(BTN_ObjectList_t ID, uint32_t NewValue);
void TestSetBTN_Selves_PressedThreshold_mS(BTN_ObjectList_t ID, uint16_t NewValue);
void TestSetBTN_Selves_LongPressedThreshold_mS(BTN_ObjectList_t ID, uint16_t NewValue);
void TestSetBTN_Selves_DefaultState(BTN_ObjectList_t ID, ButtonDefaultState_t NewValue);

void TestWrapperBTN_Update_State_Machine(BTN_Object_t *self, uint32_t time_mS);
void TestWrapperBTN_Transition_Away_From_Unpressed(BTN_Object_t *self, uint32_t time_ms);

ButtonState_t TestGetBTN_Selves_State(BTN_ObjectList_t ID)
{
	return selves[ID].State;
}

uint16_t TestGetBTN_Selves_Timer_mS(BTN_ObjectList_t ID)
{
	return selves[ID].Timer_mS;
}

uint16_t TestGetBTN_Selves_PressedThreshold_mS(BTN_ObjectList_t ID)
{
	return selves[ID].PressedThreshold_mS;
}

uint16_t TestGetBTN_Selves_LongPressedThreshold_mS(BTN_ObjectList_t ID)
{
	return selves[ID].LongPressedThreshold_mS;
}

ButtonDefaultState_t TestGetBTN_Selves_DefaultState(BTN_ObjectList_t ID)
{
	return selves[ID].DefaultState;
}

bool TestGetBTN_Selves_IsInitialized(BTN_ObjectList_t ID)
{
	return selves[ID].IsInitialized;
}

ReadButton_ptr_t TestGetBTN_Selves_ReadButtonFunction(BTN_ObjectList_t ID)
{
	return selves[ID].ReadButtonFunction;
}

Notification_ptr_t TestGetBTN_Selves_NotificationFunction(BTN_ObjectList_t ID)
{
	return selves[ID].NotificationFunction;
}

void TestSetBTN_Selves_ReadButtonFunction(BTN_ObjectList_t ID, ReadButton_ptr_t NewValue)
{
	selves[ID].ReadButtonFunction = NewValue;
	return;
}

void TestSetBTN_Selves_NotificationFunction(BTN_ObjectList_t ID, Notification_ptr_t NewValue)
{
	selves[ID].NotificationFunction = NewValue;
	return;
}

void TestSetBTN_Selves_State(BTN_ObjectList_t ID, ButtonState_t NewValue)
{
	selves[ID].State = NewValue;
	return;
}

void TestSetBTN_Selves_Timer_mS(BTN_ObjectList_t ID, uint32_t NewValue)
{
	selves[ID].Timer_mS = NewValue;
	return;
}

void TestSetBTN_Selves_PressedThreshold_mS(BTN_ObjectList_t ID, uint16_t NewValue)
{
	selves[ID].PressedThreshold_mS = NewValue;
	return;
}

void TestSetBTN_Selves_LongPressedThreshold_mS(BTN_ObjectList_t ID, uint16_t NewValue)
{
	selves[ID].LongPressedThreshold_mS = NewValue;
	return;
}

void TestSetBTN_Selves_DefaultState(BTN_ObjectList_t ID, ButtonDefaultState_t NewValue)
{
	selves[ID].DefaultState = NewValue;
	return;
}

void TestWrapperBTN_Update_State_Machine(BTN_Object_t *self, uint32_t time_mS)
{
	return Update_State_Machine(self, time_mS);
}

void TestWrapperBTN_Transition_Away_From_Unpressed(BTN_Object_t *self, uint32_t time_ms)
{
	Transition_Away_From_Unpressed(self, time_ms);
	return;
}
#endif
