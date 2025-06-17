#ifndef STORAGE_H
#define STORAGE_H

#include <Arduino.h> // Muss VOR dem Einsatz von String kommen

void initStorage();
void saveLieferdienste(const String& json);
String loadLieferdienste();
void saveRuhetage(const String& data);
String loadRuhetage();

#endif
