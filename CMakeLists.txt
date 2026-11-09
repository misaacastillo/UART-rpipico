cmake_minimum_required(VERSION 3.17)

if(NOT DEFINED ENV{PICO_SDK_PATH})
    message(FATAL_ERROR "Set PICO_SDK_PATH to the Raspberry Pi Pico SDK checkout")
endif()
include("$ENV{PICO_SDK_PATH}/pico_sdk_init.cmake")

project(pico_uart_examples C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
pico_sdk_init()

add_executable(uart_tx example/hello_uart/tx.c)
target_link_libraries(uart_tx PRIVATE pico_stdlib hardware_uart)
pico_enable_stdio_uart(uart_tx 0)
pico_enable_stdio_usb(uart_tx 0)
pico_add_extra_outputs(uart_tx)

add_executable(uart_rx example/hello_uart/rx.c example/hello_uart/uart_line.c)
target_link_libraries(uart_rx PRIVATE pico_stdlib hardware_uart)
pico_enable_stdio_uart(uart_rx 0)
pico_enable_stdio_usb(uart_rx 1)
pico_add_extra_outputs(uart_rx)
