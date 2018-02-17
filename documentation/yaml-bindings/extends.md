$extends
========

`$extends` shall be a list of strings. Each string should be a filename of YAML file, relative to *distortos* path. Each
YAML file listed in `$extends` is loaded recursively and used as if it's contents appeared directly at the beginning of
currently processed file. The only difference is that values of duplicate keys (which are disallowed in a single YAML
file) are overwritten, except when they are mappings, in which case they are merged. This feature can be considered a
direct analogy of `#include` preprocessor directive from C/C++.

Examples
--------

```
$extends:
- 'source/chip/STM32/STM32L4/chipYaml/ST_STM32L476RG.yaml'
```
