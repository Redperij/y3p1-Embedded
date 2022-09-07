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