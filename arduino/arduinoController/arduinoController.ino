#include <ros.h>
#include <std_msgs/Float64.h>
#include <std_msgs/String.h>

#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <MeMegaPi.h>

//motors
MeMegaPiDCMotor m_moteurG(PORT1B);
MeMegaPiDCMotor m_moteurD(PORT2B);

//ultra sensor
MeUltrasonicSensor m_ultraSensor(PORT_5);
bool isrunning = false;

ros::NodeHandle nh;

//Pub
std_msgs::Float64 str_f;
ros::Publisher pub("foo", &str_f);

void messageCb(const std_msgs::Float64& msg)
{
  Serial.println("message recu !");
  int speed = static_cast<int>(msg.data);
   m_moteurG.run(speed);
   m_moteurD.run(-speed);
   isrunning = true;
   //pub.publish(&msg);
}

//Sub
ros::Subscriber<std_msgs::Float64> sub("superFloat", &messageCb);

void setup()
{
  
    nh.initNode();
    nh.advertise(pub);
    nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();

  if(isrunning)
  {
      if(m_ultraSensor.distanceCm() < 5)
      {
        isrunning = false;
         m_moteurG.run(0);
         m_moteurD.run(0);
      }
  }
    delay(1);
}