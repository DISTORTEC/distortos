ST,STM32-USARTv1
================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-USART-v1-group`;
- `has-CR1-OVER8-bit`, required, boolean, selects whether `OVER8` bit is implemented (`true`) or not (`false`) in
U(S)ART's `CR1` register;
- `UARTx` or `USARTx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-USART-v1`;
  - `interrupt`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vector`, required, string, name of interrupt (sub)vector used by this peripheral;
  - `RX`, `TX`, optional, mapping, all key-value pairs are specific to particular GPIO implementation; selects
  configuration of pins;

Examples
--------

```
UARTs:
  compatible:
  - ST,STM32-USARTs-v1-group
  has-CR1-OVER8-bit: false
  USART1:
    compatible:
    - ST,STM32-USART-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: USART1
    $labels:
    - USART1
  USART2:
    compatible:
    - ST,STM32-USART-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: USART2
    $labels:
    - USART2
  USART3:
    compatible:
    - ST,STM32-USART-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: USART3
    $labels:
    - USART3
  UART4:
    compatible:
    - ST,STM32-USART-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: UART4
    $labels:
    - UART4
  UART5:
    compatible:
    - ST,STM32-USART-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: UART5
    $labels:
    - UART5
!Reference {label: USART3}:
  RX:
    port: !Reference {label: GPIOD}
    pin: 9
    mode: alternate-function
    alternate-function: 7
    output-speed: very-high
    pull: up
  TX:
    port: !Reference {label: GPIOD}
    pin: 8
    mode: alternate-function
    alternate-function: 7
    output-speed: very-high
```
