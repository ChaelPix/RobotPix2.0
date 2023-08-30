#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/Vector3.h>

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

//-------Variables
bool _isStopped = false;
//-------Ros Node
ros::NodeHandle nh;

//-------PUBs
std_msgs::Bool callback_msg;
ros::Publisher pubMotorsCallBack("/MotorsCallBack", &callback_msg);

//-----Motors - x: speed, y: 0 = Stop, 1 = Forward, 2 = Backward, 3 = Left, 4 = Right
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
  }
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

//-----------Setup
void setup()
{
  nh.initNode();

  //Subs
  nh.subscribe(subMotorsControl);
  nh.subscribe(subStop);

  //Pubs
  nh.advertise(pubMotorsCallBack);

  SetupGyro();
}

void _loop()
{
  nh.spinOnce();
  UpdateGyro();
  delay(1);
}

void loop()
{
  _loop();
}