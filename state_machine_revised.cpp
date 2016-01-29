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
#include <iostream>
using namespace std;

class Machine
{
  class State *current;
  public:
    Machine();
    void setCurrent(State *s)
    {
        current = s;
    }
    void on();
    void off();
};

class State
{
  public:
    virtual void on(Machine *m)
    {
        cout << "   already ON\n";
    }
    virtual void off(Machine *m)
    {
        cout << "   already OFF\n";
    }
};

void Machine::on()
{
  current->on(this);
}

void Machine::off()
{
  current->off(this);
}

class ON: public State
{
  public:
    ON()
    {
        cout << "   ON-ctor ";
    };
    ~ON()
    {
        cout << "   dtor-ON\n";
    };
    void off(Machine *m);
};

class OFF: public State
{
  public:
    OFF()
    {
        cout << "   OFF-ctor ";
    };
    ~OFF()
    {
        cout << "   dtor-OFF\n";
    };
    void on(Machine *m)
    {
        cout << "   going from OFF to ON";
        m->setCurrent(new ON());
        delete this;
    }
};

void ON::off(Machine *m)
{
  cout << "   going from ON to OFF";
  m->setCurrent(new OFF());
  delete this;
}

Machine::Machine()
{
  current = new OFF();
  cout << '\n';
}

void main(void) {

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


while(1) {
	  void(Machine:: *ptrs[])() =
	  {
	    Machine::off, Machine::on
	  };

	  Machine fsm;
	  int num;
	  while (1)
	  {
	    cout << "Enter 0/1: ";
	    cin >> num;
	    (fsm. *ptrs[num])();
	  }

}
