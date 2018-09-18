ARM,NVIC
========

- `compatible`, required, list of strings:
  - [0], required, string, `ARM,NVIC`;
  - [1], required, string, `interrupt-controller`;
- `core-vectors`, required, list;
- `chip-vectors`, required, list;

`core-vectors` and `chip-vectors` items
---------------------------------------

Each item corrensponding to an implemented interrupt shall be a mapping:
- `name`, required, string, vector name;
- `subvectors`, optional, list of strings, names of subvectors;

Each item corrensponding to an unimplemented interrupt shall be empty.

Examples
--------

```
NVIC:
  compatible:
  - ARM,NVIC
  - interrupt-controller
  core-vectors:
  - name: Reset
  - name: NMI
  - name: HardFault
  -
  -
  -
  -
  -
  -
  -
  - name: SVC
  -
  -
  - name: PendSV
  - name: SysTick
  chip-vectors:
  - name: WWDG
  -
  - name: RTC
  - name: FLASH
  - name: RCC
  - name: EXTI0_1
  - name: EXTI2_3
  - name: EXTI4_15
  -
  - name: DMA1_Ch1
  - name: DMA1_Ch2_3
  - name: DMA1_Ch4_5
  - name: ADC1
  - name: TIM1_BRK_UP_TRG_COM
  - name: TIM1_CC
  -
  - name: TIM3
  - name: TIM6
  -
  - name: TIM14
  - name: TIM15
  - name: TIM16
  - name: TIM17
  - name: I2C1
  - name: I2C2
  - name: SPI1
  - name: SPI2
  - name: USART1
  - name: USART2
  - name: USART3_6
    subvectors:
    - USART3
    - USART4
    - USART5
    - USART6
  -
  - name: USB
```

```
NVIC:
  compatible:
  - ARM,NVIC
  - interrupt-controller
  core-vectors:
  - name: Reset
  - name: NMI
  - name: HardFault
  - name: MemManage
  - name: BusFault
  - name: UsageFault
  -
  -
  -
  -
  - name: SVC
  - name: DebugMon
  -
  - name: PendSV
  - name: SysTick
  chip-vectors:
  - name: WWDG
  - name: PVD
  - name: TAMPER
  - name: RTC
  - name: FLASH
  - name: RCC
  - name: EXTI0
  - name: EXTI1
  - name: EXTI2
  - name: EXTI3
  - name: EXTI4
  - name: DMA1_Channel1
  - name: DMA1_Channel2
  - name: DMA1_Channel3
  - name: DMA1_Channel4
  - name: DMA1_Channel5
  - name: DMA1_Channel6
  - name: DMA1_Channel7
  - name: ADC1
  -
  -
  -
  -
  - name: EXTI9_5
  - name: TIM1_BRK_TIM15
  - name: TIM1_UP_TIM16
  - name: TIM1_TRG_COM_TIM17
  - name: TIM1_CC
  - name: TIM2
  - name: TIM3
  - name: TIM4
  - name: I2C1_EV
  - name: I2C1_ER
  - name: I2C2_EV
  - name: I2C2_ER
  - name: SPI1
  - name: SPI2
  - name: USART1
  - name: USART2
  - name: USART3
  - name: EXTI15_10
  - name: RTC_Alarm
  - name: CEC
  - name: TIM12
  - name: TIM13
  - name: TIM14
  -
  -
  - name: FSMC
  -
  - name: TIM5
  - name: SPI3
  - name: UART4
  - name: UART5
  - name: TIM6_DAC
  - name: TIM7
  - name: DMA2_Channel1
  - name: DMA2_Channel2
  - name: DMA2_Channel3
  - name: DMA2_Channel4_5
  - name: DMA2_Channel5
```
