// AlarmSchedule.h
#ifndef ALARM_SCHEDULE_H
#define ALARM_SCHEDULE_H

#include <Arduino.h>

struct TimePoint {
    uint8_t hour;
    uint8_t minute;

    // Operador < (menor que)
    bool operator<(const TimePoint& other) const {
        return (hour < other.hour) || (hour == other.hour && minute < other.minute);
    }

    // Operador == (igualdad)
    bool operator==(const TimePoint& other) const {
        return hour == other.hour && minute == other.minute;
    }

    // Operador <= (menor o igual)
    bool operator<=(const TimePoint& other) const {
        return (*this < other) || (*this == other);
    }

    // Operador >= (mayor o igual)
    bool operator>=(const TimePoint& other) const {
        return !(*this < other);
    }

    // Operador > (mayor que)
    bool operator>(const TimePoint& other) const {
        return !(*this <= other);
    }
};

// Representa la programación de encendido/apagado para un día
struct DailySchedule {
    bool active = false;  // Si hay alarma para ese día
    TimePoint on;
    TimePoint off;
};

// Clase que contiene toda la programación semanal
class AlarmSchedule {
public:
    AlarmSchedule();
    bool shouldBeOn(uint8_t currentDay, uint8_t hour, uint8_t minute) const;
    bool setScheduleForDay(uint8_t day, const TimePoint& onTime, const TimePoint& offTime);
    DailySchedule getDaySchedule(uint8_t dayOfWeek) const;
    bool clearScheduleForDay(uint8_t day);
    bool isLedOnAt(uint8_t dayOfWeek, int currentHour, int currentMinute) const;
    bool isActive(uint8_t day) const;
    bool getOnOffTime(uint8_t day, TimePoint& onTime, TimePoint& offTime) const;

private:
    DailySchedule schedule[7]; // 0 = domingo, 6 = sábado
    bool isValidTime(const TimePoint& time) const;
};

#endif // ALARM_SCHEDULE_H
