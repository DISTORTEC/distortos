ST,STM32
========

- `compatible`, required, list of strings:
  - [0], required, string, exact model of chip, e.g. `ST,STM32F407VG`;
  - [1], required, string, chip family, e.g. `ST,STM32F4`;
  - [2], required, string, chip type, `ST,STM32`;
- `VDD`, optional, reference, reference to mapping with regulator connected to chip's VDD pins;

Examples
--------

```
# fragment of source/chip/STM32/STM32F7/chipYaml/ST_STM32F769NI.yaml
chip:
  compatible:
  - ST,STM32F779NI
  - ST,STM32F7
  - ST,STM32

# fragment of source/board/ST_32F769IDISCOVERY/ST_32F769IDISCOVERY.yaml
$extends:
- 'source/chip/STM32/STM32F7/chipYaml/ST_STM32F769NI.yaml'
...
chip:
  VDD: !Reference {label: VDD}
...
regulators:
  VDD:
    compatible:
    - fixed-regulator
    always-on: true
    output-voltage: 3.3
    $labels:
    - VDD
```
