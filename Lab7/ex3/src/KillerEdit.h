#ifndef KILLEREDIT_H_
#define KILLEREDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include "LpcUart.h"
#include <string>

class KillerEdit: public PropertyEdit {
public:
	KillerEdit(LiquidCrystal *lcd_, LpcUart *duart_, std::string editTitle, int16_t lbound = -999, int16_t ubound = 9999);
	virtual ~KillerEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	void displayDebugValue();
	int getValue();
	void setValue(int value);
private:
	void save();
	LiquidCrystal *lcd;
	LpcUart *duart;
	std::string title;
	int value;
	int edit;
	bool focus;
	int16_t lbound; //Lower bound
	int16_t ubound; //Upper bound
};

#endif /* KILLEREDIT_H_ */
