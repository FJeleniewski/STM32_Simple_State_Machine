/*
 * button.c
 *
 *  Created on: Dec 2, 2021
 *      Author: fjele
 */

#include "main.h"

#include "button.h"

//Button initialization
void ButtonInit(Button_t* Key, GPIO_TypeDef* Port, uint16_t Pin, uint32_t TimerDebounce, uint32_t TimerLongPress, uint32_t TimerRepeat)
{
	Key->State = IDLE;

	Key->Port = Port;
	Key->Pin = Pin;

	Key->TimerDebounce = TimerDebounce;
	Key->TimerLongPress = TimerLongPress;
	Key->TimerRepeat = TimerRepeat;
}

//Time setting functions
void ButtonSetTimerDebounce(Button_t* Key, uint32_t Milliseconds)
{
	Key->TimerDebounce = Milliseconds;
}

void ButtonSetTimerLongPress(Button_t* Key, uint32_t Milliseconds)
{
	Key->TimerLongPress = Milliseconds;
}

void ButtonSetTimerRepeat(Button_t* Key, uint32_t Milliseconds)
{
	Key->TimerRepeat = Milliseconds;
}

//Register callback's
void ButtonRegisterPressCallback(Button_t* Key, void* Callback)
{
	Key->ButtonPress = Callback;
}

void ButtonRegisterLongPressCallback(Button_t* Key, void* Callback)
{
	Key->ButtonLongPress = Callback;
}

void ButtonRegisterRepeatCallback(Button_t* Key, void* Callback)
{
	Key->ButtonRepeat = Callback;
}

void ButtonRegisterReleaseCallback(Button_t* Key, void* Callback)
{
	Key->ButtonRelease = Callback;
}

//States routines
void ButtonIdleRoutine(Button_t* Key)
{
	if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->Port, Key->Pin))
	{
		Key->State = DEBOUNCE;
		Key->LastTick = HAL_GetTick();
	}

}

void ButtonDebounceRoutine(Button_t* Key)
{
	if ((HAL_GetTick() - Key->LastTick) > Key->TimerDebounce)
	{
		if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(Key->Port, Key->Pin))
		{
			Key->State = PRESS;
			Key->LastTick = HAL_GetTick();

			if (Key->ButtonPress != NULL)
			{
				Key->ButtonPress();
			}
		}
		else
		{
			Key->State = IDLE;
		}
	}

}

void ButtonPressRoutine(Button_t* Key)
{
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->Port, Key->Pin))
	{
		Key->State = RELEASE;
	}
	else
	{
		if ((HAL_GetTick() - Key->LastTick) > Key->TimerLongPress)
		{
			Key->State = REPEAT;
			Key->LastTick = HAL_GetTick();

			if (Key->ButtonLongPress != NULL)
			{
				Key->ButtonLongPress();
			}
		}
	}

}

void ButtonRepeatRoutine(Button_t* Key)
{
	if (GPIO_PIN_SET == HAL_GPIO_ReadPin(Key->Port, Key->Pin))
	{
		Key->State = RELEASE;
	}
	else
	{
		if ((HAL_GetTick() - Key->LastTick) > Key->TimerRepeat)
		{
			Key->LastTick = HAL_GetTick();

			if (Key->ButtonRepeat != NULL)
			{
				Key->ButtonRepeat();
			}
		}
	}

}

void ButtonReleaseRoutine(Button_t* Key)
{
	if (Key->ButtonRelease != NULL)
	{
		if(Key->ButtonRelease() <= 0)
		{
			Key->State = IDLE;
		}
	}
	else
	{
		Key->State = IDLE;
	}
}


//State machine
void ButtonTask(Button_t* Key)
{
	switch(Key->State)
	{
	case IDLE:
		ButtonIdleRoutine(Key);
		break;

	case DEBOUNCE:
		ButtonDebounceRoutine(Key);
		break;

	case PRESS:
		ButtonPressRoutine(Key);
		break;

	case REPEAT:
		ButtonRepeatRoutine(Key);
		break;

	case RELEASE:
		ButtonReleaseRoutine(Key);
		break;

	default:
		break;
	}
}
