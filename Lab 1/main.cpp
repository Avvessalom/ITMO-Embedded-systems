#include "hal.h"

int num1 = 3;
int num2 = 8;
bool button = false;
int delay = 500;
bool sw[4] = {false, false, false, false};

unsigned int leds_num[8] = {GPIO_PIN_3, GPIO_PIN_4, GPIO_PIN_5,
                           GPIO_PIN_6, GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12};
unsigned int leds_num2[8] = {GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6,
                           GPIO_PIN_8, GPIO_PIN_9, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_12};
unsigned int leds_num3[8] = {GPIO_PIN_12, GPIO_PIN_11, GPIO_PIN_9,
                           GPIO_PIN_8, GPIO_PIN_6, GPIO_PIN_5, GPIO_PIN_4, GPIO_PIN_3};
unsigned int sw_num[4] = {GPIO_PIN_4, GPIO_PIN_8, GPIO_PIN_10, GPIO_PIN_12};

void change_signal(int i){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
    switch(i){
        case 0:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
            break;
    }
}

void set_yellow(){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
}

bool check_button(){
    GPIO_PinState state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_15);
    if(state == GPIO_PIN_RESET){
        return true;
    } else {
        return false;
    }
}

void lamp(){
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_8, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_9, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
    while(true){
        change_signal(2);
        button = check_button();
    }
}

void anime(int i){
    HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, leds_num2[i], GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOD, leds_num3[i], GPIO_PIN_SET);
    HAL_Delay(delay);
    HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, leds_num2[i], GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOD, leds_num3[i], GPIO_PIN_RESET);
}

int umain(){
    int i = 0;
    
    while(1){
        for(int j = 0; j<4; j++){
            sw[j] = GPIO_PIN_SET==HAL_GPIO_ReadPin(GPIOE, sw_num[j]);
        }
        if(!sw[0] && sw[1] && !sw[2] && !sw[3]){
                change_signal(0);
                anime(i);
                i++;
                if (i == 8) {
                    i = 0;
                }
                if (check_button()) {
                    change_signal(1);
                    HAL_Delay(300);
                    while(1){
                        if (check_button()){
                            break;
                        }
                    }
                }
        } else {
            set_yellow();
            for (int i = 0;i<4;i++){
                if(sw[i]){
                    HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_SET);
                } else {
                    HAL_GPIO_WritePin(GPIOD, leds_num[i], GPIO_PIN_RESET);
                }
            }
        }
    }
    
    return 0;
}
