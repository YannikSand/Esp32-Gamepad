import serial
import pydirectinput
import time

# Initialize serial port (adjust COM port and baud rate as needed)
ser = serial.Serial('COM3', 115200)

# Variables to track the currently pressed keys and their last press time
pressed_keys = set()  
key_last_pressed_time = {}  

# Mapping received data to keypresses
key_map = {
    "a": "a",
    "w": "w",
    "d": "d",
    "s": "s",
    "f": "f",
    "space": "space",  
    "q": "q",
    "e": "e"
}

# Interval to check key release (400 ms)
key_release_interval = 0.4

while True:
    if ser.in_waiting > 0:
        try:
            # Read and decode the input
            data = ser.readline().decode('latin-1').strip()
            print(f"Received from COM3: {data}")  # Debug print of raw input

            # If "null" is received, release all keys
            if data == "null":
                for key in list(pressed_keys):  # Use a copy of the set to modify it while iterating
                    pydirectinput.keyUp(key)  
                    pressed_keys.remove(key)  
                    key_last_pressed_time.pop(key, None)  
                continue  

            # Handle "space" as a special case
            if "space" in data:
                # Simulate a single press for space (not held)
                pydirectinput.keyDown("space") 
                pydirectinput.keyUp("space")  # Release the space key immediately
                print("Space key pressed once") 

                # Remove "space" from the data to avoid double processing
                data = data.replace("space", "")

            # Process other keys
            for key in data:  
                if key in key_map: 
                    mapped_key = key_map[key]

                    if mapped_key not in pressed_keys:
                        pydirectinput.keyDown(mapped_key)  
                        pressed_keys.add(mapped_key)  
                        key_last_pressed_time[mapped_key] = time.time()  # Track the last press time for this key

            # Check for keys to release if they haven't been pressed in 400ms
            for key in list(pressed_keys):  # Use a copy of the set to modify it while iterating
                if time.time() - key_last_pressed_time.get(key, 0) > key_release_interval:
                    pydirectinput.keyUp(key) 
                    pressed_keys.remove(key) 
                    key_last_pressed_time.pop(key, None)  # Remove it from the last pressed time dictionary

        except UnicodeDecodeError as e:
            print(f"Decoding error: {e}")
