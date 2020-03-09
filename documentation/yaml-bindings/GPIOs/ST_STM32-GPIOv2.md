ST,STM32-GPIOv2
===============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-GPIOs-v2-group`;
- `GPIOx` (`x` - upper case character), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-GPIO-v2`;

Examples
--------

```
GPIOs:
  compatible:
  - ST,STM32-GPIOs-v2-group
  GPIOA:
    compatible:
    - ST,STM32-GPIO-v2
    $labels:
    - GPIOA
  GPIOB:
    compatible:
    - ST,STM32-GPIO-v2
    $labels:
    - GPIOB
  GPIOC:
    compatible:
    - ST,STM32-GPIO-v2
    $labels:
    - GPIOC
```

GPIO mapping for ST,STM32-GPIOv2
================================

- `port`, required, reference, reference to the GPIO port;
- `pin`, required, integer, number of the pin;
- `mode`, optional, string, mode of the pin, one of: `alternate-function`, `analog`, `input`, `output`; required if
parent mapping does not expect any specific mode of the pin;

GPIO mapping for ST,STM32-GPIOv2, alternate function mode
---------------------------------------------------------

- `alternate-function`, required, integer, selects alternate function of the pin;
- `open-drain`, optional, boolean, selects "push-pull" (`false`) or "open drain" (`true`) mode of the pin; default -
`false`;
- `output-speed`, required, string, output speed of the pin, one of: `low`, `medium`, `high`, `very-high`;
- `pull`, optional, string, pull-up/-down configuration of the pin, one of: `none`, `up`, `down`; default - `none`;

Examples
--------

```
!Reference {label: SDMMC1}:
  CK:
    port: !Reference {label: GPIOC}
    pin: 12
    mode: alternate-function
    alternate-function: 12
    output-speed: very-high
  CMD:
    port: !Reference {label: GPIOD}
    pin: 2
    mode: alternate-function
    alternate-function: 12
    output-speed: very-high
  D0:
    port: !Reference {label: GPIOC}
    pin: 8
    mode: alternate-function
    alternate-function: 12
    output-speed: very-high
  D1:
    port: !Reference {label: GPIOC}
    pin: 9
    mode: alternate-function
    alternate-function: 12
    output-speed: very-high
  D2:
    port: !Reference {label: GPIOC}
    pin: 10
    mode: alternate-function
    alternate-function: 12
    output-speed: very-high
  D3:
    port: !Reference {label: GPIOC}
    pin: 11
    mode: alternate-function
    alternate-function: 12
    output-speed: very-high
```

GPIO mapping for ST,STM32-GPIOv2, input and output modes (legacy)
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
