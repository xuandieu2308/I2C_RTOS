
#include "ds1307_driver.h"
#include "uart_driver.h"
#include "ds1307_service.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include "esp_log.h"

typedef struct {
    uint8_t hour, min, sec, day, month;
    uint16_t year;
} time_data_t;

static QueueHandle_t time_queue;
static SemaphoreHandle_t time_sem;
static SemaphoreHandle_t time_mutex;

void task_read_ds1307(void *pvParameters) {
    ESP_LOGI("TASK", "task_read_ds1307 bat dau chay");
    time_data_t time;
    while (1) {
        ds1307_service_get_time(&time.hour, &time.min, &time.sec, &time.day, &time.month, &time.year);
        xSemaphoreTake(time_mutex, portMAX_DELAY);
        xQueueSend(time_queue, &time, portMAX_DELAY);
        xSemaphoreGive(time_mutex);
        xSemaphoreGive(time_sem);
    ESP_LOGI("ESP1", "Nhan duoc: %02d:%02d:%02d %02d/%02d/%04d", time.hour, time.min, time.sec, time.day, time.month, time.year);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void task_send_uart(void *pvParameters) {
    ESP_LOGI("TASK", "task_send_uart bat dau chay");
    time_data_t time;
    while (1) {
        if (xSemaphoreTake(time_sem, portMAX_DELAY) == pdTRUE) {
            xSemaphoreTake(time_mutex, portMAX_DELAY);
            xQueueReceive(time_queue, &time, 0);
            xSemaphoreGive(time_mutex);

            ESP_LOGI("ESP1", "Gui di: %02d:%02d:%02d %02d/%02d/%04d", time.hour, time.min, time.sec, time.day, time.month, time.year);
            uint8_t buf[7];
            buf[0] = time.hour;
            buf[1] = time.min;
            buf[2] = time.sec;
            buf[3] = time.day;
            buf[4] = time.month;
            buf[5] = (uint8_t)(time.year & 0xFF);
            buf[6] = (uint8_t)((time.year >> 8) & 0xFF);
            uart_send_bytes(buf, 7);
        }
    }
}

void app_main() {
    ds1307_init();
    ds1307_service_set_time(10, 28, 0, 2, 8, 2025); // Set thời gian ban đầu
    uart_init();
    time_queue = xQueueCreate(5, sizeof(time_data_t));
    time_sem = xSemaphoreCreateBinary();
    time_mutex = xSemaphoreCreateMutex();
    xTaskCreate(task_read_ds1307, "read_ds1307", 2048, NULL, 5, NULL);
    xTaskCreate(task_send_uart, "send_uart", 2048, NULL, 5, NULL);
}
