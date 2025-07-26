#include <IRremote.h>

// --- CONFIGURATION: CHOOSE MODE ---
// Uncomment ONE of the following lines to select the mode:
#define RECEIVE_MODE    // Uncomment this to CAPTURE the IR code from your remote
//#define SEND_MODE       // Uncomment this to SEND the captured IR code to your AC
// ---------------------------------

// Pin definitions
const int IR_RECEIVE_PIN = 8; // Pin for the IR receiver's OUT signal
const int IR_SEND_PIN = 3;     // Pin for the IR LED's resistor

// --- IMPORTANT: PASTE YOUR CAPTURED RAW DATA HERE ---
// After running in RECEIVE_MODE, copy the full rawData array from the Serial Monitor
// and paste it below. Make sure to remove the '+' and '-' signs, just list the numbers.
// This example array is just a placeholder; REPLACE IT with your actual AC ON/OFF code.
unsigned int acOnOffRawData[] = {
  // The initial large negative number (-3276750) has been removed.
  // All '+' and '-' signs have been removed.
  8900, 4400, // Leader mark and space
  600, 1600, 600, 1600, 600, 500, 600, 500,
  600, 550, 600, 500, 600, 1600, 600, 1600,
  600, 1600, 600, 1600, 600, 1600, 600, 1600,
  600, 1600, 600, 550, 600, 500, 600, 1600,
  600, 500, 600, 500, 600, 550, 600, 500,
  600, 550, 550, 1600, 600, 1650, 550, 1650,
  550, 550, 600, 500, 600, 500, 600, 550,
  600, 500, 600, 500, 600, 550, 550, 550,
  600, 500, 600, 550, 550, 550, 550, 550,
  600, 500, 600, 1600, 600, 550, 550, 1650,
  550, 550, 600, 500, 600, 550, 550, 550,
  550, 550, 600, 500, 600, 550, 550, 550,
  600, 500, 600, 500, 600, 550, 550, 550,
  600, 500, 600, 1600, 600, 550, 550, 550,
  600, 500, 600, 500, 600, 550, 550, 550,
  600, 500, 600, 550, 550, 550, 550, 550,
  600, 500, 600, 550, 550, 550, 600, 500,
  600, 500, 600, 550, 550, 550, 600, 1600,
  600, 500, 600, 550, 550, 550, 550, 550,
  600, 500, 600, 550, 550, 550, 600, 500,
  600, 550, 550, 550, 550, 550, 600, 500,
  600, 550, 550, 550, 550, 550, 600, 550,
  550, 1650, 550, 550, 550, 1650, 550, 600,
  500, 600, 500, 600, 550, 1650, 550, 550,
  550, 1650, 550, 1700, 500, 1700, 500, 600,
  500, 600, 550, 550, 550, 1650, 550, 1650,
  550 // Last value
};
const int AC_ON_OFF_RAW_LENGTH = sizeof(acOnOffRawData) / sizeof(acOnOffRawData[0]);
// ---------------------------------------------------


void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  Serial.println(F("--- AC ON/OFF Test Code ---"));

#ifdef RECEIVE_MODE
  Serial.println(F("Mode: RECEIVE (Capturing IR codes)"));
  Serial.print(F("Connect IR Receiver OUT to pin: "));
  Serial.println(IR_RECEIVE_PIN);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start receiver, built-in LED blinks on reception
#endif

#ifdef SEND_MODE
  Serial.println(F("Mode: SEND (Transmitting IR code)"));
  Serial.print(F("Connect IR LED Anode (via resistor) to pin: "));
  Serial.println(IR_SEND_PIN);
  if (AC_ON_OFF_RAW_LENGTH == 0) {
    Serial.println(F("ERROR: acOnOffRawData array is empty! Please capture code first."));
    while (true); // Halt execution if no code is defined
  }
  Serial.print(F("Ready to send code with length: "));
  Serial.println(AC_ON_OFF_RAW_LENGTH);
  // You need to initialize the sender! This line was commented out in your provided code.
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);
#endif
}

void loop() {
#ifdef RECEIVE_MODE
  // Check if an IR signal has been received and decoded
  if (IrReceiver.decode()) {
    Serial.println(F("\n--- IR Signal Captured! ---"));
    Serial.println(F("Copy the 'rawData' array below and paste it into 'acOnOffRawData' in SEND_MODE."));
    Serial.println(F("---------------------------"));

    // Print the raw timing data, which is essential for AC remotes
    IrReceiver.printIRResultRawFormatted(&Serial, true); // 'true' prints as uint16_t array

    // Also print a short summary for general info
    IrReceiver.printIRResultShort(&Serial);

    // Resume receiving for the next signal
    IrReceiver.resume();
  }
#endif

#ifdef SEND_MODE
  Serial.println(F("\nSending AC ON/OFF command..."));
  // Send the raw data. 38 kHz is the standard frequency for most IR devices.
  IrSender.sendRaw(acOnOffRawData, AC_ON_OFF_RAW_LENGTH, 38);
  Serial.println(F("Command sent."));

  // Wait before sending again. Adjust this delay as needed.
  // For a real application, you'd trigger this with a button press or other event.
  delay(5000); // Send every 5 seconds for testing
#endif
}
