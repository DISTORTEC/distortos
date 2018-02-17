ST,STM32-HSE
============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-HSE`;
  - [1], required, string, `fixed-clock`;
- `frequency`, required, integer, frequency of crystal/ceramic resonator (`bypass` not present or set to `false`) or
external clock (`bypass` set to `true`) connected to HSE pin(s), Hz;
- `bypass`, optional, boolean, selects bypass mode in HSE, `false` if crystal or ceramic resonator is connected to HSE's
IN and OUT pins, `true` if external clock is connected to HSE's IN pin;

Examples
--------

```
# fragment of source/chip/STM32/STM32F7/chipYaml/ST_STM32F769NI.yaml
clocks:
  HSE:
    compatible:
    - ST,STM32-HSE
    - fixed-clock
    frequency: 0
    $labels:
    - HSE

# fragment of source/board/ST_32F769IDISCOVERY/ST_32F769IDISCOVERY.yaml
!Reference {label: HSE}:
  frequency: 25000000
  bypass: true
```
