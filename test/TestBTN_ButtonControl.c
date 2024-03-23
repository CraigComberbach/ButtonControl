#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include "BTN_ButtonControl.h"
#include "UnityHelper.h"
#include "unity.h"

enum TestButtons
{
	AUTOMATIC_BUTTON,
	MANUAL_BUTTON,
};

/**********Test Wrappers**********/
extern ButtonState_t TestGetBTN_Selves_State(BTN_ObjectList_t ID);
extern uint16_t TestGetBTN_Selves_Timer_mS(BTN_ObjectList_t ID);
extern uint16_t TestGetBTN_Selves_PressedThreshold_mS(BTN_ObjectList_t ID);
extern uint16_t TestGetBTN_Selves_LongPressedThreshold_mS(BTN_ObjectList_t ID);
extern ButtonDefaultState_t TestGetBTN_Selves_DefaultState(BTN_ObjectList_t ID);
extern bool TestGetBTN_Selves_IsInitialized(BTN_ObjectList_t ID);
extern ReadButton_ptr_t TestGetBTN_Selves_ReadButtonFunction(BTN_ObjectList_t ID);
extern Notification_ptr_t TestGetBTN_Selves_NotificationFunction(BTN_ObjectList_t ID);

extern void TestSetBTN_Selves_ReadButtonFunction(BTN_ObjectList_t ID, ErrorCode_t (*NewValue)(BTN_ObjectList_t ID));
extern void TestSetBTN_Selves_NotificationFunction(BTN_ObjectList_t ID, void (*NewValue)(BTN_ObjectList_t, ButtonState_t));
extern void TestSetBTN_Selves_State(BTN_ObjectList_t ID, ButtonState_t NewValue);
extern void TestSetBTN_Selves_Timer_mS(BTN_ObjectList_t ID, uint32_t NewValue);
extern void TestSetBTN_Selves_PressedThreshold_mS(BTN_ObjectList_t ID, uint16_t NewValue);
extern void TestSetBTN_Selves_LongPressedThreshold_mS(BTN_ObjectList_t ID, uint16_t NewValue);
extern void TestSetBTN_Selves_DefaultState(BTN_ObjectList_t ID, ButtonDefaultState_t NewValue);

extern void TestWrapperBTN_Update_State_Machine(BTN_Object_t *self, uint32_t time_mS);
extern void TestWrapperBTN_Transition_Away_From_Unpressed(BTN_Object_t *self, uint32_t time_ms);

/**********Global Variables**********/
ErrorCode_t ReturnedValue;
ErrorCode_t (*Happy_ReadButtonFunction)(BTN_ObjectList_t);
uint16_t Happy_ThresholdForPress_mS;
uint16_t Happy_ThresholdForLongPress_mS;
void (*Happy_NotificationFunction)(BTN_ObjectList_t, ButtonState_t);
BTN_Object_t *ButtonObject_Automatic = NULL;
BTN_Object_t *ButtonObject_Manual = NULL;

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
	Happy_ThresholdForPress_mS = 5;
	Happy_ThresholdForLongPress_mS = 7;

	BTN_Aquire_Object(&ButtonObject_Automatic, BTN_AUTOMATICALLY_INITIALIZED);
	BTN_Initialize(ButtonObject_Automatic,
				   &Fake_ReadButtonFunction,
				   &Fake_NotificationFunction,
				   AUTOMATIC_BUTTON,
				   5,
				   7,
				   NORMALLY_HIGH);

	BTN_Return_Object(&ButtonObject_Manual);
}

void tearDown(void)
{
	BTN_Return_Object(&ButtonObject_Automatic);
	BTN_Return_Object(&ButtonObject_Manual);
}

void test_Initialize_Button_HappyPathNormallyHigh(void)
{
	BTN_Aquire_Object(&ButtonObject_Manual, BTN_MANUALLY_INITIALIZED);

	ReturnedValue = BTN_Initialize(ButtonObject_Manual,
								   Happy_ReadButtonFunction,
								   Happy_NotificationFunction,
								   MANUAL_BUTTON,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   NORMALLY_HIGH);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_IsInitialized(BTN_MANUALLY_INITIALIZED) == true);
}

void test_Initialize_Button_HappyPathNormallyLow(void)
{
	BTN_Aquire_Object(&ButtonObject_Manual, BTN_MANUALLY_INITIALIZED);

	ReturnedValue = BTN_Initialize(ButtonObject_Manual,
								   Happy_ReadButtonFunction,
								   Happy_NotificationFunction,
								   MANUAL_BUTTON,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   NORMALLY_LOW);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_IsInitialized(BTN_MANUALLY_INITIALIZED) == true);
}

void test_Initialize_Button_ObjectIsNull(void)
{
	ReturnedValue = BTN_Initialize(NULL,
								   Happy_ReadButtonFunction,
								   Happy_NotificationFunction,
								   AUTOMATIC_BUTTON,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   NORMALLY_HIGH);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Initialize_Button_ReadButtonIsNull(void)
{
	BTN_Aquire_Object(&ButtonObject_Manual, BTN_MANUALLY_INITIALIZED);

	ReturnedValue = BTN_Initialize(ButtonObject_Manual,
								   NULL,
								   Happy_NotificationFunction,
								   MANUAL_BUTTON,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   NORMALLY_HIGH);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Initialize_Button_NotificationIsNull(void)
{
	BTN_Aquire_Object(&ButtonObject_Manual, BTN_MANUALLY_INITIALIZED);

	ReturnedValue = BTN_Initialize(ButtonObject_Manual,
								   Happy_ReadButtonFunction,
								   NULL,
								   MANUAL_BUTTON,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   NORMALLY_HIGH);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Initialize_Button_InvalidDefaultState(void)
{
	BTN_Aquire_Object(&ButtonObject_Manual, BTN_MANUALLY_INITIALIZED);

	ReturnedValue = BTN_Initialize(ButtonObject_Manual,
								   Happy_ReadButtonFunction,
								   Happy_NotificationFunction,
								   MANUAL_BUTTON,
								   Happy_ThresholdForPress_mS,
								   Happy_ThresholdForLongPress_mS,
								   3);

	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Initialize_Button_LongpressThresholdIsGreaterThanPressThreshold(void)
{
	ReturnedValue = BTN_Initialize(ButtonObject_Automatic,
								   Happy_ReadButtonFunction,
								   Happy_NotificationFunction,
								   AUTOMATIC_BUTTON,
								   5,
								   5,
								   NORMALLY_HIGH);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Button_Aquire_Object_HappyPath(void)
{
	ReturnedValue = BTN_Aquire_Object(&ButtonObject_Manual, BTN_MANUALLY_INITIALIZED);

	TEST_ASSERT_NOT_NULL(ButtonObject_Manual);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Aquire_Object_InvalidButtonID_Low(void)
{
	ReturnedValue = BTN_Aquire_Object(&ButtonObject_Manual, -1);

	TEST_ASSERT_NULL(ButtonObject_Manual);
	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Button_Aquire_Object_InvalidButtonID_High(void)
{
	ReturnedValue = BTN_Aquire_Object(&ButtonObject_Manual, NUMBER_OF_BUTTON_OBJECTS);

	TEST_ASSERT_NULL(ButtonObject_Manual);
	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Button_Aquire_Object_ButtonObjectNotNull(void)
{
	ReturnedValue = BTN_Aquire_Object(&ButtonObject_Automatic, BTN_AUTOMATICALLY_INITIALIZED);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Button_Return_Object_HappyPath(void)
{
	TEST_ASSERT_TRUE(BTN_Initialize(ButtonObject_Automatic,
									Happy_ReadButtonFunction,
									Happy_NotificationFunction,
									AUTOMATIC_BUTTON,
									123,
									465,
									NORMALLY_LOW) == SUCCESS);
	//TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_ReadButtonFunction(BTN_AUTOMATICALLY_INITIALIZED), Happy_ReadButtonFunction);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_PressedThreshold_mS(BTN_AUTOMATICALLY_INITIALIZED) == 123);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_LongPressedThreshold_mS(BTN_AUTOMATICALLY_INITIALIZED) == 465);
	//TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_NotificationFunction(BTN_AUTOMATICALLY_INITIALIZED) == Happy_NotificationFunction);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_DefaultState(BTN_AUTOMATICALLY_INITIALIZED) == NORMALLY_LOW);

	ReturnedValue = BTN_Return_Object(&ButtonObject_Automatic);

	TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_ReadButtonFunction(BTN_AUTOMATICALLY_INITIALIZED), NULL);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_PressedThreshold_mS(BTN_AUTOMATICALLY_INITIALIZED) == 0);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_LongPressedThreshold_mS(BTN_AUTOMATICALLY_INITIALIZED) == 0);
	TEST_ASSERT_EQUAL_PTR(TestGetBTN_Selves_NotificationFunction(BTN_AUTOMATICALLY_INITIALIZED), NULL);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_DefaultState(BTN_AUTOMATICALLY_INITIALIZED) == NORMALLY_HIGH);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_State(BTN_AUTOMATICALLY_INITIALIZED) == UNPRESSED);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_Timer_mS(BTN_AUTOMATICALLY_INITIALIZED) == 0);
	TEST_ASSERT_TRUE(TestGetBTN_Selves_IsInitialized(BTN_AUTOMATICALLY_INITIALIZED) == false);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Return_Object_AlreadyReturned(void)
{
	ReturnedValue = BTN_Return_Object(&ButtonObject_Manual);

	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Button_Reset_Object_HappyPath(void)
{
	ReturnedValue = BTN_Reset_Object(ButtonObject_Automatic);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Reset_Object_SelfIsNull(void)
{
	ReturnedValue = BTN_Reset_Object(ButtonObject_Manual);

	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Button_Current_State_HappyPath(void)
{
	ButtonState_t State = 0;

	ReturnedValue = BTN_Current_State(ButtonObject_Automatic, &State);

	TEST_ASSERT_TRUE(State == UNPRESSED);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Current_State_SelfIsNull(void)
{
	ButtonState_t Dummy = -1;
	BTN_Return_Object(&ButtonObject_Automatic);

	ReturnedValue = BTN_Current_State(ButtonObject_Automatic, &Dummy);

	TEST_ASSERT_TRUE(Dummy == -1);
	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Update_State_Machine_NoElapsedTime(void)
{
	TestWrapperBTN_Update_State_Machine(&ButtonObject_Automatic, 0);

	TEST_ASSERT_TRUE(ReturnedValue == false);
}

void test_Update_State_Machine_NotEnoughElapsedTime(void)
{
	TestWrapperBTN_Update_State_Machine(&ButtonObject_Automatic, Happy_ThresholdForPress_mS - 1);

	TEST_ASSERT_TRUE(TestGetBTN_Selves_State(BTN_AUTOMATICALLY_INITIALIZED) == UNPRESSED);
}

void test_Transition_Away_From_Unpressed_NotEnoughElapsedTime(void)
{
	TestWrapperBTN_Transition_Away_From_Unpressed(&ButtonObject_Automatic, Happy_ThresholdForPress_mS - 1);

	TEST_ASSERT_TRUE(TestGetBTN_Selves_State(BTN_AUTOMATICALLY_INITIALIZED) == UNPRESSED);
}
