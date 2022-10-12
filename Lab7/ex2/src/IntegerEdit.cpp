/*
 * IntegerEdit.cpp
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#include "IntegerEdit.h"
#include <cstdio>

IntegerEdit::IntegerEdit(LiquidCrystal *lcd_, LpcUart *duart_, std::string editTitle, int16_t lbound, int16_t ubound): lcd(lcd_), duart(duart_), title(editTitle), lbound(lbound), ubound(ubound) {
	value = 0;
	edit = 0;
	focus = false;
}

IntegerEdit::~IntegerEdit() {}

void IntegerEdit::increment() {
	if(this->edit < this->ubound) this->edit++;
}

void IntegerEdit::decrement() {
	if(this->edit > this->lbound) this->edit--;
}

void IntegerEdit::accept() {
	save();
}

void IntegerEdit::cancel() {
	this->edit = this->value;
}

void IntegerEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool IntegerEdit::getFocus() {
	return this->focus;
}

void IntegerEdit::display() {
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

void IntegerEdit::displayDebugValue() {
	char buff[128];
	snprintf(buff, 128, "%s\r\n      %4d      \r\n", this->title.c_str(), this->value);
	this->duart->write(buff);
}

void IntegerEdit::save() {
	//Set current value to be same as edit value
	this->value = this->edit;
	//TODO: save current value for example to EEPROM for permanent storage
}

int IntegerEdit::getValue() {
	return this->value;
}

void IntegerEdit::setValue(int value) {
	if (value <= this->lbound) this->edit = this->lbound;
	else if (value >= this->ubound) this->edit = this->ubound;
	else this->edit = value;
	save();
}
