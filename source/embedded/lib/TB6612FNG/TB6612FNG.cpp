#include "TB6612FNG.h"

#include "stm32f1xx_hal.h"

TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

TB6612FNG::TB6612FNG(
    GPIO_TypeDef* _IN1Port, uint16_t _IN1Pin,
    GPIO_TypeDef* _IN2Port, uint16_t _IN2Pin,
    GPIO_TypeDef* _SPEEDPort, uint16_t _SPEEDPin,
    GPIO_TypeDef* _STBYPort, uint16_t _STBYPin)
{
    IN1Port = _IN1Port;
    IN2Port = _IN2Port;
    SPEEDPort = _SPEEDPort;
    STBYPort = _STBYPort;

    IN1Pin = _IN1Pin;
    IN2Pin = _IN2Pin;
    SPEEDPin = _SPEEDPin;
    STBYPin = _STBYPin;

    // Enables necessary ports clock
    if (IN1Port == GPIOA || IN2Port == GPIOA || SPEEDPort == GPIOA || STBYPort == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    if (IN1Port == GPIOB || IN2Port == GPIOB || SPEEDPort == GPIOB || STBYPort == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    if (IN1Port == GPIOC || IN2Port == GPIOC || SPEEDPort == GPIOC || STBYPort == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();
    if (IN1Port == GPIOD || IN2Port == GPIOD || SPEEDPort == GPIOD || STBYPort == GPIOD)
        __HAL_RCC_GPIOD_CLK_ENABLE();
    if (IN1Port == GPIOE || IN2Port == GPIOE || SPEEDPort == GPIOE || STBYPort == GPIOE)
        __HAL_RCC_GPIOE_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct;

    // Initialises IN1
    GPIO_InitStruct.Pin = IN1Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // No pullup/down resistors
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW; // Up to 2 Mhz
    HAL_GPIO_Init(IN1Port, &GPIO_InitStruct);

    // Initialises IN2
    GPIO_InitStruct.Pin = IN2Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(IN2Port, &GPIO_InitStruct);

    // Initialises STBY
    GPIO_InitStruct.Pin = STBYPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(STBYPort, &GPIO_InitStruct);

    initialisePWM();
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
}

void TB6612FNG::initialisePWM()
{
    // Initialises Timer3, at 7500 Hz
    TIM_OC_InitTypeDef sConfigOC;

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 49;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 127; // Duty cycle: [0 - 127]
    //htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 49;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

    GPIO_InitTypeDef GPIO_InitStruct;
    __TIM3_CLK_ENABLE();

    /**TIM3 GPIO Configuration
     PA6     ------> TIM3_CH1
     */
    GPIO_InitStruct.Pin = SPEEDPin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SPEEDPort, &GPIO_InitStruct);
}

void TB6612FNG::forward(uint16_t speed)
{
    if (cur_standby)
        standby(false);

    HAL_GPIO_WritePin(IN1Port, IN1Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN2Port, IN2Pin, GPIO_PIN_RESET);
}

void TB6612FNG::reverse(uint16_t speed)
{
    if (cur_standby)
        standby(false);

    HAL_GPIO_WritePin(IN1Port, IN1Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IN2Port, IN2Pin, GPIO_PIN_SET);
}

void TB6612FNG::brake()
{
    HAL_GPIO_WritePin(IN1Port, IN1Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(IN2Port, IN2Pin, GPIO_PIN_SET);
}

void TB6612FNG::standby(bool enable)
{
    cur_standby = enable;
    if (enable)
        HAL_GPIO_WritePin(STBYPort, STBYPin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(STBYPort, STBYPin, GPIO_PIN_SET);
}

void TB6612FNG::drive(int8_t speed)