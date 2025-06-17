#pragma once
#include <Arduino.h>

String pickLieferdienstForToday();

void drawGameScreen(const String& dienstName);
void handleGameTouch();
void resetToMainMenu();
