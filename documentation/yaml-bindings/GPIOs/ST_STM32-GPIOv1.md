ST,STM32-GPIOv1
===============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-GPIOs-v1-group`;
- `GPIOx` (`x` - upper case character), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-GPIO-v1`;

Examples
--------

```
GPIOs:
  compatible:
  - ST,STM32-GPIOs-v1-group
  GPIOA:
    compatible:
    - ST,STM32-GPIO-v1
    $labels:
    - GPIOA
  GPIOB:
    compatible:
    - ST,STM32-GPIO-v1
    $labels:
    - GPIOB
  GPIOC:
    compatible:
    - ST,STM32-GPIO-v1
    $labels:
    - GPIOC
  GPIOD:
    compatible:
    - ST,STM32-GPIO-v1
    $labels:
    - GPIOD
```

GPIO mapping for ST,STM32-GPIOv1
================================

- `port`, required, reference, reference to the GPIO port;
- `pin`, required, integer, number of the pin;
- `mode`, optional, string, mode of the pin, one of: `alternate-function`, `analog`, `input`, `output`; required if
parent mapping does not expect any specific mode of the pin;

GPIO mapping for ST,STM32-GPIOv2, alternate function mode
---------------------------------------------------------

- `open-drain`, optional, boolean, selects "push-pull" (`false`) or "open drain" (`true`) mode of the pin; default -
`false`;
- `output-speed`, required, string, output speed of the pin, one of: `2 MHz`, `10 MHz`, `50 MHz`;
- `pull`, optional, string, pull-up/-down configuration of the pin, one of: `none`, `up`, `down`; default - `none`;

GPIO mapping for ST,STM32-GPIOv2, input mode
--------------------------------------------

- `pull`, optional, string, pull-up/-down configuration of the pin, one of: `none`, `up`, `down`; default - `none`;

Examples
--------

```
!Reference {label: USART2}:
  RX:
    port: !Reference {label: GPIOA}
    pin: 3
    mode: input
    pull: up
  TX:
    port: !Reference {label: GPIOA}
    pin: 2
    mode: alternate-function
    output-speed: 50 MHz
```

GPIO mapping for ST,STM32-GPIOv1, input and output modes (legacy)
-----------------------------------------------------------------

- `active-low`, optional, boolean, selects whether this input/output is "active high" (`false`) or "active low"
(`true`);

Examples
--------

```
buttons:
  compatible:
  - GPIO-inputs
  b1:
    GPIO:
      port: !Reference {label: GPIOC}
      pin: 13
      active-low: true
    alternative-names:
    - user
```
