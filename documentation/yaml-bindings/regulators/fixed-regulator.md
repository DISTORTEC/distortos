fixed-regulator
===============

- `compatible`, required, list of strings:
  - [0], required, string, `fixed-regulator`;
- `always-on`, required, boolean, selects whether this regulator is always on (`true`) or not (`false`);
- `output-voltage`, required, float, output voltage, Volts;

Examples
--------

```
regulators:
  VDD:
    compatible:
    - fixed-regulator
    always-on: true
    output-voltage: 3.3
    $labels:
    - VDD
```
