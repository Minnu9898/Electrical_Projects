/*
 * i2c.c
 *
 * Created: 22-01-2026 22:40:16
 *  Author: MinnuDiary
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "i2c.h"

#define SCL_CLOCK 100000UL

void i2c_init(void)
{
	TWSR = 0x00;                          // Prescaler = 1
	TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
	TWCR = (1 << TWEN);                  // Enable TWI
}

void i2c_start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

void i2c_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

uint8_t i2c_read_ack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}

uint8_t i2c_read_nack(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
	return TWDR;
}