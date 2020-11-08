// Standard library includes.
#include <stdint.h>
#include <stdlib.h>
// Vendor-provided device header file.
//#include "stm32f4xx.h"

#include "STM32F401RE.h"
// #include "tone.h"

void tone(int freq, int delay) {

    // configureFlash();

    // configureClock();

    // PA0 connected to speaker
    // set mode to alt func
    pinMode(GPIOA,0,2);
    // set alt func lower register to AF02 to match TIM5_CH1 output
    // GPIOA->AFRL.AFRL0 = 0b0010;
    GPIOA->AFRL &= ~(0b11 << 2); // 00 in bits 3:2
    GPIOA->AFRL |= (0b1 << 1); // 1 in bit 1
    GPIOA->AFRL &= ~(0b1 << 0); // 0 in bit 0


    int duty = 50;
    int arr = 255;
    int psc = (int)((84000000/((arr+1)*freq)));

    configureTIM5(psc,arr,duty);
    delay_millis(TIM2, delay);
    configureTIM5(psc,arr,0);
}

void configureTIM5(int psc, int arr, int duty) {

    // Disable slave mode
    // TIM5->SMCR.SMS = 0;
    TIM5->SMCR &= ~(0b111 << 0);

    // Enable timer
    // RCC->APB1ENR.TIM5EN = 1;
    RCC->APB1ENR |= (0b1 << 3);

    TIM5->PSC = psc;
    TIM5->ARR = arr;
    TIM5->CCR1 = (int)((arr)*duty/100);
    
    // Select PWM mode 1 on channel 1
    // TIM5->CCMR1.OC1M = 0b110;
    TIM5->CCMR1 |= (0b11 << 5);
    TIM5->CCMR1 &= ~(0b1 << 4);

    // Enable preload register
    // TIM5->CCMR1.OC1PE = 1;
    TIM5->CCMR1 |= (0b1 << 3);

    // Enable fast register
    // TIM5->CCMR1.OC1FE = 1;
    TIM5->CCMR1 |= (0b1 << 2);

    // Enable auto-reload preload register
    // TIM5->CR1.ARPE = 1;
    TIM5->CR1 |= (0b1 << 7);

    // Initialize registers
    // TIM5->EGR.UG = 1;
    TIM5->EGR |= (0b1 << 0);

    // Set OC1 polarity to active high
    // TIM5->CCER.CC1P = 0;
    TIM5->CCER &= ~(0b1 << 1);

    // Enable OC1 output
    // TIM5->CCER.CC1E = 1;
    TIM5->CCER |= (0b1 << 0);

    // Activate upcounting
    // TIM5->CR1.DIR = 0;
    TIM5->CR1 &= ~(0b1 << 4);

    // Select CK_INT as counter clock source
    // TIM5->CR1.CEN = 1; 
    TIM5->CR1 |= (0b1 << 0);
}

