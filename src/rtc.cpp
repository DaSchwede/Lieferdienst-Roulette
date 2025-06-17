#include "rtc.h"
#include <Wire.h>
#include <RTClib.h>

RTC_DS1307 rtc;

void initRTC(bool setFromCompileTime) {
  Wire.begin(21, 22);  // SDA, SCL (GPIO21, GPIO22)

  if (!rtc.begin()) {
    Serial.println("❌ RTC nicht gefunden!");
    return;
  }

  if (!rtc.isrunning()) {
    Serial.println("⚠️ RTC läuft nicht, Uhrzeit wird gesetzt.");
    if (setFromCompileTime) {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Setze Zeit beim Kompilieren
    }
  }

  DateTime now = rtc.now();
  Serial.print("🕒 RTC-Zeit: ");
  Serial.print(now.day()); Serial.print(".");
  Serial.print(now.month()); Serial.print(".");
  Serial.print(now.year()); Serial.print(" ");
  Serial.print(now.hour()); Serial.print(":");
  Serial.print(now.minute()); Serial.print(":");
  Serial.println(now.second());
}

String getWochentagName() {
  const char* tage[] = {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"};
  return tage[rtc.now().dayOfTheWeek()];
}

int getWochentag() {
  return rtc.now().dayOfTheWeek();  // 0=Sonntag
}

String getDatum() {
  DateTime now = rtc.now();
  char buf[11];
  snprintf(buf, sizeof(buf), "%02d.%02d.%04d", now.day(), now.month(), now.year());
  return String(buf);
}

String getZeit() {
  DateTime now = rtc.now();
  char buf[9];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
  return String(buf);
}
