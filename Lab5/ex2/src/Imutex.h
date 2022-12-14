/*
 * Imutex.h
 *
 *  Created on: 5 Oct 2022
 *      Author: marin
 */

#ifndef IMUTEX_H_
#define IMUTEX_H_

class Imutex { 
public: 
    Imutex(); 
    ~Imutex(); 
    void lock(); 
    void unlock(); 
private: 
    bool enable; 
};

#endif /* IMUTEX_H_ */
