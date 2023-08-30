//-------Components
MeMegaPiDCMotor m_moteurG(PORT2B);
MeMegaPiDCMotor m_moteurD(PORT4B);

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
  m_moteurG.run(-speed);
  m_moteurD.run(-speed);
  SendMotorsOk();
}
void TurnRight(int speed)
{
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