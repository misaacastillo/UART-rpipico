#ifndef UART_LINE_H
#define UART_LINE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define UART_LINE_MAX_BYTES 99

enum uart_line_result {
    UART_LINE_PENDING,
    UART_LINE_READY,
    UART_LINE_DROPPED,
};

struct uart_line {
    char text[UART_LINE_MAX_BYTES + 2]; /* Optional CR and NUL. */
    size_t length;
    bool dropping;
};

#ifdef __cplusplus
extern "C" {
#endif

/* Start with zero-initialized state. Read text after READY and before the next call. */
enum uart_line_result uart_line_feed(struct uart_line *line, uint8_t byte);

#ifdef __cplusplus
}
#endif

#endif
