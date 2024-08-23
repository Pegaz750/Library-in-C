#ifndef ST7789_H
#define ST7789_H

// Definicje komend ST7789
#define ST7789_NOP        0x00  // No operation
#define ST7789_SWRESET    0x01  // Software reset
#define ST7789_RDDID      0x04  // Read display ID
#define ST7789_RDDST      0x09  // Read display status

#define ST7789_SLPIN      0x10  // Sleep in
#define ST7789_SLPOUT     0x11  // Sleep out
#define ST7789_PTLON      0x12  // Partial mode on
#define ST7789_NORON      0x13  // Normal display mode on

#define ST7789_INVOFF     0x20  // Inversion off
#define ST7789_INVON      0x21  // Inversion on
#define ST7789_DISPOFF    0x28  // Display off
#define ST7789_DISPON     0x29  // Display on
#define ST7789_CASET      0x2A  // Column address set
#define ST7789_RASET      0x2B  // Row address set
#define ST7789_RAMWR      0x2C  // Memory write
#define ST7789_RAMRD      0x2E  // Memory read

#define ST7789_MADCTL     0x36  // Memory access control
#define ST7789_COLMOD     0x3A  // Interface pixel format

// Definicje parametrów kolorów w RGB565
#define ST7789_COLOR_MODE_16bit  0x55  // 16-bit color mode (RGB565)
#define ST7789_COLOR_MODE_18bit 0x66   //RGB666 (18bit)

/*Horizontal mirror*/
#define ST7789_MADCTL_MH  0x04
/* Page Address Order ('0': Top to Bottom, '1': the opposite) */
#define ST7789_MADCTL_MY  0x80
/* Column Address Order ('0': Left to Right, '1': the opposite) */
#define ST7789_MADCTL_MX  0x40
/* Page/Column Order ('0' = Normal Mode, '1' = Reverse Mode) */
#define ST7789_MADCTL_MV  0x20
/* Line Address Order ('0' = LCD Refresh Top to Bottom, '1' = the opposite) */
#define ST7789_MADCTL_ML  0x10
/* RGB/BGR Order ('0' = RGB, '1' = BGR) */
#define ST7789_MADCTL_RGB 0x00
#define ST7789_MADCTL_BGR 0x01

// Definicje kolorów w RGB565
#define WHITE       0xFFFF 
#define BLACK       0x0000 
#define RED         0xF800  
#define GREEN       0x07E0  
#define BLUE        0x001F  
#define YELLOW      0xFFE0  
#define CYAN        0x07FF  
#define MAGENTA     0xF81F  
#define GRAY        0x8430
#define BRED        0XF81F
#define GRED        0XFFE0
#define GBLUE       0X07FF
#define BROWN       0XBC40
#define BRRED       0XFC07
#define DARKBLUE    0X01CF
#define LIGHTBLUE   0X7D7C
#define GRAYBLUE    0X5458
#define LIGHTGREEN  0X841F
#define LGRAY       0XC618
#define LGRAYBLUE   0XA651
#define LBBLUE      0X2B12


// Definicje rozdzielczości
#define ST7789_RES_135x240  0
#define ST7789_RES_240x240  1
#define ST7789_RES_170x320  2

// Definicje orientacji ekranu
#define ST7789_ROTATION_0   0
#define ST7789_ROTATION_90  1
#define ST7789_ROTATION_180 2
#define ST7789_ROTATION_270 3



void ST7789_Init(uint8_t display_size, uint8_t rotation);
void ST7789_Fill_Color(uint16_t color);
void ST7789_SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

#endif /* ST7789_H */