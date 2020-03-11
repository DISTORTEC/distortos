ST,STM32-SDMMCv1
================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-SDMMCs-v1-group`;
- `SDIO-variant`, optional, boolean, selects "SDMMC" (`false`) or "SDIO" (`true`) variant of the driver; default -
`false`;
- `SDMMCx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-SDMMC-v1`;
  - `interrupt`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vector`, required, string, name of interrupt (sub)vector used by this peripheral;
  - `DMA`, required, list;
  - `CK`, `CMD`, `D0`, `D1`, `D2`, `D3`, optional, mapping, all key-value pairs are specific to particular GPIO
  implementation; selects configuration of pins; allowed combinations are: none, {`CK`, `CMD`, `D0`} or
  {`CK`, `CMD`, `D[0-3]`};

`DMA` items
-----------

Each item corresponding to a valid DMA configuration shall be a mapping:
- `controller`, required, reference, reference to DMA controller;
- `channel`, required, integer, DMA channel identifier;
- `request`, required, integer, DMA request identifier;

Examples
--------

```
SDMMCs:
  compatible:
  - ST,STM32-SDMMCs-v1-group
  SDMMC1:
    compatible:
    - ST,STM32-SDMMC-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: SDMMC1
    DMA:
    - controller: !Reference
        label: DMA2
      channel: 3
      request: 4
    - controller: !Reference
        label: DMA2
      channel: 6
      request: 4
    $labels:
    - SDMMC1
  SDMMC2:
    compatible:
    - ST,STM32-SDMMC-v1
    interrupt:
      controller: !Reference
        label: NVIC
      vector: SDMMC2
    DMA:
    - controller: !Reference
        label: DMA2
      channel: 0
      request: 11
    - controller: !Reference
        label: DMA2
      channel: 5
      request: 11
    $labels:
    - SDMMC2
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
!Reference {label: SDMMC2}:
  CK:
    port: !Reference {label: GPIOD}
    pin: 6
    mode: alternate-function
    alternate-function: 11
    output-speed: very-high
  CMD:
    port: !Reference {label: GPIOD}
    pin: 7
    mode: alternate-function
    alternate-function: 11
    output-speed: very-high
  D0:
    port: !Reference {label: GPIOG}
    pin: 9
    mode: alternate-function
    alternate-function: 11
    output-speed: very-high
```
