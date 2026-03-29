/*
 * uart.h
 *
 *  Created on: Mar 3, 2026
 *      Author: MinnuDiary
 */

#ifndef UART_H_
#define UART_H_

#include "stm32f4xx.h"
#include <stdint.h>

void UART2_tx_init (void);
void UART2_rxtx_init (void);
void uart2_write(int ch);

#endif /* UART_H_ */
