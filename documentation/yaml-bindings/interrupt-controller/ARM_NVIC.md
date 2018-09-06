ARM,NVIC
========

- `compatible`, required, list of strings:
  - [0], required, string, `ARM,NVIC`;
  - [1], required, string, `interrupt-controller`;
- `core-vectors`, required, mapping;
- `chip-vectors`, required, mapping;

In `core-vectors` and `chip-vectors` mappings each key shall be an integer - exception number - and each value shall be
a string - vector name.

Examples
--------

```
NVIC:
  compatible:
  - ARM,NVIC
  - interrupt-controller
  core-vectors:
    1: Reset
    2: NMI
    3: HardFault
    11: SVC
    14: PendSV
    15: SysTick
  chip-vectors:
    16: WWDG
    18: RTC
    19: FLASH
    20: RCC
    21: EXTI0_1
    22: EXTI2_3
    23: EXTI4_15
    25: DMA_Ch1
    26: DMA_Ch2_3
    27: DMA_Ch4_5
    28: ADC
    29: TIM1_BRK_UP_TRG_COM
    30: TIM1_CC
    32: TIM3
    33: TIM6
    35: TIM14
    36: TIM15
    37: TIM16
    38: TIM17
    39: I2C1
    40: I2C2
    41: SPI1
    42: SPI2
    43: USART1
    44: USART2
    45: USART3_6
    47: USB
```

```
NVIC:
  compatible:
  - ARM,NVIC
  - interrupt-controller
  core-vectors:
    1: Reset
    2: NMI
    3: HardFault
    4: MemManage
    5: BusFault
    6: UsageFault
    11: SVC
    12: DebugMon
    14: PendSV
    15: SysTick
  chip-vectors:
    16: WWDG
    17: PVD
    18: TAMP_STAMP
    19: RTC_WKUP
    20: FLASH
    21: RCC
    22: EXTI0
    23: EXTI1
    24: EXTI2
    25: EXTI3
    26: EXTI4
    27: DMA1_Channel1
    28: DMA1_Channel2
    29: DMA1_Channel3
    30: DMA1_Channel4
    31: DMA1_Channel5
    32: DMA1_Channel6
    33: DMA1_Channel7
    34: ADC1
    39: EXTI9_5
    40: TIM1_BRK_TIM15
    41: TIM1_UP_TIM16
    42: TIM1_TRG_COM_TIM17
    43: TIM1_CC
    44: TIM2
    45: TIM3
    46: TIM4
    47: I2C1_EV
    48: I2C1_ER
    49: I2C2_EV
    50: I2C2_ER
    51: SPI1
    52: SPI2
    53: USART1
    54: USART2
    55: USART3
    56: EXTI15_10
    57: RTC_Alarm
    58: CEC
    59: TIM12
    60: TIM13
    61: TIM14
    64: FSMC
    66: TIM5
    67: SPI3
    68: UART4
    69: UART5
    70: TIM6_DAC
    71: TIM7
    72: DMA2_Channel1
    73: DMA2_Channel2
    74: DMA2_Channel3
    75: DMA2_Channel4_5
    76: DMA2_Channel5
```
