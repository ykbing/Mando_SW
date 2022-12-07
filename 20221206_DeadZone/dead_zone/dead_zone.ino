#include <MsTimer2.h>

#define encodrpinA1 2
#define encodrpinB1 3
#define INA 4
#define PWM 5
#define check_pin 6

volatile int32_t encoderPos = 0;

long pid_pwm = 0.0;
int deadzone_count = 0;
int old_deadzone_count = 0;

void Timer()
{
  deadzone_count = abs(encoderPos) <= 2 ? deadzone_count+=1 : deadzone_count;
  
  if(old_deadzone_count != deadzone_count)
  {
      pid_pwm++; 
  }
  
  old_deadzone_count = deadzone_count;
  
  if(pid_pwm > 0)
  {
    motor_control(1, pid_pwm);
  }
  else{
    motor_control(-1, pid_pwm);
  }
  
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
  MsTimer2::set(50, Timer); // 500ms period
  MsTimer2::start();
  interrupt_setup();
  Serial.begin(9600);
}

void loop() 
{
  Serial.print("encoderPos : ");
  Serial.print(encoderPos);
  Serial.print("  deadzone_count : ");
  Serial.print(deadzone_count);
  Serial.print("  pwm : ");
  Serial.println(pid_pwm);
}
