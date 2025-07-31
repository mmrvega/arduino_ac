#include <IRremote.h> // Include the IRremote library
#include <EEPROM.h>

// IR send pin
const int IR_SEND_PIN = 8; 

// Mode detection pin: GND = heat mode, floating/high = cold mode
const int MODE_PIN = 4;

// Total number of bits
const uint16_t NUMBER_OF_BITS = 104;

void sendMainAndZero(uint32_t* mainData, uint32_t* zeroData) {
  IrSender.sendPulseDistanceWidthFromArray(38, 8950, 4350, 600, 1600, 600, 550, mainData, 104, PROTOCOL_IS_LSB_FIRST, 0, 0);
  delay(186);
  IrSender.sendPulseDistanceWidthFromArray(38, 8850, 4450, 500, 1700, 500, 650, zeroData, 104, PROTOCOL_IS_LSB_FIRST, 0, 0);
}

void setup() {
  Serial.begin(9600);

  pinMode(MODE_PIN, INPUT_PULLUP); // GND = LOW = heat mode

  IrSender.begin(IR_SEND_PIN, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN);

  // Read saved toggle state from EEPROM
  byte lastState = EEPROM.read(0);

  // Define IR codes
  uint32_t tRawDatacold[] = {0xE083C3, 0x802000A0, 0x5A002080, 0x60}; // cold 23
  uint32_t tRawDataheat[] = {0xE0A3C3, 0x808000A0, 0x5A002080, 0xE0}; // heat 28
  uint32_t tRawDataNormal[] = {0x0C4, 0x0, 0x1000000, 0xC5};        // zero (normal)
  uint32_t tRawDataC5[] = {0x5C4, 0x0, 0x1000000, 0xCA};            // C5

  // Select mode based on pin 4
  uint32_t* selectedModeData = digitalRead(MODE_PIN) == LOW ? tRawDataheat : tRawDatacold;

  if (lastState == 0) {
    EEPROM.write(0, 1);  // Next time, use alternate
    Serial.println("NORMAL");

    sendMainAndZero(selectedModeData, tRawDataNormal);
    delay(330000);

    sendMainAndZero(selectedModeData, tRawDataNormal);
    delay(360000);

    sendMainAndZero(selectedModeData, tRawDataNormal);
    delay(5000);

    sendMainAndZero(selectedModeData, tRawDataNormal);
    delay(5000);

    sendMainAndZero(selectedModeData, tRawDataNormal);

  } else {
    EEPROM.write(0, 0);  // Next time, use alternate
    Serial.println("C5");

    sendMainAndZero(selectedModeData, tRawDataC5);
    delay(330000);

    sendMainAndZero(selectedModeData, tRawDataC5);
    delay(360000);

    sendMainAndZero(selectedModeData, tRawDataC5);
    delay(5000);

    sendMainAndZero(selectedModeData, tRawDataC5);
    delay(5000);

    sendMainAndZero(selectedModeData, tRawDataC5);
  }
}

void loop() {
  // Nothing here – only runs once on power-up
}
