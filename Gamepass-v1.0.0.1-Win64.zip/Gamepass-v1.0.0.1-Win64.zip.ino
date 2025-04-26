#include <Arduino.h>
#include <XboxSeriesXControllerESP32_asukiaaa.hpp>
#include <Wire.h>
#include <Mecanum_motor.h>
Mecanum carx(0x55, &Wire);

float normalizeJoystick(int raw) {
  return (raw - 32767.0) / 32767.0;
}

int btnY, btnX, btnB, btnA, btnLB, btnRB;
int btnSelect, btnStart, btnXbox, btnShare, btnLS, btnRS;
int btnDirUp, btnDirRight, btnDirDown, btnDirLeft;
int32_t joyLHori, joyLVert, joyRHori, joyRVert;
int trigLT, trigRT;

int relay_PIN_1 = 26;
int relay_PIN_2 = 27;
int relay_PIN_3 = 21;
int relay_PIN_4 = 19;
int relay_PIN_5 = 18;
int relay_PIN_6 = 17;
int relay_PIN_7 = 16;
int relay_PIN_8 = 15;
// Required to replace with your xbox address
XboxSeriesXControllerESP32_asukiaaa::Core
xboxController("0c:35:26:73:a0:b9");

void setup() {
  pinMode(relay_PIN_1, OUTPUT);
  pinMode(relay_PIN_2, OUTPUT);
  pinMode(relay_PIN_3, OUTPUT);
  pinMode(relay_PIN_4, OUTPUT);
  pinMode(relay_PIN_5, OUTPUT);
  pinMode(relay_PIN_6, OUTPUT);
  pinMode(relay_PIN_7, OUTPUT);
  pinMode(relay_PIN_8, OUTPUT);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  Serial.begin(115200);
  Wire.begin(4, 5);
  carx.begin();
  Serial.println("Starting NimBLE Client");
  xboxController.begin();
}

void loop() {
  xboxController.onLoop();
  if (xboxController.isConnected()) {
    if (xboxController.isWaitingForFirstNotification()) {
      Serial.println("waiting for first notification");
    } else {
      btnY = xboxController.xboxNotif.btnY;
      btnX = xboxController.xboxNotif.btnX;
      btnB = xboxController.xboxNotif.btnB;
      btnA = xboxController.xboxNotif.btnA;
      btnLB = xboxController.xboxNotif.btnLB;
      btnRB = xboxController.xboxNotif.btnRB;
      trigLT = xboxController.xboxNotif.trigLT;
      trigRT = xboxController.xboxNotif.trigRT;
      if (btnLB == 1 && btnRB == 0) {
        digitalWrite(relay_PIN_1, 1);
        digitalWrite(relay_PIN_2, 0);
        if (digitalRead(23) == 0) {
          digitalWrite(relay_PIN_1, 0);
          digitalWrite(relay_PIN_2, 1);
        }
      } else if (btnRB == 1 && btnLB == 0) {
        digitalWrite(relay_PIN_1, 0);
        digitalWrite(relay_PIN_2, 1);
        if (digitalRead(22) == 0) {
          digitalWrite(relay_PIN_1, 1);
          digitalWrite(relay_PIN_2, 0);
        }
      } else {
        digitalWrite(relay_PIN_1, 0);
        digitalWrite(relay_PIN_2, 0);
      }
      joyLHori = xboxController.xboxNotif.joyLHori;
      joyLVert = xboxController.xboxNotif.joyLVert;
      joyRHori = xboxController.xboxNotif.joyRHori;
      joyRVert = xboxController.xboxNotif.joyRVert;
      float zz = normalizeJoystick(joyLHori) * 1.5f;
      float yy = normalizeJoystick(joyRHori) * 0.30f;
      float xx = normalizeJoystick(joyRVert) * 0.30f;
      float erorr = 0.025;
      if (abs(zz) < erorr) {
        zz = 0;
      } else {
        zz = zz - erorr;
      }
      if (abs(yy) < erorr) {
        yy = 0;
      } else {
        yy = yy - erorr;
      }
      if (abs(xx) < erorr) {
        xx = 0;
      } else {
        xx = xx - erorr;
      }
      carx.Set_motorxyz(-xx, yy, -zz);
    }
  } else {
    Serial.println("not connected");
    carx.Set_motorxyz(0, 0, 0);
    if (xboxController.getCountFailedConnection() > 2) {
      ESP.restart();
    }
  }
  delay(30);
}
