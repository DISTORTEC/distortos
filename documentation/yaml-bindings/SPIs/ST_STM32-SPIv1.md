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
  - `RX-DMA`, required, list;
  - `TX-DMA`, required, list;
  - `MISO`, `MOSI`, `SCK`, optional, mapping, all key-value pairs are specific to particular GPIO implementation;
  selects configuration of pins; allowed combinations are: none, {`MISO`, `SCK`}, {`MOSI`, `SCK`} or
  {`MISO`, `MOSI`, `SCK`};

`RX-DMA` and `TX-DMA` items
---------------------------

Each item corresponding to a valid DMA configuration shall be a mapping:
- `controller`, required, reference, reference to DMA controller;
- `channel`, required, integer, DMA channel identifier;
- `request`, required, integer, DMA request identifier;

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
    - ST,STM32-SPI-v1
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
!Reference {label: SPI2}:
  MISO:
    port: !Reference {label: GPIOB}
    pin: 14
    mode: alternate-function
    alternate-function: 0
    output-speed: very-high
    pull: up
  MOSI:
    port: !Reference {label: GPIOB}
    pin: 15
    mode: alternate-function
    alternate-function: 0
    output-speed: very-high
  SCK:
    port: !Reference {label: GPIOB}
    pin: 13
    mode: alternate-function
    alternate-function: 0
    output-speed: very-high
```
