/*FUNCTION st7789_spi_transmit is in Functions.c */

#include "main.h"
#include "ST7789_new.h"
#include "Functions.h"
#include "fonts.h"

#define LCD_RST1 			GPIOB->BSRR|=GPIO_BSRR_BS6
#define LCD_RST0 			GPIOB->BRR|=GPIO_BRR_BR6

#define	LCD_DC1				GPIOB->BSRR|=GPIO_BSRR_BS7
#define	LCD_DC0 			GPIOB->BRR|=GPIO_BRR_BR7

#define	LCD_BLK1 			GPIOB->BSRR|=GPIO_BSRR_BS8
#define	LCD_BLK0 			GPIOB->BRR|=GPIO_BRR_BR8

#define	LCD_CS1				GPIOB->BSRR|=GPIO_BSRR_BS1
#define	LCD_CS0 			GPIOB->BRR|=GPIO_BRR_BR1

/*RESOLUTION STRUCT*/
typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t X_SHIFT;
    uint8_t Y_SHIFT;
}ST7789_Resolution;

static ST7789_Resolution current_resolution;

static void ST7789_WriteCommand(uint8_t cmd)
{

	LCD_DC0;
	st7789_spi_transmit(&cmd,1);
}

static void ST7789_WriteData(uint8_t *buff, uint32_t buff_size)
{
	LCD_DC1;
	st7789_spi_transmit(buff,buff_size);

}

static void ST7789_WriteSmallData(uint8_t data)
{

	LCD_DC1;
	st7789_spi_transmit(&data,1);

}

void ST7789_Init(uint8_t display_size, uint8_t rotation){
    /*DISPLAY RESET*/
    LCD_BLK1;
	LCD_RST0;
	delay(50);
    LCD_RST1;
    delay(50);
    ST7789_WriteCommand(ST7789_SWRESET);
    delay(140);
    /*CONFIG*/
    /*COLORMODE*/
    ST7789_WriteCommand(ST7789_COLMOD);                 //COLORMODE
    ST7789_WriteSmallData(ST7789_COLOR_MODE_16bit);     //16-bit/pixel (RGB565)
    /*PORCH CONTROL*/
    ST7789_WriteCommand(0xB2);				            //Porch control
	uint8_t data[] = {0x0C, 0x0C, 0x00, 0x33, 0x33};
	ST7789_WriteData(data, sizeof(data));
    /*Internal LCD Voltage generator settings*/
    ST7789_WriteCommand(0xB7);                          //Gate Control
    ST7789_WriteSmallData(0x35);                        //Domyślna wartość
    ST7789_WriteCommand(0xBB);                          //VCOM setting
    ST7789_WriteSmallData(0x20);                        //0.725V (domyślnie 0.75V dla 0x20)
    ST7789_WriteCommand(0xC0);                          //LCMCTRL
    ST7789_WriteSmallData(0x2C);                        //Domyślna wartość
    ST7789_WriteCommand(0xC2);                          //VDV and VRH command Enable
    ST7789_WriteSmallData(0x01);                        //Domyślna wartość
    ST7789_WriteCommand(0xC3);                          //VRH set
    ST7789_WriteSmallData(0x0B);                        //+-4.45V (domyślnie +-4.1V dla 0x0B)
    ST7789_WriteCommand(0xC4);                          //VDV set
    ST7789_WriteSmallData(0x20);                        //Domyślna wartość
    ST7789_WriteCommand(0xC6);                          //Frame rate control in normal mode
    ST7789_WriteSmallData(0x0F);                        //Domyślna wartość (60Hz)
    /*POWER CONTROL*/
    ST7789_WriteCommand(0xD0);                          //Power control
    ST7789_WriteSmallData(0xA4);                        //Domyślna wartość
    ST7789_WriteSmallData(0xA1);                        //Domyślna wartość
    /*RESOLUTION*/
    switch (display_size) {
        case ST7789_RES_135x240:
            current_resolution.width = 135;
            current_resolution.height = 240;
            ST7789_WriteCommand(ST7789_CASET);                                      //Column Address Set
            uint8_t column_addr_135x240[] = {0x00, 0x00, 0x00, 0xEF};               //135 columns
            ST7789_WriteData(column_addr_135x240, sizeof(column_addr_135x240));
            ST7789_WriteCommand(ST7789_RASET);                                      //Row Address Set
            uint8_t row_addr_135x240[] = {0x00, 0x00, 0x01, 0x3F};                  //240 rows
            ST7789_WriteData(row_addr_135x240, sizeof(row_addr_135x240));
            break;
        case ST7789_RES_240x240:
            current_resolution.width = 240;
            current_resolution.height = 240;
            ST7789_WriteCommand(ST7789_CASET);                                      //Column Address Set
            uint8_t column_addr_240x240[] = {0x00, 0x00, 0x00, 0xEF};               //240 columns
            ST7789_WriteData(column_addr_240x240, sizeof(column_addr_240x240));
            ST7789_WriteCommand(ST7789_RASET);                                      //Row Address Set
            uint8_t row_addr_240x240[] = {0x00, 0x00, 0x00, 0xEF};                  //240 rows
            ST7789_WriteData(row_addr_240x240, sizeof(row_addr_240x240));
            break;
        case ST7789_RES_170x320:
            current_resolution.width = 170;
            current_resolution.height = 320;
            ST7789_WriteCommand(ST7789_CASET);                                      //Column Address Set
            uint8_t column_addr_170x320[] = {0x00, 0x00, 0x00, 0xA1};               //170 columns
            ST7789_WriteData(column_addr_170x320, sizeof(column_addr_170x320));
            ST7789_WriteCommand(ST7789_RASET);                                      //Row Address Set
            uint8_t row_addr_170x320[] = {0x00, 0x00, 0x01, 0x3F};                  //320 rows
            ST7789_WriteData(row_addr_170x320, sizeof(row_addr_170x320));
            break;
    }
    /*GAMMA SETTING*/
     ST7789_WriteCommand(0xE0);                         //Gamma curve selected
    uint8_t gamma_data[] = {0xD0, 0x04, 0x0D, 0x11, 0x13, 0x2B, 0x3F, 0x54, 0x4C, 0x18, 0x0D, 0x0B, 0x1F, 0x23};
    ST7789_WriteData(gamma_data, sizeof(gamma_data));
    ST7789_WriteCommand(0xE1);                          //Gamma curve adjustment
    uint8_t gamma_adjustment[] = {0xD0, 0x04, 0x0C, 0x11, 0x13, 0x2C, 0x3F, 0x44, 0x51, 0x2F, 0x1F, 0x1F, 0x20, 0x23};
    ST7789_WriteData(gamma_adjustment, sizeof(gamma_adjustment));
    /*ROTATION*/
    uint8_t madctl_data = 0x00;
    uint16_t temp_width;
    ST7789_WriteCommand(ST7789_MADCTL);                 //Ustawienie kontrolera dostępu do pamięci
    switch (rotation) {
        case ST7789_ROTATION_0:
            if(display_size & ST7789_RES_135x240){
                current_resolution.X_SHIFT = 53;
                current_resolution.Y_SHIFT = 40;
            }
            else if(display_size & ST7789_RES_240x240){
                current_resolution.X_SHIFT = 0;
                current_resolution.Y_SHIFT = 80;
            }
            else if(display_size & ST7789_RES_170x320){
                current_resolution.X_SHIFT = 35;
                current_resolution.Y_SHIFT = 0;
            }

            madctl_data = (ST7789_MADCTL_MX | ST7789_MADCTL_MY | ST7789_MADCTL_RGB);   //0° rotation
            break;
        case ST7789_ROTATION_90:
            if(display_size & ST7789_RES_135x240){
                current_resolution.X_SHIFT = 40;
                current_resolution.Y_SHIFT = 52;
            }
            else if(display_size & ST7789_RES_240x240){
                current_resolution.X_SHIFT = 80;
                current_resolution.Y_SHIFT = 0;
            }
            else if(display_size & ST7789_RES_170x320){
                current_resolution.X_SHIFT = 0; //0
                current_resolution.Y_SHIFT = 35; //35
            }

            temp_width = current_resolution.width;
            current_resolution.width = current_resolution.height;
            current_resolution.height = temp_width;
            madctl_data = (ST7789_MADCTL_MY | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);   //90° rotation
            break;
        case ST7789_ROTATION_180:
            if(display_size & ST7789_RES_135x240){
                current_resolution.X_SHIFT = 52;
                current_resolution.Y_SHIFT = 40;
            }
            else if(display_size & ST7789_RES_240x240){
                current_resolution.X_SHIFT = 0;
                current_resolution.Y_SHIFT = 0;
            }
            else if(display_size & ST7789_RES_170x320){
                current_resolution.X_SHIFT = 35;
                current_resolution.Y_SHIFT = 0;
            }
            madctl_data =  (ST7789_MADCTL_MY | ST7789_MADCTL_MX | ST7789_MADCTL_RGB);                                         //180° rotation
            break;
        case ST7789_ROTATION_270:
            if(display_size & ST7789_RES_135x240){
                current_resolution.X_SHIFT = 40;
                current_resolution.Y_SHIFT = 53;
            }
            else if(display_size & ST7789_RES_240x240){
                current_resolution.X_SHIFT = 0;
                current_resolution.Y_SHIFT = 0;
            }
            else if(display_size & ST7789_RES_170x320){
                current_resolution.X_SHIFT = 35;
                current_resolution.Y_SHIFT = 0;
            }

            temp_width = current_resolution.width;
            current_resolution.width = current_resolution.height;
            current_resolution.height = temp_width;
            madctl_data = (ST7789_MADCTL_MX | ST7789_MADCTL_MV | ST7789_MADCTL_RGB);   //270° rotation
            break;
    }
    ST7789_WriteSmallData(madctl_data);
    /*END COMMANDS*/
    ST7789_WriteCommand(ST7789_INVON);                  //Inversion ON
    ST7789_WriteCommand(ST7789_SLPOUT);                 //Out of sleep mode
    ST7789_WriteCommand(ST7789_NORON);                  //Normal Display on
    ST7789_Fill_Color(BLACK);                           //Wypełnienie kolorem czarnym
    ST7789_WriteCommand(ST7789_DISPON);                 //Główne włączenie wyświetlacza
}


// void ST7789_Fill_Color(uint16_t color){
//     ST7789_SetAddressWindow(0, 0, current_resolution.width - 1, current_resolution.height - 1);
//     uint16_t i;
// 	for (i = 0; i < current_resolution.width; i++){
// 	    for (uint16_t j = 0; j < current_resolution.height; j++) {
// 			uint8_t data[] = {color >> 8, color & 0xFF};
// 			ST7789_WriteData(data, sizeof(data));
// 		}
//     }
// }


void ST7789_Fill_Color(uint16_t color) {
    ST7789_SetAddressWindow(0, 0, current_resolution.width - 1, current_resolution.height - 1);
    
    // Sprawdzenie warunków brzegowych
    for (uint16_t i = 0; i < current_resolution.width; i++) {
        for (uint16_t j = 0; j < current_resolution.height; j++) {
            uint8_t data[] = {color >> 8, color & 0xFF};
            ST7789_WriteData(data, sizeof(data));
        }
    }
}


void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    uint16_t x_start = x0 + current_resolution.X_SHIFT, x_end = x1 + current_resolution.X_SHIFT;
	uint16_t y_start = y0 + current_resolution.Y_SHIFT, y_end = y1 + current_resolution.Y_SHIFT;

    // Upewnij się, że wartości są w dozwolonym zakresie
    if (x_start > current_resolution.width) x_start = current_resolution.width;
    if (x_end > current_resolution.width) x_end = current_resolution.width;
    if (y_start > current_resolution.height) y_start = current_resolution.height;
    if (y_end > current_resolution.height) y_end = current_resolution.height;

    ST7789_WriteCommand(ST7789_CASET); // Column Address Set
    uint8_t data1[] = {x_start >> 8, x_start & 0xFF, x_end >> 8, x_end & 0xFF};
    ST7789_WriteData(data1, sizeof(data1));

    ST7789_WriteCommand(ST7789_RASET); // Row Address Set
    uint8_t data2[] = {y_start >> 8, y_start & 0xFF, y_end >> 8, y_end & 0xFF};
    ST7789_WriteData(data2, sizeof(data2));
    ST7789_WriteCommand(ST7789_RAMWR);
}



