#include "rtc.h"
#include <avr/io.h>

/* ===================== I2C LOW LEVEL ===================== */
static void I2C_Init(void)
{
	TWSR = 0x00;       // Prescaler = 1
	TWBR = 0x48;       // 100 kHz SCL
	TWCR = (1 << TWEN);
}

static void I2C_Start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

static void I2C_Stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

static void I2C_Write(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1 << TWINT)));
}

/* ===================== UTILITIES ===================== */
uint8_t decToBcd(uint8_t val)
{
	return ((val / 10) << 4) | (val % 10);
}

/* ===================== RTC FUNCTIONS ===================== */
void RTC_Init(void)
{
	I2C_Init();

	// Enable Alarm1 interrupt
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0E);
	I2C_Write(0x05);
	I2C_Stop();

	RTC_ClearAlarm();     // Clear old alarms
	RTC_SetAlarm_5PM();   // Set alarm at 5 PM
}

void RTC_SetTime(uint8_t hh, uint8_t mm, uint8_t ss)
{
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x00);   // Seconds register

	I2C_Write(decToBcd(ss));
	I2C_Write(decToBcd(mm));
	I2C_Write(decToBcd(hh));

	I2C_Stop();
}

void RTC_SetAlarm_5PM(void)
{
	// Alarm1 registers start at 0x07
	// Alarm condition: 17:00:00 every day
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x07);

	I2C_Write(0x00);          // Seconds = 00
	I2C_Write(0x40);          // Minutes = 00
	I2C_Write(decToBcd(10));  // Hours = 17 (5 PM)
	I2C_Write(0x80);          // Ignore date/day

	I2C_Stop();
}

void RTC_SetAlarm_501PM(void)
{
	// Alarm2 registers start at 0x0B
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0B);

	I2C_Write(decToBcd(41));   // min = 01
	I2C_Write(decToBcd(10));  // hour = 17 (5 PM)
	I2C_Write(0x80);           // daily

	I2C_Stop();
}

void RTC_ClearAlarm(void)
{
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0F);  // Status register

	I2C_Start();
	I2C_Write((DS3231_ADDR << 1) | 1);
	uint8_t status = TWDR;
	I2C_Stop();

	status &= ~(1 << 0);   // Clear A1F bit

	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0F);
	I2C_Write(status);
	I2C_Stop();
}

uint8_t RTC_GetAlarmSource(void)
{
	uint8_t status;

	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0F);   // Status register
	I2C_Start();
	I2C_Write((DS3231_ADDR << 1) | 1);

	status = TWDR;
	I2C_Stop();

	return status;
}

void RTC_ClearAlarmFlags(void)
{
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0F);
	I2C_Write(0x00);   // clear A1F & A2F
	I2C_Stop();
}