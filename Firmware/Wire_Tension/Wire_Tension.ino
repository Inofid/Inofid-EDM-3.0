#include <TimerOne.h>
#include <GyverPID.h>

GyverPID regulator_1(150, 1, 0, 50); // объявляем PID регулятор

// Первый шаговый двигатель
#define STEP1_PIN 3
#define DIR1_PIN 2
#define ENABLE_1_PIN 6

// Второй шаговый двигатель
#define STEP2_PIN 5
#define DIR2_PIN 4
#define ENABLE_2_PIN 7

volatile unsigned long stepInterval1 = 0;
volatile unsigned long stepInterval2 = 0;
volatile bool stepState1 = LOW;
volatile bool stepState2 = LOW;
bool pick = 0;

int spd1 = 0, spd2 = 0;
unsigned long i = 0;

void setup() {
  pinMode(12, INPUT); // кнопка управления направлением вращения второго шаговика,  подаёт 5В во включённом состоянии, в выключенном через резистор 10кОм подтягивается к GND
  pinMode(ENABLE_1_PIN, OUTPUT); // пины enable драйверов шаговых двигателей (TMC2209)
  pinMode(ENABLE_2_PIN, OUTPUT);
  pinMode(8, OUTPUT);  // сигнальный светодиод (через резистор 220 Ом)
  pinMode(9, OUTPUT);  // от пина напрямую на пищалку подавать сигнал нельзя, так как слишком большой ток. Этот пин управляет драйвером UCC27517DBVR, который подаёт на пищалку 12В и выдерживает необходимый ток, от чего пищалка орёт как положено.

  pinMode(STEP1_PIN, OUTPUT); // пины драйверов шаговых двигателей
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(STEP2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);

  digitalWrite(STEP1_PIN, LOW);
  digitalWrite(STEP2_PIN, LOW);

  digitalWrite(ENABLE_1_PIN, LOW);
  digitalWrite(ENABLE_2_PIN, LOW);

  Timer1.initialize(10);
  Timer1.attachInterrupt(timerISR);

  setSpeed1(0);
  setSpeed2(0);

  PIDR();
}

void loop() {
  i++; // переменная i — костыль, который используется в качестве замены таймера, так как он сбит управлением шаговыми двигателями
  int ads = analogRead(A0);  // опрос пина OUT датчика AS5600 на рычаге, определяющим натяжение проволоки (AS5600 необходимо перевести в аналоговый режим)
  if ((ads < 15) || (ads > 500)) { // если вне рабочей зоны, значит рычаг в начальном положении (обрыв проволоки, либо стартовое положение и нужно вручную наклонить рычаг для запуска работы)
    setSpeed1(0); // останавливаем шаговые двигатели
    setSpeed2(0);
    digitalWrite(ENABLE_1_PIN, HIGH); // разблокируем шаговые двигатели
    digitalWrite(ENABLE_2_PIN, HIGH);
    alarm();  // запусаем пищалку и мигание светодиодом
  }
  else {
    digitalWrite(ENABLE_1_PIN, LOW); // активируем шаговые двигатели
    digitalWrite(ENABLE_2_PIN, LOW);

    regulator_1.input = ads; // расчёт скорости 1-го шагового двигателя с помощью PID регулятора исходя из положения рычага
    spd1 = regulator_1.getResult();
    setSpeed1(spd1);

    spd2 = map(analogRead(A1), 0, 1023, 0, 5000); // скорость второго шагового двигателя задаётся потенциометром на пине A1
    spd2 = constrain(spd2, 0, 5000);  // шаги в сек
    if (digitalRead(12)) setSpeed2(spd2);  // на пине D12 кнопка, подтягивающая 5В. Пин так же подтянут к земле через резистор 10кОм. Кнопка определяет направление вращения 2-го шагового двигателя 
    else setSpeed2(-spd2);

    if (i  > 40) {
      if (abs(spd2 - abs(spd1)) > 200) alarm_slippage();  // если скорости шаговых двигателей сильно различаются, значит проволока проскальзывает — издаём звуковой сигнал и мигаем светодиодом с интервалом, который задаётся костыльной переменной i
      i = 0;
    }
  }
}
