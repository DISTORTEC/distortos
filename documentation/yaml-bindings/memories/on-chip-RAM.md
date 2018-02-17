on-chip-RAM
===========

- `compatible`, required, list of strings:
  - [0], required, string, `on-chip-RAM`;
- `address`, required, integer, address of the memory;
- `size`, required, integer, size of the memory in bytes;
- `sections`, optional, list of strings, names of sections from linker script which may be put into this memory;

Examples
--------

```
memories:
  SRAM:
    compatible:
    - on-chip-RAM
    address: 536870912
    size: 327680
    $labels:
    - SRAM
  ITCM:
    compatible:
    - on-chip-RAM
    address: 0
    size: 16384
    $labels:
    - ITCM
  BKPSRAM:
    compatible:
    - on-chip-RAM
    address: 1073889280
    size: 4096
    sections:
    - .BKPSRAM.noinit
    - .noinit
    $labels:
    - BKPSRAM
```
