/*
FQBN: esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi

ESP32 core 2.0.17
Using library GFX Library for Arduino at version 1.5.0 in folder: C:\Users\brouzda\Documents\Arduino\libraries\GFX_Library_for_Arduino 
Using library SPI at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.17\libraries\SPI 
Using library Wire at version 2.0.0 in folder: C:\Users\brouzda\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.17\libraries\Wire 

Použita
GFX Library for Arduino, 1.5.0
ze spravce knihoven.

1.5.1 nelze použít, protože zjevně chce ESP32 core 3.x

Použity fonty formátu Adafruit_GFX
- aby to fungovalo, NESMÍ být includovaná knihovna U8g2 !!!

*/


#if __has_include(<U8g2lib.h>)
  #error V knihovnach NESMI byt knihovna U8g2!
#else
  // OK, tak to ma byt
#endif

/*******************************************************************************
 ******************************************************************************/

#include <Arduino_GFX_Library.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin
#define TFT_BL 2

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    41 /* DE */, 40 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    14 /* R0 */, 21 /* R1 */, 47 /* R2 */, 48 /* R3 */, 45 /* R4 */,
    9 /* G0 */, 46 /* G1 */, 3 /* G2 */, 8 /* G3 */, 16 /* G4 */, 1 /* G5 */,
    15 /* B0 */, 7 /* B1 */, 6 /* B2 */, 5 /* B3 */, 4 /* B4 */,
    0 /* hsync_polarity */, 180 /* hsync_front_porch */, 30 /* hsync_pulse_width */, 16 /* hsync_back_porch */,
    0 /* vsync_polarity */, 12 /* vsync_front_porch */, 13 /* vsync_pulse_width */, 10 /* vsync_back_porch */,
    1 /* pclk_active_neg */
);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);


/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

#include "fonts/PragatiNarrow-Regular20pt8b.h"

#include "src/toolkit/DeviceInfo.h"

void setup(void)
{
    Serial.begin(115200);
    delay(1500);
    char devinfo[100];
    formatDeviceInfo( devinfo );
    Serial.println( devinfo );

    gfx->begin();
    gfx->fillScreen(BLACK);

    pinMode(TFT_BL, OUTPUT);
    // zapnout podsvícení = cca 395 mA odběru (240/80 MHz); bez toho jen 85 mA
    digitalWrite(TFT_BL, HIGH);

    // Tohle bez U8g2 v knihovně není!
    // Podporu češtiny ve fontu získáte takto: https://github.com/petrbrouzda/fontconvert8-iso8859-2
    // a lepší helper pro ní je zde: https://github.com/petrbrouzda/ExtGfx/blob/main/src/extgfx/TextPainter.h 
    // gfx->setUTF8Print(true);
}

void loop()
{
    gfx->setFont( &PragatiNarrow_Regular20pt8b);
    gfx->setCursor(random(gfx->width()), random(gfx->height()));
    gfx->setTextColor( random(0xffff) );
    gfx->println("AHOJ!");

    delay(3000); 
}
