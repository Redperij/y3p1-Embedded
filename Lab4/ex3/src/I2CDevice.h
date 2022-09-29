/*
 * I2CDevice.h
 *
 *  Created on: 29 Sep 2022
 *      Author: marin
 */

#ifndef I2CDEVICE_H_
#define I2CDEVICE_H_

#include "chip.h"
#include "board.h"
#include "I2C.h"

class I2CDevice {
public:
	I2CDevice(I2C *i2c_interface, uint8_t dev_addr = 0x4d);
	I2CDevice(const I2CDevice &) = delete;
	virtual ~I2CDevice();
	bool write(uint8_t com, uint8_t *trdata, const uint16_t size);
	bool read(uint8_t com, uint8_t *rdata, uint16_t size);
private:
	I2C *i2c_interface;
	uint8_t dev_addr = 0x4d; //Available A0 = 0x48, A1= 0x49, A2 = 0x4a, A3 = 0x4b, A4 = 0x4c, A5 = 0x4d, A6 = 0x4e, A7 = 0x4f.
};

#endif /* I2CDEVICE_H_ */
