$labels
=======

`$labels` shall be a list of strings. Each string can then be used as a target of `Reference`. `$labels` may be used in
all mappings.

Examples
--------

```
clocks:
  HSE:
    compatible:
    - ST,STM32-HSE
    - fixed-clock
    frequency: 0
    $labels:
    - HSE
memories:
  flash:
    compatible:
    - on-chip-flash
    address: 134217728
    size: 16384
    $labels:
    - flash
  SRAM:
    compatible:
    - on-chip-RAM
    address: 536870912
    size: 4096
    $labels:
    - SRAM
```
