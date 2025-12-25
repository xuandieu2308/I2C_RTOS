
#ifndef UART_DRIVER_H
#define UART_DRIVER_H

#include <stdint.h>

void uart_init(void);
void uart_send_bytes(uint8_t *buf, uint8_t len);

#endif // UART_DRIVER_H
