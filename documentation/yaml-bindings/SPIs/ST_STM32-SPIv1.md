ST,STM32-SPIv1
==============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-SPIs-v1-group`;
- `SPIx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-SPI-v1`;
  - `interrupt`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vector`, required, string, name of interrupt (sub)vector used by this peripheral;

Examples
--------

```
SPIs:
  compatible:
  - ST,STM32-SPIs-v1-group
  SPI1:
    compatible:
    - ST,STM32-SPI-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: SPI1
    $labels:
    - SPI1
  SPI2:
    compatible:
    - ST,STM32-SPI-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: SPI2
    $labels:
    - SPI2
```
