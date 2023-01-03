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
int32_t pos_error_sum = 0;

volatile float angular_speed = 0;
volatile float angular_speed_old = 0;

float target_angular_speed = 0;
float angular_speed_error = 0;
float angular_speed_error_old = 0;
float angular_speed_error_d = 0;
double angular_speed_error_sum = 0;

// PID gain angular position control
float p = 0.8;
float pd = 1.5;
float pi = 0.2;

// PID gain angular speed control
float pw = 0.8;  // 0.8
float pw_d = 1.5;
float pw_i = 1.5;  //1.5

int pos_pid_pwm = 0;
int angular_pos_pid_pwm = 0;
int angular_speed_pid_pwm = 0;

int dead_zone_threshold_positive = 10;
int dead_zone_threshold_negative = 9;

#if ARDUINO >= 100
const int led_pin = LED_BUILTIN;  // 1.0 built in LED pin var
#else
const int led_pin = 13;     // default to pin 13
#endif

int input_vel = 0;
char data_buffer[4] = {0};
char motor_direction;

void Timer()
{
  static boolean output = HIGH;
  
  digitalWrite(led_pin, output);
  digitalWrite(check_pin, output);
  output = !output;

  target_Pos += input_vel;

  pos_error = target_Pos - encoderPos;
  pos_error_d = pos_error - pos_error_old;
  
  pos_error_sum += pos_error;
  pos_error_sum = (pos_error_sum > 50) ? 50 : pos_error_sum;
  pos_error_sum = (pos_error_sum < -50) ? -50: pos_error_sum;
  if(fabs(pos_error)<=2) pos_error_sum = 0;

  pos_pid_control();

  angular_speed = encoderPos - encoderPos_old;
  angular_speed_error = target_angular_speed - angular_speed;
  angular_speed_error_d = angular_speed - angular_speed_old;
  
  angular_speed_error_sum += angular_speed_error;
  
  angular_speed_error_sum = (angular_speed_error_sum > 200) ? 200 : angular_speed_error_sum;
  angular_speed_error_sum = (angular_speed_error_sum < -200) ? -200 : angular_speed_error_sum;
  
  angular_speed_pid_control();

  angular_speed_old = angular_speed;
  encoderPos_old = encoderPos;
  pos_error_old = pos_error;
  
  digitalWrite(check_pin, LOW);
}

int angular_speed_pid_control(void){
  int angular_speed_pid_pwm1 = 0;
  
  angular_speed_pid_pwm1 = pw * angular_speed_error + pw_d * angular_speed_error_d + pw_i * angular_speed_error_sum;
  
  angular_speed_pid_pwm1 = (angular_speed_pid_pwm1 >= 255) ? 255 : angular_speed_pid_pwm1;
  angular_speed_pid_pwm1 = (angular_speed_pid_pwm1 <= -255) ? -255 : angular_speed_pid_pwm1;

  angular_speed_pid_pwm = dead_zone_pwm_correction(angular_speed_pid_pwm1);

  angular_speed_pid_pwm += inverse_speed_mode();

  if(target_Pos == 0)
  {
    if(angular_speed_pid_pwm > 0)
    {
      motor_control(1, angular_speed_pid_pwm);
    }
  
    else if (angular_speed_pid_pwm < 0)
    {
      motor_control(-1, -angular_speed_pid_pwm);
    }
    else {
      motor_control(0, -angular_speed_pid_pwm);
    }
  }
}

int pos_pid_control(void)
{
  pos_pid_pwm = p * pos_error + pd * pos_error_d + pi *pos_error_sum ;

  pos_pid_pwm = (pos_pid_pwm >= 255) ? 255 : pos_pid_pwm;
  pos_pid_pwm = (pos_pid_pwm <= -255) ? -255 : pos_pid_pwm;

  if(target_Pos != 0 && target_angular_speed == 0)
  {
    if(pos_pid_pwm > 0)
    {
      motor_control(1, pos_pid_pwm);
    }
    else if(pos_pid_pwm < 0){
      motor_control(-1, -pos_pid_pwm);
    }
  }
}

int dead_zone_pwm_correction(int input_pwm)
{
  int pwm_out;

  if(input_pwm > 0)
  {
    pwm_out = input_pwm + dead_zone_threshold_positive;
  }
  else if(input_pwm < 0)
  {
    pwm_out = input_pwm - dead_zone_threshold_negative;
  }
  else
  {
    pwm_out = 0;
  }
  pwm_out = (pwm_out >= 255) ?  255 : pwm_out;
  pwm_out = (pwm_out <= -255) ?  -255 : pwm_out;

  return pwm_out;
}

int inverse_speed_mode(void)
{
  float pwm = 0;
  pwm = target_angular_speed * 2.3;
  return pwm;
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
  switch(direc)
  {
    case 1: digitalWrite(INA, LOW);
            analogWrite(PWM,pwm);
            break;
    case 0: digitalWrite(INA, LOW);
            analogWrite(PWM,0);
            break;
    case -1: digitalWrite(INA, HIGH);
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


void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(check_pin, OUTPUT);
  MsTimer2::set(50, Timer); // 500ms period
  MsTimer2::start();
  interrupt_setup();
  Serial.begin(115200);
}

void loop() {
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

      encoderPos = 0;
      encoderPos_old = 0;
      target_angular_speed = 0;
      target_Pos = 0;
      angular_speed_error_sum = 0;
      pos_error_sum = 0;
      input_vel = 0;
      
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
      target_angular_speed = data;
     } 

     if (check_char == 'p') // Position Control
     {
      motor_direction = Serial.read();

      encoderPos = 0;
      encoderPos_old = 0;
      target_angular_speed = 0;
      target_Pos = 0;
      angular_speed_error_sum = 0;
      pos_error_sum = 0;
      input_vel = 0;
      
      for (int i = 0; i < 4; i++) 
      {
        data_buffer[i] = Serial.read();
      }
      
      Serial.write(data_buffer[0]);
      Serial.write(data_buffer[1]);
      Serial.write(data_buffer[2]);
      Serial.write(data_buffer[3]);
      
      s=String(data_buffer);
      data = s.toInt();
      
      if(motor_direction =='r') data = -data;
      
      Serial.print("  : ");      
      Serial.println(data);
      
      delay(1000);
      
      target_Pos = data;
     }

     if (check_char == 'v') // Velocity Control
     {
      motor_direction = Serial.read();

      encoderPos = 0;
      encoderPos_old = 0;
      target_angular_speed = 0;
      target_Pos = 0;
      angular_speed_error_sum = 0;
      pos_error_sum = 0;
      input_vel = 0;
      
      for (int i = 0; i < 4; i++) 
      {
        data_buffer[i] = Serial.read();
      }
      
      Serial.write(data_buffer[0]);
      Serial.write(data_buffer[1]);
      Serial.write(data_buffer[2]);
      Serial.write(data_buffer[3]);
      
      s=String(data_buffer);
      data = s.toInt();
      
      if(motor_direction =='r') data = -data;
      
      Serial.print("  : ");      
      Serial.println(data);
      
      delay(1000);
      
      input_vel = data;
     } 
 }
 Serial.print("Speed: ");
 Serial.print(angular_speed);
 Serial.print("  EncoderPos: ");
 Serial.println(encoderPos);
}

//ex) (s)(motordirection)(speed) , __sr040__
