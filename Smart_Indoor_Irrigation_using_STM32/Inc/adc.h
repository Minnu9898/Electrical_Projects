/*
 * adc.h
 *
 *  Created on: Mar 18, 2026
 *      Author: MinnuDiary
 */

#ifndef ADC_H_
#define ADC_H_

#include<stdio.h>

void pa1_adc_init(void);
void start_conversion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
