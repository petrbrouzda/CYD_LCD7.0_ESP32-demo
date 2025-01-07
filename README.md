# CYD_LCD7.0_ESP32-demo
Demo práce s levným 7" displejem s ESP32-S3 procesorem (CYD, 8048S070).
Odkaz na displej: https://s.click.aliexpress.com/e/_Dn1RQ6d

Detailní popis naleznete ve článku zde: https://pebrou.wordpress.com/2025/01/07/levny-7-displej-s-esp32-s3-poznamky/ 

Jsou zde tři samply:

## HelloWorld_gfx128
Základní demo z balíku dokumentace k desce (https://www.laskakit.cz/user/related_files/7-0inch_esp32-8048s070.zip)
- V library manageru **nesmíte** mít knihovnu „GFX library for Arduino“
- Z balíku musíte nakopírovat knihovnu Arduino_GFX-master do knihoven Arduina

## HelloWorld_gfx150
Upravená inicializace desky, aby fungovala s „GFX library for Arduino“ 1.5.0 z library manageru.

Plus demo, jak vykreslovat české texty pomocí fontů ve formátu **U8g2** - a k nim tedy musí být nainstalovaná knihovna U8g2.

## HelloWorld_gfx150_adafruit_font
Upravená inicializace desky, aby fungovala s „GFX library for Arduino“ 1.5.0 z library manageru.

Pro vykreslení textu se používá font ve formátu **Adafruit_GFX**.

