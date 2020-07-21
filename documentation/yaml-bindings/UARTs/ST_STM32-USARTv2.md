ST,STM32-USARTv2
================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-USART-v2-group`;
- `has-CR-M1-bit`, required, boolean, selects whether `M1` bit is implemented (`true`) or not (`false`) in U(S)ART's
`CR` register;
- `UARTx` or `USARTx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-USART-v2`;
  - `interrupt`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vector`, required, string, name of interrupt (sub)vector used by this peripheral;
  - `CTS`, `RTS`, `RX`, `TX`, optional, mapping, all key-value pairs are specific to particular GPIO implementation;
  selects configuration of pins;

Examples
--------

```
UARTs:
  compatible:
  - ST,STM32-USARTs-v2-group
  has-CR-M1-bit: true
  USART1:
    compatible:
    - ST,STM32-USART-v2
    interrupt:
      controller: !Reference
        label: NVIC
      vector: USART1
    $labels:
    - USART1
  USART2:
    compatible:
    - ST,STM32-USART-v2
    interrupt:
      controller: !Reference
        label: NVIC
      vector: USART2
    $labels:
    - USART2
  USART3:
    compatible:
    - ST,STM32-USART-v2
    interrupt:
      controller: !Reference
        label: NVIC
      vector: USART3
    $labels:
    - USART3
!Reference {label: USART3}:
  CTS:
    port: !Reference {label: GPIOD}
    pin: 11
    mode: alternate-function
    alternate-function: 7
    output-speed: very-high
    pull: up
  RTS:
    port: !Reference {label: GPIOD}
    pin: 12
    mode: alternate-function
    alternate-function: 7
    output-speed: very-high
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
