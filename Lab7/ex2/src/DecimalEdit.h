/*
 * DecimalEdit.h
 *
 *  Created on: 12 Oct 2022
 *      Author: marin
 */

#ifndef DECIMALEDIT_H_
#define DECIMALEDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include "LpcUart.h"
#include <string>

class DecimalEdit: public PropertyEdit {
public:
	DecimalEdit(LiquidCrystal *lcd_, LpcUart *duart_, std::string editTitle, float lbound = -999, float ubound = 9999, float step = 0.1);
	virtual ~DecimalEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	void displayDebugValue();
	float getValue();
	float getStep();
	void setValue(float value);
private:
	void save();
	LiquidCrystal *lcd;
	LpcUart *duart;
	std::string title;
	float value;
	float edit;
	bool focus;
	float lbound; //Lower bound
	float ubound; //Upper bound
	float step; //Determines increment / decrement step per up/down event.
};

#endif /* DECIMALEDIT_H_ */
