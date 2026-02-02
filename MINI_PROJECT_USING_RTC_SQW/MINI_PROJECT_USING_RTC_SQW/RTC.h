#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

#define DS3231_ADDR 0x68

void RTC_Init(void);
void RTC_SetTime(uint8_t hh, uint8_t mm, uint8_t ss);
void RTC_ClearAlarm(void);
void RTC_SetAlarm_5PM(void);
void RTC_SetAlarm_501PM(void);
uint8_t RTC_GetAlarmSource(void);
void RTC_ClearAlarmFlags(void);
uint8_t decToBcd(uint8_t val);   // make BCD converter public

#endif