| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-C5 | ESP32-C6 | ESP32-H2 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- | -------- | -------- | -------- |

# I2C RTOS Layered Firmware Example

This project demonstrates an **I2C-based firmware architecture** using **ESP-IDF** and **FreeRTOS**, designed with a **layered firmware structure**.

The firmware is organized into clear layers to separate **application logic**, **services**, and **hardware drivers**, improving maintainability and scalability.

---

## Firmware layering architecture

The project is divided into the following layers:

Application Layer
├── app_main.c

Service Layer
├── uart_service.c
└── uart_service.h

Hardware Driver Layer
├── ds1307_driver.c
├── ds1307_driver.h
├── uart_driver.c
└── uart_driver.h
### 1. Application Layer
- Entry point of the firmware
- Creates FreeRTOS tasks
- Calls service-layer APIs
- Does not access hardware directly

### 2. Service Layer
- Provides abstraction between application and hardware
- Manages data flow and communication logic
- Ensures application is hardware-independent

### 3. Hardware Layer
- Low-level drivers
- Directly interfaces with ESP-IDF drivers (I2C, UART)
- Handles peripheral initialization and raw read/write

---

## How the example works

- The **DS1307 RTC** is connected via I2C
- I2C initialization and communication are handled in the hardware layer
- Application layer requests time data through service or driver APIs
- FreeRTOS ensures tasks run concurrently and safely

---

## Hardware required

- ESP32 development board
- DS1307 RTC module
- Jumper wires

### I2C wiring (DS1307)

| Signal | ESP32 GPIO |
|------|-----------|
| SDA  | GPIO21 |
| SCL  | GPIO22 |
| GND  | GND |
| VCC  | 3.3V / 5V (module dependent) |

Pull-up resistors are required on SDA and SCL (usually included on DS1307 module).

---

## I2C configuration

- I2C port: I2C_NUM_0
- Mode: Master
- Clock speed: 100 kHz
- SDA pin: GPIO21
- SCL pin: GPIO22

---

## FreeRTOS usage

- Application tasks are created in `app_main.c`
- I2C access is isolated in driver layer
- Architecture allows easy addition of:
  - Mutex for I2C bus protection
  - Queues for inter-task communication

---

## Example folder contents

I2C_RTOS
├── esp_receiver
│ ├── app
│ │ └── app_main.c
│ ├── hardware
│ │ ├── uart_driver.c
│ │ └── uart_driver.h
│ └── main
│ └── CMakeLists.txt
│
├── esp_sender
│ ├── app
│ │ └── app_main.c
│ ├── hardware
│ │ ├── ds1307_driver.c
│ │ ├── ds1307_driver.h
│ │ ├── uart_driver.c
│ │ └── uart_driver.h
│ ├── service
│ │ ├── uart_service.c
│ │ └── uart_service.h
│
└── README.md

## How to build and flash

```bash
idf.py set-target esp32
idf.py build
idf.py -p PORT flash monitor
Expected behavior

I2C is initialized at startup

DS1307 RTC time is read successfully

Data can be forwarded via UART if required

Clear separation between logic, service, and hardware

Troubleshooting
I2C device not responding

Check SDA/SCL wiring

Verify DS1307 I2C address (0x68)

Ensure pull-up resistors are present

Try lowering I2C clock speed

Build issues

Check component dependencies in CMakeLists.txt

Ensure correct folder structure is preserved

Technical support and feedback

ESP32 Forum: https://esp32.com/

ESP-IDF Issues: https://github.com/espressif/esp-idf/issues
