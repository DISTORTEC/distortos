ST,STM32-LSE
============

- `compatible`, required, list of strings:
  - [0], required, string, `ST,STM32-LSE`;
  - [1], required, string, `fixed-clock`;
- `frequency`, required, integer, frequency of crystal/ceramic resonator (`bypass` not present or set to `false`) or
external clock (`bypass` set to `true`) connected to LSE pin(s), Hz;
- `bypass`, optional, boolean, selects bypass mode in LSE, `false` if crystal or ceramic resonator is connected to LSE's
IN and OUT pins, `true` if external clock is connected to LSE's IN pin;

Examples
--------

```
# fragment of source/chip/STM32/STM32G0/chipYaml/ST_STM32G0B1RE.yaml
clocks:
  ...
  LSE:
    compatible:
    - ST,STM32-LSE
    - fixed-clock
    frequency: 0
    bypass: false
    $labels:
    - LSE

# fragment of source/board/ST_NUCLEO-G0B1RE/ST_NUCLEO-G0B1RE.yaml
!Reference {label: LSE}:
  frequency: 32768
```
