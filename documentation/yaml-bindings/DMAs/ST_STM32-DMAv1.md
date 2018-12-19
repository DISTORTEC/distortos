ST,STM32-DMAv1
==============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-DMAs-v1-group`;
- `DMAx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-DMA-v1`;
  - `interrupts`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vectors`, required, list of strings, names of interrupt (sub)vector used by this peripheral, number of items
	shall be equal to number of channels;

Examples
--------

```
DMAs:
  compatible:
  - ST,STM32-DMAs-v1-group
  DMA1:
    compatible:
    - ST,STM32-DMA-v1
    interrupts:
      controller: !Reference
        label: NVIC
      vectors:
      - DMA1_Ch1
      - DMA1_Ch2
      - DMA1_Ch3
      - DMA1_Ch4
      - DMA1_Ch5
      - DMA1_Ch6
      - DMA1_Ch7
    $labels:
    - DMA1
  DMA2:
    compatible:
    - ST,STM32-DMA-v1
    interrupts:
      controller: !Reference
        label: NVIC
      vectors:
      - DMA2_Ch1
      - DMA2_Ch2
      - DMA2_Ch3
      - DMA2_Ch4
      - DMA2_Ch5
    $labels:
    - DMA2
```
