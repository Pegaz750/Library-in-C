#include "main.h"
#include "STM32_Config.h"
#include "Functions.h"
#include "ST7789_new.h"
#include "fonts.h"


volatile uint8_t licznik_10ms = 0;                  //SysTick

int main(void)
{
    GPIO_Init();
    Timer_Init();
    SPI1_Init();
    ST7789_Init(ST7789_RES_170x320, ST7789_ROTATION_90);
    ST7789_Fill_Color(BROWN);
	  delay(1000);
    ST7789_Fill_Color(RED);
    delay(1000);
    ST7789_Fill_Color(YELLOW);
    delay(1000);
    ST7789_Fill_Color(MAGENTA);
    //ST7789_Write();
    //ST7789_WriteString(10, 20, "Hello There", Font_11x18, YELLOW, BLACK);
    SysTick_Init();

    //ST7789_Test();
    

	do{ 
		do{
			//czynności asynchroniczne jesli potrzebene
		}while(licznik_10ms < 1); //czekanie na dopełnienie do 10ms
		licznik_10ms = 0;
	}while(1);
}

void SysTick_Handler(void)
{
	licznik_10ms = 1;
}


void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
