#ifndef __TB6612FNG_H__
#define __TB6612FNG_H__

#include "stm32f1xx.h"
#include <stdint.h>

class TB6612FNG {
private:
    /* data */
public:
    /**
     *  @brief Initialises the pins needed for the motor controller.
     *  
     *  @param IN1Port: Port of the direction pin 1
     *  @param IN1Pin: Pin of the direction pin 1
     * 
     *  @param IN2Port: Port of the direction pin 2
     *  @param IN2Pin: Pin of the direction pin 2
     * 
     *  @param SPEEDPort: Port of the speed pin
     *  @param SPEEDPin: Pin of the speed pin
     * 
     *  @param STBYPort: Port of the standby pin
     *  @param SYBYPin: Pin of the standby pin
     *  @retval None
     */
    TB6612FNG(
        GPIO_TypeDef* IN1Port, uint16_t IN1Pin,
        GPIO_TypeDef* IN2Port, uint16_t IN2Pin,
        GPIO_TypeDef* SPEEDPort, uint16_t SPEEDPin,
        GPIO_TypeDef* STBYPort, uint16_t SYBYPin);

    void drive(uint16_t speed);
    void drive(uint16_t speed, uint16_t duration);

    void brake();

    void standby();

private:
    void forward(uint16_t speed);
    void reverse(uint16_t speed);

    void initialisePWM(GPIO_TypeDef* SPEEDPort, uint16_t SPEEDPin);
};

#endif // __TB6612FNG_H__
