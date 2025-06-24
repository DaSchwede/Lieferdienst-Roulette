#pragma once
#include <Arduino.h>

extern bool wasTouchedLastFrame;  // Zugriff auf globale Touch-Variable

String pickLieferdienstForToday();

void drawGameScreen(const String& dienstName);
void handleGameTouch();
void resetToMainMenu();

