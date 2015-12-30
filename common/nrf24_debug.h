/*
 *
 * NRF24L01 debugging library
 * ===========================
 *
 * This is an extension to NRF24 library. You do not need it in release build
 * The only requirement for these functions is nrf24_putc function
 *
 * DEPENDENCY: void nrf24_putc(char ch); - puts character to the source you've chosen.
 * It can be UART, or some other debugging interface
 *
 * Author: Anton Kiselev(anton.kisel@gmail.com)
*/

#ifndef NRF24L01_DEBUG_H_
#define NRF24L01_DEBUG_H_

#include "nrf24.h"

/* you need to define this function(or override it with macro in nrf24_hw.h) */
void nrf24_putc(char ch);

/* prints hex byte */
void nrf24_print_hex(uint8_t val);

/* helper function. prints 0-terminated string */
void nrf24_print_cstr(char *str);
	
/* prints "#{name}: #{payload.to_hex}" */
void nrf24_print_named_hex_val(char *name, uint8_t *payload, unsigned sz);

/* print most useful registers */
void nrf24_print_registers();

/** print status register content and bits */
void nrf24_print_status();

/** rx/tx FIFO status and bits */
void nrf24_print_fifo_status();

/* print register contents. prints name as well*/
void nrf24_print_register(uint8_t reg, char *name, unsigned sz);

#endif
