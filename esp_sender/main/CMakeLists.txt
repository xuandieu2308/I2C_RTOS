idf_component_register(
    SRCS "../app/app_main.c" "../service/ds1307_service.c" "../hardware/ds1307_driver.c" "../hardware/uart_driver.c"
    INCLUDE_DIRS "../app" "../service" "../hardware"
)
