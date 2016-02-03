// Keijo.cpp : Defines the entry point for the console application.
//

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include "stdafx.h"
#include <iostream>

#define TICKRATE_HZ1 (100)

int timer = 0;
bool tickFlag = false;
int k = 1;

#ifdef __cplusplus
extern "C" {
#endif
	void SysTick_Handler(void)
	{
		static uint32_t ms = 0;
		ms++;

		if (ms >= 1000) {
			tickFlag = true;
		}
	}
#ifdef __cplusplus
}
#endif

class Event {
public:
	enum eventType { eEnter, eExit, eKey, eTick };
	eventType type;
	int value;
	Event(eventType tyyppi, int arvo) {
		type = tyyppi;
		value = arvo;
	};
};

class StateMachine {
private:
	enum state { StateL, StateU, State1, State2, State3, State4 };
	void Locked(const Event& e);
	void Unlocked(const Event& e);
	void S1(const Event& e);
	void S2(const Event& e);
	void S3(const Event& e);
	void S4(const Event& e);
public:
	StateMachine() { SetState(StateL); };
	state currentState = State1;
	void HandleState(const Event& e);
	void SetState(state newState);
};

void StateMachine::SetState(state newState)
{
	Event exit = { Event::eExit,0 };
	Event enter = { Event::eEnter,0 };
	HandleState(exit);
	currentState = newState;
	HandleState(enter);
}

void StateMachine::Locked(const Event& e)
{
	switch (e.type) {
	case Event::eEnter:
		printf("Ovi on lukittu.\n");
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		if (e.value == 3) {
			printf("Numero 1 ok\n");
			SetState(State1);
		}
		else {
			SetState(StateL);
		}
		break;
	case Event::eTick:
		break;
	}

}

void StateMachine::S1(const Event& e)
{

	switch (e.type) {
	case Event::eEnter:
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		if (e.value == 3) {
			printf("Numero 2 ok\n");
			SetState(State2);
		}
		else {
			SetState(StateL);
		}
		break;
	case Event::eTick:
		timer++;
		if (timer >= 5) {
			SetState(StateL);
		}
		break;
	}
}

void StateMachine::S2(const Event& e)
{

	switch (e.type) {
	case Event::eEnter:
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		if (e.value == 4) {
			printf("Numero 3 ok\n");
			SetState(State3);
		}
		else {
			SetState(StateL);
		}
		break;
	case Event::eTick:
		timer++;
		if (timer >= 5) {
			SetState(StateL);
		}
		break;
	}
}

void StateMachine::S3(const Event& e)
{

	switch (e.type) {
	case Event::eEnter:
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		if (e.value == 1) {
			printf("Numero 4 ok\n");
			SetState(State4);
		}
		else {
			SetState(StateL);
		}
		break;
	case Event::eTick:
		timer++;
		if (timer >= 5) {
			SetState(StateL);
		}
		break;
	}
}

void StateMachine::S4(const Event& e)
{
	switch (e.type) {
	case Event::eEnter:
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		if (e.value == 2) {
			printf("Numero 5 ok\n");
			SetState(StateU);
		}
		else {
			SetState(StateL);
		}
		break;
	case Event::eTick:
		timer++;
		if (timer >= 5) {
			SetState(StateL);
		}
		break;
	}
}

void StateMachine::Unlocked(const Event& e)
{
	switch (e.type) {
	case Event::eEnter:
		printf("Ovi on avattu.\n");
		break;
	case Event::eExit:
		break;
	case Event::eKey:
		break;
	case Event::eTick:
		if (timer >= 5) {
			SetState(StateL);
		}
		break;
	}
}


void StateMachine::HandleState(const Event& e)
{
	switch (currentState) {
	case StateL: Locked(e); break;
	case State1: S1(e); break;
	case State2: S2(e); break;
	case State3: S3(e); break;
	case State4: S4(e); break;
	case StateU: Unlocked(e); break;
	}
}


void main(void) {
	/*

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 10, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 10);

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 16, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 16);

	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 3, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 1, 3);

	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 0, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
	Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 0);

	Board_Init();

	Chip_Clock_SetSysTickClockDiv(1);

	uint32_t sysTickRate = Chip_Clock_GetSysTickClockRate();

	SysTick_Config(sysTickRate / TICKRATE_HZ1);

	*/

	Event tick = { Event::eTick, 0 };
	Event key = { Event::eKey, 0 };

	StateMachine fsm;

	key.type = Event::eKey;

	while (1) {

		if (tickFlag) {
			tickFlag = false;
			fsm.HandleState(tick);
		}

		std::cout << "Anna numero: " << std::endl;
		std::cin >> k;
		key.value = k;
		fsm.HandleState(key);


	}

}
