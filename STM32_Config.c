#include "main.h"
#include "STM32_Config.h"
/*  PB3 -  SPI_SCK
    PB4 -  SPI_CS
    PB5 -  SPI_SDA
    PB6 -  RESET
    PB7 -  DC
    PB8 -  BLK
*/
void SysTick_Init(void){
    /* Base Clock 16MHz */
    SysTick -> LOAD = 159999;
	SysTick -> VAL = 0;
	SysTick -> CTRL = (SysTick_CTRL_CLKSOURCE_Msk|
                         SysTick_CTRL_TICKINT_Msk|
	  	                 SysTick_CTRL_ENABLE_Msk);
}

void GPIO_Init(void){
    /* SPI1 PB4 i PB3 */
     RCC->IOPENR |= RCC_IOPENR_GPIOBEN;         //Wlaczenie zegara dla portu B

     GPIOB->MODER &=~ (GPIO_MODER_MODE3|        //RESET
                       GPIO_MODER_MODE4|        //RESET
                       GPIO_MODER_MODE5);       //RESET

     GPIOB->MODER |= (GPIO_MODER_MODE3_1|       //AF
                      GPIO_MODER_MODE4_0|       //Output
                      GPIO_MODER_MODE5_1);      //AF

    GPIOB->OTYPER &=~ (GPIO_OTYPER_OT3 |        //Push-Pull
                       GPIO_OTYPER_OT4 |
                       GPIO_OTYPER_OT5);

    GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED3|   //High-Speed
                      GPIO_OSPEEDR_OSPEED4 |
                      GPIO_OSPEEDR_OSPEED5);

    GPIOB->PUPDR &=~ (GPIO_PUPDR_PUPD3 |       //RESET (no pull-up/down)
                      GPIO_PUPDR_PUPD4 |
                      GPIO_PUPDR_PUPD5);       
    
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPD4_0);      //pull-up
 
    GPIOB->AFR[0] |= ((0x0 << GPIO_AFRL_AFSEL3_Pos) |    //AF0
                     (0x0 << GPIO_AFRL_AFSEL5_Pos));

    /* Inne Porty i Piny */
    GPIOB->MODER &=~ (GPIO_MODER_MODE6 |
                      GPIO_MODER_MODE7 |
                      GPIO_MODER_MODE8);
    GPIOB->MODER |= (GPIO_MODER_MODE6_0 |   //Output
                     GPIO_MODER_MODE7_0 |   //Output
                     GPIO_MODER_MODE8_0);   //Output               
}

void SPI1_Init(void){
    RCC->APBENR2 |= RCC_APBENR2_SPI1EN;     //Wlaczenie zegara dla SPI1

    SPI1->CR1 = 0;                          //Reset CR1
    SPI1->CR2 = 0;                          //Reset CR2

    SPI1->CR1 |= (SPI_CR1_MSTR |            //Master mode
                  SPI_CR1_BR_0 |            //speed control (1MHz)
                  SPI_CR1_BR_1 |
                  SPI_CR1_CPOL |            //CPOL = 1
                  SPI_CR1_CPHA |            //CPAH = 1
                  SPI_CR1_SSM |             //Software Slave menagment
                  SPI_CR1_SSI);
    SPI1->CR2 |= (SPI_CR2_DS_0 |            //8 data bit transfer
                  SPI_CR2_DS_1 | 
                  SPI_CR2_DS_2 |
                  SPI_CR2_TXEIE);           //Przerwanie od flagi TXIE (empty)

    SPI1->CR1 |= SPI_CR1_SPE;               //SPI enable

    NVIC_EnableIRQ(SPI1_IRQn);              //Włączenie przerwania SPI1
    NVIC_SetPriority(SPI1_IRQn, 0);         //Ustawienie priorytetu przerwania
    
}

void Timer_Init(void) {
    // Włącz zegar TIM3
    RCC->APBENR1 |= RCC_APBENR1_TIM3EN;

    // Reset TIM3
    TIM3->CR1 &= ~TIM_CR1_CEN; // Upewnij się, że TIM3 jest zatrzymany
    TIM3->CNT = 0; // Reset licznika

    // Ustawienie preskalera i ARR
    TIM3->PSC = 15; // Preskaler: (16 MHz / (15 + 1) = 1 MHz, czyli 1 µs na krok)
    TIM3->ARR = 999; // ARR dla 1 ms (1 MHz / 1000 = 1 ms)

    // Wyczyść flagę przepełnienia
    TIM3->SR &= ~TIM_SR_UIF;
}