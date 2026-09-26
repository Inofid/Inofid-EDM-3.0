void PIDR(){  
  regulator_1.setDirection(NORMAL);
  regulator_1.setLimits(-8000, 8000); // скорость в шаг/сек
  regulator_1.setpoint = 130;           // значение analogRead() к которому стремится PID регулятор
  
  regulator_1.Kp = 100;
  regulator_1.Ki = 5;
  regulator_1.Kd = 0;  
}
