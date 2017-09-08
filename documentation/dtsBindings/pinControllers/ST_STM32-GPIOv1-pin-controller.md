ST STM32-GPIOv1-pin-controller
==============================

Required properties
-------------------

- `compatible` shall be `"ST,STM32-GPIOv1-pin-controller"`;

ST STM32-GPIOv1-pin-controller subnodes
=======================================

Required properties
-------------------

- `ST,GPIO-bank-name` should be the name of GPIO port, e.g. `"GPIOA"`;
- `#gpio-cells` shall be `<2>`;
    - first cell is the pin number;
    - second cell is the polarity: 0 - active high, 1 - active low;
- `reg` should be the address and size of the device's control block, e.g. `<0x40010800 1024>` for 1 kB control block
located at 0x40010800;
- `status` shall be `"okay"` if the device is enabled, `"disabled"` otherwise;

