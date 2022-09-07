#include <ESP32Servo.h>
#include "YandexVars.h"

#define LED_BUILTIN 2
#define SERVO_ROTATE_PIN 12
#define SERVO_ELEV_1_PIN 13
#define SERVO_ELEV_2_PIN 5
#define SERVO_TAKE_PIN 23

int OUT_PINS[] = {
  LED_BUILTIN,
  SERVO_ROTATE_PIN,
  SERVO_ELEV_1_PIN,
  SERVO_ELEV_2_PIN,
  SERVO_TAKE_PIN,
  18
};
int INP_PINS[] = {
  19
};


Servo
SERVO_ROTATE,
SERVO_ELEV_1,
SERVO_ELEV_2,
SERVO_TAKE
;

void rotate_servo(int pin, int angle) {
  Serial.println(String("") + pin + ": " + angle);
  if (pin == SERVO_ROTATE_PIN) {
    SERVO_ROTATE.write(angle);
  } else if (pin == SERVO_ELEV_1_PIN) {
    SERVO_ELEV_1.write(180 - angle);
  } else if (pin == SERVO_ELEV_2_PIN) {
    SERVO_ELEV_2.write(angle);
  } else if (pin == SERVO_TAKE_PIN) {
    SERVO_TAKE.write(angle);
  }
}

void elevate(int elev) {
  rotate_servo(SERVO_ELEV_1_PIN, 30 + (elev * 60 / 100));
  rotate_servo(SERVO_ELEV_2_PIN, 10 + (elev * 10 / 100));
}

void rotate(int rot) {
  rotate_servo(SERVO_ROTATE_PIN, 90 + rot * 180 / 100);
}

void button_wait() {
  digitalWrite(LED_BUILTIN, HIGH);
  bool u = true;
  for (int i = 0; i < 1000000; ++i) {
    u &= digitalRead(19);
    //    Serial.println(digitalRead(19));
  }
  while (!u) {
    u = true;
    for (int i = 0; i < 1000000; ++i) {
      u &= digitalRead(19);
      //      Serial.println(digitalRead(19));
    }
    delay(100);
  }
  digitalWrite(19, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
}

YandexVariable vars(token);

void wait_value(String val) {
  while (vars.get("rover_responce") != val) {
    delay(500);
  }
}

void setup() {
  Serial.begin(9600);
  for (int pin : OUT_PINS) {
    pinMode(pin, OUTPUT);
  }
  for (int pin : INP_PINS) {
    pinMode(pin, INPUT);
  }
  SERVO_ROTATE.attach(SERVO_ROTATE_PIN, 0, 4095);
  SERVO_ELEV_1.attach(SERVO_ELEV_1_PIN);
  SERVO_ELEV_2.attach(SERVO_ELEV_2_PIN, 0, 2047);
  SERVO_TAKE.attach(SERVO_TAKE_PIN);

  digitalWrite(18, HIGH);
  digitalWrite(19, HIGH);

    rotate_servo(13, 90);
    rotate_servo(5, 90);
    rotate_servo(23, 30);
    delay(2000);
    rotate_servo(12, 100);
    delay(3000);
    WiFiConnect(ssid, password);
    vars.reconnect();
  //  button_wait();
  //  rotate_servo(12, 0);
  //  delay(1000);
  //  rotate_servo(23, 0);
  //  delay(500);
  //  rotate_servo(13, 0);
  //  rotate_servo(5, 0);
  //  delay(2000);
  //  rotate_servo(23, 50);
  //  delay(2000);
  //  rotate_servo(12, 0);
  //  delay(1000);
  //  rotate_servo(13, 90);
  //  rotate_servo(5, 90);
  //  delay(1000);
  //  rotate_servo(12, 100);
  //  delay(1000);
  //  rotate_servo(13, 0);
  //  rotate_servo(5, 0);
  //  delay(1000);
  //  rotate_servo(23, 30);
  //  delay(1000);
  //  rotate_servo(13, 90);
  //  rotate_servo(5, 90);
  //  rotate_servo(23, 30);
  //  delay(2000);
  //  rotate_servo(12, 100);
  //  vars.set("rover_responce", "1");
  //  wait_value("2");
//  rotate_servo(13, 180);
//  rotate_servo(5, 180);
//  rotate_servo(23, 30);
//  delay(1000);
//  rotate_servo(23, 0);
//  delay(1000);
//  rotate_servo(12, 80);
//  delay(2000);
//  rotate_servo(13, 110);
//  rotate_servo(5, 120);
//
//  button_wait();
//
//  rotate_servo(13, 120);
//  rotate_servo(5, 120);
//  rotate_servo(23, 120);
//  delay(2000);
//  rotate_servo(12, 180);
//
//  button_wait();
//
//  rotate_servo(13, 180);
//  rotate_servo(5, 180);
//  rotate_servo(23, 180);
//  delay(1000);
//  rotate_servo(12, 0);



  //  vars.set("rover_responce", "1");
  //  wait_value("2");
//  Serial.println(SERVO_ROTATE.read());
//  Serial.println(SERVO_ELEV_1.read());
//  Serial.println(SERVO_ELEV_2.read());
//  Serial.println(SERVO_TAKE.read());

}

void loop() {
  vars.reconnect();
  String val = vars.get("rover_progress");
  if (val == "3") {
    rotate_servo(12, 0);
    delay(3000);
    rotate_servo(23, 0);
    delay(1000);
    rotate_servo(5, 30);
    rotate_servo(13, 30);
    delay(2000);
    rotate_servo(23, 90);
    delay(1000);
    rotate_servo(12, 180);
    delay(3000);
    rotate_servo(5, 0);
    rotate_servo(13, 0);
    delay(2000);
    rotate_servo(23, 0);
    delay(1000);
    rotate_servo(13, 90);
    rotate_servo(5, 90);
    rotate_servo(23, 30);
    delay(2000);
    rotate_servo(12, 100);
    delay(3000);
  }
}
