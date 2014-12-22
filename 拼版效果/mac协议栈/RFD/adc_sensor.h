
#ifndef  _adc_sensor_h
#define  _adc_sensor_h

#include "top.h"
#include "generic.h"
uint16_t   SENSOR_DATA_VALUE[10] ;
uint8_t    ADC_INTERRUPT_FLAG;
uint8_t  adc_init(void);
uint8_t  sensor_init(void);
void     read_data_task(void);

#endif
