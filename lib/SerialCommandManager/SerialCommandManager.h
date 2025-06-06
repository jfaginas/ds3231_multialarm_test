#ifndef SERIAL_COMMAND_MANAGER_H
#define SERIAL_COMMAND_MANAGER_H

#include <Arduino.h>
#include "DS3231Manager.h"
#include "AlarmSchedule.h"


class SerialCommandManager {
public:
    SerialCommandManager(DS3231Manager& rtc, AlarmSchedule& schedule);
    void begin();
    void update();
    void setInputCallback(std::function<void(bool)> callback);
    void printPrompt();
    bool isUserTyping() const { return userIsTyping; }
    void printLineWithPrompt(const String& line);
    void printStatus(const String& label, const String& value);

private : 
    DS3231Manager& rtc;
    AlarmSchedule& alarmSchedule;
    String buffer;
    bool userIsTyping = false;
    unsigned long lastInputTime = 0;
    const unsigned long INPUT_TIMEOUT_MS = 10000;
    void showAllSchedules();
    std::function<void(bool)> onUserInputStateChange = nullptr;;
    void processCommand(const String& command);
    void parseSchedCommand(const String& command);
    void parseSetCommand(const String& command);
    void parseClearCommand(const String& command);
};

#endif
