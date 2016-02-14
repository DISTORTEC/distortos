distortos [![Build Status](https://travis-ci.org/DISTORTEC/distortos.svg)](https://travis-ci.org/DISTORTEC/distortos)
=========

Advanced real-time operating system for deeply embedded targets

[Homepage](http://distortos.org/)<br/>
[Documentation](http://distortos.org/documentation/)<br/>
[Source Code @ GitHub](https://github.com/DISTORTEC/distortos)<br/>
[Mailing List](https://lists.sourceforge.net/lists/listinfo/distortos-development)<br/>

- fully preemptive
- round-robin and FIFO scheduling
- all basic primitives aim for maximum compatibility with POSIX, thread support library from C++11 and other established
  standards
- POSIX pthread implementation
- support for various mutex types (normal, error-checking, recursive) and mutex protocols (normal, priority inheritance,
  priority protection)
- software timers
- POSIX-style signals
- static design where it is possible and makes sense
- object-oriented design
- written in C++11

Targets
-------

Currently supported targets for *distortos* are:

- ARM Cortex-M3 and ARM Cortex-M4(F) (ARMv7-M(E) architecture): STM32F4

Motivation and goals
--------------------

First of all - *distortos* doesn't aim to be *the smallest* or *the fastest* RTOS available. It's size and execution
overhead are proportional to the features offered. This of course doesn't mean that it uses hundreds of kilobytes of ROM
and RAM or all available processing power! It uses "just enough" to implement its functionality in a portable and
efficient way.

There are many RTOSes available in many flavours and for many platforms, so why another? There are many reasons, but
generally *distortos* was born of dissatisfaction with all of these existing solutions... *Distortos* aims to have *all*
good features and *none* of the broken ones.

The second important reason is the state of support for various C++ features in existing RTOSes, which usually varies
from "non existent" to "explicitly disabled" (with minor exceptions of course).

Despite the popular belief, using C++ does not automatically lead to slow/bloated/incomprehensible/... code, which
*distortos* will try to demonstrate. Source code of *distortos* is self-explanatory, simple and consists of short
functions. This contrasts with "typical" RTOS source code in the form of extremely long functions with very complex
control flow and heavy use of macros. And that would be the third reason - showing that source code of fully-featured
RTOS can be *simple*, while still being *efficient* and *feature-rich*.

Last, but not the least, most of these existing RTOSes require the developer to state some values during configuration -
max number of threads in the system, max number of supported priorities, etc. - this makes no sense at all, because a
good, scalable design doesn't need such values.

License
-------

This project is licensed under the terms of [Mozilla Public License Version 2.0](https://www.mozilla.org/MPL/2.0/). This
relatively new license is something in between other popular open source licenses like GPL or BSD. Most of the questions
are answered in the [MPL 2.0 FAQ](https://www.mozilla.org/MPL/2.0/FAQ.html).

Configuration & building
------------------------

To configure & build *distortos* you need:
- [GNU Make](http://www.gnu.org/software/make/) build system;
- (optionally) [tup](http://gittup.org/tup/) (version 0.7.3 or above) build system;
- [arm-none-eabi bleeding-edge-toolchain](https://sourceforge.net/projects/bleeding-edge/); alternatively you can try
  any other arm-none-eabi toolchain, but C++ exception handling code will increase the size of binary and incorrect
  newlib's _reent struct configuration will dramatically increase RAM requirements (bleeding-edge-toolchain has the
  exceptions properly disabled and uses small variant of _reent struct - see
  [here](http://www.freddiechopin.info/en/articles/35-arm/87-bleeding-edge-toolchain-o-co-chodzi)) - such combinations
  are not tested extensively;
- [AWK](https://en.wikipedia.org/wiki/AWK);
- POSIX-compatible shell (e.g. [Bash](https://www.gnu.org/software/bash/));
- [GNU Coreutils](http://www.gnu.org/software/coreutils/coreutils.html) or a set of compatible utilities;
- [kconfig-frontends](http://ymorin.is-a-geek.org/projects/kconfig-frontends) (especially *mconf* tool) to create or
  edit the configurations;

Make sure the tools are available in your system's *PATH* environment variable.

To create or edit a configuration run `make menuconfig` in the root of the project. You can either start from
scratch or edit one of existing configurations (from `configurations/<board>/<variant>` folder). When creating a
new configuration make sure the name of created file is `distortosConfiguration.mk`.

DO NOT edit `distortosConfiguration.mk` files manually! *kconfig-frontends* tools make sure that multiple pre- and
post-conditions are satisfied, and these conditions can easily be violated by manual modifications of the configuration.

To select the configuration of your choice execute
`make configure CONFIG_PATH=<path-to-distortosConfiguration.mk>`, where
`<path-to-distortosConfiguration.mk>` is the relative path to selected `distortosConfiguration.mk` file. This
will create `selectedConfiguration.mk` file which is needed to build *distortos*. If
`<path-to-distortosConfiguration.mk>` is in the form `configurations/<board>/<variant>`, you can omit
`configurations/` prefix - `make configure CONFIG_PATH=configurations/STM32F4DISCOVERY/test` and
`make configure CONFIG_PATH=STM32F4DISCOVERY/test` will both select the same configuration. You can execute
`make configure` with no arguments if the selected `distortosConfiguration.mk` file is in the main folder of the
project - in that case `.` is used as `CONFIG_PATH`.

To build just execute `make` (if using *GNU Make*) or `tup` (if using *tup*) command in the main directory of the
project. If you need to see verbose compilation log, execute `make VERBOSE=1` (if using *GNU Make*) or
`tup --verbose` (if using *tup*).

You can generate API reference with [doxygen](http://www.stack.nl/~dimitri/doxygen/) by running `make doxygen`. Exact
contents of generated HTML pages depends on your configuration - it is affected by `distortosConfiguration.mk` file
selected with `selectedConfiguration.mk` file.

You can remove build outputs with standard `make clean` command. This will only remove the files generated during
compilation phase. To remove all generated files (*tup* database, build outputs, *doxygen* outputs and
`selectedConfiguration.mk` file) you can run `make distclean`.

#### If you use tup and Linux

You need to set *suid* bit on your *tup* executable (`` sudo chmod +s `which tup` ``) and you need to make sure that
`fuse.conf` (usually `/etc/fuse.conf`) has `user_allow_other` option enabled.

#### If you use tup and Windows

Don't use 64-bit tools (even if you have 64-bit system), as *tup* cannot currently handle 64-bit executables.

#### Tools for Windows

You can get *GNU Make*, *AWK* (*GAWK*), *Bash* and *GNU Coreutils* by installing [MSYS2](https://msys2.github.io/) and
executing `pacman -S gawk make` in the shell of this software distro - the binaries will be placed in
`<installation path>\usr\bin` folder. *kconfig-frontends* binaries for *Windows* can be found on
[distortos.org](http://distortos.org/) in [Downloads](http://distortos.org/downloads/) >
[Tools](http://distortos.org/downloads/tools/) >
[kconfig-frontends for Windows](http://distortos.org/downloads/tools/kconfig-frontends-for-windows/). *tup* and
*arm-none-eabi bleeding-edge-toolchain* binaries for *Windows* are available from their main websites.
