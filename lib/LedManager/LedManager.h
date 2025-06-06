#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include <Arduino.h>
#include <RTClib.h>
#include "AlarmSchedule.h"
#include "SerialCommandManager.h"

class LedManager {
public:
    LedManager(uint8_t pin, const AlarmSchedule& schedule, const SerialCommandManager& serialManager);
    void begin();
    void update(const DateTime& now);

private:
    uint8_t ledPin;
    bool currentState;
    const AlarmSchedule& schedule;
    const SerialCommandManager& serialManager;
};

#endif
