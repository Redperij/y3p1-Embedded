/*
 * Menu.h
 *
 *  Created on: 7 Sep 2022
 *      Author: marin
 */

#ifndef LEDCONTROLLER_H_
class LedController;
#endif

#ifndef MENU_H_
#define MENU_H_

#include <cstdint>
#include "PrintITM.h"
#include "LedController.h"

class Menu {
public:
	Menu();
	Menu(const Menu &) = delete;
	virtual ~Menu();
	int print();
	bool move_up();
	bool move_down();
	void switch_selected_led();

private:
	PrintITM console;
	LedController led_controller;
	uint8_t selected_led;
};

#endif /* MENU_H_ */
