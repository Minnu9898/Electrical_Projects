//Find the code RTC_SetAlarm_Time(18, 47, 0);  to manipulate time in main.c

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "rtc.h"

#define LED_PIN   PB0
#define RELAY_PIN PB1

volatile uint8_t rtc_triggered = 0;


ISR(INT0_vect)
{
	rtc_triggered = 1;
}


static void MCU_Sleep(void)
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();
	sleep_cpu();
	sleep_disable();
}


int main(void)
{
	/* GPIO */
	DDRB |= (1 << LED_PIN) | (1 << RELAY_PIN);
	PORTB |= (1 << RELAY_PIN);
	
	DDRD &= ~(1 << DDD2);        // INT0 input
	PORTD |= (1 << PORTD2);      // pull-up

	/* INT0 */
	EICRA |= (1 << ISC01);       // falling edge
	EIMSK |= (1 << INT0);

	sei();

	RTC_Init();

	
	// Time: 18:09:00
	// Date: 07-02-2026 (Saturday = 7)
	RTC_SetTimeDate(0, 29, 22, 4, 11, 2, 26);  

	/* Alarm Time */
	RTC_SetAlarm_Time(22,30, 0);  //  change time When Alaaarm is required

	while (1)
	{
		MCU_Sleep();

		if (rtc_triggered)
		{
			PORTB |= (1 << LED_PIN);
			PORTB &= ~(1 << RELAY_PIN);

			for (uint8_t i = 0; i < 60; i++)
			_delay_ms(1000);     // ON for 1 minute

			PORTB &= ~(1 << LED_PIN);
			PORTB |= (1 << RELAY_PIN);

			RTC_ClearAlarmFlags();
			rtc_triggered = 0;

			RTC_SetAlarm_Time(18, 37, 0); // set for next day
		}
	}
}
/* 
CONNECTION --------------------------------------
GOB = GROUND OF BOARD  
VCCOB=VCC OF BOARD
RTC CONNECTION 
	GRND-GOB
	VCC - VCCOB
	SDA - A4
	SCL -A5
	SQW -PIN NUM 2
	32K
LED POSITIVE - RESISTOR - PIN 8
LED NEGATIVE - GOB
RELAY CONNECTIONS 
	IN -PIN 9
	GRD -GOB
	VCC - VCCOB

RELAY SCREWS =NO, COM, NC
COM - ADAPTER POSITIVE
NO ( oppo. red led) - PUMP POSITIVE
ADAPTER NEGATIVE TO PUMP NEGATIVE

*/
