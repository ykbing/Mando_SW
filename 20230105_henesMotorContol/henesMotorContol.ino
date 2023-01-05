// Front Motor Drive
#define FMOTOR_PWM 5
#define FMOTOR_ENA 6
#define FMOTOR_ENB 7

// Rear Motor Drive
#define RMOTOR_PWM 2
#define RMOTOR_ENA 3
#define RMOTOR_ENB 4

#include <MsTimer2.h>

static boolean output = HIGH;

int f_speed = 0, r_speed = 0;

char data_buffer[4] = {0};
char motor_direction;

void front_motor_control(int fmotor_pwm)
{
   if (fmotor_pwm > 0) // forward
  {
    digitalWrite(FMOTOR_ENA, HIGH);
    digitalWrite(FMOTOR_ENB, LOW);
    analogWrite(FMOTOR_PWM, fmotor_pwm);
  }
  else if (fmotor_pwm < 0) // backward
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
   if (rmotor_pwm > 0) // forward
  {
    digitalWrite(RMOTOR_ENA, HIGH);
    digitalWrite(RMOTOR_ENB, LOW);
    analogWrite(RMOTOR_PWM, rmotor_pwm);
  }
  else if (rmotor_pwm < 0) // backward
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

void motor_control(int front_speed, int rear_speed)
{
  front_motor_control(front_speed);
  rear_motor_control(rear_speed);  
}

void control_callback()
{  
  digitalWrite(13, output);
  output =!output;

  motor_control(f_speed, r_speed);
}

void setup() 
{
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);

  // Front Motor Drive Pin Setup
  pinMode(FMOTOR_PWM, OUTPUT);
  pinMode(FMOTOR_ENA, OUTPUT);  // L298 motor control direction
  pinMode(FMOTOR_ENB, OUTPUT);

  // Rear Motor Drive Pin Setup
  pinMode(RMOTOR_PWM, OUTPUT);
  pinMode(RMOTOR_ENA, OUTPUT);  // L298 motor control direction
  pinMode(RMOTOR_ENB, OUTPUT);
  
  MsTimer2::set(10, control_callback); //
  MsTimer2::start();
  Serial.begin(115200);
}

void loop() 
{
  // put your main code here, to run repeatedly:
  int data;
  String s;
  char check_char;

  if (Serial.available() > 0) 
  {
    delay(4);
    // Check for packet header character 0xff
    
    check_char = Serial.read();
    if (check_char == 's')  // Speed Control
    {
      // Insert header into array
      //data_buffer[0] = 's';
      // Read remaining 3 characters of data and insert into array
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
      f_speed = data;
      r_speed = f_speed;
     }
  Serial.print("Speed: ");
  Serial.print(f_speed);
  }
}
