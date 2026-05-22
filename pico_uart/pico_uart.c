#include <pico/stdio_usb.h>
#include <pico/time.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

// UART defines
// By default the stdout UART is `uart0`, so we will use the second one
#define UART_ID uart0
#define BAUD_RATE 115200

// Use pins 4 and 5 for UART1
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define UART_TX_PIN 0
#define UART_RX_PIN 1

void uart_read_string(char *buf, size_t max_len) {
    size_t i = 0;
    while (i < max_len - 1) {
        // Block until a character is available
        char c = uart_getc(UART_ID);
        // Treat newline or carriage return as end of string
        if (c == '\n' || c == '\r') {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';  // Null-terminate the string
}

int main() {
    // Initialize chosen serial port (USB CDC for printf)
    stdio_init_all();

    // Initialize UART hardware
    uart_init(UART_ID, BAUD_RATE);

    // Set the GPIO pin mux to the UART function
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    char buffer[1000];

    while(!stdio_usb_connected());

    sleep_ms(10);

    char send_string[1000]; 
    sprintf(send_string, "hello world %d\r\n\0", 550);
    uart_puts(uart0, send_string); 

    while (true) {
        sleep_ms(100);
        uart_puts(uart0, send_string); 
    }

    return 0;
}