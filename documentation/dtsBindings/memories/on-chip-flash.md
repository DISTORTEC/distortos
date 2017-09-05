on-chip-flash
=============

Required properties
-------------------

- `compatible` shall be `"on-chip-flash"`;
- `reg` should be the address and size of the memory, e.g. `<0x8000000 1048576>` for 1 MB of flash located at 0x8000000;
- `label` is a string with device's label;
