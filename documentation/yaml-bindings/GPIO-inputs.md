GPIO-inputs
===========

`GPIO-inputs` shall be a mapping. The name (key) of the mapping will be used as the name of generated objects. There may
be any number of `GPIO-inputs` mappings.

- `compatible`, required, list of strings:
  - [0], required, string, `GPIO-inputs`;
- `<GPIO-input-name>` (`<GPIO-input-name>` - string), optional - zero or more, mapping:
  - `GPIO`, required, mapping, all key-value pairs are specific to particular GPIO implementation, legacy "input" mode
  of GPIO mapping is expected;
  - `alternative-names`, optional, list of strings, one or more alternative names of input;

Examples
--------

```
buttons:
  compatible:
  - GPIO-inputs
  b1:
    GPIO:
      port: !Reference {label: GPIOC}
      pin: 13
      active-low: true
    alternative-names:
    - user
```
