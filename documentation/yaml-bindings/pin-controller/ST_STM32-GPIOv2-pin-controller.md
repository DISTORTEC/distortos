ST,STM32-GPIOv2-pin-controller
==============================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-GPIO-v2-pin-controller`;
- `AF-bits`, required, integer, number of implemented AF bits for single pin in GPIO alternate function registers
(`GPIOx_AFRL` and `GPIOx_AFRH`);
- `has-high-speed`, required, boolean, selects whether "high speed" option is implemented (`true`) or not (`false`) in
GPIO port output speed register (`GPIOx_OSPEEDR`);

Examples
--------

```
pin-controller:
  compatible:
  - ST,STM32-GPIO-v2-pin-controller
  AF-bits: 3
  has-high-speed: false
```
