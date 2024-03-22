#include <errno.h>
#include <stddef.h>
#include "ButtonControl.h"
#include "UnityHelper.h"
#include "unity.h"

ErrorCode_t ReturnedValue;

/**********Happy Path Definitions**********/
ErrorCode_t (*Happy_ReadButtonFunction)(Button_ObjectList_t);
uint8_t Happy_ButtonToReference;
Button_ObjectList_t Happy_ButtonID;
uint16_t Happy_ThresholdForPress_mS;
uint16_t Happy_ThresholdForLongPress_mS;
void (*Happy_NotificationFunction)(Button_ObjectList_t, ButtonState_t);
ButtonDefaultState_t Happy_DefaultState;
Button_Object_t *ButtonObject = NULL;

/*************Fake Functions************/
ErrorCode_t Fake_ReadButtonFunction(Button_ObjectList_t dummy);
void Fake_NotificationFunction(Button_ObjectList_t Object, ButtonState_t Status);

ErrorCode_t Fake_ReadButtonFunction(Button_ObjectList_t dummy)
{
	return SUCCESS;
}
void Fake_NotificationFunction(Button_ObjectList_t Object, ButtonState_t Status)
{
	return;
}

void setUp(void)
{
	ReturnedValue = 0;
	Happy_ReadButtonFunction = &Fake_ReadButtonFunction;
	Happy_NotificationFunction = &Fake_NotificationFunction;
	Happy_ButtonToReference = 3;
	Button_ObjectList_t Happy_ButtonID = BUTTON_TEST1;
	Happy_ThresholdForPress_mS = 5;
	Happy_ThresholdForLongPress_mS = 7;
	Happy_DefaultState = NORMALLY_HIGH;

	Button_Aquire_Object(&ButtonObject, Happy_ButtonID);
}

void tearDown(void)
{
	Button_Return_Object(&ButtonObject);
}

void test_Initialize_Button_HappyPath(void)
{
	ReturnedValue = Initialize_Button(Happy_ReadButtonFunction,
									  Happy_ButtonToReference,
									  Happy_ButtonID,
									  Happy_ThresholdForPress_mS,
									  Happy_ThresholdForLongPress_mS,
									  Happy_NotificationFunction,
									  NORMALLY_HIGH);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);

	ReturnedValue = Initialize_Button(Happy_ReadButtonFunction,
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
	ReturnedValue = Initialize_Button(NULL,
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
	ReturnedValue = Initialize_Button(Happy_ReadButtonFunction,
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
	ReturnedValue = Initialize_Button(Happy_ReadButtonFunction,
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
	ReturnedValue = Initialize_Button(Happy_ReadButtonFunction,
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
	ReturnedValue = Initialize_Button(Happy_ReadButtonFunction,
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
	ReturnedValue = Initialize_Button(Happy_ReadButtonFunction,
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
	Button_Return_Object(&ButtonObject);

	ReturnedValue = Button_Aquire_Object(&ButtonObject, Happy_ButtonID);

	TEST_ASSERT_NOT_NULL(ButtonObject);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Aquire_Object_InvalidButtonID_Low(void)
{
	Button_Return_Object(&ButtonObject);

	ReturnedValue = Button_Aquire_Object(&ButtonObject, -1);

	TEST_ASSERT_NULL(ButtonObject);
	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Button_Aquire_Object_InvalidButtonID_High(void)
{
	Button_Return_Object(&ButtonObject);

	ReturnedValue = Button_Aquire_Object(&ButtonObject, NUMBER_OF_BUTTON_OBJECTS);

	TEST_ASSERT_NULL(ButtonObject);
	TEST_ASSERT_TRUE(ReturnedValue == ERANGE);
}

void test_Button_Aquire_Object_ButtonObjectNotNull(void)
{
	Button_Object_t *LocalButtonObject = &ButtonObject;
	TEST_ASSERT_NOT_NULL(LocalButtonObject);

	ReturnedValue = Button_Aquire_Object(&LocalButtonObject, Happy_ButtonID);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Button_Aquire_Object_AlreadyOwned(void)
{
	Button_Return_Object(&ButtonObject);

	Button_Aquire_Object(&ButtonObject, Happy_ButtonID);
	ReturnedValue = Button_Aquire_Object(&ButtonObject, Happy_ButtonID);

	TEST_ASSERT_TRUE(ReturnedValue == EINVAL);
}

void test_Button_Return_Object_HappyPath(void)
{
	ReturnedValue = Button_Return_Object(&ButtonObject);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Return_Object_AlreadyReturned(void)
{
	Button_Return_Object(&ButtonObject);

	ReturnedValue = Button_Return_Object(&ButtonObject);

	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Button_Reset_Object_HappyPath(void)
{
	ReturnedValue = Button_Reset_Object(ButtonObject);

	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Reset_Object_SelfIsNull(void)
{
	Button_Return_Object(&ButtonObject);

	ReturnedValue = Button_Reset_Object(ButtonObject);

	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}

void test_Button_Current_State_HappyPath(void)
{
	ButtonState_t State = 0;

	ReturnedValue = Button_Current_State(ButtonObject, &State);

	TEST_ASSERT_TRUE(State == UNPRESSED);
	TEST_ASSERT_TRUE(ReturnedValue == SUCCESS);
}

void test_Button_Current_State_SelfIsNull(void)
{
	ButtonState_t Dummy = -1;
	Button_Return_Object(&ButtonObject);

	ReturnedValue = Button_Current_State(ButtonObject, &Dummy);

	TEST_ASSERT_TRUE(Dummy == -1);
	TEST_ASSERT_TRUE(ReturnedValue == EPERM);
}
