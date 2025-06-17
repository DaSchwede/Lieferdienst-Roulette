#include <Arduino.h>
#include "webserver.h"
#include "storage.h"
#include "rtc.h"
#include "tft_touch.h"
#include "pinconfig.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include "start_button.h"
#include "play.h"
#include "app_state.h"

AppState currentState = MAIN_MENU;


bool wasTouchedLastFrame = false;


void setup() {
  Serial.begin(115200);
  initRTC(true);  // Zeit beim ersten Mal setzen
  initStorage();
  setupWiFiAndWebServer();
  initDisplay();  // Nur Touch initialisieren
  drawStaticLayout();
   // 🔑 Zufallsgenerator initialisieren – robust über RTC-Zeit
  String datum = getDatum();  // z. B. "17.06.2025"
  String zeit  = getZeit();   // z. B. "00:19:13"
  datum.replace(".", "");    // → "17062025"
  zeit.replace(":", "");     // → "001913"
  randomSeed((datum + zeit).toInt());

  Serial.println("RTC-basiertes randomSeed gesetzt.");
 }

void loop() {
  bool nowTouched = tftTouch.touched();

  if (currentState == MAIN_MENU && nowTouched && !wasTouchedLastFrame) {
    if (tftTouch.isStartButtonTouched()) {
      String zufall = pickLieferdienstForToday();
      drawGameScreen(zufall);
      currentState = GAME_SCREEN;
    }
  } else if (currentState == GAME_SCREEN) {
    handleGameTouch();
  }

  wasTouchedLastFrame = nowTouched;
  loopWebServer();
  delay(100);
}


