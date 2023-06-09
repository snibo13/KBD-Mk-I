
/** 
  * @brief  ST7735 Registers  
  */ 
#define  LCD_REG_0               0x00 /* No Operation: NOP */
#define  LCD_REG_1               0x01 /* Software reset: SWRESET */
#define  LCD_REG_4               0x04 /* Read Display ID: RDDID */
#define  LCD_REG_9               0x09 /* Read Display Statu: RDDST */
#define  LCD_REG_10              0x0A /* Read Display Power: RDDPM */
#define  LCD_REG_11              0x0B /* Read Display: RDDMADCTL */
#define  LCD_REG_12              0x0C /* Read Display Pixel: RDDCOLMOD */  
#define  LCD_REG_13              0x0D /* Read Display Image: RDDIM */
#define  LCD_REG_14              0x0E /* Read Display Signal: RDDSM */                           
#define  LCD_REG_16              0x10 /* Sleep in & booster off: SLPIN */ 
#define  LCD_REG_17              0x11 /* Sleep out & booster on: SLPOUT */
#define  LCD_REG_18              0x12 /* Partial mode on: PTLON */ 
#define  LCD_REG_19              0x13 /* Partial off (Normal): NORON */
#define  LCD_REG_32              0x20 /* Display inversion off: INVOFF */
#define  LCD_REG_33              0x21 /* Display inversion on: INVON */
#define  LCD_REG_38              0x26 /* Gamma curve select: GAMSET */
#define  LCD_REG_40              0x28 /* Display off: DISPOFF */
#define  LCD_REG_41              0x29 /* Display on: DISPON */
#define  LCD_REG_42              0x2A /* Column address set: CASET */ 
#define  LCD_REG_43              0x2B /* Row address set: RASET */
#define  LCD_REG_44              0x2C /* Memory write: RAMWR */  
#define  LCD_REG_45              0x2D /* LUT for 4k,65k,262k color: RGBSET */
#define  LCD_REG_46              0x2E /* Memory read: RAMRD*/
#define  LCD_REG_48              0x30 /* Partial start/end address set: PTLAR */ 
#define  LCD_REG_52              0x34 /* Tearing effect line off: TEOFF */ 
#define  LCD_REG_53              0x35 /* Tearing effect mode set & on: TEON */ 
#define  LCD_REG_54              0x36 /* Memory data access control: MADCTL */ 
#define  LCD_REG_56              0x38 /* Idle mode off: IDMOFF */ 
#define  LCD_REG_57              0x39 /* Idle mode on: IDMON */ 
#define  LCD_REG_58              0x3A /* Interface pixel format: COLMOD */
#define  LCD_REG_177             0xB1 /* In normal mode (Full colors): FRMCTR1 */
#define  LCD_REG_178             0xB2 /* In Idle mode (8-colors): FRMCTR2 */   
#define  LCD_REG_179             0xB3 /* In partial mode + Full colors: FRMCTR3 */ 
#define  LCD_REG_180             0xB4 /* Display inversion control: INVCTR */
#define  LCD_REG_192             0xC0 /* Power control setting: PWCTR1 */ 
#define  LCD_REG_193             0xC1 /* Power control setting: PWCTR2 */ 
#define  LCD_REG_194             0xC2 /* In normal mode (Full colors): PWCTR3 */
#define  LCD_REG_195             0xC3 /* In Idle mode (8-colors): PWCTR4 */ 
#define  LCD_REG_196             0xC4 /* In partial mode + Full colors: PWCTR5 */ 
#define  LCD_REG_197             0xC5 /* VCOM control 1: VMCTR1 */ 
#define  LCD_REG_199             0xC7 /* Set VCOM offset control: VMOFCTR */ 
#define  LCD_REG_209             0xD1 /* Set LCM version code: WRID2 */ 
#define  LCD_REG_210             0xD2 /* Customer Project code: WRID3 */ 
#define  LCD_REG_217             0xD9 /* NVM control status: NVCTR1 */
#define  LCD_REG_218             0xDA /* Read ID1: RDID1 */ 
#define  LCD_REG_219             0xDB /* Read ID2: RDID2 */ 
#define  LCD_REG_220             0xDC /* Read ID3: RDID3 */ 
#define  LCD_REG_222             0xDE /* NVM Read Command: NVCTR2 */ 
#define  LCD_REG_223             0xDF /* NVM Write Command: NVCTR3 */
#define  LCD_REG_224             0xE0 /* Set Gamma adjustment (+ polarity): GAMCTRP1 */                          
#define  LCD_REG_225             0xE1 /* Set Gamma adjustment (- polarity): GAMCTRN1 */ 