/*
 * Imutex.cpp
 *
 *  Created on: 5 Oct 2022
 *      Author: marin
 */

#include "Imutex.h"
#include "chip.h"
 
Imutex::Imutex() : enable(false) {} 
 
Imutex::~Imutex() {}
 
void Imutex::lock() 
{ 
    enable = (__get_PRIMASK() & 1) == 0; 
    __disable_irq(); 
} 
 
void Imutex::unlock() 
{ 
    if(enable) __enable_irq(); 
}