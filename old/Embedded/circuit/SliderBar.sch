EESchema Schematic File Version 2
LIBS:SliderBar
LIBS:ac-dc
LIBS:adc-dac
LIBS:Altera
LIBS:analog_devices
LIBS:analog_switches
LIBS:atmel
LIBS:audio
LIBS:Battery_Management
LIBS:bbd
LIBS:Bosch
LIBS:brooktre
LIBS:Connector
LIBS:contrib
LIBS:cypress
LIBS:dc-dc
LIBS:Decawave
LIBS:device
LIBS:digital-audio
LIBS:Diode
LIBS:Display
LIBS:driver_gate
LIBS:dsp
LIBS:DSP_Microchip_DSPIC33
LIBS:elec-unifil
LIBS:ESD_Protection
LIBS:Espressif
LIBS:FPGA_Actel
LIBS:ftdi
LIBS:gennum
LIBS:Graphic
LIBS:hc11
LIBS:infineon
LIBS:intel
LIBS:interface
LIBS:intersil
LIBS:ir
LIBS:Lattice
LIBS:LED
LIBS:LEM
LIBS:linear
LIBS:Logic_74xgxx
LIBS:Logic_74xx
LIBS:Logic_CMOS_4000
LIBS:Logic_CMOS_IEEE
LIBS:logic_programmable
LIBS:Logic_TTL_IEEE
LIBS:maxim
LIBS:MCU_Microchip_PIC10
LIBS:MCU_Microchip_PIC12
LIBS:MCU_Microchip_PIC16
LIBS:MCU_Microchip_PIC18
LIBS:MCU_Microchip_PIC24
LIBS:MCU_Microchip_PIC32
LIBS:MCU_NXP_Kinetis
LIBS:MCU_NXP_LPC
LIBS:MCU_NXP_S08
LIBS:MCU_Parallax
LIBS:MCU_ST_STM8
LIBS:MCU_ST_STM32
LIBS:MCU_Texas_MSP430
LIBS:Mechanical
LIBS:memory
LIBS:microchip
LIBS:microcontrollers
LIBS:modules
LIBS:Motor
LIBS:motor_drivers
LIBS:motorola
LIBS:nordicsemi
LIBS:nxp
LIBS:onsemi
LIBS:opto
LIBS:Oscillators
LIBS:philips
LIBS:power
LIBS:Power_Management
LIBS:powerint
LIBS:pspice
LIBS:references
LIBS:regul
LIBS:Relay
LIBS:RF_Bluetooth
LIBS:rfcom
LIBS:RFSolutions
LIBS:Sensor_Current
LIBS:Sensor_Humidity
LIBS:sensors
LIBS:silabs
LIBS:siliconi
LIBS:supertex
LIBS:Switch
LIBS:texas
LIBS:Transformer
LIBS:Transistor
LIBS:triac_thyristor
LIBS:Valve
LIBS:video
LIBS:wiznet
LIBS:Worldsemi
LIBS:Xicor
LIBS:xilinx
LIBS:zetex
LIBS:Zilog
LIBS:graphic_symbols
LIBS:nelieru_customs
LIBS:74xgxx
LIBS:74xx
LIBS:actel
LIBS:allegro
LIBS:cmos_ieee
LIBS:cmos4000
LIBS:conn
LIBS:leds
LIBS:logo
LIBS:microchip_dspic33dsc
LIBS:microchip_pic10mcu
LIBS:microchip_pic12mcu
LIBS:microchip_pic16mcu
LIBS:microchip_pic18mcu
LIBS:microchip_pic24mcu
LIBS:microchip_pic32mcu
LIBS:motors
LIBS:msp430
LIBS:nxp_armmcu
LIBS:relays
LIBS:stm8
LIBS:stm32
LIBS:switches
LIBS:transf
LIBS:transistors
LIBS:ttl_ieee
LIBS:valves
LIBS:SliderBar-cache
EELAYER 25 0
EELAYER END
$Descr A3 16535 11693
encoding utf-8
Sheet 1 1
Title "SliderBar"
Date "2018-01-18"
Rev "1.0"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L STM32F030K6Tx U?
U 1 1 5A607F29
P 8150 5525
F 0 "U?" H 6150 6650 50  0000 L BNN
F 1 "STM32F030K6Tx" H 10150 6650 50  0000 R BNN
F 2 "LQFP32" H 10150 6600 50  0001 R TNN
F 3 "" H 8150 5525 50  0001 C CNN
	1    8150 5525
	1    0    0    -1  
$EndComp
$Comp
L TB6612FNG U?
U 1 1 5A608053
P 12925 5375
F 0 "U?" H 13375 6075 50  0000 L CNN
F 1 "TB6612FNG" H 13375 5975 50  0000 L CNN
F 2 "Housings_SSOP:SSOP-24_5.3x8.2mm_Pitch0.65mm" H 13375 5975 50  0001 C CNN
F 3 "" H 13375 5975 50  0001 C CNN
	1    12925 5375
	1    0    0    -1  
$EndComp
$Comp
L HT7333 REG?
U 1 1 5A60819A
P 2225 1650
F 0 "REG?" H 1925 1350 60  0000 C CNN
F 1 "HT7333" H 1975 1950 60  0000 C CNN
F 2 "" H 2225 1650 60  0001 C CNN
F 3 "" H 2225 1650 60  0001 C CNN
	1    2225 1650
	1    0    0    -1  
$EndComp
$Comp
L FT232RL U?
U 1 1 5A60858C
P 10650 2175
F 0 "U?" H 10000 3075 50  0000 L CNN
F 1 "FT232RL" H 11050 3075 50  0000 L CNN
F 2 "Housings_SSOP:SSOP-28_5.3x10.2mm_Pitch0.65mm" H 10650 2175 50  0001 C CNN
F 3 "" H 10650 2175 50  0001 C CNN
	1    10650 2175
	1    0    0    -1  
$EndComp
$EndSCHEMATC
