#include "BluetoothSerial.h"
#include <SoftwareSerial.h>

BluetoothSerial SerialBT;
SoftwareSerial OpenLCD(0, 21); // Set the pins for the LCD (0 is RX, 21 is TX)

// Define pins for buttons
const int buttonAPin = 17;
const int buttonWPin = 16;
const int buttonDPin = 19;
const int buttonSPin = 12;
const int buttonFPin = 5;
const int buttonQPin = 13;
const int buttonSpacePin = 4;
const int buttonEPin = 27;

// Input log variables
String inputLog = "";       // Stores the last 5 inputs
const int maxLogSize = 5;   // Maximum number of characters in the log
String lastPressedKey = ""; // Tracks the last key added to avoid duplicates

bool lastConnectionStatus = false;  // Track last known connection status
unsigned long lastButtonPressTime = 0;  // Track time of last button press
const unsigned long nullTimeout = 200;  // Timeout for sending null in milliseconds

void setup() {
  Serial.begin(115200);  // Initialize serial monitor
  OpenLCD.begin(9600);   // Initialize LCD

  // Start Bluetooth
  if (!SerialBT.begin("ESP32 GamePad", true)) {
    Serial.println("Bluetooth initialization failed.");
    OpenLCD.print("Error: Not ready");
  } else {
    Serial.println("Bluetooth initialized successfully.");
    OpenLCD.print("Ready");  // Initial message before pairing
  }

  // Set up buttons as inputs with pull-up resistors
  pinMode(buttonAPin, INPUT_PULLUP);
  pinMode(buttonWPin, INPUT_PULLUP);
  pinMode(buttonDPin, INPUT_PULLUP);
  pinMode(buttonQPin, INPUT_PULLUP);
  pinMode(buttonSPin, INPUT_PULLUP);
  pinMode(buttonFPin, INPUT_PULLUP);
  pinMode(buttonSpacePin, INPUT_PULLUP);
  pinMode(buttonEPin, INPUT_PULLUP);

  // Clear the LCD on startup
  clearLCD();
}

void loop() {
  // Store keys to send
  String keysToSend = "";

  // Check each button state and determine which key to add
  if (digitalRead(buttonAPin) == LOW) {
    keysToSend = "a";
  } else if (digitalRead(buttonWPin) == LOW) {
    keysToSend = "w";
  } else if (digitalRead(buttonQPin) == LOW) {
    keysToSend = "q";
  } else if (digitalRead(buttonSPin) == LOW) {
    keysToSend = "s";
  } else if (digitalRead(buttonDPin) == LOW) {
    keysToSend = "d";
  } else if (digitalRead(buttonFPin) == LOW) {
    keysToSend = "f";
  } else if (digitalRead(buttonSpacePin) == LOW) {
    keysToSend = "space";
  } else if (digitalRead(buttonEPin) == LOW) {
    keysToSend = "e";
  }

  // Only update the log if a new key is pressed
  if (keysToSend != "" && keysToSend != lastPressedKey) {
    lastPressedKey = keysToSend;   // Update the last pressed key
    updateInputLog(keysToSend);   // Update the input log
    sendKey(keysToSend);          // Send the key
    lastButtonPressTime = millis();  // Update the time of the last button press
  }

  // If no keys are pressed and the timeout has passed, send "null"
  if (millis() - lastButtonPressTime > nullTimeout && keysToSend == "") {
    sendKey("null");  // Send "null" if no button is pressed within the timeout
    lastPressedKey = "";  // Reset last pressed key when no key is pressed
  }

  // Small delay to prevent overloading the CPU
  delay(10);
}

// Update the input log and display it on the LCD
void updateInputLog(const String& key) {
  // Map "space" to "_" for better display
  String logKey = key == "space" ? "_" : key;

  // Add the new input to the log
  inputLog += logKey;

  // Ensure the log doesn't exceed max size
  if (inputLog.length() > maxLogSize) {
    inputLog = inputLog.substring(inputLog.length() - maxLogSize);
  }

  // Format the log with spaces between characters
  String formattedLog = "";
  for (int i = 0; i < inputLog.length(); i++) {
    formattedLog += inputLog[i];
    if (i < inputLog.length() - 1) {
      formattedLog += " "; // Add a space between characters
    }
  }

  // Update the LCD
  updateLCD(formattedLog);
}

// Send key press and update the last button press time
void sendKey(const String& keys) {
  static String lastSentKeys = "";

  // Only send the keys if they are different from the last sent keys
  if (keys != lastSentKeys) {
    SerialBT.println(keys);  // Send multiple keys if pressed
    Serial.println(keys);    // Print the keys to the serial monitor for debugging
    lastSentKeys = keys;     // Update the last sent keys
  }
}

// Clear the LCD
void clearLCD() {
  OpenLCD.write(0xFE);  // Command flag
  OpenLCD.write(0x01);  // Clear screen
  delay(10);            // Small delay to let the LCD clear
}

// Update the LCD with the current log
void updateLCD(const String& log) {
  clearLCD();              // Clear the LCD before writing
  OpenLCD.write(0xFE);     // Command to set cursor position
  OpenLCD.write(0x80);     // Set cursor to the beginning of the first line
  OpenLCD.print(log);      // Print the formatted log to the LCD
}
