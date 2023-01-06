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

int input_Fspeed = 0, input_Rspeed = 0;

signed long encoderFcount = 0;
signed long encoderRcount = 0;

int32_t targetPos = 0;
int32_t pos_error = 0;
int32_t pos_error_old = 0;
int32_t pos_error_d = 0;
int32_t pos_error_sum = 0;

char data_buffer[4] = {0};
char motor_direction;

float P = 3.0;
float Pd = 5.0;
float Pi = 0.2;

int pos_pid_pwm = 0;

void Timer()
{
  pos_error = targetPos - encoderRcount;
  pos_error_d = pos_error - pos_error_old;
  
  pos_error_sum += pos_error;
  pos_error_sum = (pos_error_sum > 50) ? 50 : pos_error_sum;
  pos_error_sum = (pos_error_sum < -50) ? -50: pos_error_sum;
  if(fabs(pos_error)<=2) pos_error_sum = 0;
  
  pos_pid_control();
  pos_error_old = pos_error;
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


int pos_pid_control(void)
{
  pos_pid_pwm = P * pos_error + Pd * pos_error_d + Pi *pos_error_sum ;

  pos_pid_pwm = (pos_pid_pwm >= 255) ? 255 : pos_pid_pwm;
  pos_pid_pwm = (pos_pid_pwm <= -255) ? -255 : pos_pid_pwm;

  motor_control(pos_pid_pwm);
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

void motor_control(int input_speed)
{
  front_motor_control(input_speed);
  rear_motor_control(input_speed);  
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
  
  MsTimer2::set(10, Timer); //
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
    
    if (check_char == 'P')  // Position Control
    {
      targetPos = 0;
      encoderFcount= 0;
      encoderRcount= 0;          
      clearEncoderCount(1); 
      clearEncoderCount(2);
      
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
     
  encoderFcount = readEncoder(1);
  encoderRcount = readEncoder(2);

  Serial.print("targetPos = ");
  Serial.print(targetPos);
  Serial.print("  encoderRCount = ");
  Serial.print(encoderRcount);
  Serial.print("  pos_pid_pwm = ");
  Serial.println(pos_pid_pwm);
  }
}
