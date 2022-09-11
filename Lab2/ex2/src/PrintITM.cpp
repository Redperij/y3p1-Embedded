/*
 * PrintITM.cpp
 *
 *  Created on: 7 Sep 2022
 *      Author: marin
 */

#include "PrintITM.h"

PrintITM::PrintITM() {
	ITM_init();
}

PrintITM::~PrintITM() {
}

int PrintITM::print(const char * format, ...) {
	char buf[501];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, sizeof(buf), format, args);
	va_end(args);
	return ITM_write(buf);
}

int PrintITM::print(const char * format, va_list args){
	char buf[501];
	vsnprintf(buf, sizeof(buf), format, args);
	return ITM_write(buf);
}

#ifdef _STRING_
int PrintITM::print(const std::string format, ...) {
	int check = 0;
	va_list args;
	va_start(args, format);
	check = print(format.c_str(), args);
	va_end(args);
	return check;
}
#endif