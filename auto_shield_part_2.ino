#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

float pixels[64];

const int smokeSensor1 = 2;
const int smokeSensor2 = 3;
const int smokeSensor3 = 4;

const int relay1 = 8;
const int relay2 = 9;
const int relay3 = 10;

const float tempThreshold = 50.0;

void setup() {
  Serial.begin(9600);
  Serial.println(F("Smoke and Temperature Detection System"));

  if (!amg.begin()) {
    Serial.println("Could not find a valid AMG8833 sensor, check wiring!");
    while (1);
  }

  pinMode(smokeSensor1, INPUT);
  pinMode(smokeSensor2, INPUT);
  pinMode(smokeSensor3, INPUT);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  
  delay(100);
}

void loop() {
  bool smokeDetected = (digitalRead(smokeSensor1) == HIGH) || 
                       (digitalRead(smokeSensor2) == HIGH) || 
                       (digitalRead(smokeSensor3) == HIGH);

  amg.readPixels(pixels);
  float maxTemp = getMaxTemp(pixels);

  if (smokeDetected && maxTemp >= tempThreshold) {
    Serial.println("Smoke detected and temperature >= 50°C. Turning off all relays.");
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
  } else {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
  }

  delay(1000);
}

float getMaxTemp(float pixels[64]) {
  float maxTemp = pixels[0];
  for (int i = 1; i < 64; i++) {
    if (pixels[i] > maxTemp) {
      maxTemp = pixels[i];
    }
  }
  return maxTemp;
}
