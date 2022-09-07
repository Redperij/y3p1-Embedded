/*
 * PrintITM.h
 *
 *  Created on: 7 Sep 2022
 *      Author: marin
 */

#ifndef PRINTITM_H_
#define PRINTITM_H_

#include "ITM_write.h"
#include <cstdio>
#include <cstdarg>

class PrintITM {
public:
	PrintITM();
	PrintITM(const PrintITM &) = delete;
	virtual ~PrintITM();
	int print(const char * format, ...);
};

#endif /* PRINTITM_H_ */
