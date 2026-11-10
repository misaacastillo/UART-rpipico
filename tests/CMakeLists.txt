cmake_minimum_required(VERSION 3.17)
project(uart_rpipico_host_tests LANGUAGES C CXX)

enable_testing()
add_subdirectory(../example/hello_uart uart_line)

add_executable(test_uart_line test_uart_line.c)
target_link_libraries(test_uart_line PRIVATE uart_rpipico::line)
add_test(NAME uart_line_framing COMMAND test_uart_line)

add_executable(test_cpp_consumer test_cpp_consumer.cpp)
target_link_libraries(test_cpp_consumer PRIVATE uart_rpipico::line)
target_compile_features(test_cpp_consumer PRIVATE cxx_std_11)
add_test(NAME uart_line_cpp_consumer COMMAND test_cpp_consumer)
