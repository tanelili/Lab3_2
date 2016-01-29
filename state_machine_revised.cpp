/*
===============================================================================
 Name        : StateMachine Lab 3.2
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
 */
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#include <cr_section_macros.h>
#include <cstdio>
#include <iostream>
#define TICKRATE_HZ1 (100)	/* 100 ticks per second */


class Event {
public:	
	enum eventType { eEnter, eExit, eKey, eTick };
	eventType type;
	int value;
};


class StateMachine {
public:
	void HandleState(const Event& e);
	void SetState(state newState);
	// other declarations
private:
	enum state { Locked, Unlocked, State1, State2, State3, State4 };
	state currentState;
	void HandleLocked(const Event& e);
	void HandleUnlocked(const Event& e);
	// other declarations
};


void StateMachine::SetState(state newState)
{
	event exit = {eExit, 0};
	event enter = {eEnter, 0};
	HandleState(exit);
	currentState = newState;
	HandleState(enter);
}

void StateMachine::HandleUnlocked(const Event& e)
{
	switch(e.type) {
	case Event::eEnter:
		timer = 0;
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		led_set(e.value, 1);
		break;
	case Event::eTick:
		timer++;
		if(timer >= 5) SetState(State2);
		break;
	}

}

void StateMachine::HandleLocked(const Event& e)
{
	switch(e.type) {
	case Event::eEnter:
		timer = 0;
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		break;
	case Event::eTick:
		led_set(1, 0);
		led_set(2, 0);
		led_set(3, 0);
		led_set(4, 0);
		SetState(State1);
		break;
	}
}

void StateMachine::HandleState(const Event& e)
{
	switch(currentState) {
	case Locked: HandleLocked(e); break;
	case Unlocked: HandleUnlocked(e); break;
	case State1: HandleState1(e); break;
	case State2: HandleState2(e); break;
	case State3: HandleState3(e); break;
	case State4: HandleState4(e); break;
	}
}

void main() {

	/* Set up and initialize all required blocks and
	   functions related to the board hardware */
	Board_Init();

	/* The sysTick counter only has 24 bits of precision, so it will
  	   overflow quickly with a fast core clock. You can alter the
	   sysTick divider to generate slower sysTick clock rates.  */
	Chip_Clock_SetSysTickClockDiv(1);

	/* A SysTick divider is present that scales the sysTick rate down
	   From the core clock. Using the SystemCoreClock variable as a
	   rate reference for the SysTick_Config() function won't work,
	   so get the sysTick rate by calling Chip_Clock_GetSysTickClockRate() */
	uint32_t sysTickRate = Chip_Clock_GetSysTickClockRate();

	// Enable and setup SysTick Timer at a periodic rate
	SysTick_Config(sysTickRate / TICKRATE_HZ1);

	//    Nabbulat ulkoisesta levystä    */

	// Määritellään nappi 1 toimimaan inputtina
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 10);

	// Määritellään nappi 2 toimimaan inputtina
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 16, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 16);

	// Määritellään nappi 3 toimimaan inputtina
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 3);

	// Määritellään nappi 4 toimimaan inputtina
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 0);


	Event tick = { Event::eTick, 0 };
	Event key;
	key.type = Event::eKey;
	key.value = 2;
while(1) {
	if(tickFlag) {
		tickFlag = false;
		fsm.HandleState(tick);
		}

	k = isPressed();
	if(k > 0) {
		key.value = k;
		fsm.HandleState(key);
		}
	}
}
