#include "DS3231Manager.h"

DS3231Manager::DS3231Manager() {}

void DS3231Manager::begin() {
    if (!rtc.begin()) {
        Serial.println("No se pudo encontrar el RTC");
        while (true); // Error crítico
    }
    if (rtc.lostPower()) {
        Serial.println("RTC sin alimentación, se ajusta la hora actual");
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

void DS3231Manager::update() {
    // Por ahora puede estar vacío o tener lógica futura
}

DateTime DS3231Manager::now() {
    return rtc.now();
}

String DS3231Manager::formattedTime() {
    DateTime now = rtc.now();
    char buf[20];
    snprintf(buf, sizeof(buf), "%02d/%02d/%04d %02d:%02d",
             now.day(), now.month(), now.year(), now.hour(), now.minute());
    return String(buf);
}

bool DS3231Manager::setDateTime(const String& input) {
    int d, m, y, h, min;
    if (sscanf(input.c_str(), "%d/%d/%d %d:%d", &d, &m, &y, &h, &min) != 5) {
        Serial.println("Formato inválido. Usa SET=DD/MM/YYYY HH:MM");
        return false;
    }

    // Validación de fecha y hora
    const uint8_t diasPorMes[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    if (m < 1 || m > 12) {
        Serial.println("Mes inválido.");
        return false;
    }

    int maxDia = diasPorMes[m - 1];
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || (y % 400 == 0))) {
        maxDia = 29;  // Año bisiesto
    }

    if (y < 2000 || y > 2099 || d < 1 || d > maxDia || h < 0 || h > 23 || min < 0 || min > 59) {
        Serial.println("Fecha u hora inválida.");
        return false;
    }

    rtc.adjust(DateTime(y, m, d, h, min, 0));
    return true;
}

DateTime DS3231Manager::getDateTime() {
    return rtc.now();
}

void DS3231Manager::setAlarm(uint8_t hour, uint8_t minute) {
    alarmHour = hour;
    alarmMinute = minute;
    Serial.printf("Alarma programada a las %02d:%02d\n", alarmHour, alarmMinute);
}

bool DS3231Manager::checkAlarm() {
    DateTime now = rtc.now();
    return now.hour() == alarmHour && now.minute() == alarmMinute;
}