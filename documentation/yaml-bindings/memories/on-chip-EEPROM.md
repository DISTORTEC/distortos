on-chip-EEPROM
==============

- `compatible`, required, list of strings:
  - [0], required, string, `on-chip-EEPROM`;
- `address`, required, integer, address of the memory;
- `size`, required, integer, size of the memory in bytes;

Examples
--------

```
memories:
  EEPROM:
    compatible:
    - on-chip-EEPROM
    address: 134742016
    size: 512
    $labels:
    - EEPROM
```
