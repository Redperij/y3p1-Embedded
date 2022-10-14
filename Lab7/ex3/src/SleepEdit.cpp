/*
 * SleepEdit.cpp
 *
 *  Created on: 14 Oct 2022
 *      Author: marin
 */

#include "SleepEdit.h"
#include "systick.h"
#include <cstdio>

SleepEdit::SleepEdit(LiquidCrystal *lcd_, LpcUart *duart_, std::string editTitle, int16_t lbound, int16_t ubound): lcd(lcd_), duart(duart_), title(editTitle), lbound(lbound), ubound(ubound) {
	value = 0;
	edit = 0;
	focus = false;
}

SleepEdit::~SleepEdit() {}

void SleepEdit::increment() {
	if(this->edit < this->ubound) this->edit++;
}

void SleepEdit::decrement() {
	if(this->edit > this->lbound) this->edit--;
}

void SleepEdit::accept() {
	save();
	this->duart->write("RESIDENT SLEEPER\r\n");
	Sleep(10000); //Sleeps 10 seconds, generating watchdog timeout.
}

void SleepEdit::cancel() {
	this->edit = this->value;
}

void SleepEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool SleepEdit::getFocus() {
	return this->focus;
}

void SleepEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%4d]     ", edit);
	}
	else {
		snprintf(s, 17, "      %4d      ", edit);
	}
	lcd->print(s);
}

void SleepEdit::displayDebugValue() {
	char buff[128];
	snprintf(buff, 128, "%s\r\n      %4d      \r\n", this->title.c_str(), this->value);
	this->duart->write(buff);
}

void SleepEdit::save() {
	//Set current value to be same as edit value
	this->value = this->edit;
	//TODO: save current value for example to EEPROM for permanent storage
}

int SleepEdit::getValue() {
	return this->value;
}

void SleepEdit::setValue(int value) {
	if (value <= this->lbound) this->edit = this->lbound;
	else if (value >= this->ubound) this->edit = this->ubound;
	else this->edit = value;
	save();
}