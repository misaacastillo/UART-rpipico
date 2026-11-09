#include "hardware/uart.h"
#include "pico/stdlib.h"

int main(void) {
    gpio_set_function(0, GPIO_FUNC_UART);
    uart_init(uart0, 115200);

    while (true) {
        uart_puts(uart0, "Hello from Pico A!\n");
        sleep_ms(1000);
    }
}
