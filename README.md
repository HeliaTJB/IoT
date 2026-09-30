# IoT & Embedded Systems Projects

[![Platform](https://img.shields.io/badge/Platform-Arduino%20%7C%20ESP32%20%7C%20ESP8266-blue)](#technologies)
[![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-orange)](#technologies)
[![IoT](https://img.shields.io/badge/Focus-IoT%20%26%20Embedded%20Systems-green)](#areas-of-focus)
[![Status](https://img.shields.io/badge/Status-Active-brightgreen)](#repository-status)

A collection of selected projects and experiments developed while exploring **embedded systems, wireless communication, and Internet of Things (IoT)**.

This repository documents hands-on work with microcontrollers and connected embedded devices, ranging from fundamental hardware experiments to networked IoT systems involving sensors, remote control, persistent storage, and OTA firmware updates.

---

## Table of Contents

* [Areas of Focus](#areas-of-focus)
* [Technologies](#technologies)
* [Project Overview](#project-overview)
* [Repository Structure](#repository-structure)
* [Selected Projects](#selected-projects)
* [Development Approach](#development-approach)
* [Repository Status](#repository-status)

---

## Areas of Focus

* Embedded Systems
* Internet of Things (IoT)
* Wireless Communication
* Microcontroller Programming
* Sensor & Actuator Integration
* Networked Embedded Systems
* Remote Device Control
* Persistent Data Storage
* Firmware Management

---

## Technologies

### Platforms

![Arduino](https://img.shields.io/badge/Arduino-00979D?logo=arduino\&logoColor=white)
![ESP32](https://img.shields.io/badge/ESP32-E7352C?logo=espressif\&logoColor=white)
![ESP8266](https://img.shields.io/badge/ESP8266-000000?logo=espressif\&logoColor=white)

### Programming

![C](https://img.shields.io/badge/C-A8B9CC?logo=c\&logoColor=black)
![C++](https://img.shields.io/badge/C%2B%2B-00599C?logo=cplusplus\&logoColor=white)
![Arduino Framework](https://img.shields.io/badge/Arduino%20Framework-00979D?logo=arduino\&logoColor=white)

### Communication & Networking

![WiFi](https://img.shields.io/badge/Wi--Fi-802.11-blue)
![HTTP](https://img.shields.io/badge/HTTP-REST-lightgrey)
![HTTPS](https://img.shields.io/badge/HTTPS-TLS-green)
![TCP/IP](https://img.shields.io/badge/TCP%2FIP-Networking-orange)
![RF](https://img.shields.io/badge/RF-433%20MHz-purple)

### Embedded Technologies

![EEPROM](https://img.shields.io/badge/Storage-EEPROM-yellow)
![LittleFS](https://img.shields.io/badge/Storage-LittleFS-blueviolet)
![OTA](https://img.shields.io/badge/Firmware-OTA-red)
![JSON](https://img.shields.io/badge/Data-JSON-lightgrey)

---

## Project Overview

| Section                       | Platform | Main Topics                                          |
| ----------------------------- | -------- | ---------------------------------------------------- |
| **01 · Arduino Basics**       | Arduino  | GPIO, displays, hardware interfacing                 |
| **02 · ESP32 IoT**            | ESP32    | Wi-Fi, HTTP/HTTPS, sensors, remote control           |
| **03 · ESP32 Networking**     | ESP32    | Network servers, TCP/IP, client-server communication |
| **04 · ESP8266 Storage**      | ESP8266  | EEPROM, LittleFS, file storage, web interfaces       |
| **05 · ESP8266 Advanced IoT** | ESP8266  | Relay control, RF, Wi-Fi, APIs, JSON, OTA            |

---

## Repository Structure

```text
iot-projects/
│
├── 01-arduino-basics/
│   ├── 7segment/
│   ├── p6/
│   ├── p7/
│   ├── p8/
│   ├── p9/
│   └── ...
│
├── 02-esp32-iot/
│   ├── led-http/
│   ├── temperature-http/
│   ├── touch-http/
│   ├── rgb-http/
│   ├── p10/
│   ├── p11-remote-led/
│   └── final-project/
│
├── 03-esp32-networking/
│   └── network-server/
│
├── 04-esp8266-storage/
│   ├── eeprom/
│   ├── littlefs-web-server/
│   └── littlefs-file-upload/
│
└── 05-esp8266-advanced-iot/
    ├── v1-relay-class/
    ├── v2-class-architecture/
    ├── v3-full-class-system/
    ├── v4-networked-system/
    ├── v5-rf-relay/
    └── v6-integrated-system/
```

---

## Selected Projects

### ESP32 IoT Projects

A collection of ESP32-based experiments exploring wireless connectivity and network-connected hardware.

Projects cover:

* Temperature monitoring
* Touch sensing
* LED and RGB control
* HTTP/HTTPS communication
* Remote hardware control
* Network-connected displays

---

### ESP8266 Storage

Experiments focused on persistent data storage and file management on ESP8266.

Topics include:

* EEPROM read/write operations
* LittleFS
* File management
* Embedded web interfaces
* File upload and storage

---

### ESP8266 Advanced IoT System

A larger ESP8266-based system developed incrementally through multiple stages.

The system combines:

* Relay control
* RF remote control
* Wi-Fi connectivity
* HTTP/HTTPS communication
* REST-style APIs
* JSON configuration
* EEPROM
* LittleFS
* OTA firmware updates
* Time synchronization
* Scheduled operations
* TM1637 display integration

The different versions are preserved to document the evolution of the system's architecture and functionality.

---

## Development Approach

The projects in this repository follow an incremental approach to embedded and IoT development.

The work progresses from fundamental hardware interaction and microcontroller programming toward more integrated systems involving:

```text
Hardware
   ↓
Microcontroller Programming
   ↓
Sensors & Actuators
   ↓
Wi-Fi Connectivity
   ↓
Network Communication
   ↓
Persistent Storage
   ↓
Remote Control
   ↓
Integrated IoT Systems
```

This structure reflects the progression of experimentation and development across different embedded platforms.

---

## Repository Status

This repository is an evolving collection of selected academic, experimental, and personal projects.

Some projects are complete implementations, while others are smaller experiments developed to explore a specific hardware component, communication protocol, or embedded-system concept.

New projects and previously developed code will be added and documented over time.

---

## Notes

Some projects may require specific hardware components and Arduino libraries.

Sensitive information such as Wi-Fi credentials, API keys, and private configuration values should be replaced with example values before deployment.
