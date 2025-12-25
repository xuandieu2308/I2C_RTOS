#include "ds1307_driver.h"
#include "driver/i2c.h"
void ds1307_init(void) {
    // Khởi tạo I2C cho DS1307
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000
    };
    i2c_param_config(I2C_NUM_0, &conf);
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
}
void ds1307_read_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *day, uint8_t *month, uint16_t *year) {
    // Đọc thời gian và ngày tháng năm từ DS1307 qua I2C
    uint8_t reg = 0x00;
    uint8_t buf[7];
    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); // Tạo một lệnh I2C mới
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x68 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x68 << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, buf, 7, I2C_MASTER_LAST_NACK);//
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    // Chuyển đổi BCD sang DEC
    *sec   = ((buf[0] >> 4) * 10) + (buf[0] & 0x0F);
    *min   = ((buf[1] >> 4) * 10) + (buf[1] & 0x0F);
    *hour  = ((buf[2] >> 4) * 10) + (buf[2] & 0x0F);
    *day   = ((buf[4] >> 4) * 10) + (buf[4] & 0x0F);
    *month = ((buf[5] >> 4) * 10) + (buf[5] & 0x0F);
    *year  = 2000 + ((buf[6] >> 4) * 10) + (buf[6] & 0x0F);
}
// kiểm tra get đúng k 
void ds1307_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t day, uint8_t month, uint16_t year) {
    uint8_t buf[7];
    buf[0] = ((sec / 10) << 4) | (sec % 10);
    buf[1] = ((min / 10) << 4) | (min % 10);
    buf[2] = ((hour / 10) << 4) | (hour % 10);
    buf[3] = 0; // Day of week
    buf[4] = ((day / 10) << 4) | (day % 10);
    buf[5] = ((month / 10) << 4) | (month % 10);
    buf[6] = (((year % 100) / 10) << 4) | (year % 10);
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x68 << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x00, true);
    i2c_master_write(cmd, buf, 7, true);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
}
