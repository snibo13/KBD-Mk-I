#include "lcd.h"

#define SPI_WriteComm lcd_command;
#define SPI_WriteData lcd_data;

static const initialise_display()
{
    HW_Reset(); // VBP>=11, VFP>=7

    Delay(120);

    SPI_WriteComm(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x10);

    SPI_WriteComm(0xC0);
    SPI_WriteData(0x3B); // Scan line
    SPI_WriteData(0x00);

    SPI_WriteComm(0xC1);
    SPI_WriteData(0x0B); // VBP
    SPI_WriteData(0x02);

    SPI_WriteComm(0xC2);
    SPI_WriteData(0x07);
    SPI_WriteData(0x02);

    SPI_WriteComm(0xCC);
    SPI_WriteData(0x10);

    // SPI_WriteComm(0xCD);//RGB format
    // SPI_WriteData(0x08);

    SPI_WriteComm(0xB0); // IPS
    SPI_WriteData(0x00); // 255
    SPI_WriteData(0x11); // 251
    SPI_WriteData(0x16); // 247  down
    SPI_WriteData(0x0e); // 239
    SPI_WriteData(0x11); // 231
    SPI_WriteData(0x06); // 203
    SPI_WriteData(0x05); // 175
    SPI_WriteData(0x09); // 147
    SPI_WriteData(0x08); // 108
    SPI_WriteData(0x21); // 80
    SPI_WriteData(0x06); // 52
    SPI_WriteData(0x13); // 24
    SPI_WriteData(0x10); // 16
    SPI_WriteData(0x29); // 8    down
    SPI_WriteData(0x31); // 4
    SPI_WriteData(0x18); // 0

    SPI_WriteComm(0xB1); //  IPS
    SPI_WriteData(0x00); //  255
    SPI_WriteData(0x11); //  251
    SPI_WriteData(0x16); //  247   down
    SPI_WriteData(0x0e); //  239
    SPI_WriteData(0x11); //  231
    SPI_WriteData(0x07); //  203
    SPI_WriteData(0x05); //  175
    SPI_WriteData(0x09); //  147
    SPI_WriteData(0x09); //  108
    SPI_WriteData(0x21); //  80
    SPI_WriteData(0x05); //  52
    SPI_WriteData(0x13); //  24
    SPI_WriteData(0x11); //  16
    SPI_WriteData(0x2a); //  8  down
    SPI_WriteData(0x31); //  4
    SPI_WriteData(0x18); //  0

    SPI_WriteComm(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x11);

    SPI_WriteComm(0xB0); // VOP  3.5375+ *x 0.0125
    SPI_WriteData(0x6d); // 5D

    SPI_WriteComm(0xB1); // VCOM amplitude setting
    SPI_WriteData(0x37); //

    SPI_WriteComm(0xB2); // VGH Voltage setting
    SPI_WriteData(0x81); // 12V

    SPI_WriteComm(0xB3);
    SPI_WriteData(0x80);

    SPI_WriteComm(0xB5); // VGL Voltage setting
    SPI_WriteData(0x43); //-8.3V

    SPI_WriteComm(0xB7);
    SPI_WriteData(0x85);

    SPI_WriteComm(0xB8);
    SPI_WriteData(0x20);

    SPI_WriteComm(0xC1);
    SPI_WriteData(0x78);

    SPI_WriteComm(0xC2);
    SPI_WriteData(0x78);

    SPI_WriteComm(0xD0);
    SPI_WriteData(0x88);

    SPI_WriteComm(0xE0);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x02);

    SPI_WriteComm(0xE1);
    SPI_WriteData(0x03);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x04);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x20);
    SPI_WriteData(0x20);

    SPI_WriteComm(0xE2);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xE3);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x11);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xE4);
    SPI_WriteData(0x22);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xE5);
    SPI_WriteData(0x05);
    SPI_WriteData(0xEC);
    SPI_WriteData(0xA0);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x07);
    SPI_WriteData(0xEE);
    SPI_WriteData(0xA0);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xE6);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x11);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xE7);
    SPI_WriteData(0x22);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xE8);
    SPI_WriteData(0x06);
    SPI_WriteData(0xED);
    SPI_WriteData(0xA0);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x08);
    SPI_WriteData(0xEF);
    SPI_WriteData(0xA0);
    SPI_WriteData(0xA0);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xEB);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x40);
    SPI_WriteData(0x40);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);

    SPI_WriteComm(0xED);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xBA);
    SPI_WriteData(0x0A);
    SPI_WriteData(0xBF);
    SPI_WriteData(0x45);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);
    SPI_WriteData(0x54);
    SPI_WriteData(0xFB);
    SPI_WriteData(0xA0);
    SPI_WriteData(0xAB);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);
    SPI_WriteData(0xFF);

    SPI_WriteComm(0xEF);
    SPI_WriteData(0x10);
    SPI_WriteData(0x0D);
    SPI_WriteData(0x04);
    SPI_WriteData(0x08);
    SPI_WriteData(0x3F);
    SPI_WriteData(0x1F);

    SPI_WriteComm(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x13);

    SPI_WriteComm(0xEF);
    SPI_WriteData(0x08);

    SPI_WriteComm(0xFF);
    SPI_WriteData(0x77);
    SPI_WriteData(0x01);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);
    SPI_WriteData(0x00);

    SPI_WriteComm(0x11);

    Delay(120);

    SPI_WriteComm(0x29);

    SPI_WriteComm(0x36);
    SPI_WriteData(0x08);

    SPI_WriteComm(0x3A);
    SPI_WriteData(0x70);
}