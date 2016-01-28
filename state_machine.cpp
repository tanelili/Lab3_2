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

/*
//Kombinaatio
//1
Chip_GPIO_GetPinState(LPC_GPIO, 1,3)
// 2
Chip_GPIO_GetPinState(LPC_GPIO, 1,3)
// 3
Chip_GPIO_GetPinState(LPC_GPIO, 0,16)
// 4
Chip_GPIO_GetPinState(LPC_GPIO, 0,0)
// 5
Chip_GPIO_GetPinState(LPC_GPIO, 0,10)

 */

volatile int timer = 0;
volatile int ms = 0;

enum eventType { eEnter, eExit, eKey, eTick };
struct event {
	eventType type;
	int value;
};

class StateMachine {
public:
	StateMachine ();
	void SetState(state newState);
	void Locked(event &e);
	void Unlocked(event &e);
	void S1(event &e);
	void S2(event &e);
	void S3(event &e);
	void S4(event &e);
	void HandleState(event &e);
};

StateMachine::StateMachine() {
	SetState(Locked);
}

void StateMachine::SetState(state newState)
{
	event exit = {eExit, 0};
	event enter = {eEnter, 0};
	currentState = newState;

}
// Lukko kiinni, punainen palaa
void StateMachine::Locked(event &e)
{
	switch(e.type) {
	case eEnter:
		Board_LED_Set(0, true);
		timer = 0;
		break;
	case eExit:
		break;
	case eKey:
		if(e.value == 3){
			SetState(S1);
		}
		break;
	}

}

// Lukko auki, vihreä palaa
void StateMachine::Unlocked(event &e)
{
	switch(e.type) {
	case eEnter:
		//Tässä ovi aukeaa
		timer = 0;
		break;
	case eExit:
		break;
	case eKey:
		break;
	case eTick:
		// led_set(1, 0);
		SetState(Locked);
		break;
	}
}
void StateMachine::S1(event &e)
{
	switch(e.type) {
	case eKey:
		if (e.value == 3){
			SetState(S2);
		}else{
			SetState(Locked);
		}
		break;
	case eTick:
		timer++;
		if(timer >= 5) SetState(Locked);
		break;
	}

}

void StateMachine::S2(event &e)
{
	switch(e.type) {
	case eEnter:
		timer = 0;
		break;
	case eExit:
		break;
	case eKey:
		if (e.value == 2){
			SetState(S3);
		}else{
			SetState(Locked);
		}
		break;
	case eTick:
		timer++;
		if(timer >= 5) SetState(Locked);
		break;
	}

}

void StateMachine::S3(event &e)
{
	switch(e.type) {
	case eEnter:
		timer = 0;
		break;
	case eExit:
		break;
	case eKey:
		if (e.value == 4){
			SetState(S4);
		}else{
			SetState(Locked);
		}
		break;
	case eTick:
		timer++;
		if(timer >= 5) SetState(Locked);
		break;
	}

}

void StateMachine::S4(event &e)
{
	switch(e.type) {
	case eEnter:
		timer = 0;
		break;
	case eExit:
		break;
	case eKey:
		if (e.value == 1){
			SetState(Unlocked);
		}else{
			SetState(Locked);
		}
		break;
	case eTick:
		timer++;
		if(timer >= 5) SetState(Locked);
		break;
	}

}

void StateMachine::HandleState(event &e)
{
	if (currentState == Locked){
		Locked(e);
	} else if (currentState == Unlocked){
		Unlocked(e);
	}
}

void SysTick_Handler(void)
{
	// implement real time clock with hours, minutes and seconds here
	ms++;
	if ( ms == 100) {
		timer++;
	}
}

void main () {

// #if defined (__USE_LPCOPEN)
	// Read clock settings and update SystemCoreClock variable
	SystemCoreClockUpdate();
// #if !defined(NO_BOARD_LIB)
	// Set up and initialize all required blocks and
	// functions related to the board hardware
	Board_Init();

	/* The sysTick counter only has 24 bits of precision, so it will
			   overflow quickly with a fast core clock. You can alter the
			   sysTick divider to generate slower sysTick clock rates. */
	Chip_Clock_SetSysTickClockDiv(1);

	/* A SysTick divider is present that scales the sysTick rate down
			   from the core clock. Using the SystemCoreClock variable as a
			   rate reference for the SysTick_Config() function won't work,
			   so get the sysTick rate by calling Chip_Clock_GetSysTickClockRate() */
	uint32_t sysTickRate = Chip_Clock_GetSysTickClockRate();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(sysTickRate / TICKRATE_HZ1);
	/* Nabulat */
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


	while(1){

		StateMachine ();



		// Enter an infinite loop, just incrementing a counter

	}
	return 0 ;
}
