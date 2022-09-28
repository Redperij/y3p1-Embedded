/*
 * I2CHandler.h
 *
 *  Created on: 26 Sep 2022
 *      Author: marin
 */

#ifndef I2CHANDLER_H_
#define I2CHANDLER_H_

#include "chip.h"
#include "board.h"

class I2CHandler {
public:
	I2CHandler(uint8_t i2c_dev_addr = 0x4d, unsigned int i2c_bitrate = 55000, unsigned int i2c_clk_divider = 1309);
	I2CHandler(const I2CHandler &) = delete;
	virtual ~I2CHandler();
	//Functions
	void SetupXferRecAndExecute(uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize);
private:
	//Variables
	I2CM_XFER_T i2cTranRec; // I2C transfer record
	uint8_t i2c_dev_addr = 0x4d; // Awailable A0 = 0x48, A1= 0x49, A2 = 0x4a, A3 = 0x4b, A4 = 0x4c, A5 = 0x4d, A6 = 0x4e, A7 = 0x4f.
	unsigned int i2c_bitrate = 55000; // 55000Hz = 55kHz
	unsigned int i2c_clk_divider = 1309; // 72000kHz / 1309 ~= 55kHz
	static bool i2c_initialised;
};

/* Standard I2C mode */
//#define I2C_MODE    (0) //Don't care?

/* SysTick rate in Hz */
//#define TICKRATE_HZ         (10) //Don't care?
#endif /* I2CHANDLER_H_ */
