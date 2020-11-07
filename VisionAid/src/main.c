// main.c
// Sam Freisem-Kirov  Anuragini Arora
// sfreisemkirov@hmc.edu
// 11/5/20

#include "main2.h"

void setup() {
    // RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN_Pos);

    // GPIOA->MODER &= ~(0b11 << LED_PIN*2);
    // GPIOA->MODER |= (0x01 << LED_PIN*2);
    
    // GPIOA->BSRR = (1 << (LED_PIN + 16));

    // // Initialize timer
    // RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // TIM2EN
    // uint32_t psc_div = (uint32_t) ((SystemCoreClock/1e6)-1); // Set prescaler to give 1 µs time base
    // DELAY_TIM->PSC = (psc_div - 1); // Set prescaler division factor
    // DELAY_TIM->EGR |= TIM_EGR_UG; // Generate an update event to update prescaler value
    // DELAY_TIM->CR1 |= TIM_CR1_CEN; // Set CEN = 1

    // // Setup timer parameters
    // DELAY_TIM->ARR = 1000e3; // Set ARR to 500 ms
    // DELAY_TIM->EGR |= TIM_EGR_UG;     // Force update
    // DELAY_TIM->SR &= ~TIM_SR_UIF; // Reset UIF
    // DELAY_TIM->CNT = 0; // Reset CNT


    configureFlash();
    configureClock();
    
    // Enable LED as output
    RCC->AHB1ENR.GPIOAEN = 1;
    pinMode(GPIOA, LED_PIN, GPIO_OUTPUT);

    // Enable button as input
    RCC->AHB1ENR.GPIOCEN = 1;
    pinMode(GPIOC, BUTTON_PIN, GPIO_INPUT);

    // Initialize timer
    RCC->APB1ENR |= (1 << 0); // TIM2EN
    initTIM(DELAY_TIM);

    
  
    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= (1 << 14); 
    // 2. Set EXTICR4 for PC13
    *SYSCFG_EXTICR4 |= (0b0010 << 4*1);

    // Enable interrupts globally
    __enable_irq();

    // Configure interrupt for falling edge of GPIO PC13
    // 1. Configure mask bit
    EXTI->IMR |= (1 << BUTTON_PIN);
    
    // 2. Disable rising edge trigger
    EXTI->RTSR &= ~(1 << BUTTON_PIN);

    // 3. Enable falling edge trigger
    EXTI->FTSR |= (1 << BUTTON_PIN);
    
    // 4. Turn on EXTI interrupt in NVIC_ISER1
    *NVIC_ISER1 |= (1 << 8);



    // Enable interrupts for TIMx
    // DELAY_TIM->DIER |= TIM_DIER_UIE;
    // NVIC_EnableIRQ(TIM2_IRQn); // IRQn 28

}

int main(void) {
    setup();    
    while(1){
        delay_millis(TIM2, 200);
        //__WFI();
    }
}

// void TIM2_IRQHandler(){
//     volatile int pin_val = (GPIOA->IDR >> LED_PIN) & 0x1;
//     if(pin_val) GPIOA->BSRR = (1 << (LED_PIN + 16));
//     else GPIOA->BSRR = (1 << LED_PIN);
//     DELAY_TIM->SR &= ~TIM_SR_UIF;
// }

void EXTI15_10_IRQHandler(void){
        // Check that the button EXTI_13 was what triggered our interrupt
    if (EXTI->PR & (1 << BUTTON_PIN)){
        // If so, clear the interrupt
        EXTI->PR |= (1 << BUTTON_PIN);

        // Then toggle the LED
        togglePin(GPIOA, LED_PIN);

    }
}