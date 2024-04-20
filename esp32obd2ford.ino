#include "BluetoothSerial.h"
#include "ELMduino.h"
#include <TFT_eSPI.h>
#include "RTClib.h"
#include "arduino_secrets.h"
#include "agency12pt7b.h"
#include "agency14pt7b.h"
#include "agency20pt7b.h"
#include "agency22pt7b.h"

TFT_eSPI tft = TFT_eSPI();
RTC_DS3231 rtc;

TFT_eSprite daySprite = TFT_eSprite(&tft);
TFT_eSprite dateSprite = TFT_eSprite(&tft);
TFT_eSprite tempSprite = TFT_eSprite(&tft);
TFT_eSprite monthSprite = TFT_eSprite(&tft);
TFT_eSprite hourSprite = TFT_eSprite(&tft);
TFT_eSprite ecuSprite = TFT_eSprite(&tft);

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

String getMonthName(uint8_t month) {
    switch(month) {
        case 1: return "January";
        case 2: return "February";
        case 3: return "March";
        case 4: return "April";
        case 5: return "May";
        case 6: return "June";
        case 7: return "July";
        case 8: return "August";
        case 9: return "September";
        case 10: return "October";
        case 11: return "November";
        case 12: return "December";
        default: return "Unknown";
    } 
}

void setup () {
    Serial.begin(115200);
    rtc.begin();
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    tft.init();
    tft.setRotation(1);
    //tft.pushImage(0, 0, 320, 240, adjust);
    tft.fillScreen(TFT_BLACK);
    tft.drawLine(106, 2, 106, 80, 0x0cb6);
    tft.drawLine(107, 2, 107, 80, 0x0cb6);

    daySprite.createSprite(45, 35);
    daySprite.setFreeFont(&agency14pt7b);
    daySprite.setTextColor(TFT_WHITE);

    dateSprite.createSprite(40, 40);
    dateSprite.setFreeFont(&agency20pt7b);
    dateSprite.setTextColor(TFT_WHITE);

    tempSprite.createSprite(45, 35);
    tempSprite.setFreeFont(&agency14pt7b);
    tempSprite.setTextColor(TFT_WHITE);

    monthSprite.createSprite(95, 30);
    monthSprite.setFreeFont(&agency12pt7b);
    monthSprite.setTextColor(TFT_WHITE);

    ecuSprite.createSprite(120, 20);
    ecuSprite.setTextSize(1.5);
    ecuSprite.fillSprite(TFT_BLACK);
    ecuSprite.setTextDatum(MC_DATUM);

    hourSprite.createSprite(148, 60);
    hourSprite.setFreeFont(&agency22pt7b);
    hourSprite.setTextColor(TFT_WHITE);
}

void loop ()  {
    DateTime now = rtc.now();
    int temperature = rtc.getTemperature();
    int hourValue = now.hour();
    int minuteValue = now.minute();

    daySprite.fillSprite(TFT_BLACK);
    daySprite.setTextDatum(MC_DATUM);
    daySprite.drawString(daysOfTheWeek[now.dayOfTheWeek()], 22, 17);
    daySprite.pushSprite(5, 8);

    dateSprite.fillSprite(0x0cb6);
    dateSprite.setTextDatum(MC_DATUM);
    dateSprite.drawString(String(now.day()), 20, 20);
    dateSprite.pushSprite(60, 5);

    tempSprite.fillSprite(TFT_BLACK);
    tempSprite.setTextDatum(MC_DATUM);
    tempSprite.drawString(String(temperature) + "'C", 22, 17);
    tempSprite.pushSprite(110, 24);

    monthSprite.fillSprite(TFT_BLACK);
    monthSprite.setTextDatum(MR_DATUM);
    monthSprite.drawString(getMonthName(now.month()), 95, 15);
    monthSprite.pushSprite(5, 48);

    ecuSprite.setTextColor(TFT_RED);
    ecuSprite.drawString("ECU : Disconnected", 60, 10);
    ecuSprite.pushSprite(3, 85);

    String hourStr = String(hourValue);
    String minuteStr = String(minuteValue);
    if (hourValue < 10) hourStr = "0" + hourStr;
    if (minuteValue < 10) minuteStr = "0" + minuteStr;
    hourSprite.fillSprite(TFT_BLACK);
    hourSprite.setTextDatum(MC_DATUM);
    hourSprite.drawString(hourStr + ":" + minuteStr, 74, 30, 7);
    hourSprite.pushSprite(162, 15);

    delay(1000);
}