NRF24 sdk
=========

low-level library for NRF24L01 suitable for Arduino, STM32, NRF24LE1


/lib - contains cross-platform defines and functions
/nrf24le1 - contains hardware-dependent defines for NRF24LE1(radio+MCU)
/arduino - sample arduino sketch


to get this lib for other platforms, define following hardware-specific functions

```
uint8_t nrf24_spi_rw(uint8_t value);
void nrf24_ce_low();
void nrf24_ce_high();
void nrf24_csn_low();
void nrf24_csn_high();
void nrf24_putc(char x);
void nrf24_ce_pulse();
```

Arduino example:

```
/* read&write single byte via dedicated RF SPI interface  */
uint8_t nrf24_spi_rw(uint8_t value)
{
  return SPI.transfer(value);
}

void nrf24_ce_low()
{
  digitalWrite(nrf24_ce_pin, LOW);
}

void nrf24_ce_high()
{
  digitalWrite(nrf24_ce_pin, HIGH);
}

void nrf24_csn_low()
{
  digitalWrite(nrf24_csn_pin, LOW);
}

void nrf24_csn_high()
{
  digitalWrite(nrf24_csn_pin, HIGH);
}

void nrf24_putc(char x)
{
  Serial.write(x);
}
/* >10us CE HIGH pulse to start transmitting data */

void nrf24_ce_pulse()
{
  nrf24_ce_high();
  delay(1);
  nrf24_ce_low();
}

```

For usage example see the Arduino example and the NRF24L01 datasheet

http://www.nordicsemi.com/eng/content/download/2726/34069/file/nRF24L01P_Product_Specification_1_0.pdf

