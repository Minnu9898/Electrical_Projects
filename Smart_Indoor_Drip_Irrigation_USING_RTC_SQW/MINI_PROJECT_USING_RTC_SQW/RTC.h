#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>

#define DS3231_ADDR 0x68

void RTC_Init(void);

void RTC_SetTimeDate(uint8_t ss, uint8_t mm, uint8_t hh,
uint8_t day, uint8_t date,
uint8_t month, uint8_t year);

void RTC_SetAlarm_Time(uint8_t hh, uint8_t mm, uint8_t ss);

void RTC_ClearAlarmFlags(void);

#endif /* RTC_H_ */