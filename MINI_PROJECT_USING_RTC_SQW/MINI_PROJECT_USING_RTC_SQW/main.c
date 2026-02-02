
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "rtc.h"

#define LED_PIN PB0
#define RELAY_PIN PB1

volatile uint8_t rtc_triggered = 0;

ISR(INT0_vect)
{
	rtc_triggered = 1;   // Alarm triggered
}

static void MCU_Sleep(void)
{
	set_sleep_mode(SLEEP_MODE_PWR_DOWN);
	sleep_enable();
	sei();          // enable interrupts
	sleep_cpu();    // MCU sleeps here
	sleep_disable();
}

int main(void)
{
	DDRB |= (1 << LED_PIN) | (1 << RELAY_PIN);        // LED and Relay as output
	DDRD &= ~(1 << DDD2);          // INT0 input
	PORTD |= (1 << PORTD2);        // pull-up

	EICRA |= (1 << ISC01);         // Falling edge trigger
	EIMSK |= (1 << INT0);          // Enable INT0

	sei();                          // Global interrupt enable

	RTC_Init();
	RTC_SetAlarm_5PM();             // Only use Alarm1 for simplicity

	while (1)
	{
		MCU_Sleep();                // Sleep until alarm

		if (rtc_triggered)
		{
			// LED ON for 1 minute
			PORTB |= (1 << LED_PIN);	//LED ON
			PORTB |= (1 << RELAY_PIN);   // Relay ON ? Pump ON
			for (uint8_t i = 0; i < 60; i++)
			_delay_ms(1000);    // 1 second * 60 = 1 minute

			// LED OFF for 1 minute
			PORTB &= ~(1 << LED_PIN);    // LED OFF
			PORTB &= ~(1 << RELAY_PIN);  // Relay OFF ? Pump OFF
			
			for (uint8_t i = 0; i < 60; i++)
			_delay_ms(1000);

			// Clear alarm flags for next day
			RTC_ClearAlarmFlags();
			rtc_triggered = 0;

			// Set Alarm again for next day
			RTC_SetAlarm_5PM();
		}
	}
}