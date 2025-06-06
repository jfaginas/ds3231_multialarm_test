#include <Arduino.h>
#include "DS3231Manager.h"
#include "SerialCommandManager.h"
#include "AlarmSchedule.h"
#include "LedManager.h"

#define LED_BUILTIN 2

DS3231Manager rtc;
AlarmSchedule alarmSchedule;
SerialCommandManager serialCmd(rtc, alarmSchedule);
LedManager ledManager(LED_BUILTIN, alarmSchedule, serialCmd);

const unsigned long PRINT_INTERVAL_MS = 60000;
unsigned long lastPrint = 0;
bool allowPrint = true;

void setup() {
    serialCmd.begin();
    rtc.begin();
    ledManager.begin();

    serialCmd.setInputCallback([](bool inputActive) {
        allowPrint = !inputActive;
    });
}

void loop() {
    rtc.update();
    serialCmd.update();

    DateTime nowTime = rtc.getDateTime();
    ledManager.update(nowTime);

    unsigned long now = millis();
    if (allowPrint && now - lastPrint >= PRINT_INTERVAL_MS) {
        lastPrint = now;
        serialCmd.printLineWithPrompt(rtc.formattedTime());
    }
}