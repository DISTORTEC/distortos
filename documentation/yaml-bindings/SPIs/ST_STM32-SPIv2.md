ST,STM32-SPIv2
==============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-SPIs-v2-group`;
- `SPIx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-SPI-v2`;

Examples
--------

```
SPIs:
  compatible:
  - ST,STM32-SPIs-v2-group
  SPI1:
    compatible:
    - ST,STM32-SPI-v2
    $labels:
    - SPI1
  SPI2:
    compatible:
    - ST,STM32-SPI-v2
    $labels:
    - SPI2
  SPI3:
    compatible:
    - ST,STM32-SPI-v2
    $labels:
    - SPI3
```
