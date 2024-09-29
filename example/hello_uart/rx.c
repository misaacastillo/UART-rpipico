#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();  // Inicializa el estándar de entrada y salida

    // Configura UART con baudrate 115200
    uart_init(uart0, 115200);
    gpio_set_function(0, GPIO_FUNC_UART);  // TX (Pin GP0)
    gpio_set_function(1, GPIO_FUNC_UART);  // RX (Pin GP1)

    while (true) {
        if (uart_is_readable(uart0)) {
            char buffer[100];   // Buffer para almacenar el mensaje recibido
            int i = 0;

            // Lee los datos de UART hasta encontrar un '\n' (fin del mensaje)
            while (uart_is_readable(uart0)) {
                char c = uart_getc(uart0);  // Obtiene el carácter recibido
                if (c == '\n' || i >= 99) {
                    break;  // Termina si se recibe un salto de línea o el buffer está lleno
                }
                buffer[i++] = c;
            }
            buffer[i] = '\0';  // Termina la cadena con un carácter NULL
            
            // Imprime el mensaje en una sola línea
            printf("%s\n", buffer);  // Imprime el mensaje recibido y salta a la siguiente línea
        }
    }

    return 0;
}
