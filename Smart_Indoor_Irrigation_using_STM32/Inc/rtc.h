

#ifndef RTC_H_
#define RTC_H_

#include "I2C.h"

void RTC_SET_TIME(uint8_t hour, uint8_t min, uint8_t sec);
void RTC_GET_TIME(uint8_t *hour, uint8_t *min, uint8_t *sec);

#endif /* RTC_H_ */
