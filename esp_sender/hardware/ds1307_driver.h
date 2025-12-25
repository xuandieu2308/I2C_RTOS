
#ifndef DS1307_DRIVER_H
#define DS1307_DRIVER_H

#include <stdint.h>

void ds1307_init(void);
void ds1307_read_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *day, uint8_t *month, uint16_t *year);

void ds1307_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint16_t year);

#endif // DS1307_DRIVER_H
