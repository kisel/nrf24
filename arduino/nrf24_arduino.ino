#include "SPI.h"
#include "nrf24.h"
#include "nrf24_debug.h"

/* Implementation hardware-dependent functions for Arduino platform and demo app
 * NRF24 should be powered with 3.3v and connected to SPI (see ICSP connector pinout)
 * connect CE to pin 2, CSN to pin 3
 * CE: pin2
 * CSN: pin3
 * 
 * pin4 is used as a mode selector pin.
 * software pullup is used to act as a receiver(default)
 * connect GND to pin4 to switch mode to tranmitter
 *
 * to compile, copy files library files to the sketch directory
 *
 * */

#include "Arduino.h"
uint8_t nrf24_spi_rw(uint8_t value);
void nrf24_ce_low();
void nrf24_ce_high();
void nrf24_csn_low();
void nrf24_csn_high();
void nrf24_putc(char x);
void nrf24_ce_pulse();
void setup();
void loop();

const int nrf24_ce_pin = 2;
const int nrf24_csn_pin = 3;
const int led_pin = 11;

int mode_pin = 4;
int MODE_RX = HIGH;
int MODE_TX = LOW;


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

void setup()
{
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  SPI.begin();
  Serial.begin(9600);
  pinMode(led_pin, OUTPUT);
  pinMode(nrf24_ce_pin, OUTPUT);
  pinMode(nrf24_csn_pin, OUTPUT);
  pinMode(mode_pin, INPUT);
  digitalWrite(mode_pin, HIGH); /* pullup */
  nrf24_gpio_reset();
}

void loop()
{
  delay(NRF24_POWERUP_INIT_MS);
  Serial.println("NRF24L01 info");
  nrf24_default_init();
  nrf24_standby1();
  delay(NRF24_POWERUP_INIT_MS);

  uint8_t addr[5] = {0xC1, 0xC1, 0xC1, 0xC1, 0xC1};



  char buf[32];
  int bufsz;
  nrf24_print_registers();
  nrf24_print_status();
  for(;;) {
    if (digitalRead(mode_pin) == MODE_RX) {
      digitalWrite(led_pin, LOW);
      nrf24_rx_mode();
      nrf24_write_reg_mb(RX_ADDR_P1, addr, 5);
      bufsz = nrf24_rx_read_payload((uint8_t*)buf);
      Serial.print("received ");
      Serial.print(bufsz);
      Serial.println(" bytes");

      if(bufsz != 0) {
        Serial.write(buf, bufsz);
      }
      Serial.println("\n---");

    } else {
      digitalWrite(led_pin, LOW);
      Serial.println("transmitting hello");
      nrf24_write_reg_mb(TX_ADDR, addr, 5);
      nrf24_write_reg_mb(RX_ADDR_P0, addr, 5);

      nrf24_tx_transmit(W_TX_PAYLOAD, (uint8_t*)"hello", 5);
      digitalWrite(led_pin, HIGH);
      while(!nrf24_tx_empty()) delay(10); /* wait tx */
    }
    nrf24_print_registers();
    nrf24_print_status();
    nrf24_clear_interrupts();
    delay(1000);
  }
}


