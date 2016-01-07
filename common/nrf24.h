/* NRF24L01+ SDK
 *
 * Minimalistic portable SDK for NRF24L01+
 * 
 * GOALS: 
 * - KISS API, low level, as close to the datasheet, as possible
 * - minimal dependencies, platform portability
 * - small compiled size
 *
 * DEPENDENCIES: you need to implement functions from nrf24_hw.h
 *
 *
 * Author: Anton Kiselev(anton.kisel@gmail.com)
 * 
 * requires following platform-dependent functions defined in nrf24l01_hw.h header
 * nrf24_ce_set, nrf24_ce_clear, nrf24_spi_select, nrf24_spi_unselect, nrf24_spi_send
 *
 *
 * Implementation notes:
 * You don't need another abstraction, since you'll have to read the Datasheet anyway,
 * unless you're trying trivial Arduino example, so why waste time learning yet another API
 *
 * */

#ifndef NRF24_DEFINED
#define NRF24_DEFINED

#include <stdint.h>
#include "nrf24_defines.h"

/* send & receive 1 byte via SPI */
uint8_t nrf24_spi_rw(uint8_t value);

/* NRF24L01 radio GPIO pins */
void nrf24_ce_low();
void nrf24_ce_high();
void nrf24_csn_low();
void nrf24_csn_high();

/* read 8bit nrf24l01 register */
uint8_t nrf24_read_reg(uint8_t reg);

/* write to 8bit nrf24l01 register */
void nrf24_write_reg(uint8_t reg, uint8_t val);

/* read multibyte nrf24l01 register */
void nrf24_read_reg_mb(uint8_t reg, uint8_t *val, uint8_t sz);

/* write nrf24l01 register */
void nrf24_write_reg_mb(uint8_t reg, uint8_t *val, uint8_t sz);

/* set/clear bit for 8bit nrf24 register */
void nrf24_reg_setbit(uint8_t reg, uint8_t mask_to_set, uint8_t mask_to_clear);

/* initalize CE, CSN GPIO properly.
required if there are multiple SPI devices on the same bus */
#define nrf24_gpio_reset() do { nrf24_ce_low();	nrf24_csn_high(); } while(0)

/* got to standby1 mode from power down or rx modes */
void nrf24_standby1();

/* RX_ADDR_P1=addr. RX_ADDR_P1 will be used as a primary receive pipe */
#define nrf24_rx_set_addr(addr, addr_sz) do{ \
	nrf24_write_reg(RX_ADDR_P1, addr, addr_sz);\
}while(0)

/** go RX from standby1 mode */
void nrf24_rx_mode();

/* read payload to data buffer
returns the number of bytes received.
returns 0 if the RX buffer is empty */
uint8_t nrf24_rx_read_payload(uint8_t payload[32]);

/* 1-byte command. returns STATUS register */
uint8_t nrf24_command(uint8_t cmd);

/* nowait transmit data. standby1 mode expected. cmd is W_TX_PAYLOAD or W_TX_PAYLOAD_NOACK
you can check nrf24_tx_empty to see if transmission was completed
this function does NOT set TX_ADDR, RX_ADDR_P0 - you need to set them yourself
to receive ACK you MUST set RX_ADDR_P0=TX_ADDR
*/
void nrf24_tx_transmit(uint8_t cmd, uint8_t *payload, uint8_t payload_sz);

/* go RX from standby1 mode */
#define nrf24_tx_empty() (nrf24_read_reg(FIFO_STATUS) & (1<<TX_EMPTY))

/* RX_ADDR_P0=TX_ADDR=addr; RX_ADDR_P0 will be used to receive ACK packet */
#define nrf24_tx_set_addr(addr, addr_sz) do{ \
  nrf24_write_reg(TX_ADDR, addr, addr_sz);\
	nrf24_write_reg(RX_ADDR_P0, addr, addr_sz);\
}while(0)

/* sets TX_ADDR=addr; RX_ADDR_P0=addr and starts payload transmission. see nrf24_transmit(W_TX_PAYLOAD) */
#define nrf24_tx_transmit_ack(payload, sz) \
  nrf24_tx_transmit(W_TX_PAYLOAD, payload, sz)

/* see nrf24_transmit(W_TX_PAYLOAD_NOACK) */
#define nrf24_tx_transmit_noack(payload, sz) \
  nrf24_tx_transmit(W_TX_PAYLOAD_NOACK, payload, sz)

/* default nrf24 settings are not the best choice. to make nrf24l01 work
properly
FEATURES, DYNAMIC PAYLOAD needs to be enabled
*/
void nrf24_default_init();

/* reset interrupt bits in status */
#define nrf24_clear_interrupts() \
  nrf24_reg_setbit(STATUS, (1<<MAX_RT)|(1<<TX_DS)|(1<<RX_DR), 0);

// NRF24L01 Timings
#define NRF24_POWERUP_INIT_MS 100 /* there is a 100ms delay after VCC is up */
#define NRF24_SETTLING_US 130 /* settling delay for RX/TX */
#define NRF24_TX_PULSE_US 10 /* 10us CE HIGH pulse to start TX */

#endif //NRF24_DEFINED
