#include <IRremote.h> // Include the IRremote library

// Define the digital pin where your IR LED's resistor is connected
const int IR_SEND_PIN = 3; 

// --- The Encoded Raw Data for 104 Bits ---
// Define the total number of bits in the payload
const uint16_t NUMBER_OF_BITS = 104; // Use uint16_t for clarity and type matching

void setup() {
  Serial.begin(9600); // Initialize serial communication for debugging
  Serial.println(F("--- Sending Specific Pulse Distance Width Code (IRremote 4.4.2) ---"));
  Serial.print(F("IR LED connected to pin: "));
  Serial.println(IR_SEND_PIN);

  // Initialize the IR sender with LED feedback (built-in LED will blink during transmission)
  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);

  Serial.println(F("IR Sender initialized."));
  Serial.print(F("Prepared to send "));
  Serial.print(NUMBER_OF_BITS);
  Serial.println(F(" bits of custom Pulse Distance Width code."));
}

void loop() {
  Serial.println(F("\nTransmitting command..."));

 // uint32_t tRawData[]={0x2C4, 0x0, 0x1000000, 0xC7};// c0 = (0x0c4,0xc5),c1 = (0x1c4,0xc6)for c2 = (0x2c4 , 0xc7) for c3 = (0x3c4, 0xc8) for c4 = (0x4c4, 0xc9)for c5 = (0x5c4,0xca)
 //   IrSender.sendPulseDistanceWidthFromArray(38, 8850, 4450, 500, 1700, 500, 650, &tRawData[0], 104, PROTOCOL_IS_LSB_FIRST, 0, 0);
    uint32_t tRawData[]={0x0C4, 0x0, 0x1000000, 0xC5};
    IrSender.sendPulseDistanceWidthFromArray(38, 8850, 4450, 500, 1700, 500, 650, &tRawData[0], 104, PROTOCOL_IS_LSB_FIRST, 0, 0);


  Serial.println(F("Command sent. Waiting 5 seconds before next send..."));

  // Pause for a longer period before repeating the entire sequence for testing.
  delay(5000); 
}
