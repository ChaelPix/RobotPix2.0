#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Empty.h>

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

#define SERIAL_PORT "/dev/ttyAMA0"


//-------Components
MeMegaPiDCMotor m_moteurG(PORT2B);
MeMegaPiDCMotor m_moteurD(PORT4B);


//-------Ros Node
ros::NodeHandle nh;

//-------PUBs
std_msgs::Bool callback_msg;
ros::Publisher pubMotorsCallBack("/MotorsCallBack", &callback_msg);


//-----Motors
void MoveForward(const std_msgs::Float32& msg)
{
  int speed = static_cast<int>(msg.data);
  m_moteurG.run(-speed);
  m_moteurD.run(speed);
  SendMotorsOk();
}
void MoveBackward(const std_msgs::Float32& msg)
{
  int speed = static_cast<int>(msg.data);
  m_moteurG.run(speed);
  m_moteurD.run(-speed);
  SendMotorsOk();
}
void TurnLeft(const std_msgs::Float32& msg)
{
  int speed = static_cast<int>(msg.data);
  m_moteurG.run(-speed);
  m_moteurD.run(-speed);
  SendMotorsOk();
}
void TurnRight(const std_msgs::Float32& msg)
{
  int speed = static_cast<int>(msg.data);
  m_moteurG.run(speed);
  m_moteurD.run(speed);
  SendMotorsOk();
}
void StopMotors()
{
  m_moteurG.stop();
  m_moteurD.stop();
  SendMotorsOk();
}
void SendMotorsOk()
{
  callback_msg.data = true;
  pubMotorsCallBack.publish(&callback_msg);
}
//----STOP
void stopRos(const std_msgs::Float32& msg)
{
  StopMotors();
}

//--------SUBs
ros::Subscriber<std_msgs::Float32> subMotorsMoveForward("motorsMoveForward", &MoveForward);
ros::Subscriber<std_msgs::Float32> subMotorsMoveBackward("motorsMoveBackward", &MoveBackward);
ros::Subscriber<std_msgs::Float32> subMotorsTurnLeft("motorsTurnLeft", &TurnLeft);
ros::Subscriber<std_msgs::Float32> subMotorsTurnRight("motorsTurnRight", &TurnRight);
ros::Subscriber<std_msgs::Empty> subMotorsStop("motorsStop", &StopMotors);
ros::Subscriber<std_msgs::Float32> subStop("stopRos", &stopRos);

//-----------Setup
void setup()
{
  nh.initNode();

  //Subs
  nh.subscribe(subMotorsMoveForward);
  nh.subscribe(subMotorsMoveBackward);
  nh.subscribe(subMotorsTurnLeft);
  nh.subscribe(subMotorsTurnRight);
  nh.subscribe(subMotorsStop);
  nh.subscribe(subStop);

  //Pubs
  nh.advertise(pubMotorsCallBack);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}