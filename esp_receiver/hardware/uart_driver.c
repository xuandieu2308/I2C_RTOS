#include "uart_driver.h"
#include "driver/uart.h"
#include "freertos/FreeRTOS.h"
uint8_t uart_receive_bytes(uint8_t *data, uint8_t len) {
    // Nhận nhiều byte từ UART thực tế
    int rxBytes = uart_read_bytes(UART_NUM_1, data, len, 1000 / portTICK_PERIOD_MS);
    return rxBytes > 0 ? rxBytes : 0;
}
void uart_init(void) {
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(UART_NUM_1, &uart_config);
    uart_set_pin(UART_NUM_1, 17, 16, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // TX=17, RX=16
    uart_driver_install(UART_NUM_1, 1024, 1024, 0, NULL, 0);
}
