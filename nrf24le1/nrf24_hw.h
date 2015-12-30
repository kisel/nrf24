#ifndef HAL_NRFL01_HW_H_
#define HAL_NRFL01_HW_H_

/* NRF24L01 hardware dependencies for NRF24LE1 chip

before trying to receive or transmit data you MUST call NRF24_HW_RADIO_ENABLE
without that little sneaky bastard(NRF24_HW_RADIO_ENABLE) radio will not work

*/

/* include nrf24le1 register definitions */
#include "nrf24le1.h"

#define NRF24_HW_RADIO_ENABLE RFCKEN = 1 /* enables clock for RF module */
#define NRF24_HW_RADIO_DISABLE RFCKEN = 0 /* disable clock for RF module */

#define NRF24_HW_SPI_WRITE(d) SPIRDAT = (d);
#define NRF24_HW_SPI_READ() SPIRDAT
#define NRF24_HW_SPI_BUSY (!(SPIRSTAT & 0x02U))

#define NRF24_HW_CSN_LOW() RFCSN = 0U
#define NRF24_HW_CSN_HIGH() RFCSN = 1U

#define NRF24_HW_CE_LOW() RFCE = 0U
#define NRF24_HW_CE_HIGH() RFCE = 1U

/* pulse > 10us */
#define NRF24_HW_CE_PULSE() \
  do { \
  uint8_t count; \
  count = 20U; \
  NRF24_HW_CE_HIGH();  \
    while(count--) {} \
  NRF24_HW_CE_LOW();  \
  } while(0)
	
#endif

