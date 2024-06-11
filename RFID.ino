#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN = D3;     // Configurable, see typical pin layout above
constexpr uint8_t SS_PIN = D4;      // Configurable, see typical pin layout above

MFRC522 rfid(SS_PIN, RST_PIN);      // Instance of the class

void setup() {
  Serial.begin(9600);
  SPI.begin();          // Init SPI bus
  rfid.PCD_Init();      // Init MFRC522
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if (!rfid.PICC_ReadCardSerial()) {
    return;
  }

  // Reset the tag variable
  String tag = "";

  // Read the UID
  for (byte i = 0; i < rfid.uid.size; i++) {
    tag += String(rfid.uid.uidByte[i] < 0x10 ? "0" : ""); // Add leading zero if needed
    tag += String(rfid.uid.uidByte[i], HEX);             // Convert byte to hex string
  }
  tag.toUpperCase(); // Convert to upper case to ensure consistency
  Serial.println(tag);

  // Halt PICC
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  // Compare the tag
  if (tag == "83727430") { // Example tag value, replace with your actual tag value
    Serial.println("Access granted");
  } else {
    Serial.println("Access denied");
  }
}
