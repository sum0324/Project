#include <Stepper.h>   // 스테퍼 모터 전용 함수

#define baudRate 9600
#define LM35 A0      // LM35 핀을 Analog 0번 핀으로
#define CDS A1      // CDS 핀을 Analog 1번 핀으로
#define Stepper_Speed_MAX 50
#define Stepper_Speed_NORM 25
#define Stepper_Speed_MIN 0
#define Boolean_ZERO 0

#define STEPS 2048      // 스테퍼 모터를 2048 스텝만큼 동작시킬 상수
#define IN1 2      
#define IN2 3
#define IN3 4
#define IN4 5    // 2 ~ 5 까지 스테퍼 모터와 연결된 핀
  
Stepper stepper(STEPS, IN1, IN3, IN2, IN4);   // Stepper 클래스 객체를 stepper()로 대체(인스턴스)

void setup() {
  Serial.begin(baudRate);    // 시리얼 모니터 통신 9600bps
  stepper.setSpeed(Stepper_Speed_NORM);
}

void loop() {
  float Reading_Temp = analogRead(LM35);
  float TempC = (5.0*Reading_Temp*100.0)/1024.0;
  float Light_Lev = analogRead(CDS)/ 1023.0;   // 온도, 밝기를 float형으로 선언
  int Stepper_Speed = map(TempC, 0.0, 50.0, Stepper_Speed_MIN, Stepper_Speed_MAX);

  stepper.step(STEPS); //오른쪽으로 한바퀴
  delay(1000); //1초 딜레이
  stepper.step(STEPS); //왼쪽으로 한바퀴
  delay(1000);

  Serial.print("TempC : ");      // TempC 시리얼 모니터에 출력
  Serial.print(TempC);
  Serial.print("℃");             // \t : TempC출력과의 일정 간격 유지를 위한 개행문자, \n(줄 바꿈)과 다름
  Serial.print("\tLight level : ");
  Serial.print(Light_Lev);
  Serial.print("\n");


  if(TempC >= 45) { // LM35값 읽은 결과 25℃ 이상일 때 평균 TempC : 45℃
    Stepper_Speed = Stepper_Speed_MAX;   // 온도가 25℃ 이상일 때 모터 스텝 수 50으로 설정 -> 2π(모터 한 바퀴) = approx 41s.
  } 
  else if(TempC < 45){
    Stepper_Speed = Stepper_Speed_MIN; 
  }
}
  
  if(Light_Lev <= 0.25) {  
    Stepper_Speed = Stepper_Speed_MAX;
  } 
  else if(Light_Lev > 0.25){
    Stepper_Speed = Stepper_Speed_MIN; 
  }
  delay(2000);
}