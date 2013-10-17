int irin = 2;
int green = 11;
int red = 12;
int led = 13;
int Pin1  =  5;
int Pin2  =  6;
int Pin3  =  9;
int Pin4  =  10;
int bitsize = 4;
int timeout = 200;
int count_unit = 100;
int one = 16;
int zero = 0;
int flag_receive=0;
volatile int enableMortor=0;
volatile int enableMelody=0;


void setup(){
  pinMode(irin, INPUT); 
  pinMode(Pin1, OUTPUT); 
  pinMode(Pin2, OUTPUT); 
  pinMode(Pin3, OUTPUT); 
  pinMode(Pin4, OUTPUT); 
  pinMode(green, OUTPUT); 
  pinMode(red, OUTPUT); 
  attachInterrupt(0, IR_Receive,FALLING);
  Serial.begin(57600);
  Serial.println("Ready");
}


void loop(){
  digitalWrite(led,LOW);
  LED_Change();
  delay(10);
}

int  receiveData[4];
// int  receiveRawData[6];

void IR_Receive(){
  if(flag_receive==0){ 
    for(volatile int i  =  zero; i<bitsize;i++) {
      volatile int count = zero;
      while(digitalRead(irin)  ==  LOW); // LEDが消灯するまで待つ
      while(digitalRead(irin)  ==  HIGH) { // LEDが点灯するまでの時間を計る
        count++;
        delayMicroseconds(count_unit);// 100μ秒単位で計る。そのぐらい大雑把で問題ない。
        if (count > timeout) break; // あまり長い時間点灯しない場合はエラー。
      }
      if (count > timeout) break;
      //receiveRawData[i] = count;

      receiveData[i]  =  (count > one); //1.6ms(1600μ秒)より長ければ1

    }
    for(int i=0;i<bitsize;i++){
      Serial.print(receiveData[i]);
      Serial.print(",");
    }
    Serial.println("END");
    delay(10);
    flag_receive=1;
    IR_RC_Bridge();

  }


}

void IR_RC_Bridge(){
  flag_receive=0;
  if(receiveData[0] == 0 && receiveData[1] == 0 && receiveData[2] == 0 && receiveData[3] == 1){
    RC_Control(1);
    Serial.println("Go Straight");
  }
  else if(receiveData[0] == 0 && receiveData[1] == 1 && receiveData[2] == 0 && receiveData[3] == 0){
    RC_Control(2);
    Serial.println("Turn Left");
  }
  else if(receiveData[0] == 0 && receiveData[1] == 0 && receiveData[2] == 1 && receiveData[3] == 1){
    RC_Control(3);
    Serial.println("Turn Right");
  }
  else if(receiveData[0] == 1 && receiveData[1] == 0 && receiveData[2] == 0 && receiveData[3] == 0){
    RC_Control(4);
    Serial.println("Go Back");
  }
  else if(receiveData[0] == 1 && receiveData[1] == 1 && receiveData[2] == 1 && receiveData[3] == 1){
    RC_Control(5);
    Serial.println("Brake");
  }
  delay(250);
}
void RC_Control(int command){ //1:Go 2:Left 3:Right 4:Back 5:Brake defalut:Stop 

  switch (command){
  case 1:
    enableMortor=1;
    digitalWrite(Pin1,HIGH);
    digitalWrite(Pin2,LOW);
    digitalWrite(Pin3,HIGH);
    digitalWrite(Pin4,LOW);
    break;
  case 2:
    enableMortor=1;
    digitalWrite(Pin1,HIGH);
    digitalWrite(Pin2,LOW);
    digitalWrite(Pin3,LOW);
    digitalWrite(Pin4,HIGH);
    break;
  case 3:
    enableMortor=1;
    digitalWrite(Pin1,LOW);
    digitalWrite(Pin2,HIGH);
    digitalWrite(Pin3,HIGH);
    digitalWrite(Pin4,LOW);
    break;
  case 4:
    enableMortor=1;
    digitalWrite(Pin1,LOW);
    digitalWrite(Pin2,HIGH);
    digitalWrite(Pin3,LOW);
    digitalWrite(Pin4,HIGH);
    break;
  case 5:
    enableMortor=0;
    enableMelody=0;
    digitalWrite(Pin1,HIGH);
    digitalWrite(Pin2,HIGH);
    digitalWrite(Pin3,HIGH);
    digitalWrite(Pin4,HIGH);
    break;
  default:
    enableMortor=0;
    enableMelody=0;
    digitalWrite(Pin1,LOW);
    digitalWrite(Pin2,LOW);
    digitalWrite(Pin3,LOW);
    digitalWrite(Pin4,LOW);
    break;
  }

}

void LED_Change(){
  if(enableMortor==0){
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
  }
  else{
    digitalWrite(green,HIGH);
    digitalWrite(red,LOW);
  }

}
















