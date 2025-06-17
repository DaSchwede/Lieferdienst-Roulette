#include <Preferences.h>
#include <Arduino.h>


Preferences prefs;

void initStorage() {
  prefs.begin("bingo", false);
}

void saveLieferdienste(const String& json) {
  prefs.putString("dienste", json);
}

String loadLieferdienste() {
  String raw = prefs.getString("dienste", "");
  if (raw == "") return "[]";
  return raw;
}

void saveRuhetage(const String& data) {
  prefs.putString("ruhetage", data);
}

String loadRuhetage() {
  return prefs.getString("ruhetage", "Sonntag");
}
