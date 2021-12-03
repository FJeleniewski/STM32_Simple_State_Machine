/*
 * button.h
 *
 *  Created on: Dec 2, 2021
 *      Author: fjele
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

//States for state machine
typedef enum
{
	IDLE = 0,
	DEBOUNCE,
	PRESS,
	REPEAT,
	RELEASE
}
B_State;

//Structure for button
typedef struct
{
	B_State 		State;

	GPIO_TypeDef* 	Port;
	uint16_t 		Pin;

	uint32_t		LastTick;
	uint32_t 		TimerDebounce;
	uint32_t 		TimerLongPress;
	uint32_t 		TimerRepeat;

	void(*ButtonPress)(void);
	void(*ButtonLongPress)(void);
	void(*ButtonRepeat)(void);
	int8_t(*ButtonRelease)(void);
}
Button_t;

//Public functions
void ButtonTask(Button_t* Key);

void ButtonSetTimerDebounce(Button_t* Key, uint32_t Milliseconds);
void ButtonSetTimerLongPress(Button_t* Key, uint32_t Milliseconds);
void ButtonSetTimerRepeat(Button_t* Key, uint32_t Milliseconds);

void ButtonRegisterPressCallback(Button_t* Key, void* Callback);
void ButtonRegisterLongPressCallback(Button_t* Key, void* Callback);
void ButtonRegisterRepeatCallback(Button_t* Key, void* Callback);
void ButtonRegisterReleaseCallback(Button_t* Key, void* Callback);

void ButtonInit(Button_t* Key, GPIO_TypeDef* Port, uint16_t Pin, uint32_t TimerDebounce,
		uint32_t TimerLongPress, uint32_t TimerRepeat);

#endif /* INC_BUTTON_H_ */
