/*
  MsTimer2 is a small and very easy to use library to interface Timer2 with
  humans. It's called MsTimer2 because it "hardcodes" a resolution of 1
  millisecond on timer2
  For Details see: http://www.arduino.cc/playground/Main/MsTimer2
*/
#include <MsTimer2.h>

#define encodrpinA1 2
#define encodrpinB1 3
#define INA 4
#define PWM 5
#define check_pin 6
// Switch on LED on and off each half second

volatile int32_t encoderPos = 0;
volatile int32_t encoderPos_old = 0;
int32_t target_Pos = 0;
int32_t pos_error = 0;
int32_t pos_error_old = 0;
int32_t pos_error_d = 0;

float p = 3;
float pd = 5.8;

int pid_pwm = 0;

#if ARDUINO >= 100
const int led_pin = LED_BUILTIN;	// 1.0 built in LED pin var
#else
const int led_pin = 13;			// default to pin 13
#endif

void Timer()
{
  static boolean output = HIGH;
  
  digitalWrite(led_pin, output);
  digitalWrite(check_pin, output);
  output = !output;

  pos_error = target_Pos - encoderPos;
  pos_error_d = pos_error - pos_error_old;
  encoderPos_old = encoderPos;
  pos_error_old = pos_error;

  pid_pwm = p*pos_error + pd*pos_error_d;

  if(pid_pwm > 0)
  {
    motor_control(1, pid_pwm);
  }
  else{
    motor_control(-1, pid_pwm);
  }
  
  digitalWrite(check_pin, LOW);
}

void interrupt_setup(void)
{
  delayMicroseconds(50);
  pinMode(encodrpinA1, INPUT_PULLUP);                 
  pinMode(encodrpinB1, INPUT_PULLUP);             
  attachInterrupt(0, encoder, RISING);                               
}

void motor_control(int direc, int pwm)
{
  pwm = (pwm>=255) ? 255 : pwm;
  switch(direc)
  {
    case 1: digitalWrite(INA, HIGH);
            analogWrite(PWM,pwm);
            break;
    case 0: digitalWrite(INA, LOW);
            analogWrite(PWM,0);
            break;
    case -1: digitalWrite(INA, LOW);
            analogWrite(PWM,pwm);
            break;
  }
}

void encoder()  
{                                               
  if(digitalRead(encodrpinB1)==LOW){
    encoderPos++;            
  }
  else{
    encoderPos--;          
  }
}

void setup()
{
  pinMode(led_pin, OUTPUT);
  pinMode(check_pin, OUTPUT);
  MsTimer2::set(50, Timer); // 500ms period
  MsTimer2::start();
  interrupt_setup();
  Serial.begin(115200);
  Serial.println("pos, pos_error");
  target_Pos = 500;
}

void loop()
{
  if(abs(pos_error) >= 5)
  {
    Serial.print(encoderPos);
    Serial.print(",");
    Serial.println(pos_error);
  }
//
//  if(pos_error ==0)
//  {
//    motor_control(1,0);
//  }
//  else
//  {
//    motor_control(1,40);
//  }
//  motor_control(-1, 23);   // foward 23  , backward 20
}
