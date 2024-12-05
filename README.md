# ESP32 Gamepad with Input Log and Bluetooth Support

## Overview
This project implements a custom gamepad using an **ESP32 microcontroller**, providing simple input functionality for PC games via **Bluetooth**. The gamepad features **directional controls**, **ability keys**, and an **input log display** on an LCD. 

I designed this gamepad specifically to play **Dark Souls™ II: Scholar of the First Sin**, allowing for basic movement, attacks, and item usage in the game. It is also compatible with other games or tasks that support keyboard input. Edit the controls to your likings or needs.

---

## Features
- **Directional Keys**: W, A, S, D for movement.
- **Ability Keys**:
  - **Q**: Focus Enemy
  - **E**: Use Item
  - **Space**: Dodge (single press, cannot be held)
  - **F**: Attack
- **Input Log**: Displays the last 5 button presses on the LCD with spaces between characters for readability.
- **Bluetooth Connection**: Simulates keyboard inputs via Bluetooth, allowing easy integration with PC games.

---

## How It Works
1. **Button Detection**:  
   The ESP32 reads button states and maps them to corresponding key inputs.
   
2. **LCD Input Log**:  
   The last 5 button presses are displayed on the LCD in real-time. For example:
(`_` represents a Space key press).

3. **Bluetooth Transmission**:  
Inputs are sent as keyboard signals to the connected device via Bluetooth.

---

## Hardware Requirements
- **[ability to read](https://www.wikihow.com/Teach-Yourself-to-Read)**
- **ESP32 microcontroller**  
- **LCD Screen** (connected via TX/RX pins)  
- **Push Buttons** (connected to GPIO pins):  
- W: GPIO 16  
- A: GPIO 17  
- S: GPIO 12  
- D: GPIO 19  
- Q: GPIO 13  
- E: GPIO 27  
- Space: GPIO 4  
- F: GPIO 5  

---

## Installation
1. Clone this repository:  
```bash
git clone https://github.com/YannikSand/esp32-gamepad.git





