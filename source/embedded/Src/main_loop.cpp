//
// Created by Nelieru on 06/06/2019.
//

#include <USB/CDC.h>
#include <cstring>

#include "stm32f0xx_hal.h"

#include "main_loop.h"

uint8_t pin1 = 0;
uint8_t pin2 = 0;
uint8_t pin3 = 0;
uint8_t pin4 = 0;

void main_loop()
{
    usb::CDC cdc;

    cdc.initialize();

    while (true) {
        for (int i = 0; i < 16; i++) {
            pin1 = i % 2;
            pin1 = i / 2 % 2;
            pin1 = i / 4 % 2;
            pin1 = i / 8 % 2;

            // IN1:   Low
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
            // IN2:   High
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET);
            // STBY:  High
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

            // PWM:   High
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

            HAL_Delay(100);
        }
    }
}