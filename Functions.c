#include "main.h"
#include "Functions.h"

#define SPI_BUSY                0x08
#define UNUSED(x) (void)(x)

volatile uint8_t *spi_tx_buffer;   // Wskaźnik na dane do wysłania
volatile uint32_t spi_tx_size;     // Ilość danych do wysłania
volatile uint32_t spi_tx_index;    // Aktualny indeks wysyłanych danych
volatile uint8_t SPI_FLAGA = 0;    

void st7789_spi_transmit(uint8_t *data, uint32_t size){ 
    spi_tx_buffer = data; 
    spi_tx_size = size; 
    spi_tx_index = 0;
    //Zablokowanie nowych transmisji -> Można je obsługiwać inaczej aby zmniejszyć blokowanie 
    if(SPI_FLAGA & SPI_BUSY){
        while((SPI_FLAGA & SPI_BUSY)); 
        //return;                                           //wyjscie z funkcji i jej przerwanie aktualnego wysyłania. 
    }
    // Ustawienie CS na niski stan przed rozpoczęciem transmisji 
    SPI_NSS_EN; 

    // Zapoczątkowanie transmisji przez wysłanie pierwszego bajtu 
    if (size > 0) { 
       *((uint8_t *)&(SPI1 -> DR)) = spi_tx_buffer[spi_tx_index++]; 
       //SPI1->DR = (uint16_t *)spi_tx_buffer[spi_tx_index++];
        SPI_FLAGA = SPI_BUSY;
    } 

    // Włączenie przerwań od TXE 
    SPI1->CR2 |= SPI_CR2_TXEIE; 
} 


void SPI1_IRQHandler(void) {
    // Check if the TXE flag is set, indicating the transmit buffer is empty
    if (SPI1->SR & SPI_SR_TXE){
        if (spi_tx_index < spi_tx_size) {
            // Transmit the next byte
           *((uint8_t *)&(SPI1 -> DR)) = spi_tx_buffer[spi_tx_index++];
           //SPI1->DR = (uint16_t *)spi_tx_buffer[spi_tx_index++]; 
        } else {
            // Wait for the BSY flag to be cleared, indicating the SPI is not busy
            /* Czekam az wskoczy przerwanie od pustego TXE*/
            if(!(SPI1->SR & SPI_SR_BSY)){
                SPI_NSS_DIS;
                SPI_FLAGA &=~ SPI_BUSY;
                SPI1->CR2 &=~ SPI_CR2_TXEIE;
                uint8_t temp = SPI1->DR;
                temp = SPI1->SR;
                UNUSED(temp);                           //wskazuje, ze zmienna jest świadomie nieużywana. Kompilator nie zgłosi ostrzeżenia o tej zmiennej.
            }

        }
    }
}

void delay(uint32_t ms){
    // Ustaw TIM3 do pracy
    TIM3->CR1 |= TIM_CR1_CEN;

    // Oczekiwanie na zakończenie opóźnienia
    for (uint32_t i = 0; i < ms; ++i) {
        // Czekaj aż TIM3 osiągnie wartość ARR
        while (!(TIM3->SR & TIM_SR_UIF));
        // Wyczyść flagę przepełnienia
        TIM3->SR &= ~TIM_SR_UIF;
    }

    // Wyłącz TIM3
    TIM3->CR1 &= ~TIM_CR1_CEN;
}