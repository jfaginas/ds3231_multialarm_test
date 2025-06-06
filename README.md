# ESP32 + RTC DS3231 - Reloj con alarmas semanal programada por terminal

Un ESP32 te permite programar, usando la terminal, múltiples alarmas semanales con horarios personalizados por día, utilizando un reloj de tiempo real DS3231 y un LED indicador.
Ideal como base para sistemas de automatización más complejos, especialmente con pantallas táctiles como NEXTION.
Es una versión mejorada del proyecto ds3231_test.

🔧 Características

    ⏰ Programación de intervalos de encendido/apagado para cada día de la semana.

    🌙 Soporte para alarmas que cruzan la medianoche.

    🧠 Lógica no bloqueante orientada a objetos.

    ⌨️ Interfaz por terminal serie con validación de entrada y timeout.

    🔎 Impresión periódica del estado si no hay interacción del usuario.

    🧪 Preparado para integrarse en sistemas más complejos (NEXTION, ESP-NOW, etc.).

🧱 Estructura del proyecto

```text
ds3231_multialarm_test/
├── lib/
│   ├── AlarmSchedule/
│   ├── DS3231Manager/
│   ├── LedManager/
│   └── SerialCommandManager/
├── src/
│   └── main.cpp
├── platformio.ini
└── README.md
```
## 🔌 Conexiones

---------------------------------------------------------
| Componente | ESP32 GPIO                               |
|------------|------------------------------------------|
| DS3231 SDA | GPIO21                                   |
| DS3231 SCL | GPIO22                                   |
| LED        | Interno (GPIO2 en la mayoría de módulos) |
---------------------------------------------------------

📦 Dependencias

    ESP32 (probado en devkit v1)

    DS3231 (I2C)

    Plataforma: PlatformIO

    Librerías usadas:

        RTClib para manejar el DS3231

        Wire (incluida con el core del ESP32)

⚙️ Comandos disponibles (por terminal serie)

    SET=dd/mm/yyyy hh:mm: actualiza la fecha y hora del sistema.

    SCHED=d hh1:mm1 hh2:mm2: programa una alarma para el día d (0=lunes...6=domingo).

    CLEAR=d: desactiva la alarma del día d.

    SHOW: muestra la programación semanal actual.

🧠 Lógica de las alarmas

La clase AlarmSchedule permite programar intervalos por día. Si un intervalo va, por ejemplo, de 23:00 a 01:00, el sistema lo interpreta correctamente como un intervalo que cruza la medianoche, incluso verificando el día anterior si es necesario.

🔍 Ejemplo de uso

    > SET=06/06/2025 23:58
    Fecha y hora actualizadas.

    > SCHED=5 23:59/00:02
    Programación actualizada.

    > SHOW
    Domingo    : No programado
    Lunes      : No programado
    Martes     : No programado
    Miércoles  : No programado
    Jueves     : No programado
    Viernes    : 23:59 - 00:02
    Sábado     : No programado

📡 Futuras mejoras

- Integración con pantalla NEXTION.
- Persistencia en memoria flash o EEPROM.
- Control de dispositivos reales en lugar de LED.

## 📖 Licencia

Este proyecto está licenciado bajo la licencia MIT.

## 📁 Proyecto
Desarrollado en PlatformIO con Visual Studio Code, por José Faginas como ejercicio de aprendizaje con ESP32 + DS3231.