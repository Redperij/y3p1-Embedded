/*
 * StateMachine.h
 *
 *  Created on: 29 Sep 2022
 *      Author: marin
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "Event.h"
#include "LpcUart.h"
#include "DigitalIoPin.h"
#include "I2C.h"
#include "I2CDevice.h"

class StateMachine;
typedef void (StateMachine::*state_ptr)(const Event &);

class StateMachine {
public:
	StateMachine(LpcUart *uart, I2CDevice *i2c_temp);
	virtual ~StateMachine();
	void HandleState(const Event& e);
private:
	//States.
	state_ptr currentState;
	void sblank(const Event& e);
	void sjack(const Event& e);
	void sjack_bar(const Event& e);
	void sdiner(const Event& e);
	void sjack_diner(const Event& e);
	void sjack_bar_diner(const Event& e);
	
	//Functions.
	void SetState(state_ptr newState);
	void led_switch_on(DigitalIoPin *led);
	void led_switch_off(DigitalIoPin *led);
	bool is_cold();

	//Variables.
	int timer; //Counts ticks for state.
	int timeout; //How many ticks will state remain.
	LpcUart *uart; //uart for debug prints.
	I2CDevice *i2c_temp; //i2c interface to speak with temperature sensor.
	DigitalIoPin led_jack;
    DigitalIoPin led_diner;
    DigitalIoPin led_bar;
	bool lunch; //lunch flag
	bool night; //night flag
	bool cold; //cold flag
	int8_t cold_boundary; //Which value is considered to be cold. (*set to 26 in the constructor)
};

#endif /* STATEMACHINE_H_ */