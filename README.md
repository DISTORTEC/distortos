distortos
=========

Advanced real-time operating system for deeply embedded targets

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

- ARM Cortex-M3
- ARM Cortex-M4(F)

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

Building
--------

To build *distortos* you need:
- [GNU Make](http://www.gnu.org/software/make/) or [tup](http://gittup.org/tup/) (version 0.7.3 or above) build system;
- [arm-none-eabi bleeding-edge-toolchain](https://sourceforge.net/projects/bleeding-edge/); alternatively you can try
  any other arm-none-eabi toolchain, but C++ exception handling code will increase the size of binary and incorrect
  newlib's _reent struct configuration will dramatically increase RAM requirements (bleeding-edge-toolchain has the
  exceptions properly disabled and uses small variant of _reent struct - see
  [here](http://www.freddiechopin.info/en/articles/35-arm/87-bleeding-edge-toolchain-o-co-chodzi)) - such combinations
  are not tested extensively;

Make sure the tools are available in your system's *PATH* environment variable.

To build just execute `make` (if using *GNU Make*) or `tup` (if using *tup*) command in the main directory of the
project.

#### If you use tup and Linux

You need to set *suid* bit on your *tup* executable (`` sudo chmod +s `which tup` ``) and you need to make sure that
`fuse.conf` (usually `/etc/fuse.conf`) has `user_allow_other` option enabled.

#### If you use tup and Windows

Don't use 64-bit tools (even if you have 64-bit system), as *tup* cannot currently handle 64-bit executables.

Mailing list
------------

[distortos-development](https://lists.sourceforge.net/lists/listinfo/distortos-development) is a mailing list dedicated
to discussions about development of distortos.
