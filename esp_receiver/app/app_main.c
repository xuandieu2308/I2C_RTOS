#include "uart_driver.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include "esp_log.h"

typedef struct {
    uint8_t hour, min, sec, day, month;
    uint16_t year;
} time_data_t;

static QueueHandle_t g_time_queue;
static SemaphoreHandle_t g_time_sem;
static SemaphoreHandle_t g_time_mutex;


void task_receive_uart(void *pvParameters)
{
    time_data_t time_data;
    uint8_t buf[7];
    while (1) {
        int received = uart_receive_bytes(buf, 7);
        if (received == 7) {
            time_data.hour = buf[0];
            time_data.min = buf[1];
            time_data.sec = buf[2];
            time_data.day = buf[3];
            time_data.month = buf[4];
            time_data.year = (uint16_t)buf[5] | ((uint16_t)buf[6] << 8);
            xSemaphoreTake(g_time_mutex, portMAX_DELAY);
            xQueueSend(g_time_queue, &time_data, portMAX_DELAY);
            xSemaphoreGive(g_time_mutex);
            xSemaphoreGive(g_time_sem);
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
} /* end task_receive_uart */

void task_print_time(void *pvParameters)
{
    time_data_t time_data;
    while (1) {
        if (xSemaphoreTake(g_time_sem, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(g_time_mutex, portMAX_DELAY);
            xQueueReceive(g_time_queue, &time_data, 0);
            xSemaphoreGive(g_time_mutex);
            ESP_LOGI("RECV", "Raw: %02d %02d %02d %02d %02d %02d %02d", time_data.hour, time_data.min, time_data.sec, time_data.day, time_data.month, (uint8_t)(time_data.year & 0xFF), (uint8_t)((time_data.year >> 8) & 0xFF));
            if (time_data.hour <= 23 && time_data.min <= 59 && time_data.sec <= 59 &&
                time_data.day > 0 && time_data.day <= 31 && time_data.month > 0 &&
                time_data.month <= 12 && time_data.year >= 2000 && time_data.year <= 2099) {
                ESP_LOGI("TIME", "Time: %02d:%02d:%02d Date: %02d/%02d/%04d",
                         time_data.hour, time_data.min, time_data.sec,
                         time_data.day, time_data.month, time_data.year);
            }
        }
    }
} /* end task_print_time */


void app_main(void)
{
    uart_init();
    g_time_queue = xQueueCreate(5, sizeof(time_data_t));
    g_time_sem = xSemaphoreCreateBinary();
    g_time_mutex = xSemaphoreCreateMutex();
    xTaskCreate(task_receive_uart, "receive_uart", 2048, NULL, 5, NULL);
    xTaskCreate(task_print_time, "print_time", 2048, NULL, 5, NULL);
} /* end app_main */
