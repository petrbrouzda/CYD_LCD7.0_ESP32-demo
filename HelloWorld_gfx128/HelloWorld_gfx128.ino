/*
FQBN: esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi

ESP32 core 2.0.17
Using library U8g2 at version 2.35.30 in folder: C:\Users\brouzda\Documents\Arduino\libraries\U8g2 
Using library GFX Library for Arduino at version 1.2.8 in folder: C:\Users\brouzda\Documents\Arduino\libraries\Arduino_GFX-master 
Using library SPI at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.17\libraries\SPI 
Using library Wire at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.17\libraries\Wire 

Pouzita
GFX Library for Arduino, 1.2.8
z distribucniho baliku.
                       
*/


/*******************************************************************************
 ******************************************************************************/

#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define TFT_BL 2

Arduino_ESP32RGBPanel *bus = new Arduino_ESP32RGBPanel(
    GFX_NOT_DEFINED /* CS */, GFX_NOT_DEFINED /* SCK */, GFX_NOT_DEFINED /* SDA */,
    41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
    9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
    15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */
);

Arduino_RPi_DPI_RGBPanel *gfx = new Arduino_RPi_DPI_RGBPanel(
  bus,
    800 /* width */, 0 /* hsync_polarity */, 210 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
    480 /* height */, 0 /* vsync_polarity */, 22 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */, true /* auto_flush */);

/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

void setup(void)
{
    gfx->begin();
    gfx->fillScreen(BLACK);

#ifdef TFT_BL
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);
#endif

    gfx->setUTF8Print(true);
    gfx->setFont(u8g2_font_unifont_t_extended);

}

void loop()
{
    gfx->setCursor(random(gfx->width()), random(gfx->height()));
    gfx->setTextColor( random(0xffff) );
    
    gfx->println("Ó, náhlý déšť teď zvířil prach a čilá laň běží s houfcem gazel k úkrytům.");

    delay(3000); 
}
