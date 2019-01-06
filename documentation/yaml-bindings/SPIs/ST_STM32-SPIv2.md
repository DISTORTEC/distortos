ST,STM32-SPIv2
==============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-SPIs-v2-group`;
- `SPIx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-SPI-v2`;
  - `interrupt`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vector`, required, string, name of interrupt (sub)vector used by this peripheral;
  - `RX-DMA`, required, list;
  - `TX-DMA`, required, list;

`RX-DMA` and `TX-DMA` items
---------------------------

Each item corrensponding to a valid DMA configuration shall be a mapping:
- `controller`, required, reference, reference to DMA controller;
- `channel`, required, integer, DMA channel identifier;
- `request`, required, integer, DMA request identifier;

Examples
--------

```
SPIs:
  compatible:
  - ST,STM32-SPIs-v2-group
  SPI1:
    compatible:
    - ST,STM32-SPI-v2
    interrupt:
      controller: !Reference
        label: NVIC
      vector: SPI1
    RX-DMA:
    - controller: !Reference
        label: DMA2
      channel: 0
      request: 3
    - controller: !Reference
        label: DMA2
      channel: 2
      request: 3
    TX-DMA:
    - controller: !Reference
        label: DMA2
      channel: 3
      request: 3
    - controller: !Reference
        label: DMA2
      channel: 5
      request: 3
    $labels:
    - SPI1
  SPI2:
    compatible:
    - ST,STM32-SPI-v2
    interrupt:
      controller: !Reference
        label: NVIC
      vector: SPI2
    RX-DMA:
    - controller: !Reference
        label: DMA1
      channel: 3
      request: 0
    TX-DMA:
    - controller: !Reference
        label: DMA1
      channel: 4
      request: 0
    $labels:
    - SPI2
  SPI3:
    compatible:
    - ST,STM32-SPI-v2
    interrupt:
      controller: !Reference
        label: NVIC
      vector: SPI3
    RX-DMA:
    - controller: !Reference
        label: DMA1
      channel: 0
      request: 0
    - controller: !Reference
        label: DMA1
      channel: 2
      request: 0
    TX-DMA:
    - controller: !Reference
        label: DMA1
      channel: 5
      request: 0
    - controller: !Reference
        label: DMA1
      channel: 7
      request: 0
    $labels:
    - SPI3
```
