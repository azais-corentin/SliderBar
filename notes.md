# Notes

These notes cover both the software and hardware development.

## Oscilloscope

Hantek 6022BL with OpenHantek

## Test points

* Round holes for pins
* Small loops to put a probe

## LEDs

* For GPIO LEDs, connect the LED to 3V3, use GPIO to pull down.
* Power LED on the 3v3.
* Add a LED to display errors! Use it with the `Error_Handler`:

```c
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}
```

* Add at least 2 other LEDs to display various informations (low power LEDs).

## USB

* The STM32 integrates pull up resistor for USB_DP line. It can be toggled with `HAL_PCD_DevDisconnect` and `HAL_PCD_DevConnect`. See at page 882 of reference.
* ESD protection should be as close to the MCU as possible.
* Integrate overcurrent protection using a `STMPS2151STR`.
* Use USB type C receptacle? See `usb2_0_to_usb_c_migration.pdf` page 16.
* Eventually use DFU to program the device (DFU in bootloader) while still using VCP to communicate (USB CDC in flash). See [STM32 USB VCP and DFU](https://community.st.com/s/question/0D50X00009XkfzkSAB/stm32f4-usb-vcp-cdc-and-dfu).

## DFU Principle

The Internal flash memory memory is split as follows:

* DFU area located in [0x08000000 : USBD_DFU_APP_DEFAULT_ADD-1]: Only read access
* Application area located in [USBD_DFU_APP_DEFAULT_ADD : Device's end address : Read, Write, and Erase access

Configure a GPIO input. If pressed at boot, go to DFU mode, otherwise normal application. Check the example: `Applications\USB_Device\DFU_Standalone`. There is an error in the example, line 88 of `main.c` should be 'If the button is not pressed'.

## Timers

* To get a frequency for the pwm, compute the prescaler from the SystemCoreClock if APB1 clock is SystemCoreClock. Example for 10000 Hz:

```c++
uint8_t period = 255;
uint16_t prescaler = (uint16_t)(SystemCoreClock / ((period + 1) * 10000)) - 1;
```

* Add test pin(s) to the PWM output

