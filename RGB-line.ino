#include <analogWrite.h>
#include "YandexVars.h"

#define RED_PIN 27
#define GREEN_PIN 14
#define BLUE_PIN 16
#define HOME_PIN 5
#define CAVE_PIN 12
#define TESLA_PIN 13
#define CIRCUS_PIN 26
#define BUILDING_PIN 25
#define FILM_PIN 17

int OUT_PINS[] = {
  LED_BUILTIN,
  RED_PIN, GREEN_PIN, BLUE_PIN,
  HOME_PIN, CAVE_PIN, TESLA_PIN,
  CIRCUS_PIN, BUILDING_PIN, FILM_PIN
};
int INP_PINS[] = {};

void setup() {
  Serial.begin(9600);
  for (int pin : OUT_PINS) {
    pinMode(pin, OUTPUT);
  }
  for (int pin : INP_PINS) {
    pinMode(pin, INPUT);
  }
  digitalWrite(HOME_PIN, HIGH);
  digitalWrite(CAVE_PIN, HIGH);
  digitalWrite(TESLA_PIN, HIGH);
  digitalWrite(CIRCUS_PIN, HIGH);
  digitalWrite(BUILDING_PIN, HIGH);
  digitalWrite(FILM_PIN, HIGH);
  send_signal(10, 200);
  set_process(100);
  WiFiConnect(ssid, password);
}

void set_process(int progress) {
  if (progress < 0) {
    progress = 0;
  } else if (progress > 100) {
    progress = 100;
  }
  analogWrite(RED_PIN, 100 - progress);
  analogWrite(GREEN_PIN, progress);
}

void send_signal(int am, int del) {
  for (int i = 0; i < am; ++i) {
    digitalWrite(BLUE_PIN, HIGH);
    delay(del / 2);
    digitalWrite(BLUE_PIN, LOW);
    delay(del / 2);
  }
}

void send_signal() {
  send_signal(4, 50);
}

int get_int(String num) {
  return num.charAt(0) - 48;
}


YandexVariable vars = YandexVariable(token);

void loop() {
//  digitalWrite(BUILDING_PIN, HIGH);
//  delay(1000);
//  digitalWrite(BUILDING_PIN, LOW);
//  delay(1000);
  delay(1000);
  vars.reconnect();
  String data = vars.get("rover_progress");
  Serial.println(get_int(data) * 100 / 14);
  Serial.println(data);
  if (data == "1" || data == "8") {
    String location = vars.get("rover_city");
    int value = HIGH;
    if (data == "1") {
      value = LOW;
    }
    if (location == "HOME") {
      digitalWrite(HOME_PIN, value);
    } else if (location == "CAVE") {
      digitalWrite(CAVE_PIN, value);
    } else if (location == "TESLA") {
      digitalWrite(TESLA_PIN, value);
    } else if (location == "CIRCUS") {
      digitalWrite(CIRCUS_PIN, value);
    } else if (location == "BUILDING") {
      digitalWrite(BUILDING_PIN, value);
    } else if (location == "FILM") {
      digitalWrite(FILM_PIN, value);
    }
    send_signal(10, 200);
    if (data == "1") {
      vars.set("rover_progress", "2");
    } else {
      vars.set("rover_progress", "9");
    }
  }
  set_process(get_int(data) * 100 / 14);
}
