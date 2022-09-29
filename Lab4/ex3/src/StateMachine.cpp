/*
 * StateMachine.cpp
 *
 *  Created on: 29 Sep 2022
 *      Author: marin
 */

#include "StateMachine.h"

StateMachine::StateMachine(LpcUart *uart, I2CDevice *i2c_temp) : uart(uart), i2c_temp(i2c_temp), led_jack({0, 27, false, true, false}), led_diner({0, 24, false, true, false}), led_bar({1, 0, false, true, false}) {
	this->led_jack.write(false);
    this->led_diner.write(false);
    this->led_bar.write(false);
	this->lunch = false;
	this->night = false;
	this->cold = false;
	this->timeout = 2000;
	this->cold_boundary = 26;
	this->currentState = &StateMachine::sblank;
	(this->*currentState)(Event(Event::eEnter));
}

StateMachine::~StateMachine() {}

void StateMachine::SetState(state_ptr newState) {
	(this->*currentState)(Event(Event::eExit));
	this->currentState = newState;
	(this->*currentState)(Event(Event::eEnter));
}

void StateMachine::HandleState(const Event& e) {
	(this->*currentState)(e);
}

//////////////////////////////////////////////////////////////////////////////
// States ////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void StateMachine::sblank(const Event& e) {
	switch (e.type) 
	{
	case Event::eEnter:
		this->uart->write("Entered sblank.\r\n");
		this->timer = 0;
		break;
	case Event::eExit:
		this->uart->write("Exited sblank.\r\n");
		break;
	case Event::eLunch:
		this->lunch = !(this->lunch);
		break;
	case Event::eNight:
		this->night = !(this->night);
		break;
	case Event::eTick:
		this->timer++;
		if (this->timer >= this->timeout) SetState(&StateMachine::sjack);
		break;
	default:
		break;
	}
}

void StateMachine::sjack(const Event& e) {
	switch (e.type) 
	{
	case Event::eEnter:
		this->uart->write("Entered sjack.\r\n");
		led_switch_on(&(this->led_jack));
		this->timer = 0;
		break;
	case Event::eExit:
		if (this->lunch) led_switch_off(&(this->led_jack));
		this->uart->write("Exited sjack.\r\n");
		break;
	case Event::eLunch:
		this->lunch = !(this->lunch);
		break;
	case Event::eNight:
		this->night = !(this->night);
		break;
	case Event::eTick:
		this->timer++;
		if (this->timer >= this->timeout) {
			if(this->lunch) SetState(&StateMachine::sdiner);
			else SetState(&StateMachine::sjack_bar);
		}
		break;
	default:
		break;
	}
}

void StateMachine::sjack_bar(const Event& e) {
	switch (e.type) 
	{
	case Event::eEnter:
		this->uart->write("Entered sjack_bar.\r\n");
		led_switch_on(&(this->led_bar));
		this->timer = 0;
		break;
	case Event::eExit:
		this->uart->write("Exited sjack_bar.\r\n");
		break;
	case Event::eLunch:
		this->lunch = !(this->lunch);
		break;
	case Event::eNight:
		this->night = !(this->night);
		break;
	case Event::eTick:
		this->timer++;
		if (this->timer >= this->timeout) SetState(&StateMachine::sjack_bar_diner);
		break;
	default:
		break;
	}
}

void StateMachine::sdiner(const Event& e) {
	switch (e.type) 
	{
	case Event::eEnter:
		this->uart->write("Entered sdiner.\r\n");
		led_switch_on(&(this->led_diner));
		this->timer = 0;
		break;
	case Event::eExit:
		this->uart->write("Exited sdiner.\r\n");
		break;
	case Event::eLunch:
		this->lunch = !(this->lunch);
		break;
	case Event::eNight:
		this->night = !(this->night);
		break;
	case Event::eTick:
		this->timer++;
		if (this->timer >= this->timeout) SetState(&StateMachine::sjack_diner);
		break;
	default:
		break;
	}
}

void StateMachine::sjack_diner(const Event& e) {
	switch (e.type) 
	{
	case Event::eEnter:
		this->uart->write("Entered sjack_diner.\r\n");
		led_switch_on(&(this->led_jack));
		this->timer = 0;
		break;
	case Event::eExit:
		led_switch_off(&(this->led_diner));
		if (!this->cold) led_switch_off(&(this->led_jack));
		this->uart->write("Exited sjack_diner.\r\n");
		break;
	case Event::eLunch:
		this->lunch = !(this->lunch);
		break;
	case Event::eNight:
		this->night = !(this->night);
		break;
	case Event::eTick:
		this->timer++;
		if(this->timer >= this->timeout) {
			this->cold = is_cold();
			if (this->cold) SetState(&StateMachine::sjack);
			else SetState(&StateMachine::sblank);
		}
		break;
	default:
		break;
	}
}

void StateMachine::sjack_bar_diner(const Event& e) {
	switch (e.type) 
	{
	case Event::eEnter:
		this->uart->write("Entered sjack_bar_diner.\r\n");
		led_switch_on(&(this->led_diner));
		this->timer = 0;
		break;
	case Event::eExit:
		led_switch_off(&(this->led_diner));
		led_switch_off(&(this->led_bar));
		if (!this->cold) led_switch_off(&(this->led_jack));
		this->uart->write("Exited sjack_bar_diner.\r\n");
		break;
	case Event::eLunch:
		this->lunch = !(this->lunch);
		break;
	case Event::eNight:
		this->night = !(this->night);
		break;
	case Event::eTick:
		if (!this->night) {
			this->timer++;
			if (this->timer >= this->timeout) {
				this->cold = is_cold();	
				if (this->cold) SetState(&StateMachine::sjack);
				else SetState(&StateMachine::sblank);
			}
		}
		break;
	default:
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
// Helpers ///////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

void StateMachine::led_switch_on(DigitalIoPin *led) {
	led->write(true);
}

void StateMachine::led_switch_off(DigitalIoPin *led) {
	led->write(false);
}

bool StateMachine::is_cold() {
	int i = 0;
	uint8_t data = 0;
	const uint8_t temp_reg = 0x00;
	const uint8_t cntr_reg = 0x01;
	const uint8_t ready_bit = 0x40;
	uint8_t standby_bit = 0x80;
	data = 0x00;
	//Remove standby.
	if (!(this->i2c_temp->write(cntr_reg, &data, 1))){
		this->uart->write("I2C: Unable to reach temperature sensor.\r\n");
		return this->cold;
	}
	//Wait for sensor to be ready.
	while (i < 1000000) i++;
	data = 0x00; //Expected to be 0x40.
	//Read status.
	if (!(this->i2c_temp->read(cntr_reg, &data, 1))) {
		this->uart->write("I2C: Unable to reach temperature sensor.\r\n");
		return this->cold;
	}
	//Temperature sensor is ready to transfer readings.
	if (data & ready_bit) {
		//Delay for i2c transfer due to the bug.
		while (i < 10000) i++;
		if (!this->i2c_temp->read(temp_reg, &data, 1)) {
			this->uart->write("I2C: Unable to reach temperature sensor.\r\n");
			return this->cold;
		}
		//Delay for i2c transfer due to the bug.
		while (i < 10000) i++;
		//Set back standby and exit.
		this->i2c_temp->write(cntr_reg, &standby_bit, 1);
		char msg[128];
		snprintf(msg, 128, "I2C: Temperature is: %d\r\n", (int8_t) data);
		this->uart->write(msg);
		return ((int8_t) data) <= (this->cold_boundary);
	}
	//Temperature sensor was not ready.
	//Delay for i2c transfer due to the bug.
	while (i < 10000) i++;
	//Set back standby and exit.
	this->i2c_temp->write(cntr_reg, &standby_bit, 1);
	this->uart->write("I2C: Temperature sensor was not ready for reading.\r\n");
	return this->cold;
}