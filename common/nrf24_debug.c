#include "nrf24_debug.h"
#include "nrf24_hw.h"

void nrf24_print_hex(uint8_t val)
{
  char charmap[16] = "0123456789ABCDEF";
  nrf24_putc(charmap[((val & 0xF0) >> 4)]);
  nrf24_putc(charmap[(val & 0xF)]);
}

void nrf24_print_cstr(char *str)
{
  while(*str != 0) {
    nrf24_putc(*str);
    str++;
  }
}

void nrf24_print_named_hex_val(char *name, uint8_t *payload, unsigned sz)
{
  nrf24_print_cstr(name);
  nrf24_putc(':');
  nrf24_putc(' ');
  while(sz--) {
    uint8_t val = *payload;
    nrf24_print_hex(val);
    if (sz) nrf24_putc(' ');
    payload++;
  }
  nrf24_putc('\r');
  nrf24_putc('\n');
}

void nrf24_print_register(uint8_t reg, char *name, unsigned sz)
{
  char val[32];
  nrf24_read_reg_mb(reg, val, sz);
  nrf24_print_named_hex_val(name, val, sz);
}

#define PRINT_REG_MB(reg, sz) nrf24_print_register(reg, #reg, sz)
#define PRINT_REG(reg) nrf24_print_register(reg, #reg, 1)
#define PRINT_REG_BIT(bit_name, reg_val, bit_idx) do { \
  nrf24_print_cstr(" " bit_name ": "); \
  nrf24_putc(((reg_val&(1<<bit_idx)) ? '1' : '0')); \
  nrf24_putc('\r'); \
  nrf24_putc('\n'); \
} while(0)

void nrf24_print_registers()
{
  PRINT_REG(CONFIG);
  PRINT_REG(STATUS);
  PRINT_REG(FIFO_STATUS);
  PRINT_REG(OBSERVE_TX);
  PRINT_REG(EN_AA);
  PRINT_REG(RF_CH);
  PRINT_REG(DYNPD);
  PRINT_REG(FEATURE);

  PRINT_REG_MB(TX_ADDR, 5);
  PRINT_REG_MB(RX_ADDR_P0, 5);
  PRINT_REG_MB(RX_ADDR_P1, 5);
  PRINT_REG(RX_ADDR_P2);
  PRINT_REG(RX_ADDR_P3);
  PRINT_REG(RX_ADDR_P4);
  PRINT_REG(RX_ADDR_P5);
}

void nrf24_print_status()
{
  uint8_t status = nrf24_read_reg(STATUS);
  PRINT_REG(STATUS);
  PRINT_REG_BIT("TX_FULL", status, TX_FULL_STATUS);
  PRINT_REG_BIT("MAX_RT", status, MAX_RT);
  PRINT_REG_BIT("TX_DS", status, TX_DS);
  PRINT_REG_BIT("RX_DR", status, RX_DR);
}

void nrf24_print_fifo_status()
{
  uint8_t fifo_status = nrf24_read_reg(FIFO_STATUS);
  PRINT_REG(FIFO_STATUS);
  PRINT_REG_BIT("RX_EMPTY", fifo_status, RX_EMPTY);
  PRINT_REG_BIT("RX_FULL", fifo_status, RX_FULL);
  PRINT_REG_BIT("TX_EMPTY", fifo_status, TX_EMPTY);
  PRINT_REG_BIT("TX_FULL", fifo_status, TX_FULL_FIFO);
}

/* partial reset. restores original state of some registers.
   mostly used as a reference for initial setup funcition
   and for debugging purposes and testing invalid chips */
#if 0
void nrf24_debug_reset()
{
  /* no constants here for flexibility. see datasheet for details */
  nrf24_command(NOP); /* to ensure SPI works correctly */
  nrf24_command(FLUSH_TX);
  nrf24_command(FLUSH_RX);
  nrf24_write_reg(CONFIG, 0x08);
  nrf24_write_reg(STATUS, 0x0E);
  nrf24_write_reg(DYNPD, 0);
  nrf24_write_reg(FEATURE, 0);
  nrf24_write_reg(EN_AA, 0x3f); /* 6 pipes mask */
  nrf24_write_reg(EN_RXADDR, 0x03); /* ERX_P0|ERX_P1 */
  nrf24_write_reg(SETUP_AW, 0x03); /* 5 bytes addr */
  nrf24_write_reg(SETUP_RETR, 0x03); /* delay:250us 3 retransmits */
  nrf24_write_reg(RF_CH, 0x02);
  nrf24_write_reg(RF_SETUP, 0x0E); /* 2Mbps, 0dBm TX (max power) */
}
#endif

