void timerISR() {
  static unsigned long counter1 = 0;
  static unsigned long counter2 = 0;
  static unsigned long remainder1 = 0;
  static unsigned long remainder2 = 0;
  
  // Первый двигатель
  if (stepInterval1 > 0) {
    counter1 += 10;
    if (counter1 >= stepInterval1) {
      remainder1 = counter1 - stepInterval1;
      counter1 = remainder1;
      
      if (stepState1 == LOW) {
        PORTD |= (1 << STEP1_PIN);
        stepState1 = HIGH;
      } else {
        PORTD &= ~(1 << STEP1_PIN);
        stepState1 = LOW;
      }
    }
  }
  
  // Второй двигатель
  if (stepInterval2 > 0) {
    counter2 += 10;
    if (counter2 >= stepInterval2) {
      remainder2 = counter2 - stepInterval2;
      counter2 = remainder2;
      
      if (stepState2 == LOW) {
        PORTD |= (1 << STEP2_PIN);
        stepState2 = HIGH;
      } else {
        PORTD &= ~(1 << STEP2_PIN);
        stepState2 = LOW;
      }
    }
  }
}

void setSpeed1(long stepsPerSecond) {
  if (stepsPerSecond == 0) {
    stepInterval1 = 0;
    return;
  }
  
  if (stepsPerSecond > 0) {
    PORTD |= (1 << DIR1_PIN);
  } else {
    PORTD &= ~(1 << DIR1_PIN);
    stepsPerSecond = -stepsPerSecond;
  }
  
  stepInterval1 = 500000L / stepsPerSecond;
}

void setSpeed2(long stepsPerSecond) {
  if (stepsPerSecond == 0) {
    stepInterval2 = 0;
    return;
  }
  
  if (stepsPerSecond > 0) {
    PORTD |= (1 << DIR2_PIN);
  } else {
    PORTD &= ~(1 << DIR2_PIN);
    stepsPerSecond = -stepsPerSecond;
  }
  
  stepInterval2 = 500000L / stepsPerSecond;
}
