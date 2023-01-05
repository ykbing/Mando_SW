#define FMOTOR_PWM 5
#define FMOTOR_ENA 6
#define FMOTOR_ENB 7

#define RMOTOR_PWM 2
#define RMOTOR_ENA 3
#define RMOTOR_ENB 4

#include <MsTimer2.h>

static boolean output = HIGH;

int input_Fspeed = 0, input_Rspeed = 0;

char data_buffer[4] = {0};
char motor_direction;

void front_motor_control(int fmotor_pwm)
{
   if (fmotor_pwm > 0) // Forward
  {
    digitalWrite(FMOTOR_ENA, HIGH);
    digitalWrite(FMOTOR_ENB, LOW);
    analogWrite(FMOTOR_PWM, fmotor_pwm);
  }
  else if (fmotor_pwm < 0) // Backward
  {
    digitalWrite(FMOTOR_ENA, LOW);
    digitalWrite(FMOTOR_ENB, HIGH);
    analogWrite(FMOTOR_PWM, -fmotor_pwm);
  }
  else
  {
    digitalWrite(FMOTOR_ENA, LOW);
    digitalWrite(FMOTOR_ENB, LOW);
    digitalWrite(FMOTOR_PWM, 0);
  }
}

void rear_motor_control(int rmotor_pwm)
{
   if (rmotor_pwm > 0) // Forward
  {
    digitalWrite(RMOTOR_ENA, HIGH);
    digitalWrite(RMOTOR_ENB, LOW);
    analogWrite(RMOTOR_PWM, rmotor_pwm);
  }
  else if (rmotor_pwm < 0) // Backward
  {
    digitalWrite(RMOTOR_ENA, LOW);
    digitalWrite(RMOTOR_ENB, HIGH);
    analogWrite(RMOTOR_PWM, -rmotor_pwm);
  }
  else
  {
    digitalWrite(RMOTOR_ENA, LOW);
    digitalWrite(RMOTOR_ENB, LOW);
    digitalWrite(RMOTOR_PWM, 0);
  }
}

void motor_control(int input_Fspeed, int input_Rspeed)
{
  front_motor_control(input_Fspeed);
  rear_motor_control(input_Rspeed);  
}

void control_callback()
{  
  digitalWrite(13, output);
  output =!output;

  motor_control(input_Fspeed, input_Rspeed);
}

void setup() 
{
  pinMode(13, OUTPUT);
  
  pinMode(FMOTOR_PWM, OUTPUT);
  pinMode(FMOTOR_ENA, OUTPUT);
  pinMode(FMOTOR_ENB, OUTPUT);
  
  pinMode(RMOTOR_PWM, OUTPUT);
  pinMode(RMOTOR_ENA, OUTPUT); 
  pinMode(RMOTOR_ENB, OUTPUT);
  
  MsTimer2::set(10, control_callback); //
  MsTimer2::start();
  Serial.begin(115200);
}

void loop() 
{
  int data;
  String s;
  char check_char;

  if (Serial.available() > 0) 
  {
    delay(4);
    
    check_char = Serial.read();
    
    if (check_char == 's')  // Speed Control
    {
      motor_direction = Serial.read();
      
      for (int i = 0; i < 3; i++) 
      {
        data_buffer[i] = Serial.read();
      }
      Serial.write(data_buffer[0]);
      Serial.write(data_buffer[1]);
      Serial.write(data_buffer[2]);
      
      s = String(data_buffer);
      data = s.toInt();
      
      if(motor_direction =='r') data = -data;
      
      Serial.print("  : ");      
      Serial.println(data);
      
      delay(1000);
      
      input_Fspeed = data;
      input_Rspeed = input_Fspeed;
     }
  Serial.print("Speed: ");
  Serial.println(input_Fspeed);
  }
}
