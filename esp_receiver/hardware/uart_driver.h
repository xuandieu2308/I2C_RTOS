#ifndef UART_DRIVER_H
#define UART_DRIVER_H
#include <stdint.h>
void uart_init(void);
uint8_t uart_receive_bytes(uint8_t *data, uint8_t len);
#endif // UART_DRIVER_H
