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

int relay_PIN_1 = 23;
int relay_PIN_2 = 22;
int relay_PIN_3 = 21;
int relay_PIN_4 = 19;
int relay_PIN_5 = 18;
int relay_PIN_6 = 17;
int relay_PIN_7 = 16;
int relay_PIN_8 = 15;
// Required to replace with your xbox address
XboxSeriesXControllerESP32_asukiaaa::Core
xboxController("0c:35:26:73:a1:79");

void setup() {
  pinMode(relay_PIN_1, OUTPUT);
  pinMode(relay_PIN_2, OUTPUT);
  pinMode(relay_PIN_3, OUTPUT);
  pinMode(relay_PIN_4, OUTPUT);
  pinMode(relay_PIN_5, OUTPUT);
  pinMode(relay_PIN_6, OUTPUT);
  pinMode(relay_PIN_7, OUTPUT);
  pinMode(relay_PIN_8, OUTPUT);
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
      //      Serial.println("Address: " + xboxController.buildDeviceAddressStr());
      //      Serial.println(xboxController.xboxNotif.toString());
      //      parseInput(xboxController.xboxNotif.toString());
      //      auto parsed = parseInput(xboxController.xboxNotif.toString());
      btnY = xboxController.xboxNotif.btnY;
      btnX = xboxController.xboxNotif.btnX;
      btnB = xboxController.xboxNotif.btnB;
      btnA = xboxController.xboxNotif.btnA;
      btnLB = xboxController.xboxNotif.btnLB;
      btnRB = xboxController.xboxNotif.btnRB;
      trigLT = xboxController.xboxNotif.trigLT;
      trigRT = xboxController.xboxNotif.trigRT;
      btnDirUp = xboxController.xboxNotif.btnDirUp;
      if (btnRB == 1) digitalWrite(relay_PIN_4, 1);
      if (btnLB == 1) digitalWrite(relay_PIN_4, 0);
      if (trigRT > 500){digitalWrite(relay_PIN_3, 1); delay(200);digitalWrite(relay_PIN_1, 1);delay(200);digitalWrite(relay_PIN_1, 0);}
      if (trigLT > 500){digitalWrite(relay_PIN_3, 1); delay(200);digitalWrite(relay_PIN_1, 1);delay(50);digitalWrite(relay_PIN_1, 0);}
      if(btnDirUp == 1){digitalWrite(relay_PIN_8, 0);delay(100);digitalWrite(relay_PIN_8, 1);}
      //      digitalWrite(relay_PIN_1, btnY);
      //      digitalWrite(relay_PIN_2, btnX);
      //      digitalWrite(relay_PIN_3, btnB);
      //      digitalWrite(relay_PIN_4, btnA);
      //      digitalWrite(relay_PIN_5, btnLB);
      //      digitalWrite(relay_PIN_6, btnRB);
      if (btnA == 1){digitalWrite(relay_PIN_3, 1);delay(20);digitalWrite(relay_PIN_2, 1); delay(200);digitalWrite(relay_PIN_2, 0);delay(400);digitalWrite(relay_PIN_3, 0);}
      if (btnB == 1){digitalWrite(relay_PIN_2, 0);digitalWrite(relay_PIN_3, 0);}
      if (btnY == 1)digitalWrite(relay_PIN_3, 0);
      if (btnX == 1)digitalWrite(relay_PIN_3, 1);
      joyLHori = xboxController.xboxNotif.joyLHori;
      joyLVert = xboxController.xboxNotif.joyLVert;
      joyRHori = xboxController.xboxNotif.joyRHori;
      joyRVert = xboxController.xboxNotif.joyRVert;
      float zz = normalizeJoystick(joyLHori)*1.5f;
      float yy = normalizeJoystick(joyRHori)*0.30f;
      float xx = normalizeJoystick(joyRVert)*0.30f;
      float erorr = 0.025;
      if(abs(zz)<erorr){
        zz = 0;
      }else{
        zz = zz-erorr;
      }
      if(abs(yy)<erorr){
        yy = 0;
      }else{
        yy = yy-erorr;
      }
      if(abs(xx)<erorr){
        xx = 0;
      }else{
        xx = xx-erorr;
      }
      carx.Set_motorxyz(xx,-yy,-zz);
      Serial.println(zz);
      //      unsigned long receivedAt = xboxController.getReceiveNotificationAt();
      //      uint16_t joystickMax = XboxControllerNotificationParser::maxJoy;
      //      Serial.print("joyLHori rate: ");
      //      Serial.println((float)xboxController.xboxNotif.joyLHori / joystickMax);
      //      Serial.print("joyLVert rate: ");
      //      Serial.println((float)xboxController.xboxNotif.joyLVert / joystickMax);
      //      Serial.println("battery " + String(xboxController.battery) + "%");
      //      Serial.println("received at " + String(receivedAt));
    }
  } else {
    Serial.println("not connected");
    carx.Set_motorxyz(0,0,0);
    if (xboxController.getCountFailedConnection() > 2) {
      ESP.restart();
    }
  }
  delay(30);
}