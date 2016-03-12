#ifndef NRF24_HW_H_
#define NRF24_HW_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* NRF24L01 hardware dependencies
you need to define these functions inside your project
and the linker will pick them up.

*/

/* send & receive 1 byte via SPI */
uint8_t nrf24_spi_rw(uint8_t value);

/* NRF24L01 radio GPIO pins */
void nrf24_ce_low();
void nrf24_ce_high();
void nrf24_csn_low();
void nrf24_csn_high();

/* 10us CE HIGH pulse to start transmitting data
implementation can look like:
  nrf24_ce_high();
  delay_us(10);
  nrf24_ce_low();
*/
void nrf24_ce_pulse();

#ifdef __cplusplus
}
#endif

#endif

