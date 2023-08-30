MeGyro gyro;
#include <math.h>

/*
Desc :
  Démarre le gyroscope
*/
void SetupGyro() {
  gyro.begin();

}

/*
Desc :
  Actualise le gyroscope
*/
void UpdateGyro()
{
  gyro.update();
}

/*
Desc :
  Récupère l'angle Z du robot en int
*/
int GetGyroZ() {
  return (int)gyro.getAngleZ();
}

void AngleTargetChecker(float &angleTarget) {
  if (angleTarget >= 180)
    angleTarget = 179;
  if (angleTarget <= -180)
    angleTarget = -179;
}

/*
Param :
  int angleTarget : angle à atteindre (-179;179)      (+ = Gauche, - = Droite)
*/
void AngleRotate_Set(float angleTarget, int rotatingSpeed) {

  AngleTargetChecker(angleTarget);

  float currentAngle = GetGyroZ();
  float angleDifference = angleTarget - currentAngle;
    
    angleDifference = fmod(angleDifference + 180.0, 360.0) - 180.0;

      if (abs(angleDifference) <= 180) {
        if (angleDifference > 0) {
          TurnRight(rotatingSpeed);
        } else {
          TurnLeft(rotatingSpeed);
        }
      } else {
        if (angleDifference > 0) {
          TurnLeft(rotatingSpeed);
        } else {
          TurnRight(rotatingSpeed);
        }
      }

      while ((abs(GetGyroZ() - angleTarget) > 1.0) && !_isStopped) { 
        _loop();
      }

      if(!_isStopped)
      {
        callback_msg.data = true;
        pubRotateCallBack.publish(&callback_msg);
      }
}