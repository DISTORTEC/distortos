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

GPIO mapping for ST,STM32-GPIOv1, input and output modes (legacy)
=================================================================

- `port`, required, reference, reference to GPIO port to which this input/output is connected;
- `pin`, required, integer, number of pin to which this input/output is connected;
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
