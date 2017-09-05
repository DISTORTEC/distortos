General structure of .dts files
===============================

An absolute minimum `.dts` file that will work with *distortos* shall look like on the example below:

```
/dts-v1/;

#include "<chip-dtsi>"

/ {
	compatible = "<board-manufacturer>,<board-model>", "<chip-manufacturer>,<chip-model>";
};

```

where:
- `<chip-dtsi>` is the name of appropriate chip's `.dtsi` file, e.g. `STM32F407VG.dtsi`;
- `<board-manufacturer>` is the name of board's manufacturer, e.g. `ST`;
- `<board-model>` is the mode of board, e.g. `STM32F4DISCOVERY`;
- `<chip-manufacturer>` is the name of chip's manufacturer, e.g. `ST`;
- `<chip-model>` is the model of chip, e.g. `STM32F407VG`;

Thus, a minimum `.dts` file may look like on the example below:

```
/dts-v1/;

#include "STM32F407VG.dtsi"

/ {
	compatible = "ST,STM32F4DISCOVERY", "ST,STM32F407VG";
};
```
