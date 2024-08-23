#ifndef __FUNCTIONS_H
#define __FUNCTIONS_H

#define SPI_NSS_EN         (GPIOB->BRR |= GPIO_BRR_BR4)
#define SPI_NSS_DIS        (GPIOB->BSRR |= GPIO_BSRR_BS4)

void delay(uint32_t delay_ms);
void st7789_spi_transmit(uint8_t *data, uint32_t size);

#endif