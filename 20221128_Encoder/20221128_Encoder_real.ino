#include <MsTimer2.h>

#define encodPinA 2
#define encodPinB 3

int count = 0;

void interrupt_setup(void)
{
  delayMicroseconds(50);
  pinMode(encodPinA, INPUT_PULLUP);                 
  pinMode(encodPinB, INPUT_PULLUP);             
  attachInterrupt(0, encoder, RISING);                               
}

void encoder()  
{                                               
  if(digitalRead(encodPinB)==HIGH){
    count--;            
  }
  else{
    count++;          
  }
}
  
void setup()
{
  MsTimer2::set(100,interrupt_setup);
  MsTimer2::start();
  Serial.begin (115200);
  //delay(2000); 
}

void loop()
{
  Serial.print("count: ");
  Serial.println(count);
}
