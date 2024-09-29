#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();  // Inicializa el estándar de entrada y salida

    // Configura UART con baudrate 115200
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);  // TX (Pin GP0)

    const char *mensaje = "Hola desde Pico A!\n";
    
    while (true) {
        // Envía el mensaje por UART
        uart_puts(uart0, mensaje);
        sleep_ms(1000);  // Espera 1 segundo entre envíos
    }

    return 0;
}
