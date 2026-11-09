#include "uart_line.h"

enum uart_line_result uart_line_feed(struct uart_line *line, uint8_t byte) {
    if (byte != '\n') {
        if (byte == '\0' || line->length == sizeof(line->text) - 1) {
            line->dropping = true;
        }
        if (!line->dropping) {
            line->text[line->length++] = (char)byte;
        }
        return UART_LINE_PENDING;
    }

    size_t length = line->length;
    if (length > 0 && line->text[length - 1] == '\r') {
        --length;
    }

    bool invalid = line->dropping || length > UART_LINE_MAX_BYTES;
    line->length = 0;
    line->dropping = false;
    line->text[invalid ? 0 : length] = '\0';
    return invalid ? UART_LINE_DROPPED : UART_LINE_READY;
}
