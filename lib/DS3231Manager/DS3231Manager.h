#pragma once

#include <RTClib.h>

class DS3231Manager {

public:
    DS3231Manager();
    void begin();
    void update();
    DateTime now();
    String formattedTime();

    bool setDateTime(const String& input);
    void setAlarm(uint8_t hour, uint8_t minute);
    bool checkAlarm();
    DateTime getDateTime();

private:
    RTC_DS3231 rtc;
    uint8_t alarmHour = 255;
    uint8_t alarmMinute = 255;
};
