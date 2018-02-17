ST,STM32-USARTv2
================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-USART-v2-group`;
- `has-CR-M1-bit`, required, boolean, selects whether `M1` bit is implemented (`true`) or not (`false`) in U(S)ART's
`CR` register;
- `UARTx` or `USARTx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-USART-v2`;

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
    $labels:
    - USART1
  USART2:
    compatible:
    - ST,STM32-USART-v2
    $labels:
    - USART2
  USART3:
    compatible:
    - ST,STM32-USART-v2
    $labels:
    - USART3
```
