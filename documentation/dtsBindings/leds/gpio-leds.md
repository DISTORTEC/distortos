gpio-leds
=========

Required properties
-------------------

- `compatible` shall be `"gpio-leds"`;
- `status` shall be `"okay"` if the device is enabled, `"disabled"` otherwise;

gpio-leds subnodes
==================

Required properties
-------------------

- `gpios` is a reference to appropriate GPIO and its configuration;
- `label` is a string with device's label;
- `status` shall be `"okay"` if the device is enabled, `"disabled"` otherwise;
