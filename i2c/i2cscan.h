#ifndef _I2CSCAN_H_
#define _I2CSCAN_H_

#include "board_freedom.h"

/**
 * Scan the provided i2c bus and display the result over serial.
 * @param i2cbus bus to scan.
 * @return number of devices found.
 */
int i2c_scan_linuxy(I2C *i2cbus);

/**
 * Scan the provided i2c bus and display the result on the Oled screen.
 * @param i2cbus bus to scan.
 * @return number of devices found.
 */
int i2c_scan_oled(I2C *i2cbus);


#endif
