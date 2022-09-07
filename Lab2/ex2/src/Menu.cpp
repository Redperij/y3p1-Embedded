/*
 * Menu.cpp
 *
 *  Created on: 7 Sep 2022
 *      Author: marin
 */

#include "Menu.h"

Menu::Menu() {
	this->selected_led = 0;
}

Menu::~Menu() {
}

int Menu::print() {
	bool printing_flag = true;
	printing_flag = printing_flag && this->console.print("\n\n\n\n\n\n--------------------\n");
	printing_flag = printing_flag && this->console.print("Select led:\n");
	printing_flag = printing_flag && this->console.print("RED    %s  %s", this->led_controller.get_r_state() ? "ON " : "OFF", this->selected_led == 0 ? "<----\n" : "\n");
	printing_flag = printing_flag && this->console.print("GREEN  %s  %s", this->led_controller.get_g_state() ? "ON " : "OFF", this->selected_led == 1 ? "<----\n" : "\n");
	printing_flag = printing_flag && this->console.print("BLUE   %s  %s", this->led_controller.get_b_state() ? "ON " : "OFF", this->selected_led == 2 ? "<----\n" : "\n");
	printing_flag = printing_flag && this->console.print("--------------------\n");
	return printing_flag;
}

bool Menu::move_up() {
	if(this->selected_led > 0) {
		--(this->selected_led);
		return true;
	}
	else {
		this->selected_led = 2;
		return false;
	}
}

bool Menu::move_down() {
	if(this->selected_led < 2) {
		++(this->selected_led);
		return true;
	}
	else {
		this->selected_led = 0;
		return false;
	}
}

void Menu::switch_selected_led() {
	this->led_controller.switch_led(this->selected_led);
}