Change Log
==========

All notable changes to this project will be documented in this file. This project adheres to
[Semantic Versioning](http://semver.org/).

[Unreleased](https://github.com/DISTORTEC/distortos/compare/v0.1.0...HEAD)
--------------------------------------------------------------------------

### Added

- Support for *ARMv6-M* (*ARM Cortex-M0*, *ARM Cortex-M0+* and *ARM Cortex-M1*) architecture.
- Support for all 94 [STM32F1](http://www.st.com/web/en/catalog/mmc/SC1169/SS1031) chips.
- Support and test configuration for
[NUCLEO-F103RB](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/LN1847/PF259875?icmp=nucleo-ipf_pron_pr-nucleo_feb2014&sc=nucleoF103RB-pr) board with *STM32F1* chip and
[NUCLEO-F429ZI](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/LN1847/PF262637) board with *STM32F4* chip.
- `make olddefconfig` target, which can be used to update currently selected configuration with default values of new
options.
- `updateAllConfigurations.sh` script, which can be used to run `make olddefconfig` for all configurations found in
provided (optional) search path.
- Ability to delete some folders - if you don't use their contents - without breaking *make*/*tup* build and *Kconfig*
menus. You can obviously delete anything from `configurations/`, including the entire folder. The same is true for
`documentation/` directory if you don't plan to generate *API* reference with *doxygen*. In case of `external/`,
`source/architecture/`, `source/board/` and `source/chip/` you can delete any number of complete components or group of
components - e.g. you can delete `source/board/STM32/STM32F4/STM32F4DISCOVERY/` (port for single board) and/or
`source/board/STM32/STM32F4/` (group of ports for boards), but you shouldn't delete just
`source/board/STM32/STM32F4/STM32F4DISCOVERY/include/` (part of port for single board). Additionally you can delete
entire `source/board/` and/or `test/` folders. After any deletion it is necessary to run `scripts/regenerateKconfig.sh`
script once before using `make menuconfig`. When running it "outside" of *distortos* source tree (e.g. in a project
based on "subfolder" template), provide it with the path to *distortos* in the first argument.
- Configuration of *SRAM1*, *SRAM2* and *SRAM3* regions' unification for *STM32F4* chips.
- Support for additional memory regions of *STM32F4* chips (*BKPSRAM*, *CCM*, *SRAM2*, *SRAM3*) in generated linker
scripts.
- Ability to use *ARMv7-M*'s *PRIMASK* to disable all interrupts during critical sections when "Interrupt priority
disabled in critical sections" (*ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI*) options is set to 0. This is the new default
setting, as this is a more conservative (and thus safer for a beginner) approach, which doesn't require any special
configuration of interrupt priorities (default value is 0).

### Changed

- Change values of all GPIO-related enums for *STM32F4* (Pin, PinAlternateFunction, PinMode, PinOutputSpeed and PinPull)
from ALL CAPS or PascalCase to camelCase for consistency with their use as constants and with other enums.
- Replace individual linker script generators for *STM32F1* and *STM32F4* families with generic script for
*ARMv7-M*-based chips. The new generator also supports arbitrary number of additional memories (like BKPSRAM, CCM, ...),
which can be defined with command line arguments.

### Fixed

- Fix insufficient dependencies for "PLLR" option in system clock source configuration for *STM32F4*. Main PLL's "/R"
output cannot be used as system clock if PLL is disabled.
- Fix invalid definition of templated variants of `Semaphore::tryWaitFor()` and `Semaphore::tryWaitUntil()`.

0.1.0 - 2016-02-26
------------------

### Added

- Build infrastructure for *make* and *tup*.
- Fully preemptive scheduling with 256 priority levels. FIFO and round-robin scheduling policies. Completely static
implementation.
- Static and dynamic threads. Thread can execute any function type (regular function, member function, stateless
functor, lambda) with any number of arguments of any type. Both thread types can be joined, while dynamic threads can
also be detached.
- Semaphores which can also be configured to work as binary semaphores during construction. Completely static
implementation. `constexpr` constructor. Non-blocking functions can be used in interrupt handlers.
- Mutexes which support various types (normal, error-checking, recursive) and protocols (normal, priority inheritance,
priority protection). Priority inheritance protocol works with no limitations: through any number of inheritance
"levels" and with any number of mutexes locked by threads in any order. Completely static implementation. `constexpr`
constructor.
- Condition variables which can work with mutexes. Completely static implementation. `constexpr` constructor.
- `callOnce()` and `OnceFlag` - equivalents of `std::call_once()` and `std::once_flag` from C++11 or
`pthread_once()` and `pthread_once_t` from POSIX. Completely static implementation. `OnceFlag` has `constexpr`
constructor.
- Message and FIFO queues, both in two flavors: with proper support for C++ objects and for binary serializable types
(like POD types). 256 priority levels in message queues. Available in static and dynamic versions. Non-blocking
functions can be used in interrupt handlers.
- Support for POSIX-style signals with synchronous (waits) and asynchronous (signal handlers) API. Signals can be
"generated" and "queued" with value. Completely static implementation. Non-blocking functions can be used in interrupt
handlers.
- One-shot static software timers which can execute any function type (regular function, member function, stateless
functor, lambda) with any number of arguments of any type.
- Basic device drivers for input and output pin.
- Support for *ARMv7-M* (*ARM Cortex-M3* and *ARM Cortex-M4(F)*) architecture. FPU can be used in any number of threads
and interrupt handlers at the same time.
- Support for *STM32F4* chips.
- Support and test configurations for 3 boards with *STM32F4* chips:
[32F429IDISCOVERY](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF259090),
[NUCLEO-F401RE](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/LN1847/PF260000?icmp=nucleo-ipf_pron_pr-nucleo_feb2014&sc=nucleoF401RE-pr)
and [STM32F4DISCOVERY](http://www.st.com/web/catalog/tools/FM116/SC959/SS1532/PF252419).
- Extensive functional tests for all implemented functionalities.
- Configuration with mconf tool from [kconfig-frontends](http://ymorin.is-a-geek.org/projects/kconfig-frontends).
- `README.md` file with introduction and information about configuration & building.
- Integration with [Travis CI](https://travis-ci.org/DISTORTEC/distortos) service.
