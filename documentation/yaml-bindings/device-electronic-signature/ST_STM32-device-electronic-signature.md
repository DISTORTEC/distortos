ST,STM32-device-electronic-signature
====================================

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-device-electronic-signature`;
- `unique-device-ID`, optional, mapping:
  - `fragmented`, required, boolean, selects whether unique device ID register implemented in chip is fragmented - has
  third word at offset 20 (`true`), or not - third word is at offset 8, adjacent to second word (`false`);

Examples
--------

```
device-electronic-signature:
  compatible:
  - ST,STM32-device-electronic-signature
  unique-device-ID:
    fragmented: false
```
