/*
HC-SR04 초음파 센서
http://www.devicemart.co.kr/
*/

int trig[6] = {4, 8, 12, 2, 7, 13};
int echo[6] = {3, 9, 11, 5, 6, 10};
int distance[6] = {0, 0, 0, 0, 0, 0};
int check[3] = {0, 0, 0};
int checkcool[3] = {0, 0, 0};
int memory[100];
int count = 0;
int temp = 0;
int humans = 0;
int phumans = -1;
int var = 0;



void setup() {

   for(int i = 0; i<100; i++){
      memory[i] = 0;
   }
   Serial.begin(9600);      // 통신속도 9600bps로 시리얼 통신 시작
   for(int i = 0; i<3; i++){
     pinMode(trig[i], OUTPUT);    // 트리거 핀 출력으로 선언
     pinMode(echo[i], INPUT);     // 에코 핀 입력으로 선언
   }
}
void loop()
{
   long distance1, distance2, distance3;    // 거리 측정을 위한 변수 선언
   // 트리거 핀으로 10us 동안 펄스 출력
   for(int i = 0; i<6; i++){
     digitalWrite(trig[i], LOW);        // Trig 핀 Low
   }
   delayMicroseconds(2);            // 2us 딜레이
   for(int i = 0; i<6; i++){
     digitalWrite(trig[i], HIGH);        // Trig 핀 Low
   }
   delayMicroseconds(10);            // 10us 딜레이
   for(int i = 0; i<6; i++){
     digitalWrite(trig[i], LOW);        // Trig 핀 Low
     distance[i] = pulseIn(echo[i], HIGH, 20000) * 17 / 1000;
     digitalWrite(echo[i], LOW);
   }
   
   for(int i = 0; i<3; i++){
     // if (i==1) continue;
     if((1<=distance[i] && distance[i]<=40)  || (1<=distance[i+3] && distance[i+3]<=40)){
       count = 8;
       if (check[i]==0 && i != 1) {
         for(int j=0; j<100; j++) {
           if (memory[j] == 0) {
             if (j>0 && memory[j-1] == i+1) break;
             memory[j] = i+1;
             break;
           }
         }
         check[i] = 1;
       }
     } else {
       if (check[i] == 1) check[i] = 0;
     }
   }

   temp = 0;
   for(int i = 0; i<3; i++) {
     if (check[i] == 0) {
       temp++;
     }
   }
   var = 0;
   if (temp == 3 && count>0) {
     if (count == 1) {
       for(int j=0; j<100; j++) {
         if(memory[j+1] == 0){
           if (memory[0] == memory[j]) var=0;
           for(int k=0; k<=j; k++) {
             memory[k] = 0;
           }
           break;
         }
         // Serial.print(memory[j]);
         if (memory[j]-memory[j+1]>0 && humans>0) {
           var--;
           if (memory[0]==1 && j!=0 && memory[j-1]+memory[j+2]==4) {
             var++;
           }
         }
         else if (memory[j]-memory[j+1]<0) {
           var++;
           if (memory[0]==3 && j!=0 && memory[j-1]+memory[j+2]==4 && humans>0) {
             var--;
           }
         }
       }
       //Serial.println();
       humans = humans+var;
     }
     count--;
   }
   Serial.println(humans);
   /*
   if (phumans != humans) {
     Serial.println(humans);
     phumans = humans;
   }
   */
   delay(1);
   
}
