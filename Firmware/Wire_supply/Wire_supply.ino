#include <TimerOne.h>
#include <GyverPID.h>

GyverPID regulator_1(150, 1, 0, 50), regulator_2(150, 1, 0, 50); // объявляем PID регулятор

// Первый шаговый двигатель
#define STEP1_PIN 3 
#define DIR1_PIN 2

// Второй шаговый двигатель
#define STEP2_PIN 5
#define DIR2_PIN 4

volatile unsigned long stepInterval1 = 0;
volatile unsigned long stepInterval2 = 0;
volatile bool stepState1 = LOW;
volatile bool stepState2 = LOW;

int spd1 = 0, spd2 = 0;

void setup() {
  setupADC();
  pinMode(STEP1_PIN, OUTPUT); // пины драйверов шаговых двигателей (TMC2209)
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(STEP2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);

  digitalWrite(STEP1_PIN, LOW);
  digitalWrite(STEP2_PIN, LOW);

  Timer1.initialize(10);
  Timer1.attachInterrupt(timerISR);

  setSpeed1(0);
  setSpeed2(0);

  PIDR();
}

void loop() {
  /************* расчёт скорости первого шагового двигателя *********************/
  int ads1 = fastAnalogRead(0); // более быстрая версия analogRead(A0) опрашивает первый датчик AS5600, который работает в аналоговом режиме (считываем напряжение с пина OUT) 
  if ((ads1 < 50) || (ads1 > 600)) spd1 = 0;  
  else {
    regulator_1.input = ads1;
    spd1 = regulator_1.getResult();
  }
  setSpeed1(spd1);
  // если значение меньше 50 или больше 600 (если рычаг повёрнут левее нуля, то значения будут не -1 и ниже, а наоборот, резко поднимется до 4000 и будет уменьшаться), значит рычаг в "мёртвой" зоне, что говорит об обрыве проволоки или рычаг в стартовом положении
  // в этом случае останавливаются шаговые двигатели
  // чтобы запустить систему, нужно вручную натянуть проволоку или наклонить рычаг, так чтобы он вышел из этой "мёртвой" зоны, после чего система автоматически запустится
  // магниты изначально нужно установить таким образом, чтобы в начальном положении рычага датчик выдавал значение примерно 10-15

  /************* расчёт скорости второго шагового двигателя *********************/
  int ads2 = fastAnalogRead(1); // более быстрая версия analogRead(A0) опрашивает первый датчик AS5600, который работает в аналоговом режиме (считываем напряжение с пина OUT)
  if ((ads2 < 50) || (ads2 > 600)) spd2 = 0;
  else {
    regulator_2.input = ads2;
    spd2 = regulator_2.getResult();
  }
  setSpeed2(spd2);
}
