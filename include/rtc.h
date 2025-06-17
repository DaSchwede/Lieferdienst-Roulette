#ifndef RTC_H
#define RTC_H

#include <Arduino.h>
#include <RTClib.h>

extern RTC_DS1307 rtc;  // <--- Jetzt global zugreifbar

void initRTC(bool setFromCompileTime = false);
String getWochentagName();
int getWochentag();
String getDatum();
String getZeit();

#endif
