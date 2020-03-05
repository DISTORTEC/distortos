GPIO-outputs
============

`GPIO-outputs` shall be a mapping. The name (key) of the mapping will be used as the name of generated objects. There
may be any number of `GPIO-outputs` mappings.

- `compatible`, required, list of strings:
  - [0], required, string, `GPIO-outputs`;
- `<GPIO-output-name>` (`<GPIO-output-name>` - string), optional - zero or more, mapping:
  - `GPIO`, required, mapping, all key-value pairs are specific to particular GPIO implementation, legacy "output" mode
  of GPIO mapping is expected;
  - `alternative-names`, optional, list of strings, one or more alternative names of output;

Examples
--------

```
leds:
  compatible:
  - GPIO-outputs
  ld1:
    GPIO:
      port: !Reference {label: GPIOJ}
      pin: 13
    alternative-names:
    - red
  ld2:
    GPIO:
      port: !Reference {label: GPIOJ}
      pin: 5
    alternative-names:
    - green
```
