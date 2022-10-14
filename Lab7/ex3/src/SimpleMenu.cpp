/*
 * SimpleMenu.cpp
 *
 *  Created on: 3.2.2016
 *      Author: krl
 */

#include "SimpleMenu.h"

SimpleMenu::SimpleMenu(LpcUart *duart_) : duart(duart_) {
	position = 0;
}

SimpleMenu::~SimpleMenu() {}

void SimpleMenu::addItem(MenuItem *item) {
	items.push_back(item);
}

void SimpleMenu::event(MenuItem::menuEvent e) {
	if(items.size() <= 0) return;

	switch (items[position]->event(e))
	{
	case MenuItem::unhandled:
		if(e == MenuItem::up) position++;
		else if(e == MenuItem::down) position--;

		if(position < 0) position = items.size() - 1;
		if(position >= (int) items.size()) position = 0;

		items[position]->event(MenuItem::show);
		break;
	case MenuItem::got_modified:
		this->duart->write("------------------\r\n");
		for (auto item : items) {
			item->debugDisplay();
		}
		this->duart->write("------------------\r\n");
		break;
	default:
		break;
	}
}
