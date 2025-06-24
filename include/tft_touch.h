#ifndef TFT_TOUCH_H
#define TFT_TOUCH_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include "pinconfig.h"

String korrekturUmlaute(String text);

class TFTTouch {
public:
    TFTTouch();
    void begin();
    void update();
    bool touched();
    TS_Point getPoint();
    TS_Point getMappedPoint();
    bool isStartButtonTouched();
    bool isBackButtonTouched();
    bool isSettingsButtonTouched();
    Adafruit_ILI9341& getTFT();

private:
    Adafruit_ILI9341 tft;
    XPT2046_Touchscreen touch;
    bool initialized = false; 
};

void initDisplay();
void drawStaticLayout();
void drawSettingsScreen();
extern TFTTouch tftTouch;


#endif
