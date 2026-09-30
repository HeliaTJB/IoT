# ESP8266 Smart Relay System

An evolving ESP8266-based IoT relay control system developed through several stages of experimentation and implementation.

The project gradually combines local hardware control with wireless communication, remote control, persistent configuration, scheduling, and remote firmware updates.

## Evolution

The project was developed incrementally through several versions.

### V1 — Relay Class

The first version introduces a class-based structure for controlling relays and managing relay-related functionality.

### V2 — Class-Based System

The next stage expands the class-based architecture and integrates additional components such as:

* EEPROM
* RF remote control
* Relay management
* Display
* Wi-Fi communication

### V3 — Full System

The system was further extended with:

* ESP8266 Wi-Fi
* HTTPS communication
* REST-style API communication
* LittleFS
* JSON configuration
* ArduinoOTA
* EEPROM
* RF remote control
* TM1637 display
* Time-based functionality

### V4 — Remote Relay

This version focuses on remote-control functionality and the integration of RF remote commands with relay operation.

### V5 — Cloud-Connected Relay

The later version integrates the relay controller with a remote server through HTTPS communication.

The system maintains device configuration locally while communicating with a backend service to exchange device state and control information.

## Main Features

* ESP8266-based controller
* Multiple relay control
* RF remote control
* Wi-Fi connectivity
* HTTPS communication
* Remote server communication
* JSON configuration
* LittleFS persistent storage
* EEPROM
* OTA firmware updates
* NTP-based time synchronization
* Scheduled relay operation
* TM1637 display

## Architecture

```text
                 Remote Server
                      │
                 HTTPS / API
                      │
                      ▼
              ┌───────────────┐
              │    ESP8266    │
              │               │
              │ Wi-Fi / HTTPS │
              │     OTA       │
              │   JSON Config │
              │    LittleFS   │
              └───────┬───────┘
                      │
             ┌────────┼────────┐
             │        │        │
             ▼        ▼        ▼
          Relays    RF Remote  Display
                      │
                      ▼
                 Local Control
```

## Technologies

* ESP8266
* C/C++
* Arduino
* Wi-Fi
* HTTPS
* REST APIs
* ArduinoOTA
* LittleFS
* EEPROM
* ArduinoJson
* RCSwitch
* TM1637

## Development Approach

The different directories represent development stages rather than completely independent projects. Earlier versions are preserved to document the progression of the system and the evolution of its architecture.
