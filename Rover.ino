#include <analogWrite.h>
#include "YandexVars.h"

#define SPEED_RIGHT 14
#define DIR_RIGHT 27
#define SPEED_LEFT 16
#define DIR_LEFT 17

#define SPEED_RUN_RIGHT 26
#define DIR_RUN_RIGHT 25
#define SPEED_RUN_LEFT 13
#define DIR_RUN_LEFT 12


int RUN_DATA = 0;


int OUT_PINS[] = {
  SPEED_RIGHT, DIR_RIGHT,
  SPEED_LEFT, DIR_LEFT,
  SPEED_RUN_RIGHT, DIR_RUN_RIGHT,
  SPEED_RUN_LEFT, DIR_RUN_LEFT
};
int INP_PINS[] = {10, 11, 12, 13};

YandexVariable vars = YandexVariable(token);

void wait_value(String val) {
  //  Serial.println(vars.get("rover_progress"));
  //  Serial.println(vars.get("rover_progress") == val);
  while (vars.get("rover_progress") != val) {
    if (vars.SSL_error_code()) {
      WiFiConnect(ssid, password);
      vars.reconnect();
    }
    try {
      delay(500);
    } catch (int exc) {
      WiFiConnect(ssid, password);
      vars.reconnect();
    }
  }
}


void forward_motor(int motor) {
  if (motor == 1) {
    digitalWrite(DIR_RIGHT, LOW);
    analogWrite(SPEED_RIGHT, 255);
  } else if (motor == 2) {
    digitalWrite(DIR_LEFT, HIGH);
    analogWrite(SPEED_LEFT, 0);
  } else if (motor == 3) {
    digitalWrite(DIR_RUN_RIGHT, LOW);
    analogWrite(SPEED_RUN_RIGHT, 255);
  } else if (motor == 4) {
    digitalWrite(DIR_RUN_LEFT, HIGH);
    analogWrite(SPEED_RUN_LEFT, 0);
  }
}

void back_motor(int motor) {
  if (motor == 1) {
    digitalWrite(DIR_RIGHT, HIGH);
    analogWrite(SPEED_RIGHT, 0);
  } else if (motor == 2) {
    digitalWrite(DIR_LEFT, LOW);
    analogWrite(SPEED_LEFT, 255);
  } else if (motor == 3) {
    digitalWrite(DIR_RUN_RIGHT, HIGH);
    analogWrite(SPEED_RUN_RIGHT, 0);
  } else if (motor == 4) {
    digitalWrite(DIR_RUN_LEFT, LOW);
    analogWrite(SPEED_RUN_LEFT, 255);
  }
}

void stop_motor(int motor) {
  if (motor == 1) {
    digitalWrite(DIR_RIGHT, LOW);
    analogWrite(SPEED_RIGHT, 0);
  } else if (motor == 2) {
    digitalWrite(DIR_LEFT, LOW);
    analogWrite(SPEED_LEFT, 0);
  } else if (motor == 3) {
    digitalWrite(DIR_RUN_RIGHT, LOW);
    analogWrite(SPEED_RUN_RIGHT, 0);
  } else if (motor == 4) {
    digitalWrite(DIR_RUN_LEFT, LOW);
    analogWrite(SPEED_RUN_LEFT, 0);
  }
}

void run_forward() {
  RUN_DATA = 1;
//  forward_motor(3);
//  forward_motor(4);
  forward_motor(1);
  forward_motor(2);
}

void run_back() {
  RUN_DATA = -1;
  back_motor(3);
  back_motor(4);
  back_motor(1);
  back_motor(2);
}

void run_stop() {
  RUN_DATA = 0;
  stop_motor(3);
  stop_motor(4);
  stop_motor(1);
  stop_motor(2);
}



void rotate_left() {
  forward_motor(1);
  back_motor(2);
    forward_motor(3);
    back_motor(4);
}

void rotate_right() {
  back_motor(1);
  forward_motor(2);
    back_motor(3);
    forward_motor(4);
}

void rotate_stop() {
//    stop_motor(1);
//    stop_motor(2);
//    stop_motor(2);
//    stop_motor(3);

}

void stop() {
  run_stop();
  rotate_stop();
}


void button_wait(bool rev) {
  //  digitalWrite(LED_BUILTIN, HIGH);
  bool u = !rev;
  for (int i = 0; i < 10000; ++i) {
    if (rev) {
      u |= digitalRead(13);
    } else {
      u &= digitalRead(13);
    }
    //    Serial.println(digitalRead(19));
  }
  if (rev) {
    u = !u;
  }
  while (!u) {
    u = true;
    for (int i = 0; i < 10000; ++i) {
      if (rev) {
        u |= digitalRead(13);
      } else {
        u &= digitalRead(13);
      }
      //      Serial.println(digitalRead(19));
    }
    //    delay(10);
  }
  digitalWrite(13, HIGH);
  //  digitalWrite(LED_BUILTIN, LOW);
}

void button_wait() {
  button_wait(false);
}

//YandexVariable vars(token);
//
//void wait_value(String val) {
//  while (vars.get("rover_responce") != val) {
//    delay(500);
//  }
//}

void go_way(String data) {
  int i = 0;
  bool stopped = false;
  while (i < data.length() && !stopped) {
    char cmd = data.charAt(i);
    ++i;
    int value = 0;
    value += 1000 * (((int) data.charAt(i)) - 48);
    ++i;
    value +=  100 * (((int) data.charAt(i)) - 48);
    ++i;
    value +=   10 * (((int) data.charAt(i)) - 48);
    ++i;
    value +=    1 * (((int) data.charAt(i)) - 48);
    (++i)++;
    Serial.println(String() + cmd + " " + value);
    switch (cmd) {
      case ('f'): {
          send_command(1);
          delay(value);
          send_command(4);
          break;
      } case ('r'): {
          send_command(2);
          delay(value);
          send_command(4);
          break;
      } case ('l'): {
          send_command(3);
          delay(value);
          send_command(4);
          break;
      } case ('p'): {
          send_command(4);
          delay(value);
          break;
      } case ('s'): {
          stopped = true;
          send_command(4);
          delay(value);
          break;
      } default: {
          break;
        }
    }
    delay(100);
  }
}

void setup() {
  Serial.begin(115200);
  for (int pin : OUT_PINS) {
    pinMode(pin, OUTPUT);
  }
  for (int pin : INP_PINS) {
    pinMode(pin, INPUT);
  }

  //  delay(3000);
  //  button_wait();
  run_forward();
//  rotate_stop();
  //  delay(2000);
  //  stop();
  //  delay(500);
  //  rotate_left();
  delay(1000);
  //  stop();
  //  run_forward();
  //  rotate_stop();
  //  delay(5000);
  stop();
  WiFiConnect(ssid, password);

  //  digitalWrite(SW, HIGH);
  //  Serial.begin(9600);

}

//int read_command() {
//  while (!digitalRead(13)) {}
//  int ans = (digitalRead(10) * 4) + (digitalRead(11) * 2) + digitalRead(12) + 1;
//  Serial.println(String("") + (int) digitalRead(10) + (int) digitalRead(11) + (int) digitalRead(12));
//  //  delay(100);
//  while (digitalRead(13)) {}
//  return ans;
//}

void send_command(int cmd) {
  Serial.println(cmd);
  switch (cmd) {
    case 1: {
        stop();
        run_forward();
        rotate_stop();
        break;
    } case 2: {
        stop();
        rotate_right();
        break;
    } case 3: {
        stop();
        rotate_left();
        break;
    } case 4: {
        stop();
        rotate_stop();
        break;
    } case 5: {
        stop();
        run_back();
        rotate_stop();
        break;
    } case 6: {
        stop();
        break;
    } case 7: {
        stop();
        break;
    } case 8: {
        stop();
        break;
    } default: {
        break;
      }
  }
  delay(100);
}

void loop() {
  send_command(4);
  vars.reconnect();
  
  String pr = vars.get("rover_progress");
  if (pr == "5" || pr == "9") {
    if (pr == "5") {
      vars.set("rover_progress", "6");
    } else {
      vars.set("rover_progress", "10");
    }
    go_way(vars.get("rover_way"));
    if (pr == "5") {
      vars.set("rover_progress", "7");
    } else {
      vars.set("rover_progress", "11");
    }
  }
  //  int cmd = read_command();
  //  Serial.println(cmd);
  //  switch (cmd) {
  //    case 1: {
  //        stop();
  //        run_forward();
  //        rotate_stop();
  //        break;
  //    } case 2: {
  //        stop();
  //        rotate_right();
  //        break;
  //    } case 3: {
  //        stop();
  //        rotate_left();
  //        break;
  //    } case 4: {
  //        stop();
  //        rotate_stop();
  //        break;
  //    } case 5: {
  //        stop();
  //        run_back();
  //        rotate_stop();
  //        break;
  //    } case 6: {
  //        stop();
  //        break;
  //    } case 7: {
  //        stop();
  //        break;
  //    } case 8: {
  //        stop();
  //        break;
  //    } default: {
  //        break;
  //      }
  //  }
  //  delay(100);

  ////  digitalWrite(LED_BUILTIN, HIGH);
  ////  delay(500);
  ////  digitalWrite(LED_BUILTIN, LOW);
  ////  delay(500);
  //////  run_forward();
  //////  Serial.println("1");
  //////  delay(500);
  //////  rotate_right();
  //////  Serial.println("2");
  //////  delay(1000);
  //////  run_back();
  //////  Serial.println("3");
  //////
  //////  delay(500);
  //////  Serial.println(RUN_DATA);
  //////  rotate_right();
  //////  Serial.println("4");
  //////  delay(1000);
  //  int X = analogRead(URX);
  ////  Serial.println("X");
  //  int Y = analogRead(URY);
  ////  Serial.println("Y");
  //  int b = digitalRead(SW);
  ////  Serial.println("S");
  ////  digitalWrite(LED_BUILTIN, HIGH);
  ////  delay(1000);
  ////    digitalWrite(LED_BUILTIN, LOW);
  ////delay(1000);
  //
  ////  Serial.print(X);
  ////  Serial.print(" ");
  ////  Serial.print(Y);
  ////  Serial.print(" ");
  ////  Serial.println(b);
  //
  //  if (X <= 450) {
  //    run_forward();
  //  } else if (X <= 550) {
  //    run_stop();
  //  } else {
  //    run_back();
  //  }
  //
  //  if (Y <= 450) {
  //    rotate_right();
  //  } else if (Y <= 550) {
  //    rotate_stop();
  //  } else {
  //    rotate_left();
  //  }
  //
  //  if (!b) {
  //    digitalWrite(LED_BUILTIN, HIGH);
  //  } else {
  //    digitalWrite(LED_BUILTIN, LOW);
  //  }
  //
  //  delay(1000/60);
}
