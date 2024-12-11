#include "BluetoothSerial.h"
#include <SoftwareSerial.h>

BluetoothSerial SerialBT;
SoftwareSerial OpenLCD(0, 21); // Set the pins for the LCD (0 is RX, 21 is TX)

const int buttonAPin = 17;
const int buttonWPin = 16;
const int buttonDPin = 19;
const int buttonSPin = 12;
const int buttonFPin = 5;
const int buttonQPin = 13;
const int buttonSpacePin = 4;
const int buttonEPin = 27;

String inputLog = "";       // Stores the last 5 inputs
const int maxLogSize = 5;   // Maximum number of characters in the log
String lastPressedKey = ""; // Tracks the last key sent to avoid duplicates

unsigned long lastButtonPressTime = 0;  // Time of last button press
const unsigned long nullTimeout = 200; // Timeout for sending "null" in milliseconds

void setup() {
  Serial.begin(115200);
  OpenLCD.begin(9600);

  if (!SerialBT.begin("ESP32 GamePad", true)) {
    Serial.println("Bluetooth initialization failed.");
    OpenLCD.print("Error: Not ready");
  } else {
    Serial.println("Bluetooth initialized successfully.");
    OpenLCD.print("Ready");
  }

  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonWPin, INPUT_PULLUP);
  pinMode(buttonDPin, INPUT_PULLUP);
  pinMode(buttonQPin, INPUT_PULLUP);
  pinMode(buttonSPin, INPUT_PULLUP);
  pinMode(buttonFPin, INPUT_PULLUP);
  pinMode(buttonSpacePin, INPUT_PULLUP);
  pinMode(buttonEPin, INPUT_PULLUP);

  clearLCD();
}

void loop() {
  String keysToSend = "";

  // Check button states for simultaneous presses
  bool isWPressed = digitalRead(buttonWPin) == LOW;
  bool isSpacePressed = digitalRead(buttonSpacePin) == LOW;
  bool isAPressed = digitalRead(buttonAPin) == LOW;
  bool isEPressed = digitalRead(buttonEPin) == LOW;
  bool isDPressed = digitalRead(buttonDPin) == LOW;
  bool isFPressed = digitalRead(buttonFPin) == LOW;
  bool isQPressed = digitalRead(buttonQPin) == LOW;
  bool isSPressed = digitalRead(buttonSPin) == LOW;

  // Combine the keys pressed (check for multiple simultaneous presses)
  if (isWPressed) {
    keysToSend += "w";
  }
  if (isSpacePressed) {
    keysToSend += "space";  // Space will be logged for Bluetooth but not for LCD
  }
  if (isAPressed) {
    keysToSend += "a";
  }
  if (isEPressed) {
    keysToSend += "e";
  }
  if (isDPressed) {
    keysToSend += "d";
  }
  if (isFPressed) {
    keysToSend += "f";
  }
  if (isQPressed) {
    keysToSend += "q";
  }
  if (isSPressed) {
    keysToSend += "s";
  }

  // If multiple keys are pressed together, combine them into one string (like 'aspace', 'wf', etc.)
  if (keysToSend != "" && keysToSend != lastPressedKey) {
    lastPressedKey = keysToSend;
    updateInputLog(keysToSend);  // Update the log for the LCD (space excluded)
    sendKey(keysToSend);         // Send the key to Bluetooth (space included)
    lastButtonPressTime = millis();
  }

  // Handle "null" timeout
  if (millis() - lastButtonPressTime > nullTimeout && keysToSend == "") {
    sendKey("null");
    lastPressedKey = ""; // Reset the last pressed key to allow repeats
  }

  delay(10); // Small delay to avoid overloading the CPU
}

// Update the input log and display it on the LCD
void updateInputLog(const String& key) {
  String logKey;

  // Exclude space from the LCD log (space will not be printed on the LCD)
  if (key == "space") {
    logKey = "_";  // Represent space as "_" in the LCD log
  } else {
    logKey = key;  // Log the key normally for other characters
  }

  // Add to log and maintain max size
  inputLog += logKey;
  if (inputLog.length() > maxLogSize) {
    inputLog = inputLog.substring(inputLog.length() - maxLogSize);
  }

  // Format the log for LCD (only non-space characters)
  String formattedLog = "";
  for (int i = 0; i < inputLog.length(); i++) {
    formattedLog += inputLog[i];
    if (i < inputLog.length() - 1) {
      formattedLog += " ";  // Add space only between characters, not on LCD
    }
  }

  updateLCD(formattedLog);
}

// Send key press over Bluetooth
void sendKey(const String& keys) {
  static String lastSentKeys = "";

  if (keys != lastSentKeys) {
    SerialBT.println(keys);  // Send the key combination to Bluetooth
    Serial.println(keys);    // Print to the serial monitor for debugging
    lastSentKeys = keys;     // Keep track of the last sent keys
  }
}

// Clear the LCD
void clearLCD() {
  OpenLCD.write(0xFE);
  OpenLCD.write(0x01);
  delay(10);
}

// Update the LCD with the formatted log
void updateLCD(const String& log) {
  clearLCD();
  OpenLCD.write(0xFE);
  OpenLCD.write(0x80);
  OpenLCD.print(log);  // Print the log to the LCD (no space shown)
}
