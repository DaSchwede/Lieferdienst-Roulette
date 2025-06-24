
# 📦 Lieferdienst Roulette

**Lieferdienst Roulette** ist ein ESP32-basiertes Touchscreen-Projekt, das zufällig einen aktiven Lieferdienst aus einer konfigurierbaren Liste auswählt – ideal für spontane Entscheidungen im Büro oder zuhause. Ruhetage werden dabei berücksichtigt, Wiederholungen innerhalb einer Woche vermieden, und die Statistiken dauerhaft im EEPROM gespeichert.

## 🖼️ Features

- 🎨 Farb-TFT Display (ILI9341) mit Touch-Eingabe (XPT2046)
- 🕒 Integrierte RTC (z. B. DS1307/DS3231) mit Echtzeit- und Datum-Anzeige
- 📋 Lieferdienste & Ruhetage via Web-UI konfigurierbar
- 🧠 Keine Wiederholungen innerhalb derselben Woche
- 💾 Statistik-Speicherung im I2C-EEPROM (24C32)
- 🌐 Webserver mit Captive Portal zur Verwaltung
- 📱 Touch-optimiertes UI mit START-Button und Zahnrad-Menü
- 🔁 Navigation über Zahnrad (kein Zurück-Button nötig)
- 📊 Anzeige wie oft welcher Dienst gewählt wurde

## ⚙️ Hardware

- ESP32 Dev Board
- 2.8" ILI9341 TFT Display (SPI) mit XPT2046 Touch Controller
- RTC-Modul: DS1307 oder DS3231 mit integriertem 24C32 EEPROM
- Optional: SD-Karte für spätere Features

## 📁 Projektstruktur

```
Lieferdienst-Roulette/
├── include/
│   ├── app_state.h
│   ├── gear_icon.h
│   ├── html_templates.h
│   ├── lieferdienst.h
│   ├── logo.h
│   ├── pinconfig.h
│   ├── play.h
│   ├── rtc.h
│   ├── start_button.h
│   ├── storage.h
│   ├── tft_touch.h
│   └── webserver.h
│
├── src/
│   ├── app_state.cpp
│   ├── gear_icon.cpp
│   ├── html_templates.cpp
│   ├── lieferdienst.cpp
│   ├── logo.cpp
│   ├── main.cpp
│   ├── play.cpp
│   ├── rtc.cpp
│   ├── start_button.cpp
│   ├── storage.cpp
│   ├── tft_touch.cpp
│   └── webserver.cpp
│
├── data/
│   └── lieferdienste.json
│
├── platformio.ini
└── README.md
```

## 🧪 Beispiel `lieferdienste.json`

```json
[
  {
    "name": "Pizza Leo",
    "ruhe": ["Montag"]
  },
  {
    "name": "Asia King",
    "ruhe": []
  },
  {
    "name": "Sushi Max",
    "ruhe": ["Dienstag", "Freitag"]
  }
]
```

## 🎲 Auswahlprinzip

- Zufall basierend auf RTC-Zeit (`randomSeed()`)
- Nur verfügbare Dienste ohne Ruhetag und ohne Wochendoppelung werden berücksichtigt
- Auswahl wird für den aktuellen Tag gespeichert
- Jeder Dienst bekommt eine feste Indexnummer
- EEPROM (24C32) speichert:
  - Wochenauswahl (1 Byte je Wochentag)
  - Gesamtzähler für jeden Dienst (Index → Anzahl)

## 🔧 Installation mit PlatformIO

1. [VS Code mit PlatformIO](https://platformio.org/install/ide?install=vscode) installieren
2. Projekt klonen:
   ```bash
   git clone https://github.com/DaSchwede/Lieferdienst-Roulette.git
   cd Lieferdienst-Roulette
   ```
3. Öffne das Projekt in VS Code
4. Flashen & starten:
   ```bash
   pio run --target upload
   ```

## 🌐 Web-Konfiguration

- Beim Start wird ein WLAN-Hotspot erzeugt („Lieferdienst-Setup“)
- Captive Portal zeigt ein konfigurierbares Web-Interface
- Lieferdienste & Ruhetage editierbar
- Zeit und Datum per Web-Formular setzbar

## 📊 Statistik

- Jeder Lieferdienst bekommt eine feste ID (Index)
- EEPROM merkt sich, wann welcher Dienst gezogen wurde
- Rückblick: Wie oft wurde ein Dienst insgesamt gewählt?
- Kein Dienst erscheint zweimal in derselben Woche

## 📸 Screenshots

> *Füge hier noch Screenshots vom Display, der Spielanzeige oder der Web-Oberfläche ein.*

---

## 🚀 Geplante Erweiterungen

- 🌐 REST-API für externe Webabfrage
- 💤 Sleep-Modus mit Touch-Weckfunktion
- 🔁 Wochenstatistik im Webinterface anzeigen
- 🧹 Reset-Option für Statistiken via UI
- 📶 WLAN-Persistenz mit ESP32-NVS

## 📄 Lizenz

MIT License – siehe [`LICENSE`](LICENSE)

---

> 🍕 *Projekt von [@DaSchwede](https://github.com/DaSchwede) – gegen Entscheidungsnot beim Essen!*
