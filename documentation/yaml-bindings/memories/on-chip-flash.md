on-chip-flash
=============

- `compatible`, required, list of strings:
  - [0], required, string, `on-chip-flash`;
- `address`, required, integer, address of the memory;
- `size`, required, integer, size of the memory in bytes;
- `sections`, optional, list of strings, names of sections from linker script which may be put into this memory;
- `virtualAddress`, optional, integer, virtual address of the memory, from which the code may be executed - equivalent
of VMA (Virtual Memory Address) used in linker scripts;

Examples
--------

```
memories:
  flash:
    compatible:
    - on-chip-flash
    address: 134217728
    size: 524288
    virtual-address: 2097152
    $labels:
    - flash
```
