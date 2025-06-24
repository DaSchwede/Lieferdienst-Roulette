#include "tft_touch.h"
#include "pinconfig.h"
#include "logo.h"
#include "rtc.h"
#include "start_button.h"
#include "gear_icon.h"


String korrekturUmlaute(String text) {
  text.replace("ä", "ae");
  text.replace("ö", "oe");
  text.replace("ü", "ue");
  text.replace("Ä", "Ae");
  text.replace("Ö", "Oe");
  text.replace("Ü", "Ue");
  text.replace("ß", "ss");
  return text;
}


TFTTouch tftTouch;

void initDisplay() {
    tftTouch.begin();
}


void drawStaticLayout() {
    auto& tft = tftTouch.getTFT();

    tft.fillScreen(ILI9341_WHITE);  // Hintergrund: weiß

    // Logo mittig oben
    int logoX = (tft.width() - logo_width) / 2;
    int logoY = 10;
    tft.drawRGBBitmap(logoX, 10, (uint16_t*)logo, logo_width, logo_height);

    // Text darunter – nach oben geschoben
    tft.setTextColor(ILI9341_BLUE);
    tft.setTextSize(3);
    tft.setCursor((tft.width() - 13 * 18) / 2, 95);   // „Lieferdienst“
    tft.print("Lieferdienst");
    tft.setCursor((tft.width() - 9 * 18) / 2, 135);   // „Roulette“
    tft.print("Roulette");

    // Zahnrad links oben (24x24)
    tft.drawRGBBitmap(5, 5, (uint16_t*)gear_icon, gear_icon_width, gear_icon_height);


   // START-Button als Bild (zentriert unten)
  int btnX = (tft.width() - start_button_width) / 2;
  int btnY = tft.height() - start_button_height - 10;
  tft.drawRGBBitmap(btnX, btnY, (uint16_t*)start_button, start_button_width, start_button_height);
}

void drawSettingsScreen() {
  auto& tft = tftTouch.getTFT();
  tft.fillScreen(ILI9341_WHITE);

  // Zahnrad oben links
  tft.drawRGBBitmap(5, 5, gear_icon, gear_icon_width, gear_icon_height);

  // Titel
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.setCursor(40, 10);
  tft.print("Einstellungen");

  // ⏰ Datum, Uhrzeit, Wochentag
  String datum   = getDatum();         // z. B. "17.06.2025"
  String uhrzeit = getZeit();          // z. B. "14:22:05"
  String tag     = getWochentagName(); // z. B. "Dienstag"

  tft.setTextSize(2);
  tft.setCursor(10, 50);
  tft.printf("Tag: %s", tag.c_str());

  tft.setCursor(10, 75);
  tft.printf("Datum: %s", datum.c_str());

  tft.setCursor(10, 100);
  tft.printf("Uhrzeit: %s", uhrzeit.substring(0, 5).c_str()); // nur hh:mm
}


TFTTouch::TFTTouch()
  : tft(TFT_CS, TFT_DC, TFT_RST), touch(TOUCH_CS, TOUCH_IRQ) {}

void TFTTouch::begin() {
    if (initialized) return;
    initialized = true;

    // Erst SPI starten
    SPI.begin(TFT_SCK, TFT_MISO, TFT_MOSI);

    // TFT initialisieren
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);

    // Touch initialisieren – mit korrektem SPI!
    touch.begin(SPI);
    touch.setRotation(1);
}

void TFTTouch::update() {
    // evtl. Status-Update/Interrupt-Handling
}

bool TFTTouch::touched() {
    return touch.touched();
}

TS_Point TFTTouch::getPoint() {
    return touch.getPoint();
}

TS_Point TFTTouch::getMappedPoint() {
    TS_Point p = touch.getPoint();

    // 180° gedreht: X und Y müssen invertiert werden
    int x = map(p.y, 400, 3600, 239, 0);   // Touch-Y invertiert → Display-X
    int y = map(p.x, 420, 3780, 319, 0);   // Touch-X invertiert → Display-Y

    x = constrain(x, 0, 239);
    y = constrain(y, 0, 319);

    return TS_Point(x, y, p.z);
}

bool TFTTouch::isStartButtonTouched() {
    if (!touched()) return false;

    TS_Point p = getMappedPoint();

    return (p.x >= 186 && p.x <= 224 &&
            p.y >= 67  && p.y <= 247);
}

bool TFTTouch::isBackButtonTouched() {
    if (!touched()) return false;

    TS_Point p = getMappedPoint();
    Serial.printf("Touch für Back: x=%d y=%d z=%d\n", p.x, p.y, p.z);

    // Definiere festen Bereich für den Zurück-Button
    int backMinX = 180;
    int backMaxX = 240;
    int backMinY = 120;
    int backMaxY = 200;

    return (p.x >= backMinX && p.x <= backMaxX &&
            p.y >= backMinY && p.y <= backMaxY);
}

bool TFTTouch::isSettingsButtonTouched() {
    if (!touched()) return false;

    TS_Point p = getMappedPoint();

    // Bereich: oben links (entspricht drawRGBBitmap(5,5,...))
    const int iconX = 5;
    const int iconY = 5;
    const int iconW = gear_icon_width;
    const int iconH = gear_icon_height;

    return (p.x >= iconX && p.x <= (iconX + iconW) &&
            p.y >= iconY && p.y <= (iconY + iconH));
}


Adafruit_ILI9341& TFTTouch::getTFT() {
    return tft;
}
