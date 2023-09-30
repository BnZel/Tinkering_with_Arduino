#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"
#include "ezButton.h"
#include "FastLED.h"

#define SD_CHIP_SELECT_PIN 4

#define LED_TYPE WS2812B
#define NUM_LEDS_PER_STRIP 6
#define LED_STRIP_PIN 6

TMRpcm tmrpcm;
CRGB leds[NUM_LEDS_PER_STRIP];

ezButton btn1(8);
ezButton btn2(7);
ezButton btn3(5);
ezButton btn4(3);

uint8_t hue = 0;

void setup() {

  Serial.begin(9600);
  Serial.println("starting...");

  tmrpcm.speakerPin = 9;

  FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN>(leds, NUM_LEDS_PER_STRIP);

  btn1.setDebounceTime(50);
  btn2.setDebounceTime(50);
  btn3.setDebounceTime(50);
  btn4.setDebounceTime(50);

  if (!SD.begin(SD_CHIP_SELECT_PIN)) {
    Serial.println("SD fail");
    return;
  }

  Serial.println("SD works");

  tmrpcm.setVolume(5);
  tmrpcm.quality(1);

  delay(3000);

  tmrpcm.play("Intro.wav");

  delay(3000);

  tmrpcm.play("el.wav");
}

void loop() {

  btn1.loop();
  btn2.loop();
  btn3.loop();
  btn4.loop();

  int btn1State = btn1.getState();
  int btn2State = btn2.getState();
  int btn3State = btn3.getState();
  int btn4State = btn4.getState();

  if (btn1.isPressed()) {
    Serial.println("playing 1.wav");
    tmrpcm.play("1.wav");

    displayLED(leds, CRGB::Red);
  }

  if (btn2.isPressed()) {
    Serial.println("playing 9.wav");
    tmrpcm.play("9.wav");

    rainbowLEDS();
  }

  if (btn3.isPressed()) {
    Serial.println("playing 12.wav");
    tmrpcm.play("12.wav");

    displayLED(leds, CRGB::DarkRed);
  }

  if (btn4.isPressed()) {
    Serial.println("clearing sound & leds");

    tmrpcm.pause();
    fill_solid(leds, NUM_LEDS_PER_STRIP, CRGB::Black);
    FastLED.show();
  }
}


void displayLED(CRGB leds[], CRGB colour) {
  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    leds[i] = colour;
    FastLED.show();
  }
}

void rainbowLEDS() {
  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
    leds[i] = CHSV(hue + (i * 10), 255, 255);
  }

  EVERY_N_MILLISECONDS(15) {
    hue++;
  }

  FastLED.show();
}
