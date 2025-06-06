#include "AlarmSchedule.h"

// Constructor
AlarmSchedule::AlarmSchedule() {
    // Inicializa todas las alarmas como inactivas
    for (int i = 0; i < 7; ++i) {
        schedule[i].active = false;
    }
}

bool AlarmSchedule::setScheduleForDay(uint8_t day, const TimePoint& onTime, const TimePoint& offTime) {
    if (day > 6 || !isValidTime(onTime) || !isValidTime(offTime)) return false;
    
    schedule[day].on = onTime;
    schedule[day].off = offTime;
    schedule[day].active = true;
    return true;
}

bool AlarmSchedule::clearScheduleForDay(uint8_t day) {
    if (day > 6) return false;

    schedule[day].active = false;
    return true;
}

bool AlarmSchedule::isActive(uint8_t day) const {
    if (day > 6) return false;
    return schedule[day].active;
}

bool AlarmSchedule::getOnOffTime(uint8_t day, TimePoint& onTime, TimePoint& offTime) const {
    if (day > 6 || !schedule[day].active) return false;

    onTime = schedule[day].on;
    offTime = schedule[day].off;
    return true;
}

bool AlarmSchedule::isValidTime(const TimePoint& time) const {
    return time.hour < 24 && time.minute < 60;
}

bool AlarmSchedule::shouldBeOn(uint8_t currentDay, uint8_t hour, uint8_t minute) const {
    if (currentDay >= 7) return false;

    const TimePoint& now = TimePoint{hour, minute};

    // Verifica programación actual
    const DailySchedule& today = schedule[currentDay];
    if (today.active) {
        const TimePoint& on = today.on;
        const TimePoint& off = today.off;

        if (on < off) {
            // Rango dentro del mismo día
            if (now >= on && now < off) return true;
        } else {
            // Rango que cruza medianoche
            if (now >= on || now < off) return true;
        }
    }

    // Verifica si el día anterior tenía una alarma que cruza medianoche
    int prevDay = (currentDay + 6) % 7;
    const DailySchedule& yesterday = schedule[prevDay];

    if (yesterday.active) {
        const TimePoint& on = yesterday.on;
        const TimePoint& off = yesterday.off;

        if (on > off) { // Solo tiene sentido si cruzaba medianoche
            if (now < off) return true;
        }
    }

    return false;
}