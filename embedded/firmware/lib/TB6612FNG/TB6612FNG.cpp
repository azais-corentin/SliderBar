#include "TB6612FNG.h"

#include "stm32f1xx_hal.h"

TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim3_ch1_trig;

TB6612FNG::TB6612FNG(
    GPIO_TypeDef* IN1Port, uint16_t IN1Pin,
    GPIO_TypeDef* IN2Port, uint16_t IN2Pin,
    GPIO_TypeDef* SPEEDPort, uint16_t SPEEDPin,
    GPIO_TypeDef* STBYPort, uint16_t SYBYPin)
{
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
    GPIO_InitStruct.Pin = SYBYPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(STBYPort, &GPIO_InitStruct);
}

void TB6612FNG::initialisePWM(GPIO_TypeDef* SPEEDPort, uint16_t SPEEDPin)
{
    // Initialises Timer3, at 20 Khz
    TIM_OC_InitTypeDef sConfigOC;

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 11;
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 199;
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0;
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);

    hdma_tim3_ch1_trig.Instance = DMA1_Channel6;
    hdma_tim3_ch1_trig.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim3_ch1_trig.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim3_ch1_trig.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim3_ch1_trig.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim3_ch1_trig.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_tim3_ch1_trig.Init.Mode = DMA_CIRCULAR;
    hdma_tim3_ch1_trig.Init.Priority = DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_tim3_ch1_trig);

    __HAL_LINKDMA(&htim3, hdma[TIM_DMA_ID_CC1], hdma_tim3_ch1_trig);
    __HAL_LINKDMA(&htim3, hdma[TIM_DMA_ID_TRIGGER], hdma_tim3_ch1_trig);

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
