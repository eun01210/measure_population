#include <Wire.h>
#include <Adafruit_VL53L0X.h>
#include <sensor_inte.h>

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
ft_lidar lidar;

int distance[3];
int check[3];
int memory[100];
int count = 0;
int temp = 0;
int humans = -1;
int phumans = -1;
int var = 0;

int pluspin = 13;
int minuspin = 8;
int buttonche[2];


void setup() { 
   lidar.tcasetup(lox); // 라이다 기본 세팅
   pinMode(pluspin, INPUT);
   pinMode(minuspin, INPUT);
   Serial.begin(9600);      // 통신속도 9600bps로 시리얼 통신 시작
}
void loop()
{
   long distance1, distance2, distance3;    // 거리 측정을 위한 변수 선언
   // 트리거 핀으로 10us 동안 펄스 출력

   // 라이다 값 저장 후 cm로 환산하여 기록
   int* a = lidar.front_lidar(lox);
   distance[0] = a[0]/10;
   int* b = lidar.left_lidar(lox);
   distance[1] = b[0]/10;
   int* c = lidar.right_lidar(lox);
   distance[2] = c[0]/10;

   // 라이다 감지 결과 분석 함수
   for(int i = 0; i<3; i++){ // 라이다 3개 확인
     // if (i==1) continue;
     if((1<=distance[i] && distance[i]<=40)  || (1<=distance[i+3] && distance[i+3]<=40)){ // i 번째 라이다가 감지된 경우
       count = 5; //5 카운트 추가(라이다가 감지되지 않았을 때의 딜레이, 대기 중)
       if (check[i]==0 && i != 1) { // 해당 라이다가 가운데 라이다가 아니며, 새로 감지한 라이다인 경우
         for(int j=0; j<100; j++) { // 메모리에 해당 라이다 정보를 저장하는 함수
           if (memory[j] == 0) { // j 번째 칸이 비어있을 경우
             if (j>0 && memory[j-1] == i+1) break; // 이전 칸에 동일한 라이다 정보가 있는 경우 패스
             memory[j] = i+1; // j 번째 칸에 해당 라이다 정보 저장
             break;
           }
         }
         check[i] = 1; // 해당 라이다를 감지 중인 라이다로 설정
       }
     } else { // 해당 라이다가 감지되지 않은 경우
       if (check[i] == 1) check[i] = 0; // 해당 라이다를 감지되지 않은 라이다로 설정
     }
   }

   temp = 0; // 임시 변수 선언(감지되지 않은 라이다 확인)
   for(int i = 0; i<3; i++) { // 감지되지 않은 라이다 수만큼 임시 변수++
     if (check[i] == 0) {
       temp++;
     }
   }
   var = 0; // 임시 변수2 선언(최종 인원 변동을 담당하는 변수)
   if (temp == 3 && count>0) { // 모든 라이다가 작동하지 않고 있으며, 카운트가 남아 있는 경우
     if (count == 1) { // 카운트가 1인 경우(인원 변동 적용)
       for(int j=0; j<100; j++) { // 메모리 배열 확인
         if(memory[j+1] == 0){ // 가장 마지막 메모리 값
           if (memory[0] == memory[j]) var=0; // 첫 번째 메모리 값과 마지막 메모리 값이 같은 경우 변동 X (121, 1221 등 들어갔다 나온 경우)
           for(int k=0; k<=j; k++) { // 메모리 값 초기화
             memory[k] = 0;
           }
           break;
         }
         // Serial.print(memory[j]);
         if (memory[j]-memory[j+1]>0 && humans>0) { // 사람 수가 1 이상이며, 나가는 경우(21)
           var--; // 사람 감소
           if (memory[0]==1 && j!=0 && memory[j-1]+memory[j+2]==4) { // (첫 번째 메모리가 1일때 2명 이상 들어온 경우 감지)
             var++; // 이 경우는 나가는 경우가 아님
           }
         }
         else if (memory[j]-memory[j+1]<0) { // 들어오는 경우 (12) 
           var++; // 사람 증가
           if (memory[0]==3 && j!=0 && memory[j-1]+memory[j+2]==4 && humans>0) { // (첫 번째 메모리가 2일때 2명 이상 나간 경우 감지)
             var--; // 이 경우는 들어오는 경우가 아님
           }
         }
       }
       //Serial.println();
       humans = humans+var; // 현재 인원에 인원 변동 값을 적용
       if (humans<0) humans=0; // 사람 수가 0보다 적은 경우 0으로 설정
     }
     count--; // 카운트 제거
   }
   
   // 버튼을 이용한 인원 조절
   if (!buttonche[0] && digitalRead(pluspin)) {
     buttonche[0]=1;
     humans++;
   } else if (buttonche[0] && !digitalRead(pluspin)) {
     buttonche[0]=0;
   }

   if (!buttonche[1] && digitalRead(minuspin)) {
     buttonche[1]=1;
     if (humans>0) humans--;
   } else if (buttonche[1] && !digitalRead(minuspin)) {
     buttonche[1]=0;
   }

   // Serial 모니터로 사람 수 출력(라즈베리파이와 통신 시작)
   Serial.println(humans);
   /*
   if (phumans != humans) {
     Serial.println(humans);
     phumans = humans;
   }
   */
   delay(1);
   
}

/*

[0, 1, 2, 1, 0] +-0
[0, 1, 2, 0, 1, 2] +2


*/
