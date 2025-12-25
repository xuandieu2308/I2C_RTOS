#include "ds1307_service.h"
#include "ds1307_driver.h"
void ds1307_service_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *day, uint8_t *month, uint16_t *year) {
    ds1307_read_time(hour, min, sec, day, month, year);
}

void ds1307_service_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint16_t year) {
    ds1307_set_time(hour, min, sec, day, month, year);
}
