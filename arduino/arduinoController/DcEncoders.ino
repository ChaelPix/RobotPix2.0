//  //-----------------------------------Vitesse, entre 0 et 180, nombre de tours/min des encodeurs---------------------------


// //-------Private Variables (do not touch)
// const double angle_rad = PI / 180.0;
// const double angle_deg = 180.0 / PI;


// //--------Moving
// void MoveForward(int rpm)
// {
//   Encoder_1.runSpeed(rpm);
//   Encoder_2.runSpeed(-rpm);
//   SendMotorsOk();
// }
// void MoveBackward(int rpm)
// {
//   Encoder_1.runSpeed(-rpm);
//   Encoder_2.runSpeed(rpm);
//   SendMotorsOk();
// }
// void TurnLeft(int rpm)
// {
//   Encoder_1.runSpeed(-rpm);
//   Encoder_2.runSpeed(-rpm);
//   SendMotorsOk();
// }
// void TurnRight(int rpm)
// {
//   Encoder_1.runSpeed(rpm);
//   Encoder_2.runSpeed(rpm);
//   SendMotorsOk();
// }
// void StopMotors()
// {
//   Encoder_1.setTarPWM(0);
//   Encoder_2.setTarPWM(0);
//   SendMotorsOk();
// }
// void SendMotorsOk()
// {
//   callback_msg.data = true;
//   pubMotorsCallBack.publish(&callback_msg);
// }

// //-----------Setup encoders
// void isr_process_encoder1(void) {
//   if (digitalRead(Encoder_1.getPortB()) == 0) {
//     Encoder_1.pulsePosMinus();
//   } else {
//     Encoder_1.pulsePosPlus();
//   }
// }

// void isr_process_encoder2(void) {
//   if (digitalRead(Encoder_2.getPortB()) == 0) {
//     Encoder_2.pulsePosMinus();
//   } else {
//     Encoder_2.pulsePosPlus();
//   }
// }

// void InitializeEncoders() {
//   //Set PWM 8KHz
//   TCCR1A = _BV(WGM10);
//   TCCR1B = _BV(CS11) | _BV(WGM12);

//   TCCR2A = _BV(WGM21) | _BV(WGM20);
//   TCCR2B = _BV(CS21);

//   attachInterrupt(Encoder_1.getIntNum(), isr_process_encoder1, RISING);
//   Encoder_1.setPulse(8);
//   Encoder_1.setRatio(46.67);
//   Encoder_1.setPosPid(1.8, 0, 1.2);
//   Encoder_1.setSpeedPid(0.18, 0, 0);

//   attachInterrupt(Encoder_2.getIntNum(), isr_process_encoder2, RISING);
//   Encoder_2.setPulse(8);
//   Encoder_2.setRatio(46.67);
//   Encoder_2.setPosPid(1.8, 0, 1.2);
//   Encoder_2.setSpeedPid(0.18, 0, 0);
// }
// //---------