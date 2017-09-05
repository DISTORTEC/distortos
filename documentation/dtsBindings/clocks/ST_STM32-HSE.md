ST STM32-HSE
============

Required properties
-------------------

- `compatible` shall be `"ST,STM32-HSE", "fixed-clock"`;
- `clock-cells` shall be `<0>`;
- `clock-frequency` shall be a single integer cell with output frequency of clock, expressed in hertz, e.g. `<8000000>`
for 8 MHz;
- `clock-output-names` shall be `"HSE"`;

Optional properties
-------------------

- `ST,STM32-HSE-bypass` is a boolean property which selects bypass mode of HSE;
