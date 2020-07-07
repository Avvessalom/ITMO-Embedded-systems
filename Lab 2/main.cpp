#include "hal.h"
#include <bitset>

int i = 0;
int leds_num[] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
int steps[10][8] = {
    {1,1,1,0,0,0,0,0},
    {0,1,1,1,0,0,0,0},
    {0,0,1,1,1,0,0,0},
    {0,0,0,1,1,1,0,0},
    {0,0,0,0,1,1,1,0},
    {0,0,0,0,0,1,1,1},
    {0,0,0,0,1,1,1,0},
    {0,0,0,1,1,1,0,0},
    {0,0,1,1,1,0,0,0},
    {0,1,1,1,0,0,0,0}
};

bitset<4> sw;

unsigned int sw_num[4] = {GPIO_PIN_12, GPIO_PIN_10, GPIO_PIN_8, GPIO_PIN_4};

void read_and_set(){
    for(int i = 0; i<4; i++) {
        GPIO_PinState state = HAL_GPIO_ReadPin(GPIOE, sw_num[i]);
        sw[i] = state == GPIO_PIN_SET;
    }
}

void set_pins(int num[8]){
    for (int i = 0; i < 10;i++){
        if (num[i] == 1){
            HAL_GPIO_WritePin(GPIOD,leds_num[i],GPIO_PIN_SET);
        }
    }
}

void unset_pins(int num[8]){
    for (int i = 0; i < 9;i++){
        if (num[i] == 1){
            HAL_GPIO_WritePin(GPIOD,leds_num[i],GPIO_PIN_RESET);
        }
    }
}

void TIM6_IRQ_Handler(){
    set_pins(steps[i]);
}

void TIM7_IRQ_Handler(){
    unset_pins(steps[i]);
    i++;
    if(i == 11) i=0;
    read_and_set();
    WRITE_REG(TIM6_ARR, 500 + sw.to_ulong()*200);
    WRITE_REG(TIM7_ARR, 500 + sw.to_ulong()*200);
}

int umain(){
    i = 0;
    
    registerTIM6_IRQHandler(TIM6_IRQ_Handler);
    registerTIM7_IRQHandler(TIM7_IRQ_Handler);
    
    __enable_irq();
    
    WRITE_REG(TIM6_ARR, 500);
    WRITE_REG(TIM6_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM6_PSC,0);
    
    WRITE_REG(TIM7_ARR, 500);
    WRITE_REG(TIM7_DIER, TIM_DIER_UIE);
    WRITE_REG(TIM7_PSC,1);
    
    WRITE_REG(TIM6_CR1, TIM_CR1_CEN);
    WRITE_REG(TIM7_CR1, TIM_CR1_CEN);
    
    return 0;
}
