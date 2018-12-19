ST,STM32-DMAv2
==============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-DMAs-v2-group`;
- `DMAx` (`x` - integer), optional - zero or more, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, `ST,STM32-DMA-v2`;
  - `interrupts`, required, mapping:
    - `controller`, required, reference, reference to interrupt controller;
    - `vectors`, required, list of strings, names of interrupt (sub)vector used by this peripheral, number of items
	shall be equal to number of channels;

Note that *distortos* aims to use the names *channel* and *request* consistently for all DMAs. However official
documentation for *STM32-DMAv2* uses *stream* when referring to *channel* and *channel* when referring to *request*.

Examples
--------

```
DMAs:
  compatible:
  - ST,STM32-DMAs-v2-group
  DMA1:
    compatible:
    - ST,STM32-DMA-v2
    interrupts:
      controller: !Reference
        label: NVIC
      vectors:
      - DMA1_Stream0
      - DMA1_Stream1
      - DMA1_Stream2
      - DMA1_Stream3
      - DMA1_Stream4
      - DMA1_Stream5
      - DMA1_Stream6
      - DMA1_Stream7
    $labels:
    - DMA1
  DMA2:
    compatible:
    - ST,STM32-DMA-v2
    interrupts:
      controller: !Reference
        label: NVIC
      vectors:
      - DMA2_Stream0
      - DMA2_Stream1
      - DMA2_Stream2
      - DMA2_Stream3
      - DMA2_Stream4
      - DMA2_Stream5
      - DMA2_Stream6
      - DMA2_Stream7
    $labels:
    - DMA2
```
