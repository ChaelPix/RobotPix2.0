#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>
#include <geometry_msgs/Vector3.h>

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

// MeEncoderOnBoard Encoder_1(SLOT2);
// MeEncoderOnBoard Encoder_2(SLOT4);
//-------Variables
bool _isStopped = false;
//-------Ros Node
ros::NodeHandle nh;

//-------PUBs
std_msgs::Bool callback_msg;
ros::Publisher pubMotorsCallBack("/MotorsCallBack", &callback_msg);
ros::Publisher pubRotateCallBack("/RotateCallBack", &callback_msg);
ros::Publisher pubStampCallBack("/stampCallback", &callback_msg);
std_msgs::String lcd_msg;
ros::Publisher pubLcdText("/setLcdText", &lcd_msg);

//-----Motors - x: speed, y: 0 = Stop, 1 = Forward, 2 = Backward, 3 = Left, 4 = Right, 5 = RoateSet, 6 = StampOn, 7 = StampOff
void GetMotorsControl(const geometry_msgs::Vector3& msg)
{
  int speed = static_cast<int>(msg.x); 

  switch(static_cast<int>(msg.y))
  {
    case 0: 
      StopMotors(); break;
    case 1:
      MoveForward(speed); break;
    case 2:
      MoveBackward(speed); break;
    case 3:
      TurnLeft(speed); break;
    case 4:
      TurnRight(speed); break;
    case 5:
      AngleRotate_Set(speed, static_cast<float>(msg.z)); break;
    case 6:
      StampOn(speed); break;
    case 7:
      StampOff(speed); break;
    case 8:
      StampObject(speed); break;
  }
}

void InitArduino(const std_msgs::Empty& msg)
{
  calibrateGyro();
}

//----STOP
void stopRos(const std_msgs::Float32& msg)
{
  _isStopped = true;
  StopMotors();
}

//--------SUBs
ros::Subscriber<geometry_msgs::Vector3> subMotorsControl("motorsControl", &GetMotorsControl);
ros::Subscriber<std_msgs::Float32> subStop("stopRos", &stopRos);
ros::Subscriber<std_msgs::Empty> subInit("initArduino", &InitArduino);

//-----------Setup
void setup()
{
  nh.initNode();

  //Subs
  nh.subscribe(subMotorsControl);
  nh.subscribe(subStop);
  nh.subscribe(subInit);

  //Pubs
  nh.advertise(pubMotorsCallBack);
  nh.advertise(pubRotateCallBack);
  nh.advertise(pubLcdText);
  nh.advertise(pubStampCallBack);

  //InitializeEncoders();
  SetupGyro();
}

void _delay(float ms) {
  if (ms < 0.0) {
    ms = 0.0;
  }
  long endTime = millis() + ms;
  while (millis() < endTime) _loop();
}

void _loop()
{
  // Encoder_1.loop();
  // Encoder_2.loop();
  
  nh.spinOnce();
  UpdateGyro();
  delay(1);
}

void loop()
{
  _loop();
}