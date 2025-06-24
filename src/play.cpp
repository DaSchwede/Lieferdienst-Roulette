#include "play.h"
#include "tft_touch.h"
#include "storage.h"
#include "rtc.h"
#include <ArduinoJson.h>
#include <vector>
#include "app_state.h"


extern TFTTouch tftTouch;
extern bool wasTouchedLastFrame;

String pickLieferdienstForToday() {
  String daten = loadLieferdienste();
  if (daten == "") return "Kein Dienst gespeichert";

  JsonDocument doc;
  deserializeJson(doc, daten);
  JsonArray arr = doc.as<JsonArray>();

  String heute = getWochentagName();
  std::vector<String> offene;

  for (JsonObject eintrag : arr) {
    JsonArray ruhetage = eintrag["ruhe"];
    bool hatRuhe = false;
    for (String tag : ruhetage) {
      if (tag == heute) {
        hatRuhe = true;
        break;
      }
    }
    if (!hatRuhe) {
      offene.push_back(eintrag["name"].as<String>());
    }
  }

  if (offene.size() == 0) return "Kein geoeffneter Dienst";
  return offene[random(0, offene.size())];
}

void drawGameScreen(const String& dienstName) {
    auto& tft = tftTouch.getTFT();
    tft.fillScreen(ILI9341_WHITE);

    // Titel
    tft.setTextColor(ILI9341_BLACK);
    tft.setTextSize(2);
    tft.setCursor(20, 40);
    tft.println("Heute waehlt das Glueck:");

    // Zufälliger Dienst
    String dienst = pickLieferdienstForToday();
    tft.setTextSize(3);
    int16_t x1, y1;
    uint16_t w, h;
    tft.getTextBounds(dienst, 0, 0, &x1, &y1, &w, &h);
    tft.setCursor((tft.width() - w) / 2, 100);
    tft.print(dienst);

    // Zurück-Button
    int backBtnW = 80, backBtnH = 30;
    int backX = (tft.width() - backBtnW) / 2;
    int backY = 180;
    tft.fillRect(backX, backY, backBtnW, backBtnH, ILI9341_DARKGREY);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(backX + 15, backY + 7);
    tft.print("Back");
}

void handleGameTouch() {
    bool nowTouched = tftTouch.touched();

    if (nowTouched && !wasTouchedLastFrame) {
        if (tftTouch.isBackButtonTouched()) {
            Serial.println("Zurueck-Button gedrueckt!");
            currentState = MAIN_MENU;
            drawStaticLayout();
        }
    }

    wasTouchedLastFrame = nowTouched;
}

