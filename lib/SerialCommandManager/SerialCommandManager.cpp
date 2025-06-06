#include "SerialCommandManager.h"

SerialCommandManager::SerialCommandManager(DS3231Manager& rtcRef, AlarmSchedule& scheduleRef)
    : rtc(rtcRef), alarmSchedule(scheduleRef) {}

void SerialCommandManager::begin() {
    Serial.begin(115200);
    while (!Serial); // Esperar a que el puerto esté listo
    buffer = "";
    Serial.println("\nInicialización completa. Puedes ingresar:");
    Serial.println("  SET=DD/MM/YYYY HH:MM    ➜ Configura fecha y hora actual");
    Serial.println("  SCHED=D HH:MM HH:MM     ➜ Programa encendido/apagado para el día D (0=Domingo)");
    Serial.println("                            Ejemplo: SCHED=5 08:00 18:00");
    Serial.println("  CLEAR=D                 ➜ Elimina la programación del día D");
    Serial.println("  SHOW                    ➜ Muestra toda la programación semanal");
    printLineWithPrompt("");
}

void SerialCommandManager::printPrompt() {
    Serial.print("> ");
}

void SerialCommandManager::setInputCallback(std::function<void(bool)> callback) {
    onUserInputStateChange = callback;
}

void SerialCommandManager::update() {
    while (Serial.available()) {
        char c = Serial.read();

        if (!userIsTyping) {
            userIsTyping = true;
            lastInputTime = millis();
            if (onUserInputStateChange) {
                onUserInputStateChange(true);
            }
        } else {
            lastInputTime = millis();
        }

        if (c == '\r') continue;

        if (c == '\n') {
            Serial.println();
            processCommand(buffer);
            buffer = ""; 
            userIsTyping = false;
            if (onUserInputStateChange) {
                onUserInputStateChange(false);
            }
            return;
        }

        if (c == '\b' || c == 127) {
            if (buffer.length() > 0) {
                buffer.remove(buffer.length() - 1);
                Serial.print("\b \b");
            }
        } else {
            buffer += c;
            Serial.print(c);
        }
    }

    // Timeout: si no se escribe nada en X tiempo
    if (userIsTyping && (millis() - lastInputTime > INPUT_TIMEOUT_MS)) {
        userIsTyping = false;
        buffer = "";
        if (onUserInputStateChange) {
            onUserInputStateChange(false);
        }
        Serial.println("\n[Entrada cancelada por inactividad]");
        printLineWithPrompt("");
    }
}


void SerialCommandManager::processCommand(const String& command) {
    if (command.length() == 0) return;

    if (command.startsWith("SET=")) {
        parseSetCommand(command);
    } else if (command.startsWith("SCHED=")) {
        parseSchedCommand(command);
    } else if (command.startsWith("CLEAR=")) {
        parseClearCommand(command);
    } else if (command.startsWith("SHOW")) {
        showAllSchedules();
    } else {
        Serial.println("Comando no reconocido.");
        printLineWithPrompt("");
    }
}

void SerialCommandManager::parseClearCommand(const String& command) {
    int day;
    if (sscanf(command.c_str(), "CLEAR=%d", &day) == 1) {
        if (alarmSchedule.clearScheduleForDay(day)) {
            Serial.printf("Programación del día %d eliminada.\n", day);
        } else {
            Serial.println("Error al eliminar programación. Día inválido.");
        }
    } else {
        Serial.println("Formato inválido. Usa: CLEAR=D");
    }
    printLineWithPrompt("");
}

void SerialCommandManager::parseSchedCommand(const String& command) {
    String data = command.substring(6); // elimina "SCHED="

    int day, onHour, onMin, offHour, offMin;
    if (sscanf(data.c_str(), "%d %d:%d %d:%d", &day, &onHour, &onMin, &offHour, &offMin) == 5) {
        if (day < 0 || day > 6) {
            Serial.println("Dia invalido: " + String(day));
            return;
        }
        TimePoint onTime = { (uint8_t)onHour, (uint8_t)onMin };
        TimePoint offTime = { (uint8_t)offHour, (uint8_t)offMin };
        if (alarmSchedule.setScheduleForDay(day, onTime, offTime)) {
            Serial.printf("Dia %d -> ON %02d:%02d, OFF %02d:%02d\n", day, onHour, onMin, offHour, offMin);
        } else {
            Serial.println("Horario invalido.");
        }
    } else {
        Serial.println("Formato invalido. Usa: SCHED=D HH:MM HH:MM");
    }
    printLineWithPrompt("");
}

void SerialCommandManager::parseSetCommand(const String& command) {
    if (command.length() < 15) {
        Serial.println("Comando SET demasiado corto.");
        printLineWithPrompt("");
        return;
    }

    String payload = command.substring(4);
    payload.trim();

    if (payload.indexOf("SET=") != -1) {
        Serial.println("Comando SET malformado: contiene múltiples instrucciones.");
        //printPrompt();
        printLineWithPrompt("");
        return;
    }

    Serial.println("Procesando comando SET: " + payload);

    if (rtc.setDateTime(payload)) {
        Serial.println("Fecha y hora actualizadas.");
    } else {
        Serial.println("Formato inválido. Usa: SET=DD/MM/YYYY HH:MM");
    }
    printLineWithPrompt("");
}
void SerialCommandManager::showAllSchedules() {
//     static const char* days[] = {
//         "Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado"
//     };

//     Serial.println("Programación semanal:");

//     for (int i = 0; i < 7; ++i) {
//         TimePoint on, off;
//         Serial.printf("%-12s: ", days[i]);
//         if (alarmSchedule.isActive(i) && alarmSchedule.getOnOffTime(i, on, off)) {
//             Serial.printf("%02d:%02d - %02d:%02d", on.hour, on.minute, off.hour, off.minute);
//             if (on > off) {
//                 Serial.print(" (día siguiente)");
//             }
//             Serial.println();
//         } else {
//             Serial.println("No programado");
//         }
//     }
//     printLineWithPrompt("");
// }
    static const char* days[] = {
        "Domingo    ", "Lunes      ", "Martes     ", "Miércoles  ", "Jueves     ", "Viernes    ", "Sábado     "
    };

    Serial.println("Programación semanal:");

    for (int i = 0; i < 7; ++i) {
        TimePoint on, off;
        Serial.print(days[i]);
        Serial.print(": ");
        if (alarmSchedule.isActive(i) && alarmSchedule.getOnOffTime(i, on, off)) {
            Serial.printf("%02d:%02d - %02d:%02d", on.hour, on.minute, off.hour, off.minute);
            if (on > off) {
                Serial.print(" (día siguiente)");
            }
            Serial.println();
        } else {
            Serial.println("No programado");
        }
    }
    printLineWithPrompt("");
}

void SerialCommandManager::printLineWithPrompt(const String& line) {
    Serial.println(line);
    printPrompt();
}

void SerialCommandManager::printStatus(const String& label, const String& value) {
    Serial.printf("%s %s\n", label.c_str(), value.c_str());
    //printPrompt();
    printLineWithPrompt("");
}