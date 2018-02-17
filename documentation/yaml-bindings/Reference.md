!Reference
==========

`!Reference` can be used either to extend selected mapping of just to reference it. In both cases it should be
constructed as `!Reference {label: <label-name>}` where `<label-name>` is one of the strings in `$labels` of the target
mapping.

Extending
---------

To extend a mapping just use `!Reference` as a key:

```
clocks:
  HSE:
    compatible:
    - ST,STM32-HSE
    - fixed-clock
    frequency: 0
    $labels:
    - HSE
...
!Reference {label: HSE}:
  frequency: 8000000
  bypass: true
```

After parsing this would yield:

```
clocks:
  HSE:
    compatible:
    - ST,STM32-HSE
    - fixed-clock
    frequency: 8000000
    bypass: true
    $labels:
    - HSE
```

Referencing
-----------

Some bindings require a reference to some particular mapping as a value. In these cases you should use `!Reference` as a
value:

```
buttons:
  compatible:
  - GPIO-inputs
  b1:
    GPIO:
      port: !Reference {label: GPIOC}
      pin: 13
    alternative-names:
    - user
```
