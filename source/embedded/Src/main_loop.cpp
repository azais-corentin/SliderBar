//
// Created by Nelieru on 06/06/2019.
//

#include <USB/CDC.h>
#include <cstring>

#include "stm32f0xx_hal.h"

#include "main_loop.h"

void main_loop()
{
    usb::CDC cdc;

    cdc.initialize();

    // IN1:   Low
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);
    // IN2:   High
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET);
    // STBY:  High
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, GPIO_PIN_SET);

    // PWM:   High
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

    while (true) {
    }
}