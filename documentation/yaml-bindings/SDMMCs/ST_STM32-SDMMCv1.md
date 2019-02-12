ST,STM32-SDMMCv1
================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-SDMMCs-v1-group`;
- `SDMMCx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-SDMMC-v1`;
  - `interrupt`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vector`, required, string, name of interrupt (sub)vector used by this peripheral;
  - `DMA`, required, list;

`DMA` items
-----------

Each item corrensponding to a valid DMA configuration shall be a mapping:
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
```
