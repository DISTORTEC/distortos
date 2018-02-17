ST,STM32-USARTv1
================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-USART-v1-group`;
- `has-CR1-OVER8-bit`, required, boolean, selects whether `OVER8` bit is implemented (`true`) or not (`false`) in
U(S)ART's `CR1` register;
- `UARTx` or `USARTx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-USART-v1`;

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
    $labels:
    - USART1
  USART2:
    compatible:
    - ST,STM32-USART-v1
    $labels:
    - USART2
  USART3:
    compatible:
    - ST,STM32-USART-v1
    $labels:
    - USART3
  UART4:
    compatible:
    - ST,STM32-USART-v1
    $labels:
    - UART4
  UART5:
    compatible:
    - ST,STM32-USART-v1
    $labels:
    - UART5
```
