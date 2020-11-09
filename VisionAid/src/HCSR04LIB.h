// HCSR04LIB.h
// Sam Freisem-Kirov  Anuragini Arora
// sfreisemkirov@hmc.edu
// 11/7/20

#ifndef HCSR04LIB_H
#define HCSR04LIB_H

#include "STM32F401RE.h"

#define DELAY_TIM TIM2

void initSensor(int inPin, int outPin);
int getDistance(int inPin, int outPin);

#endif