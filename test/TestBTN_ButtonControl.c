#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include "BTN_ButtonControl.h"
#include "UnityHelper.h"
#include "unity.h"

/**********Test Wrappers**********/
extern ButtonState_t TestGetBTN_Selves_State(BTN_ObjectList_t ID);
extern uint16_t TestGetBTN_Selves_Timer_mS(BTN_ObjectList_t ID);
extern uint16_t TestGetBTN_Selves_PressedThreshold_mS(BTN_ObjectList_t ID);
extern uint16_t TestGetBTN_Selves_LongPressedThreshold_mS(BTN_ObjectList_t ID);
extern ButtonDefaultState_t TestGetBTN_Selves_DefaultState(BTN_ObjectList_t ID);
extern bool TestGetBTN_Selves_IsInitialized(BTN_ObjectList_t ID);
extern ErrorCode_t (*ReadButtonFunction)(BTN_ObjectList_t ID);
TestGetBTN_Selves_ReadButtonFunction(BTN_ObjectList_t ID);
extern void (*NotificationFunction)(BTN_ObjectList_t, ButtonState_t);
TestGetBTN_Selves_NotificationFunction(BTN_ObjectList_t ID);
extern bool TestWrapperBTN_Update_State_Machine(BTN_Object_t *self, uint32_t time_mS);

/**********Global Variables**********/
ErrorCode_t ReturnedValue;
ErrorCode_t (*Happy_ReadButtonFunction)(BTN_ObjectList_t);
uint8_t Happy_ButtonToReference;
BTN_ObjectList_t Happy_ButtonID;
uint16_t Happy_ThresholdForPress_mS;
uint16_t Happy_ThresholdForLongPress_mS;
void (*Happy_NotificationFunction)(BTN_ObjectList_t, ButtonState_t);
ButtonDefaultState_t Happy_DefaultState;
BTN_Object_t *ButtonObject = NULL;

/*************Fake Functions************/
ErrorCode_t Fake_ReadButtonFunction(BTN_ObjectList_t dummy);
void Fake_NotificationFunction(BTN_ObjectList_t Object, ButtonState_t Status);

ErrorCode_t Fake_ReadButtonFunction(BTN_ObjectList_t dummy)
{
	return SUCCESS;
}
void Fake_NotificationFunction(BTN_ObjectList_t Object, ButtonState_t Status)
{
	return;
}

void setUp(void)
{
	ReturnedValue = 0;
	Happy_ReadButtonFunction = &Fake_ReadButtonFunction;
	Happy_NotificationFunction = &Fake_NotificationFunction;
	Happy_ButtonToReference = 3;
	BTN_ObjectList_t Happy_ButtonID = BUTTON_TEST1;
	Happy_ThresholdForPress_mS = 5;
	Happy_ThresholdForLongPress_mS = 7;
	Happy_DefaultState = NORMALLY_HIGH;

	BTN_Aquire_Object(&ButtonObject, Happy_ButtonID);
}

void tearDown(void)
{
	BTN_Return_Object(&ButtonObject);
}

void test_Initialize_Button_HappyPath(void)
{
	TEST_ASSERT_TRUE(TestGetBTN_Selves_IsInitialized(Happy_ButtonID) == false);

	ReturnedValue = BTN_Initialize(Happy_ReadButtonFunction,
								   Happy_ButtonToReference,
								   Happy_ButtonID,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   Happy_NotificationFunction,
								   NORMALLY_HIGH);

	TEST_ASSERT_TRUE(TestGetBTN_Selves_IsInitialized(Happy_ButtonID) == true);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);

	ReturnedValue = BTN_Initialize(Happy_ReadButtonFunction,
								   Happy_ButtonToReference,
								   Happy_ButtonID,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   Happy_NotificationFunction,
								   NORMALLY_LOW);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Initialize_Button_ReadButtonIsNull(void)
{
	ReturnedValue = BTN_Initialize(NULL,
								   Happy_ButtonToReference,
								   Happy_ButtonID,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   Happy_NotificationFunction,
								   Happy_DefaultState);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Initialize_Button_NotificationIsNull(void)
{
	ReturnedValue = BTN_Initialize(Happy_ReadButtonFunction,
								   Happy_ButtonToReference,
								   Happy_ButtonID,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   NULL,
								   Happy_DefaultState);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Initialize_Button_InvalidButtonID_Low(void)
{
	ReturnedValue = BTN_Initialize(Happy_ReadButtonFunction,
								   Happy_ButtonToReference,
								   -1,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   Happy_NotificationFunction,
								   Happy_DefaultState);

	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Initialize_Button_InvalidButtonID_High(void)
{
	ReturnedValue = BTN_Initialize(Happy_ReadButtonFunction,
								   Happy_ButtonToReference,
								   NUMBER_OF_BUTTON_OBJECTS,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   Happy_NotificationFunction,
								   Happy_DefaultState);

	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Initialize_Button_InvalidDefaultState(void)
{
	ReturnedValue = BTN_Initialize(Happy_ReadButtonFunction,
								   Happy_ButtonToReference,
								   Happy_ButtonID,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   Happy_NotificationFunction,
								   3);

	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Initialize_Button_LongpressThresholdIsGreaterThanPressThreshold(void)
{
	ReturnedValue = BTN_Initialize(Happy_ReadButtonFunction,
								   Happy_ButtonToReference,
								   Happy_ButtonID,
								   5,
								   5,
								   Happy_NotificationFunction,
								   Happy_DefaultState);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Button_Aquire_Object_HappyPath(void)
{
	BTN_Return_Object(&ButtonObject);

	ReturnedValue = BTN_Aquire_Object(&ButtonObject, Happy_ButtonID);

	TEST_ASSERT_NOT_NULL(ButtonObject);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Aquire_Object_InvalidButtonID_Low(void)
{
	BTN_Return_Object(&ButtonObject);

	ReturnedValue = BTN_Aquire_Object(&ButtonObject, -1);

	TEST_ASSERT_NULL(ButtonObject);
	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Button_Aquire_Object_InvalidButtonID_High(void)
{
	BTN_Return_Object(&ButtonObject);

	ReturnedValue = BTN_Aquire_Object(&ButtonObject, NUMBER_OF_BUTTON_OBJECTS);

	TEST_ASSERT_NULL(ButtonObject);
	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Button_Aquire_Object_ButtonObjectNotNull(void)
{
	BTN_Object_t *LocalButtonObject = &ButtonObject;
	TEST_ASSERT_NOT_NULL(LocalButtonObject);

	ReturnedValue = BTN_Aquire_Object(&LocalButtonObject, Happy_ButtonID);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Button_Aquire_Object_AlreadyOwned(void)
{
	BTN_Return_Object(&ButtonObject);

	BTN_Aquire_Object(&ButtonObject, Happy_ButtonID);
	ReturnedValue = BTN_Aquire_Object(&ButtonObject, Happy_ButtonID);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Button_Return_Object_HappyPath(void)
{
	TEST_ASSERT_TRUE(BTN_Initialize(Happy_ReadButtonFunction,
									Happy_ButtonToReference,
									Happy_ButtonID,
									123,
									465,
									Happy_NotificationFunction,
									NORMALLY_LOW) == SUCCESS);
	//TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_ReadButtonFunction(Happy_ButtonID), Happy_ReadButtonFunction);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_PressedThreshold_mS(Happy_ButtonID) == 123);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_LongPressedThreshold_mS(Happy_ButtonID) == 465);
	//TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_NotificationFunction(Happy_ButtonID) == Happy_NotificationFunction);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_DefaultState(Happy_ButtonID) == NORMALLY_LOW);

	ReturnedValue = BTN_Return_Object(&ButtonObject);

	TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_ReadButtonFunction(Happy_ButtonID), NULL);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_PressedThreshold_mS(Happy_ButtonID) == 0);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_LongPressedThreshold_mS(Happy_ButtonID) == 0);
	TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_NotificationFunction(Happy_ButtonID), NULL);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_DefaultState(Happy_ButtonID) == NORMALLY_HIGH);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_State(Happy_ButtonID) == UNPRESSED);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_Timer_mS(Happy_ButtonID) == 0);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_IsInitialized(Happy_ButtonID) == false);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Return_Object_AlreadyReturned(void)
{
	BTN_Return_Object(&ButtonObject);

	ReturnedValue = BTN_Return_Object(&ButtonObject);

	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Button_Reset_Object_HappyPath(void)
{
	ReturnedValue = BTN_Reset_Object(ButtonObject);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Reset_Object_SelfIsNull(void)
{
	BTN_Return_Object(&ButtonObject);

	ReturnedValue = BTN_Reset_Object(ButtonObject);

	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Button_Current_State_HappyPath(void)
{
	ButtonState_t State = 0;

	ReturnedValue = BTN_Current_State(ButtonObject, &State);

	TEST_ASSERT_TRUE(State == UNPRESSED);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Current_State_SelfIsNull(void)
{
	ButtonState_t Dummy = -1;
	BTN_Return_Object(&ButtonObject);

	ReturnedValue = BTN_Current_State(ButtonObject, &Dummy);

	TEST_ASSERT_TRUE(Dummy == -1);
	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Update_State_Machine_NoElapsedTime(void)
{
	ReturnedValue = TestWrapperBTN_Update_State_Machine(&ButtonObject, 0);

	TEST_ASSERT_TRUE(ReturnedValue == false);
}

void test_Update_State_Machine_NotEnoughElapsedTime(void)
{
	ReturnedValue = TestWrapperBTN_Update_State_Machine(&ButtonObject, Happy_ThresholdForPress_mS - 1);

	TEST_ASSERT_TRUE(ReturnedValue == false);
}
