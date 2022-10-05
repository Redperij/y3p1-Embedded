/*
 * RealTimeClock.cpp
 *
 *  Created on: 5 Oct 2022
 *      Author: marin
 */

#include "RealTimeClock.h"
#include <mutex>

RealTimeClock::RealTimeClock(int ticksPerSecond) : ticks_per_second(ticksPerSecond), ticks_passed(0) {
	tm init_time;
	init_time.tm_sec = 35; 
	init_time.tm_min = 58;
	init_time.tm_hour = 23;
	init_time.tm_mday = 1;
	init_time.tm_mon = 0;
	init_time.tm_year = 122;
	this->hour = init_time.tm_hour;
	this->min = init_time.tm_min;
	this->sec = init_time.tm_sec;
	this->rawtime = mktime(&init_time);
}

RealTimeClock::RealTimeClock(int ticksPerSecond, tm now) : ticks_per_second(ticksPerSecond), ticks_passed(0) {
	this->hour = now.tm_hour;
	this->min = now.tm_min;
	this->sec = now.tm_sec;
	this->rawtime = mktime(&now);
}

RealTimeClock::RealTimeClock(int ticksPerSecond, time_t rawtime) : rawtime(rawtime), ticks_per_second(ticksPerSecond), ticks_passed(0) {
	tm *init_time = localtime(&rawtime);
	this->hour = init_time->tm_hour;
	this->min = init_time->tm_min;
	this->sec = init_time->tm_sec;
}

RealTimeClock::~RealTimeClock() {}

void RealTimeClock::gettime(tm *now) {
	std::lock_guard<Imutex> lock(guard);
    now->tm_hour = this->hour;
    now->tm_min = this->min;
	now->tm_sec = this->sec;
}

void RealTimeClock::tick() {
	std::lock_guard<Imutex> lock(guard);
	this->ticks_passed++;
	if(this->ticks_passed >= this->ticks_per_second) {
		this->ticks_passed = 0;
		this->rawtime++;
		const time_t temp_time = this->rawtime;
		tm *init_time = localtime(&temp_time);
		this->hour = init_time->tm_hour;
		this->min = init_time->tm_min;
		this->sec = init_time->tm_sec;
	}
}