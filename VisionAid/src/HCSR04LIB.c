// HCSR04LIB.c
// Sam Freisem-Kirov  Anuragini Arora
// sfreisemkirov@hmc.edu
// 11/7/20

#include "HCSR04LIB.h"

void initSensor(int inPin, int outPin) {
    // initialize the GPIO pins that we are using for the ultrasonic distance sensor
    pinMode(GPIOA,inPin,GPIO_INPUT);
    pinMode(GPIOA, outPin, GPIO_OUTPUT);
    digitalWrite(GPIOA, outPin, GPIO_LOW);
}

int getDistance(int inPin, int outPin) {
    // send a trigger pulse
    digitalWrite(GPIOA, outPin, GPIO_HIGH);
    delay_micros(DELAY_TIM, 10);
    digitalWrite(GPIOA, outPin, GPIO_LOW);

    // read the output by setting a timer to count until the echo pulse is low
    while(digitalRead(GPIOA, inPin) == GPIO_LOW);
    start_count(TIM2);
    
    while(digitalRead(GPIOA, inPin) == GPIO_HIGH);
    int count = get_count_micros(TIM2);

    return count;

}