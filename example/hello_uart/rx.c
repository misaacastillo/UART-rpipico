#include <stdio.h>

#include "hardware/uart.h"
#include "pico/stdlib.h"
#include "uart_line.h"

int main(void) {
    stdio_init_all();
    gpio_set_function(1, GPIO_FUNC_UART);
    uart_init(uart0, 115200);
    gpio_pull_up(1);

    struct uart_line line = {0};
    while (true) {
        if (!uart_is_readable(uart0)) {
            tight_loop_contents();
            continue;
        }

        switch (uart_line_feed(&line, (uint8_t)uart_getc(uart0))) {
        case UART_LINE_READY:
            puts(line.text);
            break;
        case UART_LINE_DROPPED:
            puts("[discarded malformed or oversized line]");
            break;
        case UART_LINE_PENDING:
            break;
        }
    }
}
