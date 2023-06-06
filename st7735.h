#include <stdlib.h>

// Adapted from michal037/ driver-st7735s

#pragma once

#define LCD_SLEEP_ON 1
#define LCD_SLEEP_OFF 0

#define LCD_DISPLAY_ON 1
#define LCD_DISPLAY_OFF 0

#define LCD_IDLE_ON 1
#define LCD_IDLE_OFF 0

#define LCD_INVERSION_ON 1
#define LCD_INVERSION_OFF 0

// set Memory acces control
#define LCD_MACTL_MY (1 << 7)  // Row Order
#define LCD_MACTL_MX (1 << 6)  // Column order
#define LCD_MACTL_MV (1 << 5)  // Row Col Exchange
#define LCD_MACTL_ML (1 << 4)  // Vertical refresh
#define LCD_MACTL_BGR (1 << 3) // RGB or BGR order
#define LCD_MACTL_MH (1 << 2)  // Horizontal refresh
#define LCD_MACTL 0            // Default

// setTearing EffectLine
#define LCD_TEARING_MODE_V 0
#define LCD_TEARING_MODE_VH 1
#define LCD_TEARING_OFF 255

// setGammaPredefined
#define LCD_GAMMA_PREDEFINED_1 (1 << 0)
#define LCD_GAMMA_PREDEFINED_2 (1 << 1)
#define LCD_GAMMA_PREDEFINED_3 (1 << 2)
#define LCD_GAMMA_PREDEFINED_4 (1 << 3)

// setInterfacePixelFormat
#define LCD_PIXEL_FORMAT_444 3 // 12-bit / pixel
#define LCD_PIXEL_FORMAT_565 5 // 16 bit /px
#define LCD_PIXEL_FORMAT_666 6 // 18 bit / px

typedef enum lcd_status_t
{
    LCD_FAIL = -1,
    LCD_OK = 0
} lcd_status_t;

typedef lcd_t
{
    unsigned short int width;
    unsigned short int height;
    unsigned short int width_offset;
    unsigned short int height_offset;

    unsigned short int pin_comMode; // Data or command
    signed short int pin_reset;     // -1 if used

    unsigned char dataMode_activeState; // State of pin_comMode

    unsigned char reset_activeState;

    unsigned char flag_mactl_mv;

    unsigned char interface_pixel_format;
}
lcd_t;

typedef struct lcd_t *lcd_ptr_t;

extern pcd_ptr_t lcd_settings;

void lcd_delay(unsigned long int ms);
void lcd_digitalWrite(unsigned short int pin, unsigned char value);
void lcd_spiWrite(unsigned char *buffer, size_t length);
lcd_ptr_t lcd_createSettings(
    unsigned short int width,
    unsigned short int height,
    unsigned short int width_offset,
    unsigned short int height_offset,
    unsigned short int pin_communicationMode,
    signed short int pin_reset);

void lcd_deleteSettings(lcd_ptr_t settings);
void lcd_setSettingsActive(lcd_ptr_t settings);
lcd_ptr_t lcd_getSettingsActive();
lcd_status_t lcd_writeData(unsigned char *buffer, size_t length);
lcd_status_t lcd_writeCommand(unsigned char *buffer, size_t length);
lcd_status_t lcd_writeCommandByte(unsigned char command);
lcd_status_t lcd_hardwareReset();
lcd_status_t lcd_softwareReset();
lcd_status_t lcd_initialize();
lcd_status_t lcd_setSleepMode(unsigned char sleep);
lcd_status_t lcd_setIdleMode(unsigned char idle);
lcd_status_t lcd_setDisplayMode();
