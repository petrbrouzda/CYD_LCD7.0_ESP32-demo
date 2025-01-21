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

#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "BROPKA";
const char* password = "0603704056";


#include "src/TJpg_Decoder/TJpg_Decoder.h"


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


unsigned char * image;
int imageSize = 0;

void download_jpeg()
{
   HTTPClient http;

    Serial.print("[HTTP] begin...\n");

    // configure server and url
    http.begin("http://lovecka.info/tmp/test1.jpg");

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {

        // get length of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();
        imageSize = len;
        image = (unsigned char *)ps_malloc( len + 100 );
        unsigned char * out = image;

        // create buffer for read
        uint8_t buff[128] = {0};
        

        // get tcp stream
        WiFiClient * stream = http.getStreamPtr();

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // get available data size
          size_t size = stream->available();

          if (size) {
            // read up to 128 byte
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            // write it to Serial
            memcpy( out, buff, c );
            out+=c;

            if (len > 0) {
              len -= c;
            }
          }
          delay(1);
        }

        Serial.printf("read %d byte", (out-image) );
        Serial.println();
        Serial.print("[HTTP] connection closed or file end.\n");
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
}


// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool gfx_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= gfx->height() ) return 0;

  // This might work instead if you adapt the sketch to use the Adafruit_GFX library
  // tft.drawRGBBitmap(x, y, bitmap, w, h);
  gfx->draw16bitRGBBitmap(x,  y, bitmap, w, h);

  // Return 1 to decode next block
  return 1;
}


void setup(void)
{
    Serial.begin(115200);
    delay(2500);
    Serial.println( "starting" );

    gfx->begin();
    gfx->setRotation(2);
    gfx->fillScreen(BLACK);

    pinMode(TFT_BL, OUTPUT);
    // zapnout podsvícení = cca 395 mA odběru (240/80 MHz); bez toho jen 85 mA
    digitalWrite(TFT_BL, HIGH);

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    // attempt to connect to Wifi network:
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      // wait 1 second for re-trying
      delay(1000);
    }

    Serial.print("Connected to ");
    Serial.println(ssid);

    download_jpeg();

    // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The byte order can be swapped (set true for TFT_eSPI)
  TJpgDec.setSwapBytes(false);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(gfx_output);


  // Time recorded for test purposes
  uint32_t t = millis();

  // Get the width and height in pixels of the jpeg if you wish
  uint16_t w = 0, h = 0;
  TJpgDec.getJpgSize(&w, &h, image, imageSize );
  Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawJpg(0, 0, image, imageSize );

  // How much time did rendering take (ESP8266 80MHz 262ms, 160MHz 149ms, ESP32 SPI 111ms, 8bit parallel 90ms
  t = millis() - t;
  Serial.print(t); Serial.println(" ms");

}

void loop()
{
    delay(1000); 
}
