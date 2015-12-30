#include "nrf24.h"
#include "nrf24_defines.h"
#include "nrf24_hw.h" /* hardware-specific include */

/* read&write single byte via SPI */
uint8_t nrf24_spi_rw(uint8_t value)
{
	NRF24_HW_SPI_WRITE(value)
  while(NRF24_HW_SPI_BUSY) continue;
	return NRF24_HW_SPI_READ();
}

/** read 8bit nrf24l01 register */
uint8_t nrf24_read_reg(uint8_t reg)
{
	uint8_t val;
	NRF24_HW_CSN_LOW();
	nrf24_spi_rw(R_REGISTER|reg);
	val = nrf24_spi_rw(NOP);
	NRF24_HW_CSN_HIGH();
	return val;
}

/** write to 8bit nrf24l01 register */
void nrf24_write_reg(uint8_t reg, uint8_t val)
{
	NRF24_HW_CSN_LOW();
	nrf24_spi_rw(W_REGISTER|reg);
	val = nrf24_spi_rw(val);
	NRF24_HW_CSN_HIGH();
}

/** read multibyte nrf24l01 register */
void nrf24_read_reg_mb(uint8_t reg, uint8_t *val, uint8_t sz)
{
	NRF24_HW_CSN_LOW();
	nrf24_spi_rw(R_REGISTER|reg);
	while(sz--) {
	  *val = nrf24_spi_rw(NOP);
		val++;
	}
  NRF24_HW_CSN_HIGH();
}

/** write nrf24l01 register */
void nrf24_write_reg_mb(uint8_t reg, uint8_t *val, uint8_t sz)
{
	NRF24_HW_CSN_LOW();
	nrf24_spi_rw(W_REGISTER|reg);
	while(sz--) {
	  nrf24_spi_rw(*val);
		val++;
	}
  NRF24_HW_CSN_HIGH();
}

/** nowait transmit data. standby1 mode expected. cmd is W_TX_PAYLOAD or W_TX_PAYLOAD_NOACK
you can check 
`nrf24_read_reg(FIFO_STATUS) & (1<<TX_EMPTY)`
to see if transmission was completed
*/
void nrf24_tx_transmit(uint8_t cmd, uint8_t *payload, uint8_t payload_sz)
{
  NRF24_HW_CE_LOW();
  /* PRIM_RX = 0 for TX mode */
  nrf24_reg_setbit(CONFIG, 1<<PWR_UP, 1<<PRIM_RX);
    
  /* set TX FIFO */
  NRF24_HW_CSN_LOW();
  nrf24_spi_rw(cmd); /* send W_TX_PAYLOAD or W_TX_PAYLOAD_NOACK */
	/* send payload */
	while(payload_sz--) {
		nrf24_spi_rw(*payload);
		payload++;
	}
  NRF24_HW_CSN_HIGH();

	NRF24_HW_CE_PULSE();
}

/* read payload to data buffer
returns the number of bytes received.
returns 0 if the RX buffer is empty */
uint8_t nrf24_rx_read_payload(uint8_t payload[32])
{
	uint8_t status, pipe_from, payload_sz, i;
	/* check RX payload length */
	NRF24_HW_CSN_LOW();
  status = nrf24_spi_rw(R_RX_PL_WID);
	pipe_from = ((status >> 1) & 0x7);
	payload_sz = nrf24_spi_rw(NOP);
  NRF24_HW_CSN_HIGH();
	
	if (pipe_from > 5 || payload_sz > 32)
		return 0;

	/* read payload */
  NRF24_HW_CSN_LOW();
  nrf24_spi_rw(R_RX_PAYLOAD);
	for(i=0; i < payload_sz; ++i) {
		payload[i] = nrf24_spi_rw(NOP);
	}
	NRF24_HW_CSN_HIGH();

  return payload_sz;
}

/* 1-byte command. returns STATUS register */
uint8_t nrf24_command(uint8_t cmd)
{
	uint8_t status;
  NRF24_HW_CSN_LOW();
  status = nrf24_spi_rw(cmd);
	NRF24_HW_CSN_HIGH();
	return status;
}

/** set/clear bit for 8bit nrf24 register */
void nrf24_reg_setbit(uint8_t reg, uint8_t mask_to_set, uint8_t mask_to_clear)
{
  uint8_t val = nrf24_read_reg(reg);
	val = (val|mask_to_set) & (~(mask_to_clear));
  nrf24_write_reg(reg, val);
}

/** go RX from standby1 mode */
void nrf24_rx_mode()
{
  nrf24_reg_setbit(CONFIG, 1<<PWR_UP | 1<<PRIM_RX, 0);
  NRF24_HW_CE_HIGH();
}

/* got to standby1 mode from power down or rx modes */
void nrf24_standby1()
{
  NRF24_HW_CE_LOW();
  nrf24_reg_setbit(CONFIG, 1<<PWR_UP, 0); /* cfg reg PWR_UP = 1 */
}

/* default nrf24 settings are not the best choice. to make nrf24l01 work
properly
FEATURES, DYNAMIC PAYLOAD needs to be enabled
*/
void nrf24_default_init()
{
	nrf24_gpio_reset();
  nrf24_command(NOP); /* to ensure SPI works correctly */

  /* init in case device is in dirty state */  
  nrf24_command(FLUSH_TX);
  nrf24_command(FLUSH_RX);
  nrf24_write_reg(CONFIG, 0x08);
  nrf24_write_reg(STATUS, 0x0E);
	
	/* enable nessessary features*/
	nrf24_write_reg(FEATURE, 0x07); /*  enable all chip FEATUREs */
  nrf24_write_reg(DYNPD, 0x3F); /* enable dynamic payload for all 6 pipes */
}

