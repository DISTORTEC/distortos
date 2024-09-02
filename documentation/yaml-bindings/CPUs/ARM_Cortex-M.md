ARM,Cortex-M
============

- `compatible`, required, list of strings:
  - [0], required, string, type of CPU, one of: `ARM,Cortex-M0`, `ARM,Cortex-M0+`, `ARM,Cortex-M1`, `ARM,Cortex-M3`,
  `ARM,Cortex-M4`, `ARM,Cortex-M7`, `ARM,Cortex-M33`;
  - [1], required, string, architecture of CPU, one of: `ARM,ARMv6-M`, `ARM,ARMv7-M`, `ARM,ARMv8-M`;
- `revision`, required, string, revision of CPU, e.g. `r0p1`;
- `FPU`, optional, mapping:
  - `compatible`, required, list of strings:
    - [0], required, string, type of FPU, one of: `ARM,FPv4-SP-D16`, `ARM,FPv5-SP-D16`, `ARM,FPv5-D16`;

Examples
--------

```
CPUs:
  0:
    compatible:
    - ARM,Cortex-M0+
    - ARM,ARMv6-M
    revision: r0p1
```

```
CPUs:
  0:
    compatible:
    - ARM,Cortex-M4
    - ARM,ARMv7-M
    revision: r0p1
    FPU:
      compatible:
      - ARM,FPv4-SP-D16
```

```
CPUs:
  0:
    compatible:
    - ARM,Cortex-M7
    - ARM,ARMv7-M
    revision: r1p0
    FPU:
      compatible:
      - ARM,FPv5-D16
```
