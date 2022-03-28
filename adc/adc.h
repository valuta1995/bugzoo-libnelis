/**
 * API for interacting with the Analog input ADC of the BugZoo Control box.
 * @author Niels Althuisius.
 * @author Glenn Visser
 */


#ifndef _ADC_H_
#define _ADC_H_

#include "board_freedom.h"
#include "mcp4728.h"

#define ADC_RESOLUTION_BITS                 (16U)
#define ADC_RESOLUTION                      (1<<MCP4728_RESOLUTION_BITS)

struct adc_data_s {
    uint16_t data;
    float val;
};


extern adc_data_s adc_data[4];

/**
 * Initializes the ADC of the BugZoo Control box.
 */
void adc_init();

/**
 * Read a value from one of the analog input channels of the ADC of the BugZoo Control box.
 * To convert to voltage, divide the output by the maximum output (UINT16_MAX) and multiply by v_ref.
 * @param freedom_ain_channel channel number to read from.
 * @return unsigned 16 bit integer representing the read voltage level.
 */
uint16_t adc_read(uint8_t freedom_ain_channel);

/**
 * Populates a given struct with the data for all ADC inputs.
 * @param adc_data pointer to the struct in which the results will be placed.
 */
void adc_readall(adc_data_s* adc_data);


#endif
