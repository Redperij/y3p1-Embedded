/*
 * DecimalEdit.cpp
 *
 *  Created on: 12 Oct 2022
 *      Author: marin
 */

#include "DecimalEdit.h"
#include <cstdio>

DecimalEdit::DecimalEdit(LiquidCrystal *lcd_, LpcUart *duart_, std::string editTitle, float lbound, float ubound, float step): lcd(lcd_), duart(duart_), title(editTitle), lbound(lbound), ubound(ubound), step(step) {
	value = 0;
	edit = 0;
	focus = false;
}

DecimalEdit::~DecimalEdit() {}

void DecimalEdit::increment() {
	if(this->edit + this->step <= this->ubound) this->edit += this->step;
	else this->edit = this->ubound;
}

void DecimalEdit::decrement() {
	if(this->edit - this->step >= this->lbound) this->edit -= this->step;
	else this->edit = this->lbound;
}

void DecimalEdit::accept() {
	save();
}

void DecimalEdit::cancel() {
	this->edit = this->value;
}

void DecimalEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool DecimalEdit::getFocus() {
	return this->focus;
}

void DecimalEdit::display() {
	lcd->clear();
	lcd->setCursor(0,0);
	lcd->print(title);
	lcd->setCursor(0,1);
	char s[17];
	if(focus) {
		snprintf(s, 17, "     [%3.1f]     ", edit);
	}
	else {
		snprintf(s, 17, "      %3.1f      ", edit);
	}
	lcd->print(s);
}

void DecimalEdit::displayDebugValue() {
	char buff[128];
	snprintf(buff, 128, "%s\r\n      %3.1f      \r\n", this->title.c_str(), this->value);
	this->duart->write(buff);
}

void DecimalEdit::save() {
	//Set current value to be same as edit value
	this->value = this->edit;
	//TODO: save current value for example to EEPROM for permanent storage
}

float DecimalEdit::getValue() {
	return this->value;
}

float DecimalEdit::getStep() {
	return this->step;
}

void DecimalEdit::setValue(float value) {
	if (value <= this->lbound) this->edit = this->lbound;
	else if (value >= this->ubound) this->edit = this->ubound;
	else this->edit = value;
	save();
}