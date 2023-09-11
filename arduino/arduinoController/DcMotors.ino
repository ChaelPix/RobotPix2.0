//-------Components
MeMegaPiDCMotor m_moteurG(PORT2B);
MeMegaPiDCMotor m_moteurD(PORT4B);
MeMegaPiDCMotor m_moteurStamp(PORT3B);

void MoveForward(int speed)
{
  m_moteurG.run(-speed);
  m_moteurD.run(speed);
  SendMotorsOk();
}
void MoveBackward(int speed)
{
  m_moteurG.run(speed);
  m_moteurD.run(-speed);
  SendMotorsOk();
}
void TurnLeft(int speed)
{
  m_moteurG.run(speed);
  m_moteurD.run(speed);
  SendMotorsOk();
}
void TurnRight(int speed)
{
  m_moteurG.run(-speed);
  m_moteurD.run(-speed);
  SendMotorsOk();
}
void StopMotors()
{
  m_moteurG.stop();
  m_moteurD.stop();
  m_moteurStamp.stop();
  SendMotorsOk();
}
void StampOn(int speed)
{
  m_moteurStamp.run(speed);
}
void StampOff(int speed)
{
  m_moteurStamp.run(-speed);
}

void StampObject(int speed)
{
  m_moteurStamp.run(speed);
  _delay(2000);
  m_moteurStamp.stop();

  callback_msg.data = true;
  pubStampCallBack.publish(&callback_msg);
}

void SendMotorsOk()
{
  callback_msg.data = true;
  pubMotorsCallBack.publish(&callback_msg);
}