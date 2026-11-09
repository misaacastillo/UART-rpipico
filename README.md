# UART line receiver for Raspberry Pi Pico

Send newline-delimited messages between two Raspberry Pi Pico boards and view
them over USB on the receiving board. The receiver keeps a partial message
until the newline arrives, even when the UART FIFO goes empty between bytes.

The C11 line reader is independent of the Pico SDK and uses a fixed-size buffer.
It accepts LF and CRLF, handles empty lines, and discards lines with embedded
NUL bytes or more than 99 payload bytes. It resumes at the next line.

## Build

Use CMake 3.17 or newer, Ninja, Python 3, and an Arm GNU toolchain with
`arm-none-eabi-gcc`, its C++ compiler, and newlib. Clone the Pico SDK 2.2.0
checkout and its TinyUSB submodule in a directory next to this repository.

```sh
git clone --branch 2.2.0 --depth 1 https://github.com/raspberrypi/pico-sdk.git ../pico-sdk-2.2.0
git -C ../pico-sdk-2.2.0 submodule update --init --depth 1 lib/tinyusb
export PICO_SDK_PATH="$(cd ../pico-sdk-2.2.0 && pwd)"
cmake -S . -B build-pico -G Ninja -DPICO_BOARD=pico
cmake --build build-pico
```

The SDK can fetch and build picotool on the first build to create the UF2 files.
The outputs are `build-pico/uart_tx.uf2` and `build-pico/uart_rx.uf2`.
For Pico W use `-DPICO_BOARD=pico_w` and a separate build directory.

## Run on two boards

Put each board into BOOTSEL mode and copy the corresponding UF2 to its USB
drive. With both boards unplugged, connect the following signals.

| Transmitter | Receiver |
| --- | --- |
| GP0, physical pin 1, UART0 TX | GP1, physical pin 2, UART0 RX |
| GND, physical pin 3 | GND, physical pin 3 |

Power each board by USB. Use 3.3 V logic and leave the boards' power rails
separate. Open the receiver's USB CDC serial port with a terminal such as
`picocom -b 115200 /dev/ttyACM0` on Linux. On macOS the device typically appears
under `/dev/cu.usbmodem*`. Expect `Hello from Pico A!` once per second.

UART0 uses 115200 baud, 8 data bits, no parity, and 1 stop bit. The receiver
reports over USB CDC so its logging does not occupy UART0. The polling example
targets short, low-rate messages. Continuous traffic can require an
interrupt-driven or DMA receiver and a separate queue for USB output.

## Run tests without hardware

```sh
mkdir -p build-host
cc -std=c11 -Wall -Wextra -Werror -pedantic -Iexample/hello_uart \
  example/hello_uart/uart_line.c tests/test_uart_line.c -o build-host/test_uart_line
./build-host/test_uart_line
```

The tests cover fragmented input, back-to-back lines, empty lines, LF and CRLF,
the buffer boundary, oversized input, embedded NUL bytes, and recovery. These
tests exercise line parsing. The two-board run above checks wiring and USB output.

## Reuse the parser

Copy or compile `uart_line.c` and include `uart_line.h`. Initialize a separate
`struct uart_line` to zero for each byte stream. Feed one byte at a time. Read
`line.text` after `UART_LINE_READY` and before the next call. A
`UART_LINE_DROPPED` result means the parser discarded the line and is ready for
the next one.

Report build errors or bench results with the board, SDK version, and a short
reproduction. See the [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk)
for installation and board support.

This project is available under the [MIT License](LICENSE).
