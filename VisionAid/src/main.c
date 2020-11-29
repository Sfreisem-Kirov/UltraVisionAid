// main.c
// Sam Freisem-Kirov  Anuragini Arora
// sfreisemkirov@hmc.edu
// 11/5/20

#include "main.h"
#include "tone.h"

void setup() {

    configureFlash();
    configureClock();

    initUSART(USART_ID);
    
    // Enable LED as output
    RCC->AHB1ENR.GPIOAEN = 1;
    pinMode(GPIOA, LED_PIN, GPIO_OUTPUT);

    // Enable button as input
    RCC->AHB1ENR.GPIOCEN = 1;
    pinMode(GPIOC, BUTTON_PIN, GPIO_INPUT);

    // Initialize timer
    RCC->APB1ENR |= (1 << 0); // TIM2EN
    initTIM(DELAY_TIM);

    // Initialize ultrasonic distance sensor
    initSensor(INPIN,OUTPIN); //8,9
    
  
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


}

int main(void) {
    setup();
    tone(500,1000);
    while(1){
        delay_millis(DELAY_TIM, 200);

        int count = 0;
        int prevDists[7] = {0,0,0,0,0,0,0};
        int sum = 0;
        int i = 0;

        while(!digitalRead(GPIOC, BUTTON_PIN)){
            int dist = getDistance(INPIN, OUTPIN);
            sum -= prevDists[i];
            prevDists[i] = dist;
            i = (i+1) % 7;
            sum+= dist;
            if (count < 7) count++;
            int avgDist = sum/count;
            int freq = (880.0-(((660.0/18000.0)*avgDist)));
            
            tone(freq,60);
        }

        tone(0,60);
        // while button is being pressed
            // check distance
            // emit tone on that distance for a quarter of a second
            // when the button is not pressed, stop the tone

        __WFI();
    }
}


void EXTI15_10_IRQHandler(void){
        // Check that the button EXTI_13 was what triggered our interrupt
    if (EXTI->PR & (1 << BUTTON_PIN)){
        // If so, clear the interrupt
        EXTI->PR |= (1 << BUTTON_PIN);

        
        int val = getDistance(INPIN, OUTPIN);
        int dist = getDistance(INPIN, OUTPIN);
        int freq = (880.0-(((660.0/18000.0)*dist)));
        tone(freq,60);
    }
}

// Use For Testing
void printDist(int val) {
    uint8_t msg[64];

    sprintf(msg, " %d \n\r",val);

    uint8_t i = 0;
    do
    {
        sendChar(USART_ID, msg[i]);
        i += 1;
    } while (msg[i]);
}