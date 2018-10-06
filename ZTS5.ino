/*
 * ZTS4.ino
 *
 * Created: 10/1/2018 9:21:11 PM
 * Author: D1470
 */ 
#include<Wire.h>
#include<string.h>
#include<math.h>
#include<Servo.h>
Servo myservo;
Servo servol1;
float X = 0;
float Y = 0;
float p= 0;
float LI[4]={0};
float MU1X = 200, MU1Y = 200;
float MU2X = 4600, MU2Y = 200;
float MU3X = 4600, MU3Y = 4600;
float MU4X = 200, MU4Y = 4600;  
float pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;  
  float CHA1X =0,CHA2X =0,CHA3X =0,CHA4X =0;
   float CHA1Y=0,CHA2Y=0,CHA3Y=0,CHA4Y=0;
   void requestEvent()//验证是否掉线
   {
    
       Wire.write(4);
     
   }
    void readic(){//读取坐标
      String str = Wire.readStringUntil('S'); //读取接受的数据

      int pos = str.indexOf('S');
      String str2 = str.substring(0, pos); //读取位置

      int pos_X = str.indexOf("X:");

      String str_X = str.substring(pos_X + 2); //提取X轴位置
      int pos_Y = str.indexOf("Y:");

      String str_Y = str.substring(pos_Y + 2);
      int pos_P = str.indexOf("P:");
      String str_P = str.substring(pos_P + 2);
      
      X = str_X.toInt();
      Y = str_Y.toInt();
     p= str_P.toInt();
    Serial.println(X);
  }
void goal1(){
   CHA1X = X - MU1X;
    CHA1Y = Y - MU1Y;
  CHA1X = abs(CHA1X);
  CHA1Y = abs(CHA1Y);
  float TAN_1 = 0;
  float TAN1 = 0;
  TAN_1 = CHA1X / CHA1Y;
  TAN1 = atan (TAN_1) * 180 / PI;
  if (Y < MU1Y && X > MU1X) {  
    pos1 = -p + TAN1;
  }
  else if (Y > MU1Y && X > MU1X) {  
    pos1 = -p - 180 + TAN1;
  }
  else  
  {
    pos1 = -p + 180 - TAN1;
  }
}
void goal2(){
   
    CHA2X = X - MU2X; //二号位开始
    CHA2Y = Y - MU2Y;
  CHA2X = abs(CHA2X);
  CHA2Y = abs(CHA2Y);
  float TAN_2 = CHA2Y / CHA2X;
  float TAN_21 = CHA2X / CHA2Y;
  float TAN2 = 0;
  float TAN21 = 0;
  TAN2 = atan (TAN_2) * 180 / PI;
  TAN21 = atan (TAN_21) * 180 / PI;


  if (Y < MU2Y && X < MU2X) { //车在二号点左下方
    pos2 = -p + 90 + TAN21;
  }
  else if (Y > MU2Y && X < MU2X) { //车在二号位左上方
    pos2 = -p + 180 - TAN21;
    } else   {
    pos2 = -p - (180 - TAN21);
  }
  }
void goal3(){
    CHA3X = X - MU3X;  
    CHA3Y = Y - MU3Y;
  CHA3X = abs(CHA3X);
  CHA3Y = abs(CHA3Y);

  float TAN_3 = CHA3X / CHA3Y;
  float TAN3 = 0;

  TAN3 = atan (TAN_3) * 180 / PI;

  if (Y < MU3Y && X > MU3X) {  
    pos3 = -p - TAN3;
  }
  else if (Y < MU3Y && X < MU3X) {  
    pos3 = -p + TAN3;
  }
  else  {  
    pos3 = -p + 180 - TAN3;
  }
} 
void goal4(){
    CHA4X = X - MU4X;  
    CHA4Y = Y - MU4Y;
  CHA4X = abs(CHA4X);
  CHA4Y = abs(CHA4Y);
  float TAN_4 = CHA4X / CHA4Y;
  float TAN4 = 0;
  TAN4 = atan (TAN_4) * 180 / PI;
  if (Y > MU4Y && X > MU4X) {
    pos4 = -180 - p + TAN4;
  }
  else if (Y < MU4Y && X > MU4X) {
    pos4 = -p - TAN4;
  }
  else
  {
    pos4 = -p + TAN4;
  }
}
float val = 0, val2 = 0;
  float old_pos = 0;
  float new_pos = 0;
  float Z_pos = 0;
  float W_pos = 0;
void turnport(){                
  int data[ ]={pos1,pos2,pos3,pos4};
   
 
  for(int t=0;t<4;t++){
    new_pos =  data[t];
    Z_pos = new_pos - old_pos;
    val2 = Z_pos * 90 / 16 /  0.1125;
    old_pos = new_pos;
    W_pos = Z_pos+W_pos;
    if (val2 < 0)
    {
      val = -val2;
      } else {
      val = val2;
    }
    
    //Serial.println(val);
    //Serial.println(new_pos);
     
    //Serial.println(old_pos);
    if (18000 >= val2 &&val2> 0)
    {
      digitalWrite(4, HIGH);
      for (int s = 0; s <= val;s++ )
      {
        
        digitalWrite(5, HIGH);
        delayMicroseconds(80);
        digitalWrite(5, LOW); // Output low

        delayMicroseconds(80);
        
      }
    }
    else if (-18000<= val2 && val2< 0)
    {

      digitalWrite(4, LOW);
      for (int t = 0; t <= val;t++)
      {
        digitalWrite(5, HIGH);
        delayMicroseconds(80);
        digitalWrite(5, LOW); // Output low

        delayMicroseconds(80);
        
      }
    }
      else
      {
        digitalWrite(5, LOW);

    }
      
       plectrum();
       myservo.writeMicroseconds(1000);
       servol1.writeMicroseconds(1000);
    delay(2000);
    }
    Return();
  }
  void Return(){//步进回位
    val2 = W_pos * 90 / 16 /  0.1125;
    if (val2 < 0)
    {
      val = -val2;
      } else {
      val = val2;
    }
    
    if (18000 >= val2 &&val2> 0)
    {
      digitalWrite(4, LOW);
      for (int s = 0; s <= val;s++ )
      {
        
        digitalWrite(5, HIGH);
        delayMicroseconds(80);
        digitalWrite(5, LOW); // Output low

        delayMicroseconds(80);
        
      }
    }
    
    else if (-18000<= val2 && val2< 0)
    {

      digitalWrite(4, HIGH);
      for (int t = 0; t <= val;t++)
      {
        digitalWrite(5, HIGH);
        delayMicroseconds(80);
        digitalWrite(5, LOW); // Output low

        delayMicroseconds(80);
        
      }
       
    }
    
  }
  float JU1=0,JU2=0,JU3=0,JU4=0;
   
  
  void SHOOT(){//解算脉宽
     
     
    
    JU1= sqrt(CHA1X* CHA1X+ CHA1Y*CHA1Y);
     JU2= sqrt(CHA2X* CHA2X+ CHA2Y*CHA2Y);
     JU3= sqrt(CHA3X* CHA3X+ CHA3Y*CHA3Y);
     JU4= sqrt(CHA4X* CHA4X+ CHA4Y*CHA4Y);
    
    int dataLI[]={ JU1, JU2, JU3, JU4};
     
      String str = Wire.readStringUntil('T');
      if ( str.indexOf("HOO") != -1)
      {
        //myservo.writeMicroseconds(1100);
        //servol1.writeMicroseconds(1100);
        Serial.println(2);
      }
     
    } 

void plectrum(){//N20转动
   
  pinMode(12,OUTPUT);
   delay(500);
  digitalWrite(12,HIGH);
  delay(1570);
  digitalWrite(12,LOW);
   
}
void begining()//上电启动
{
   
 
    String str = Wire.readStringUntil('\n');

    if ( str .indexOf("begin") != -1)
    {
       Serial.println(1);
      myservo.writeMicroseconds(2000);
      servol1.writeMicroseconds(2000);

      delay(3500);
      myservo.writeMicroseconds(1000);
      servol1.writeMicroseconds(1000);
      delay(2000);
      myservo.writeMicroseconds(1000);
      servol1.writeMicroseconds(1000);
    
  }
}
void receiveEvent(int howMany){
  begining();//上电启动
  readic();//读取坐标
  goal1();
  goal2();
  goal3();
  goal4();//解算角度
  SHOOT();//解算脉宽
  turnport();//转动云台并射击
   
   
}
void setup()
{
  Wire.begin(4);
  Serial.begin(9600);
myservo.attach(10);
servol1.attach(9);
 
  pinMode(6, OUTPUT); // Enable: EN可以使用单片机端口控制，也可以直接连接GND使能

  pinMode(5, OUTPUT); // steps:脉冲个数

  pinMode(4, OUTPUT); // dir:为方向控制
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(7,OUTPUT);
  digitalWrite(3, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(7, HIGH);  /* add setup code here, setup code runs once when the processor starts */
  Wire.onReceive(receiveEvent);/* add main program code here, this code starts again each time it ends */
    Wire.onRequest(requestEvent);
}

void loop()
{

    /* add main program code here, this code starts again each time it ends */

}

