#ifndef _ADC_H_
#define _ADC_H_

#include "board_freedom.h"
#include "mcp4728.h"

#define ADC_RESOLUTION_BITS                 (16U)
#define ADC_RESOLUTION                      (1<<MCP4728_RESOLUTION_BITS)

struct adc_data_s {
    uint16_t data;
    float    val;
};


extern adc_data_s adc_data[4];

void adc_init();
uint16_t adc_read(uint8_t freedom_ain_channel);     // Get ADC conversion result for ain0 (ADC0_SE16), ain1 (ADC1_SE16), ain2 (ADC1_SE18) or ain3 (ADC1_SE23)
void adc_readall(adc_data_s* adc_data);


#endif
