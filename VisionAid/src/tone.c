// Standard library includes.
#include <stdint.h>
#include <stdlib.h>
// Vendor-provided device header file.
#include "stm32f4xx.h"

int tone() {

    configureFlash();

    configureClock();

    // PA0 connected to speaker
    // set mode to alt func
    pinMode(0,2);
    // set alt func lower register to AF01 to match TIM2_CH1 output
    GPIOA->GPIO_AFRL.AFRL0 = 0b0001;

    int duty = 50;
    int arrp = 255;
    int pscp = (int)((84000000/((arrp+1)*freq)));
    int pscd = 420;
    int arrd = (int)((84000000/(pscd*freq))-1);
    configureTIM2(pscp,arrp,duty);
    configureTIM5(pscd,arrd,duty);
    while (TIM5->SR.UIF != 1);
}

void configureTIM2(int psc, int arr, int duty) {

    // Disable slave mode
    TIM2->SMCR.SMS = 0;

    // Enable timer
    RCC->APB1ENR.TIM2EN = 1;

    TIM2->PSC = psc;
    TIM2->ARR = arr;
    TIM2->CCR1 = (int)((arr)*duty/100);
    
    // Select PWM mode 1 on channel 1
    TIM2->CCMR1.OC1M = 0b110;

    // Enable preload register
    TIM2->CCMR1.OC1PE = 1;

    // Enable fast register
    TIM2->CCMR1.OC1FE = 1;

    // Enable auto-reload preload register
    TIM2->CR1.ARPE = 1;

    // Initialize registers
    TIM2->EGR.UG = 1;

    // Set OC1 polarity to active high
    TIM2->CCER.CC1P = 0;

    // Enable OC1 output
    TIM2->CCER.CC1E = 1;

    // Activate upcounting
    TIM2->CR1.DIR = 0;

    // Select CK_INT as counter clock source
    TIM2->CR1.CEN = 1; 

}

void configureTIM5(int psc, int arr, int duty) {

    TIM5->SR.UIF = 0;
    TIM5->CR1.URS = 1;

    // Disable slave mode
    TIM5->SMCR.SMS = 0;

    // Enable timer
    RCC->APB1ENR.TIM5EN = 1;

    TIM5->PSC = psc;
    TIM5->ARR = arr;
    TIM5->CCR1 = (int)((arr)*duty/100);
    
    // Select PWM mode 1 on channel 1
    TIM5->CCMR1.OC1M = 0b110;

    // Enable preload register
    TIM5->CCMR1.OC1PE = 1;

    // Enable fast register
    TIM5->CCMR1.OC1FE = 1;

    // Enable auto-reload preload register
    TIM5->CR1.ARPE = 1;

    // Initialize registers
    TIM5->EGR.UG = 1;

    // Set OC1 polarity to active high
    TIM5->CCER.CC1P = 0;

    // Enable OC1 output
    TIM5->CCER.CC1E = 1;

    // Activate upcounting
    TIM5->CR1.DIR = 0;

    // Select CK_INT as counter clock source
    TIM5->CR1.CEN = 1; 
    
}