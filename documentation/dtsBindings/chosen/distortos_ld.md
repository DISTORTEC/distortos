distortos,ld
============

Properties prefixed with `distortos,ld` in `chosen` node can be used to configure memory map in generated linker script.
Each property in the form `distortos,ld,<section-name> = &<memory-label>;` selects memory in which contents of given
section will be located. In case of `.data` sections it is also possible to select the non-volatile memory into which
initializers will be loaded with another format of this property -
`distortos,ld,<section-name>,load = &<memory-label>;`.

What section names are generated and what memories are available depends on the selected chip, so the best option is to
check `.dtsi` file for selected chip, which sets all possible properties of this type to their default values.
