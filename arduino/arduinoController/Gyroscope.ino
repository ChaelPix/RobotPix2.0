MeGyro gyro;
#include <math.h>

unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 650;
float gyroOffset = 0.0;

void calibrateGyro() {
  // Prenez plusieurs lectures et faites la moyenne pour plus de précision
  float sum = 0.0;
  for (int i = 0; i < 10; ++i) {
    sum += GetGyroZ();
    delay(10);
  }
  gyroOffset = sum / 10.0;
}

float getCalibratedGyroZ() {
  // float angle = GetGyroZ() - gyroOffset;
  // float normalizedAngle = fmod((angle + 180), 360) - 180;
  // return normalizedAngle;
  return (int)gyro.getAngleZ();
}

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


int findShortestDirection(float currentAngle, float targetAngle) {

  float deltaTheta = targetAngle - currentAngle;
  float deltaThetaNormalized = fmod((deltaTheta + 180), 360) - 180;

  if (deltaThetaNormalized > 0) {
    return 1;  // Sens horaire
  } else if (deltaThetaNormalized < 0) {
    return -1;  // Sens anti-horaire
  } else {
    return 0;  // Déjà à l'angle cible
  }
}

/*
Param :
  int angleTarget : angle à atteindre (-179;179)      (+ = Gauche, - = Droite)
*/
void AngleRotate_Set(float angleTarget, int rotatingSpeed) {

  AngleTargetChecker(angleTarget);

  float currentAngle = getCalibratedGyroZ();
  findShortestDirection(currentAngle, angleTarget) == -1 ? TurnLeft(rotatingSpeed) : TurnRight(rotatingSpeed);

 
  while (((int)getCalibratedGyroZ() != (int)angleTarget) && !_isStopped)
  {
    unsigned long currentTime = millis(); 

    if (currentTime - lastPublishTime >= publishInterval) {
      lastPublishTime = currentTime;

      String lcd_msg_data = String((int)getCalibratedGyroZ()) + "/" + String(angleTarget) + '\n' + String(gyro.getAngleZ());
      lcd_msg.data = lcd_msg_data.c_str();
      pubLcdText.publish(&lcd_msg);
    }
    _loop();
  }
  
  StopMotors();
  if(!_isStopped)
  {
        callback_msg.data = true;
        pubRotateCallBack.publish(&callback_msg);
  }
}