#include "LedManager.h"

LedManager::LedManager(uint8_t pin, const AlarmSchedule& schedule, const SerialCommandManager& serialManager)
    : ledPin(pin), currentState(false), schedule(schedule), serialManager(serialManager) {}

void LedManager::begin() {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

void LedManager::update(const DateTime& now) {
    bool shouldBeOn = schedule.shouldBeOn(now.dayOfTheWeek(), now.hour(), now.minute());

    if (shouldBeOn != currentState) {
        currentState = shouldBeOn;
        digitalWrite(ledPin, currentState ? HIGH : LOW);

        if (!serialManager.isUserTyping()) {
            Serial.println(currentState ? "LED ENCENDIDO" : "LED APAGADO");
        }
    }
}

