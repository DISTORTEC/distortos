on-chip-eeprom
==============

Required properties
-------------------

- `compatible` shall be `"on-chip-eeprom"`;
- `reg` should be the address and size of the memory, e.g. `<0x8080000 512>` for 512 B of EEPROM located at 0x8080000;
- `label` is a string with device's label;
