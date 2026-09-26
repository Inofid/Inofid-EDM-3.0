void alarm() {
  /************  управление пищалкой и сигнальным светодиодом ********/
  tone(9, 800);  // пин D9 на драйвер мосфета для пищалки
  digitalWrite(8, 1); // пин светодиода
  delay(500);

  tone(9, 1400);
  digitalWrite(8, 0);
  delay(500);
  noTone(9);
}

void alarm_slippage() {
  /************  сигнал о проскальзывании проволоки ********/
    tone(9, 2000);
    digitalWrite(8, 1);
    delay(100);
    noTone(9);
    digitalWrite(8, 0);  
}
