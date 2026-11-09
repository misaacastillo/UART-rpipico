#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "uart_line.h"

static void feed_text(struct uart_line *line, const char *text) {
    while (*text != '\0') {
        enum uart_line_result result = uart_line_feed(line, (uint8_t)*text++);
        assert(result == UART_LINE_PENDING);
    }
}

static void test_fragmented_line_and_immediate_next_line(void) {
    struct uart_line line = {0};
    feed_text(&line, "hel");
    /* The producer may stop here and the UART FIFO may become empty. */
    feed_text(&line, "lo");
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, "hello") == 0);
    feed_text(&line, "next");
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, "next") == 0);
}

static void test_empty_and_crlf(void) {
    struct uart_line line = {0};
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, "") == 0);
    feed_text(&line, "hello\r");
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, "hello") == 0);
}

static void test_capacity(void) {
    struct uart_line line = {0};
    char text[UART_LINE_MAX_BYTES + 1];
    memset(text, 'x', UART_LINE_MAX_BYTES);
    text[UART_LINE_MAX_BYTES] = '\0';

    feed_text(&line, text);
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, text) == 0);

    feed_text(&line, text);
    feed_text(&line, "\r");
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, text) == 0);

    feed_text(&line, text);
    feed_text(&line, "x");
    assert(uart_line_feed(&line, '\n') == UART_LINE_DROPPED);

    feed_text(&line, text);
    feed_text(&line, "x\rextra");
    assert(uart_line_feed(&line, '\n') == UART_LINE_DROPPED);

    feed_text(&line, "ok");
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, "ok") == 0);
}

static void test_embedded_nul_and_recovery(void) {
    struct uart_line line = {0};
    feed_text(&line, "before");
    assert(uart_line_feed(&line, 0) == UART_LINE_PENDING);
    feed_text(&line, "after");
    assert(uart_line_feed(&line, '\n') == UART_LINE_DROPPED);
    assert(line.text[0] == '\0');
    feed_text(&line, "valid");
    assert(uart_line_feed(&line, '\n') == UART_LINE_READY);
    assert(strcmp(line.text, "valid") == 0);
}

int main(void) {
    test_fragmented_line_and_immediate_next_line();
    test_empty_and_crlf();
    test_capacity();
    test_embedded_nul_and_recovery();
    puts("4 UART line framing tests passed");
}
