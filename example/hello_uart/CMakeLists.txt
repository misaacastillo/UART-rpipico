add_library(uart_rpipico_line STATIC uart_line.c)
add_library(uart_rpipico::line ALIAS uart_rpipico_line)
target_include_directories(uart_rpipico_line PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(uart_rpipico_line PUBLIC c_std_11)
