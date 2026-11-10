#include <cstring>

#include "uart_line.h"

int main() {
    uart_line line{};
    const auto first = uart_line_feed(&line, 'O');
    const auto second = uart_line_feed(&line, 'K');
    const auto end = uart_line_feed(&line, '\n');

    const bool parsed = first == UART_LINE_PENDING && second == UART_LINE_PENDING &&
                        end == UART_LINE_READY && std::strcmp(line.text, "OK") == 0;
    return parsed ? 0 : 1;
}
