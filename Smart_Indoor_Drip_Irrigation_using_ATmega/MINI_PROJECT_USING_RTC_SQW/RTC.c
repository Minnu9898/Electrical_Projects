#include "rtc.h"
#include <avr/io.h>


static void I2C_Init(void)
{
	TWSR = 0x00;
	TWBR = 0x48;          // 100kHz @ 16MHz
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


static uint8_t decToBcd(uint8_t val)
{
	return ((val / 10) << 4) | (val % 10);
}


void RTC_Init(void)
{
	I2C_Init();

	// Enable Alarm1 interrupt (INTCN + A1IE)
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0E);          // Control register
	I2C_Write(0x05);          // INTCN=1, A1IE=1
	I2C_Stop();

	RTC_ClearAlarmFlags();
}

void RTC_SetTimeDate(uint8_t ss, uint8_t mm, uint8_t hh,
uint8_t day, uint8_t date,
uint8_t month, uint8_t year)
{
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x00);

	I2C_Write(decToBcd(ss));
	I2C_Write(decToBcd(mm));
	I2C_Write(decToBcd(hh));
	I2C_Write(decToBcd(day));    // 1=Sun ... 7=Sat
	I2C_Write(decToBcd(date));
	I2C_Write(decToBcd(month));
	I2C_Write(decToBcd(year));   // last 2 digits

	I2C_Stop();
}

void RTC_SetAlarm_Time(uint8_t hh, uint8_t mm, uint8_t ss)
{
	// Alarm1 registers start at 0x07
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x07);

	I2C_Write(decToBcd(ss));   // seconds
	I2C_Write(decToBcd(mm));   // minutes
	I2C_Write(decToBcd(hh));   // hours
	I2C_Write(0x80);           // ignore date/day ? daily alarm

	I2C_Stop();
}

void RTC_ClearAlarmFlags(void)
{
	I2C_Start();
	I2C_Write(DS3231_ADDR << 1);
	I2C_Write(0x0F);     // Status register
	I2C_Write(0x00);     // Clear A1F & A2F
	I2C_Stop();
}