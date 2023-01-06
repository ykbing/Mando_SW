//Fdeadzone  전진:20  후진:26
//Rdeadzone  전진:18  후진:24

#define FMOTOR_PWM 2
#define FMOTOR_ENA 3
#define FMOTOR_ENB 4

#define RMOTOR_PWM 5
#define RMOTOR_ENA 6
#define RMOTOR_ENB 7

#define ENC1_ADD 22
#define ENC2_ADD 23

#include <MsTimer2.h>
#include <SPI.h>

static boolean output = HIGH;

signed long encoderFcount = 0;
signed long encoderRcount = 0;

volatile int32_t encoderPos_old = 0;

int32_t targetPos = 0;
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

int input_vel = 0;
char data_buffer[4] = {0};
char motor_direction;

int input_Fspeed = 0, input_Rspeed = 0;

// PID gain angular position control
float p = 2.0;  //2.4
float pd = 3.8; //3.8
float pi = 0.2; //0.2

// PID gain angular position control
float pw = 0.8;     // 0.8 
float pw_d = 1.5;   // 1.5
float pw_i = 0.2;   // 0.2 

int pos_pid_pwm = 0;
int Fpos_pid_pwm = 0;
int Rpos_pid_pwm = 0;

int Fangular_speed_pid_pwm = 0;
int Rangular_speed_pid_pwm = 0;

int Fdead_zone_threshold_positive = 20;
int Fdead_zone_threshold_negative = 26;

int Rdead_zone_threshold_positive = 18;
int Rdead_zone_threshold_negative = 24;

void Timer()
{ 
  encoderFcount = readEncoder(1);
  encoderRcount = readEncoder(2);
  
  pos_error = targetPos - encoderRcount;
  pos_error_d = pos_error - pos_error_old;
  
  pos_error_sum += pos_error;
  pos_error_sum = (pos_error_sum > 50) ? 50 : pos_error_sum;
  pos_error_sum = (pos_error_sum < -50) ? -50: pos_error_sum;
  if(fabs(pos_error)<=2) pos_error_sum = 0;

  pos_pid_control();
  
  angular_speed = encoderRcount - encoderPos_old;
  angular_speed_error = target_angular_speed - angular_speed;
  angular_speed_error_d = angular_speed - angular_speed_old;
  
  angular_speed_error_sum += angular_speed_error;
  angular_speed_error_sum = (angular_speed_error_sum > 200) ? 200 : angular_speed_error_sum;
  angular_speed_error_sum = (angular_speed_error_sum < -200) ? -200 : angular_speed_error_sum;
  
  angular_speed_pid_control();

  angular_speed_old = angular_speed;
  encoderPos_old = encoderRcount;
  pos_error_old = pos_error;
}

int pos_pid_control(void)
{
  pos_pid_pwm = p * pos_error + pd * pos_error_d + pi *pos_error_sum ;

  Fpos_pid_pwm = Fdead_zone_pwm_correction(pos_pid_pwm);
  
  Fpos_pid_pwm = (Fpos_pid_pwm >= 155) ? 155 : Fpos_pid_pwm;
  Fpos_pid_pwm = (Fpos_pid_pwm <= -155) ? -155 : Fpos_pid_pwm;
  
  Rpos_pid_pwm = Rdead_zone_pwm_correction(pos_pid_pwm);

  Rpos_pid_pwm = (Rpos_pid_pwm >= 155) ? 155 : Rpos_pid_pwm;
  Rpos_pid_pwm = (Rpos_pid_pwm <= -155) ? -155 : Rpos_pid_pwm;
  
  if(targetPos != 0 && target_angular_speed == 0)
  {
    if(pos_pid_pwm > 0)
    {
      motor_control(Fpos_pid_pwm, Rpos_pid_pwm);
    }
  
    else if (pos_pid_pwm < 0)
    {
      motor_control(Fpos_pid_pwm, Rpos_pid_pwm);
    }
    else {
      motor_control(Fpos_pid_pwm, Rpos_pid_pwm);
    }
  }
}

int angular_speed_pid_control(void)
{
  int angular_speed_pid_pwm1 = 0;
  
  angular_speed_pid_pwm1 = pw * angular_speed_error + pw_d * angular_speed_error_d + pw_i * angular_speed_error_sum;

  Fangular_speed_pid_pwm = Fdead_zone_pwm_correction(angular_speed_pid_pwm1);
  
  Fangular_speed_pid_pwm = (Fangular_speed_pid_pwm >= 155) ? 155 : Fangular_speed_pid_pwm;
  Fangular_speed_pid_pwm = (Fangular_speed_pid_pwm <= -155) ? -155 : Fangular_speed_pid_pwm;

  Rangular_speed_pid_pwm = Rdead_zone_pwm_correction(angular_speed_pid_pwm1);

  Rangular_speed_pid_pwm = (Rangular_speed_pid_pwm >= 155) ? 155 : Rangular_speed_pid_pwm;
  Rangular_speed_pid_pwm = (Rangular_speed_pid_pwm <= -155) ? -155 : Rangular_speed_pid_pwm;

  if(targetPos == 0)
  {
    if(angular_speed_pid_pwm1 > 0)
    {
      motor_control(Fangular_speed_pid_pwm, Rangular_speed_pid_pwm);
    }
  
    else if (angular_speed_pid_pwm1 < 0)
    {
      motor_control(Fangular_speed_pid_pwm, Rangular_speed_pid_pwm);
    }
    else {
      motor_control(Fangular_speed_pid_pwm, Rangular_speed_pid_pwm);
    }
  }
}

int Fdead_zone_pwm_correction(int input_pwm)
{
  int pwm_out;

  if(input_pwm > 0)
  {
    pwm_out = input_pwm + Fdead_zone_threshold_positive;
  }
  else if(input_pwm < 0)
  {
    pwm_out = input_pwm - Fdead_zone_threshold_negative;
  }
  else
  {
    pwm_out = 0;
  }
  pwm_out = (pwm_out >= 155) ?  155 : pwm_out;
  pwm_out = (pwm_out <= -155) ?  -155 : pwm_out;

  return pwm_out;
}

int Rdead_zone_pwm_correction(int input_pwm)
{
  int pwm_out;

  if(input_pwm > 0)
  {
    pwm_out = input_pwm + Rdead_zone_threshold_positive;
  }
  else if(input_pwm < 0)
  {
    pwm_out = input_pwm - Rdead_zone_threshold_negative;
  }
  else
  {
    pwm_out = 0;
  }
  pwm_out = (pwm_out >= 155) ?  155 : pwm_out;
  pwm_out = (pwm_out <= -155) ?  -155 : pwm_out;

  return pwm_out;
}

void initEncoders() 
{
  pinMode(ENC1_ADD, OUTPUT);
  pinMode(ENC2_ADD, OUTPUT); 
  
  digitalWrite(ENC1_ADD,HIGH);
  digitalWrite(ENC2_ADD,HIGH);
 
  SPI.begin();
  
  SPI.transfer(0x88);                     
  SPI.transfer(0x03);                    
  digitalWrite(ENC1_ADD,HIGH);      

  digitalWrite(ENC2_ADD,LOW);     
  SPI.transfer(0x88);                   
  SPI.transfer(0x03);                   
  digitalWrite(ENC2_ADD,HIGH);       
}

long readEncoder(int encoder_no) 
{  
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;   
  
  digitalWrite(ENC1_ADD + encoder_no-1,LOW);     

  SPI.transfer(0x60);                    
  count_1 = SPI.transfer(0x00);           
  count_2 = SPI.transfer(0x00);           
  count_3 = SPI.transfer(0x00);           
  count_4 = SPI.transfer(0x00);          
  digitalWrite(ENC1_ADD+encoder_no-1,HIGH);    
  

  count_value= ((long)count_1<<24) + ((long)count_2<<16) + ((long)count_3<<8 ) + (long)count_4;
  
  return -count_value;
}

void clearEncoderCount(int encoder_no) 
{    
  digitalWrite(ENC1_ADD+encoder_no-1,LOW);   

  SPI.transfer(0x98);    
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);           
  SPI.transfer(0x00);           
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(ENC1_ADD+encoder_no-1,HIGH);  
  
  delayMicroseconds(100);  
  
  digitalWrite(ENC1_ADD+encoder_no-1,LOW);    
  SPI.transfer(0xE0);    
  digitalWrite(ENC1_ADD+encoder_no-1,HIGH);     
}

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
    digitalWrite(RMOTOR_ENA, LOW);
    digitalWrite(RMOTOR_ENB, HIGH);
    analogWrite(RMOTOR_PWM, rmotor_pwm);
  }
  else if (rmotor_pwm < 0) // Backward
  {
    digitalWrite(RMOTOR_ENA, HIGH);
    digitalWrite(RMOTOR_ENB, LOW);
    analogWrite(RMOTOR_PWM, -rmotor_pwm);
  }
  else
  {
    digitalWrite(RMOTOR_ENA, LOW);
    digitalWrite(RMOTOR_ENB, LOW);
    digitalWrite(RMOTOR_PWM, 0);
  }
}

void motor_control(int F_pid_pwm, int R_pid_pwm)
{
  front_motor_control(F_pid_pwm);
  rear_motor_control(R_pid_pwm);  
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

  initEncoders();          
  clearEncoderCount(1); 
  clearEncoderCount(2); 
  
  MsTimer2::set(50, Timer); //
  MsTimer2::start();
  Serial.begin(115200);
}

void fuc_init()
{
  targetPos = 0;
  encoderFcount= 0;
  encoderRcount= 0;          
  clearEncoderCount(1); 
  clearEncoderCount(2);
  encoderPos_old = 0;
  angular_speed = 0;
  target_angular_speed = 0;
  angular_speed_error_sum = 0;
  pos_error_sum = 0;
  Fangular_speed_pid_pwm = 0;
  Rangular_speed_pid_pwm = 0;
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
      fuc_init();
      
      motor_direction = Serial.read();
      
      for (int i = 0; i < 4; i++) 
      {
        data_buffer[i] = Serial.read();
      }
      Serial.write(data_buffer[0]);
      Serial.write(data_buffer[1]);
      Serial.write(data_buffer[2]);
      Serial.write(data_buffer[3]);
      
      s = String(data_buffer);
      data = s.toInt();
      
      if(motor_direction =='r') data = -data;
      
      Serial.print("  : ");      
      Serial.println(data);

      motor_control(input_Fspeed, input_Rspeed);
     }

    if (check_char == 'p')  // Position Control
    {
      fuc_init();
       
      motor_direction = Serial.read();
      
      for (int i = 0; i < 4; i++) 
      {
        data_buffer[i] = Serial.read();
      }
      Serial.write(data_buffer[0]);
      Serial.write(data_buffer[1]);
      Serial.write(data_buffer[2]);
      Serial.write(data_buffer[3]);
      
      s = String(data_buffer);
      data = s.toInt();
      
      if(motor_direction =='r') data = -data;
      
      Serial.print("  : ");      
      Serial.println(data);
      
      delay(1000);
      
      targetPos = data;
     }

     if (check_char == 'v')  // Velocity Control
     {
      
      fuc_init();
      
      motor_direction = Serial.read();
      
      for (int i = 0; i < 4; i++) 
      {
        data_buffer[i] = Serial.read();
      }
      Serial.write(data_buffer[0]);
      Serial.write(data_buffer[1]);
      Serial.write(data_buffer[2]);
      Serial.write(data_buffer[3]);
      
      s = String(data_buffer);
      data = s.toInt();
      
      if(motor_direction =='r') data = -data;
      
      Serial.print("  : ");      
      Serial.println(data);

      delay(2000);
      
      target_angular_speed = data;
     }
  }
  Serial.print("Speed = ");
  Serial.print(angular_speed);
  Serial.print("  encoderFCount = ");
  Serial.print(encoderFcount);
  Serial.print("  encoderRCount = ");
  Serial.print(encoderRcount);
  Serial.print("  angular_speed_error_d = ");
  Serial.print(angular_speed_error_d);
  Serial.print("  targetPos = ");
  Serial.println(targetPos);
}
