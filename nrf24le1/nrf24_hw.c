#include "nrf24_hw.h"

/*

NRF24L01 hardware functions for NRF24LE1

!!! before trying to receive or transmit data you MUST enable clock
on RF radio by executing following line:

RFCKEN = 1

*/

/* include nrf24le1 register definitions */
#include "nrf24le1.h"

#define NRF24_HW_SPI_WRITE(d) SPIRDAT = (d);
#define NRF24_HW_SPI_READ() SPIRDAT
#define NRF24_HW_SPI_BUSY (!(SPIRSTAT & 0x02U))

/* read&write single byte via dedicated RF SPI interface  */
uint8_t nrf24_spi_rw(uint8_t value)
{
  NRF24_HW_SPI_WRITE(value);
  while(NRF24_HW_SPI_BUSY) continue;
  return NRF24_HW_SPI_READ();
}

void nrf24_ce_low()
{
  RFCE = 0U;
}

void nrf24_ce_high()
{
  RFCE = 1U;
}

void nrf24_csn_low()
{
  RFCSN = 0U;
}

void nrf24_csn_high()
{
  RFCSN = 1U;
}

/* >10us CE HIGH pulse to start transmitting data */

void nrf24_ce_pulse()
{
  uint8_t count = 20;
  nrf24_ce_high();
  while(count--) continue;
  nrf24_ce_low();
}

