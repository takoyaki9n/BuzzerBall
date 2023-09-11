const uint16_t notes[] = {262, 294, 330, 349, 392, 440, 494}; // C4 to B4

const uint8_t pinSpeeker = 0;

void setup() {

}

void loop() {
  for (uint8_t i = 0; i < 5; i++) {
    for (uint8_t j = 0; j < 7; j ++) {
      tone(pinSpeeker, notes[j] << i);
      delay(125);
    }
  }
  noTone(pinSpeeker);
}
