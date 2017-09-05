fixed-regulator
===============

Required properties
-------------------

- `compatible` shall be `"fixed-regulator"`;
- `regulator-name` should be a string with name of this regulator, e.g. `"VDD"`;
- `regulator-min-microvolt` and `regulator-max-microvolt` should be a single integer cell with output voltage of this
regulator, expressed in microvolts, both values should be identical, e.g. `<3300000>` for 3.3 V output voltage;

Optional properties
-------------------

- `regulator-always-on` is a bool property which means that the regulator cannot be disabled and is always on;
