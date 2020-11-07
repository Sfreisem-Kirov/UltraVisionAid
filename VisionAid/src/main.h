// main.h
// Sam Freisem-Kirov  Anuragini Arora
// sfreisemkirov@hmc.edu
// 11/5/20

#ifndef MAIN_H
#define MAIN_H



#include "stm32f4xx.h"
#define LED_PIN 5
#define DELAY_TIM TIM2
#define BUTTON_PIN 13 // PC13

#define NVIC_ISER0 ((uint32_t *) 0xE000E100UL)
#define NVIC_ISER1 ((uint32_t *) 0xE000E104UL)
#define SYSCFG_EXTICR4 ((uint32_t *) (0x40013800UL + 0x14UL))

void setup();

#endif // MAIN_H