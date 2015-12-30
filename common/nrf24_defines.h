/* NRF24L01+ Registers and bit numbers */

/* NRF24L01 REGISTERS */
#define CONFIG        0x00 /* config register */
#define EN_AA         0x01 /* enable Auto-Acknowledge register */
#define EN_RXADDR     0x02 /* enable RX addresses register */
#define SETUP_AW      0x03 /* setup of address width register */
#define SETUP_RETR    0x04 /* setup of automatic retransmission register */
#define RF_CH         0x05 /* RF channel register */
#define RF_SETUP      0x06 /* RF setup register */
#define STATUS        0x07 /* status register */
#define OBSERVE_TX    0x08 /* transmit observe register */
#define RPD           0x09 /* received power detector */
#define RX_ADDR_P0    0x0A /* receive address data pipe0 */
#define RX_ADDR_P1    0x0B /* receive address data pipe1 */
#define RX_ADDR_P2    0x0C /* tail address byte pipe2 */
#define RX_ADDR_P3    0x0D /* tail address byte pipe3 */
#define RX_ADDR_P4    0x0E /* tail address byte pipe4 */
#define RX_ADDR_P5    0x0F /* tail address byte pipe5 */
#define TX_ADDR       0x10 /* transmit address */
#define RX_PW_P0      0x11 /* RX payload width for pipe0 */
#define RX_PW_P1      0x12 /* RX payload width for pipe1 */
#define RX_PW_P2      0x13 /* RX payload width for pipe2 */
#define RX_PW_P3      0x14 /* RX payload width for pipe3 */
#define RX_PW_P4      0x15 /* RX payload width for pipe4 */
#define RX_PW_P5      0x16 /* RX payload width for pipe5 */
#define FIFO_STATUS   0x17 /* FIFO status register */
#define DYNPD         0x1C /* Dynamic payload setup */
#define FEATURE       0x1D /* Exclusive feature setup */

/* CONFIG register bits */
#define MASK_RX_DR  6
#define MASK_TX_DS  5
#define MASK_MAX_RT 4
#define EN_CRC      3
#define CRCO        2
#define PWR_UP      1
#define PRIM_RX     0

/* FIFO_STATUS register bits */
#define TX_REUSE     6
#define TX_FULL_FIFO 5  /* _FIFO suffix due to collision with STATUS */
#define TX_EMPTY     4
#define RX_FULL      1
#define RX_EMPTY     0

/* STATUS register bits */
#define RX_DR 6
#define TX_DS 5
#define MAX_RT 4
#define TX_FULL_STATUS 0  /* _STATUS suffix due to collision with FIFO_STATUS */

/* SPI commands */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define W_TX_PAYLOAD_NOACK 0xB0 /* send */
#define R_RX_PL_WID   0x60 /* read RX payload width */
#define R_RX_PAYLOAD  0x61 /* put RX payload to FIFO */
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1 /* clear TX FIFO */
#define FLUSH_RX      0xE2 /* clear RX FIFO */
#define REUSE_TX_PL   0xE3
#define NOP           0xFF /* No Operation. Might be used to read STATUS reg */

/* FEATURE register bits */
#define EN_DPL       2
#define EN_ACK_PAY   1
#define EN_DYN_ACK   0

