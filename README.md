# ESP32 Hotspot Prank - Fake Public WiFi Portal

![ESP32](https://img.shields.io/badge/ESP32-WROOM-blue) 
![PlatformIO](https://img.shields.io/badge/PlatformIO-Compatible-orange) 
![License](https://img.shields.io/badge/License-MIT-green)
![SPIFFS](https://img.shields.io/badge/SPIFFS-4MB-yellow)

A realistic fake public WiFi hotspot simulator using ESP32 that automatically shows a captive portal when devices connect, complete with sound effects and professional-looking login page.

## 📋 Table of Contents
- [Features](#-features)
- [Requirements](#-requirements)
- [Installation](#-installation)
- [Upload Instructions](#-upload-instructions)
- [Customization](#-customization)
- [Troubleshooting](#-troubleshooting)
- [Project Structure](#-project-structure)
- [License](#-license)

## ✨ Features
- 📶 Open WiFi hotspot with captive portal
- 🔔 Automatic popup on Android/iOS/Windows
- 🎵 Sound effect playback on button click
- 🖼️ Custom logo support
- 📱 Mobile-responsive design
- 🛡️ No internet connection (pure prank)

## 🧰 Requirements
### Hardware
- ESP32 board (ESP32-WROOM recommended)
- Micro USB cable
- 4MB+ flash recommended

### Software
- [VS Code](https://code.visualstudio.com/)
- [PlatformIO](https://platformio.org/)
- Audio editor (Audacity recommended)

## Project Tree
    
    esp32-hotspot-prank/
    ├── data/                   # Files for SPIFFS
    │   ├── sound.wav           # Prank sound effect
    │   └── logo.png            # Custom logo
    ├── lib/                    # Additional libraries
    ├── src/
    │   └── main.cpp            # Main application code
    ├── platformio.ini          # PlatformIO configuration
    ├── partitions.csv          # Partition table

## 📥 Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/captvin/hotspot-prank.git
   cd hotspot-prank

2. Flash filesystem:
   ```bash
   pio run --target uploadfs

3. Flash Coding:
   ```bash
   pio run --target upload