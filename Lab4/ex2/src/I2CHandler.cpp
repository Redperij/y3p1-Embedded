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
 * @param i2c_dev_addr Address of a slave device.
 * @param i2c_bitrate Speed of transfer Hz
 * @param i2c_clk_divider Divider for LPC frequency (72MHz).
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

bool I2CHandler::write(uint8_t reg, uint8_t *trdata, const uint16_t size) {
	uint8_t arr[size + 1];
	arr[0] = reg;
	for (unsigned int i = 1; i < (unsigned int)size + 1; i++) {
		arr[i] = trdata[i - 1];
	}
	SetupXferRecAndExecute(arr, size + 1, nullptr, 0);
	return !i2cTranRec.status;
}

/**
 * @brief Reads data from the specified register.
 * 
 * @param reg Register to read from. (command)
 * @param rdata Pointer to data array to put read value to.
 * @param size Size of an array.
 * @return true (status returned 0, I2C transfer succeeded.)
 * @return false (status returned non-zero, I2C transfer encountered an error.)
 */
bool I2CHandler::read(uint8_t reg, uint8_t *rdata, uint16_t size) {
	SetupXferRecAndExecute(&reg, 1, rdata, size);
	return !i2cTranRec.status;
}

void I2CHandler::SetupXferRecAndExecute (uint8_t *txBuffPtr, uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize) {
	while(!Chip_I2CM_IsMasterPending(LPC_I2C0));
	/* Setup I2C transfer record */
	i2cTranRec.slaveAddr = this->i2c_dev_addr;
	i2cTranRec.status = 0;
	i2cTranRec.txSz = txSize;
	i2cTranRec.rxSz = rxSize;
	i2cTranRec.txBuff = txBuffPtr;
	i2cTranRec.rxBuff = rxBuffPtr;

	Chip_I2CM_XferBlocking(LPC_I2C0, &i2cTranRec);
}
