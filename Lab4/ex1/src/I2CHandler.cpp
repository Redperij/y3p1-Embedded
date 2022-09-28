/*
 * I2CHandler.cpp
 *
 *  Created on: 26 Sep 2022
 *      Author: marin
 */

#include "I2CHandler.h"

bool I2CHandler::i2c_initialised = false;

/**
 * @brief Construct a new I2CHandler::I2CHandler object
 * Initialises I2C interface only once, on the first object creation.
 * 
 * @param i2c_dev_addr address of a slave device.
 * @param i2c_bitrate Speed of transfer Hz
 * @param i2c_clk_divider 
 */
I2CHandler::I2CHandler(uint8_t i2c_dev_addr, unsigned int i2c_bitrate, unsigned int i2c_clk_divider)
						: i2c_dev_addr(i2c_dev_addr), i2c_bitrate(i2c_bitrate), i2c_clk_divider(i2c_clk_divider)
{
	if(!i2c_initialised) {
		// Initialises pin muxing for I2C interface
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 22, IOCON_DIGMODE_EN | 0);
		Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 23, IOCON_DIGMODE_EN | 0);
		Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SCL);
		Chip_SWM_EnableFixedPin(SWM_FIXED_I2C0_SDA);
		// Setup I2C handle and parameters
		Chip_I2C_Init(LPC_I2C0); // Enable I2C clock and reset I2C peripheral - the boot ROM does not do this
		Chip_I2C_SetClockDiv(LPC_I2C0, this->i2c_clk_divider); // Setup clock rate for I2C
		Chip_I2CM_SetBusSpeed(LPC_I2C0, this->i2c_bitrate); // Setup I2CM transfer rate
		Chip_I2CM_Enable(LPC_I2C0); // Enable Master Mode

		I2CHandler::i2c_initialised = true;
	}
}

I2CHandler::~I2CHandler() {}


void I2CHandler::SetupXferRecAndExecute (uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize) {
	/* Setup I2C transfer record */
	i2cTranRec.slaveAddr = this->i2c_dev_addr;
	i2cTranRec.status = 0;
	i2cTranRec.txSz = txSize;
	i2cTranRec.rxSz = rxSize;
	i2cTranRec.txBuff = txBuffPtr;
	i2cTranRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cTranRec);
}
