void PIDR(){  
  regulator_1.setDirection(REVERSE);
  regulator_1.setLimits(-18000, 18000); // скорость в шаг/сек
  regulator_1.setpoint = 150;           // значение analogRead() к которому стремится PID регулятор
  
  regulator_1.Kp = 150;
  regulator_1.Ki = 0.8;
  regulator_1.Kd = 0;
  
  regulator_2.setDirection(REVERSE);
  regulator_2.setLimits(-18000, 18000); // скорость в шаг/сек
  regulator_2.setpoint = 150;           // значение analogRead() к которому стремится PID регулятор
  
  regulator_2.Kp = 150;
  regulator_2.Ki = 0.8;
  regulator_2.Kd = 0;
  
}
