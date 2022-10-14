#include "KillerEdit.h"
#include <cstdio>

KillerEdit::KillerEdit(LiquidCrystal *lcd_, LpcUart *duart_, std::string editTitle, int16_t lbound, int16_t ubound): lcd(lcd_), duart(duart_), title(editTitle), lbound(lbound), ubound(ubound) {
	value = 0;
	edit = 0;
	focus = false;
}

KillerEdit::~KillerEdit() {}

void KillerEdit::increment() {
	if(this->edit < this->ubound) this->edit++;
}

void KillerEdit::decrement() {
	if(this->edit > this->lbound) this->edit--;
}

void KillerEdit::accept() {
	save();
	//Generate watchdog invalid feed sequence.
	LPC_WWDT->FEED = 0x00;
	LPC_WWDT->FEED = 0x11;
}

void KillerEdit::cancel() {
	this->edit = this->value;
}

void KillerEdit::setFocus(bool focus) {
	this->focus = focus;
}

bool KillerEdit::getFocus() {
	return this->focus;
}

void KillerEdit::display() {
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

void KillerEdit::displayDebugValue() {
	char buff[128];
	snprintf(buff, 128, "%s\r\n      %4d      \r\n", this->title.c_str(), this->value);
	this->duart->write(buff);
}

void KillerEdit::save() {
	//Set current value to be same as edit value
	this->value = this->edit;
	//TODO: save current value for example to EEPROM for permanent storage
}

int KillerEdit::getValue() {
	return this->value;
}

void KillerEdit::setValue(int value) {
	if (value <= this->lbound) this->edit = this->lbound;
	else if (value >= this->ubound) this->edit = this->ubound;
	else this->edit = value;
	save();
}
