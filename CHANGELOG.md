Change Log
==========

All notable changes to this project will be documented in this file. This project adheres to
[Semantic Versioning](http://semver.org/).

[Unreleased](https://github.com/DISTORTEC/distortos/compare/v0.7.0...HEAD)
--------------------------------------------------------------------------

### Added

- Added support and test configuration for [NUCLEO-F767ZI](https://www.st.com/en/evaluation-tools/nucleo-f767zi.html)
board with *STM32F7* chip.
- Added boolean `#define` in `distortosConfiguration.h` which specifies selected board, e.g.
`#define DISTORTOS_BOARD_ST_NUCLEO_F767ZI 1` for *NUCLEO-F767ZI* board.
- Added `distortos::devices::BufferingBlockDevice` class. This class tries to minimize amount of block device operations
by buffering both reads and writes. Such buffering can give signigicant gain in case of devices like *SD* cards, where
each operation may cause the device to become "busy" for a noticeable amount of time. With this class several adjacent
reads or writes can be combined into a single larger operation, which is much faster overall when you also include the
waits for the *SD* card to become "idle". Another use for this class is as a proxy between a file system and a block
device which requires specific alignment.
- Added support for [FAT](https://en.wikipedia.org/wiki/File_Allocation_Table) file system (using
[uFAT](https://github.com/dlbeer/ufat) library) in the form of `distortos::FatFileSystem` class, which is available from
`distortos/FileSystem/FatFileSystem.hpp` header.
- Added `distortos/FileSystem/Littlefs2FileSystem.hpp` header with `distortos::Littlefs2FileSystem` class, which
provides support for [littlefs-v2](https://github.com/ARMmbed/littlefs) file system.
- Added support for *STM32's* 96-bit *unique device ID*, which consists of appropriate data in *CSV* / *YAML* files for
each supported chip and `distortos/chip/uniqueDeviceId.hpp` header created by board generator.
- Added `distortos/chip/PinInitializer.hpp` header with `distortos::chip::PinInitializer` functor,
`distortos::chip::makeAlternateFunctionPinInitializer()`, `distortos::chip::makeAnalogPinInitializer()`,
`distortos::chip::makeInputPinInitializer()` and `distortos::chip::makeOutputPinInitializer()` helper functions, both
for *STM32's* *GPIOv1* and *GPIOv2*.
- Extended *Jinja2* board templates with ability to generate pin initializers for *STM32's* *SDMMC*, *SPI* and *USART*.
- Added support for *SDIO* variant to *STM32's* *SDMMCv1* driver.
- Enabled *STM32's* *SDMMCv1* driver (in *SDIO* variant) for *STM32F4* chip family. Added necessary data to *CSV* and
*YAML* files of *STM32F4* chips.

### Changed

- Simplified generated linker scripts. This change causes the build to fail at link stage with
`ld: invalid data statement` when using *binutils* version 2.27 or earlier. There is no error when using *binutils*
version 2.28 (released on 2nd March 2017) or later, so in case of problems please update your toolchain.
- Changed various aspects related to [littlefs-v1](https://github.com/ARMmbed/littlefs) file system integration:
  - Headers which declare `distortos::Littlefs1File` and `distortos::Littlefs1Directory` classes are no longer
  acceessible by the application code - these classes are never used directly, only via `distortos::File` and
  `distortos::Directory` interfaces;
  - Moved `distortos/FileSystem/littlefs1/Littlefs1FileSystem.hpp` one level up, to
  `distortos/FileSystem/Littlefs1FileSystem.hpp`;
  - `distortos::Littlefs1FileSystem`, `distortos::Littlefs1File` and [littlefs-v1](https://github.com/ARMmbed/littlefs)
  itself no longer deal with buffer alignment - `distortos::Littlefs1FileSystem` should be used only with a driver which
  either doesn't care about alignment or deals with it itself (for example via proxy
  `distortos::devices::BufferingBlockDevice`).
- All implementations of `distortos::File::read()` and `distortos::File::write()` assert that file is opened for reading
or writing respectively, instead of returning `EBADF`.
- Extracted internal `distortos::devices::SerialPort::CircularBuffer` to `estd::RawCircularBuffer`. It is a generic,
thread-safe, lock-free raw circular buffer for single-producer and single-consumer scenarios.
- Renamed `distortos::chip::ChipInputPin` and `distortos::chip::ChipOutputPin` to `distortos::chip::InputPin` and
`distortos::chip::OutputPin` respectively. Aliases for old names were added, marked as deprecated and are scheduled to
be removed after v0.8.0.

### Fixed

- Fixed mechanism of generating `distortosConfiguration.h` header and `distortosConfiguration.cmake` saved
configuration. These files now also include configuration variables added "outside" of *distortos* itself, for example
in another submodule (beside *distortos*) of an application.
- Fixed definition of `DIR` in `sys/dirent.h`, which was valid only for C++ code. `DIR` has to be a `typedef`, not a
`struct`.
- Fixed incorrect `compatible` key for `pin-controller` in `ST_STM32F030C6.yaml`.
- Some minor fixes required for *GCC 10*.

### Removed

- Removed `cmake/Toolchain-arm-none-eabi.cmake`. Board-specific toolchain files should be used exclusively.
- Removed deprecated files: `distortos/devices/communication/SpiMasterOperation.hpp`,
`distortos/devices/communication/SpiMasterOperationRange.hpp` and `distortos/chip/ChipSpiMasterLowLevel.hpp`.

[0.7.0](https://github.com/DISTORTEC/distortos/compare/v0.6.0...v0.7.0) - 2019-05-05
------------------------------------------------------------------------------------

### Added

- Added support for [NUCLEO-F042K6](https://www.st.com/en/evaluation-tools/nucleo-f042k6.html) and
[32F072BDISCOVERY](https://www.st.com/en/evaluation-tools/32f072bdiscovery.html) boards with *STM32F0* chips.
- Added basic support for *STM32's* *DMAv1* and *DMAv2*, along with data in *CSV* and *YAML* files for each supported
chip and unit tests.
- Added basic support for *STM32's* *SDMMCv1*, currently only for *STM32F7* chip family. Support includes data in *CSV*
and *YAML* files, as well as unit tests of `distortos::chip::SdMmcCardLowLevel` low-level driver, which implements
`distortos::devices::SdMmcCardLowLevel` interface.
- Added `distortos::chip::SpiMasterLowLevelDmaBased` classes for *STM32's* *SPIv1* and *SPIv2*. These classes implement
`distortos::devices::SpiMasterLowLevel` interface and use DMA for transfers.
- Added `distortos::devices::BlockDevice` and `distortos::devices::MemoryTechnologyDevice` interface classes.
- Added `distortos::devices::BlockDeviceToMemoryTechnologyDevice` class which wraps `distortos::devices::BlockDevice`
object and exposes `distortos::devices::MemoryTechnologyDevice` interface.
- Added `distortos::devices::SdCard` and `distortos::devices::SdCardSpiBased` classes, both based on
`distortos::devices::BlockDevice` interface, which can be used with *SD* cards connected via SDMMC or SPI respectively.
This code handles only *SD version 2.0* cards, has no support for run-time detection of card insertion/removal and has
no support for detecting whether card is write-protected. Code was tested with 2 GB *SDSC* and 32 GB *SDHC* cards.
- Added basic framework for file systems in the form of 3 abstract classes: `distortos::FileSystem`, `distortos::File`
and `distortos::Directory`.
- Added global `distortos::openFile()`, taking `distortos::FileSystem` reference and returning a fully functional
`FILE*` for use with `<stdio.h>` functions.
- Added support for [littlefs-v1](https://github.com/ARMmbed/littlefs) file system, provided by
`distortos::Littlefs1FileSystem`, `distortos::Littlefs1File` and `distortos::Littlefs1Directory` classes, which
implement interface of `distortos::FileSystem`, `distortos::File` and `distortos::Directory` classes.
- Added configuration of *STM32F7's* *PLLI2S*, *PLLSAI* and *PLL48CLK* to *CMake*.
- Added `distortos::Mutex::try_lock()`, `distortos::Mutex::try_lock_for()` and `distortos::Mutex::try_lock_until()`
wrappers, which implement APIs of [`std::mutex::try_lock()`](http://en.cppreference.com/w/cpp/thread/mutex/try_lock),
[`std::timed_mutex::try_lock_for()`](http://en.cppreference.com/w/cpp/thread/timed_mutex/try_lock_for) and
[`std::timed_mutex::try_lock_until()`](http://en.cppreference.com/w/cpp/thread/timed_mutex/try_lock_until) respectively.
This allows using `distortos::Mutex` with all mutex helpers from
[`<mutex>` header](https://en.cppreference.com/w/cpp/header/mutex):
[`std::lock_guard`](https://en.cppreference.com/w/cpp/thread/lock_guard),
[`std::unique_lock`](https://en.cppreference.com/w/cpp/thread/unique_lock),
[`std::scoped_lock`](https://en.cppreference.com/w/cpp/thread/scoped_lock),
[`std::try_lock()`](https://en.cppreference.com/w/cpp/thread/try_lock) and
[`std::lock()`](https://en.cppreference.com/w/cpp/thread/lock).
- Added `sys/dirent.h` and `sys/statvfs.h` headers, which are not provided by *newlib*.
- Added unit tests of `distortos::devices::BlockDeviceToMemoryTechnologyDevice` and `distortos::devices::SdCard`
classes.
- Added unit tests of *STM32's* *SPIv1* and *SPIv2* drivers.
- Added unit tests of all `estd::ContiguousRange` constructor overloads.
- Added `estd/EnumClassFlags.hpp` which provides templated bitwise operators for enum class flags.
- Added `estd/log2u.hpp` with `log2()`-like `constexpr` function for `unsigned int`.
- Added `estd/TypeFromSize.hpp` with `estd::TypeFromSize<>` template, which selects fixed width type from requested byte
size.
- Added `estd/extractBitField.hpp`, which provides a completely "inline-able" (with enabled optimizations) function
template to extract bit field from array of raw data.
- Added option to test coverage in unit tests. Enable it by setting *CMake* variable `COVERAGE` of unit test subproject.
You will then be able to use new `coverage` target to generate detailed *HTML* coverage reports with
[gcovr](https://gcovr.com/) in build folder of unit test subproject.
- Added `distortos::Semaphore::getMaxValue()` accessor and its C-API equivalent `distortos_Semaphore_getMaxValue()`.
- Added `...::getCapacity()` accessor for all variants of FIFO and message queues. Added `...::getElementSize()`
accessor for all raw variants of FIFO and message queues. In case of static queue variants, both of these accessors are
also available as `constexpr static` member functions.
- Added `...::ValueType` type alias for all non-raw variants of FIFO and message queues.

### Changed

- Implemented full support for configuring and building with *CMake*. The new *CMake* workflow does not need *Kconfig*
or any shell tools - just *CMake* (version 3.7 or later), build tool (it is recommended to use *Ninja*) and
*arm-none-eabi bleeding-edge-toolchain* (*GCC* version 5 or later). Check `README.md` for more details about usage.
- Numerous changes to SPI-based devices and SPI APIs:
  - Improved performance of interrupt-based *STM32's* *SPIv1* and *SPIv2* drivers.
  - Replaced `distortos::devices::SpiMasterOperation` and `distortos::devices::SpiMasterOperationRange` with
  `distortos::devices::SpiMasterTransfer` and `distortos::devices::SpiMasterTransfersRange` respectively. Renamed
  *STM32's* *SPIv1* and *SPIv2* `distortos::chip::ChipSpiMasterLowLevel` classes to
  `distortos::chip::SpiMasterLowLevelInterruptBased` to make it consistent with newly added
  `distortos::chip::SpiMasterLowLevelDmaBased`. Aliases for old names were added, marked as deprecated and are scheduled
  to be removed after v0.7.0.
  - Changed most of SPI-related APIs to a more *contract-based* approach. Most of error-checking was replaced with
  assertions, thus affected functions return less error codes or don't return anything.
  - Added `distortos::devices::SpiMasterHandle` and `distortos::devices::SpiDeviceSelectGuard`, which build new
  SPI-related API. These classes can be used for RAII-style locking/unlocking or selecting/deselecting of appropriate
  devices and also serve as handles for accessing core functionalities of associated objects.
  - `distortos::devices::SpiMasterBase` object is now bound to `distortos::devices::SpiMasterLowLevel` in
  `distortos::devices::SpiMasterLowLevel::startTransfer()` instead of `distortos::devices::SpiMasterLowLevel::start()`.
  - `distortos::devices::SpiMasterLowLevel::configure()` allows configuration of dummy data that will be sent if write
  buffer of transfer is `nullptr`.
  - `...::lock()` and `...::unlock()` functions in `distortos::devices::SpiEeprom` were changed to use recursive mutexes
  internally and thus take no arguments.
  - `distortos::devices::SpiEeprom` implements `distortos::devices::BlockDevice` interface. This changes return type of
  `distortos::devices::SpiEeprom::read()` and `distortos::devices::SpiEeprom::write()` from `std::pair<int, size_t>` to
  just `int`.
  - Simplified SPI drivers and interfaces: removed handling of errors which are not possible with current configuration
  and removed critical sections or bit-banding use where it makes no difference.
  - `distortos::chip::ChipSpiMasterLowLevel::Parameters` class was moved to separate header and renamed to
  `distortos::chip::SpiPeripheral`. Removed `distortos::chip::ChipSpiMasterLowLevel::spi...Parameters` static objects
  and replaced them with local objects generated for each board. This change requires the board to be regenerated.
  - Replaced `spis` and `uarts` modules with files generated for each board. This change requires the board to be
  regenerated.
  - Removed `distortos::devices::SpiEeprom::getCapacity()` and
  `distortos::devices::SpiEeprom::waitWhileWriteInProgress()`. Use functions inherited from
  `distortos::devices::BlockDevice` interface class - `distortos::devices::SpiEeprom::getSize()` and
  `distortos::devices::SpiEeprom::synchronize()`.
  - Removed `distortos::devices::SpiEeprom::getPageSize()` and `distortos::devices::SpiEeprom::isWriteInProgress()`.
  - Removed `distortos::devices::SpiDevice` class and whole public API of `distortos::devices::SpiMaster` class. Use
  functionality exposed by `distortos::devices::SpiMasterHandle` and `distortos::devices::SpiDeviceSelectGuard` classes.
  - Removed `distortos::devices::SpiMasterTransfer::getBytesTransfered()` and
  `distortos::devices::SpiMasterTransfer::finalize()`.
- All additional arguments of *CMake* functions `distortosBin()`, `distortosDmp()`, `distortosHex()`, `distortosLss()`
and `distortosSize()` are passed to the appropriate commands (`${CMAKE_OBJCOPY}`, `${CMAKE_OBJDUMP}` or
`${CMAKE_SIZE}`). This can be especially useful in case of binary files which are used to calculate firmware checksums,
where it may be necessary to pass flags like `--gap-fill 0xff`.
- `generateBoard.py` now requires only one argument - the input `*.yaml` file. Arguments with output path and
*distortos* path are both optional. Default output path is the folder of input `*.yaml` file. Default *distortos* path
is calculated as a relative path from current directory to the folder above the script.
- `generateChipYaml.py` now requires only one argument - the input `*.csv` file. Second argument - output path - is
optional and default value is `chipYaml/` in the folder of input `*.csv` file.
- Replaced `ARMv6-M-ARMv7-M-coreVectors.cpp` and `...-chipVectors.cpp` files with `...-vectorTable.cpp` generated for
each board. Data used to generate vector table is taken from chip *YAML* files, which now contain information about
*NVIC* and implemented vectors. This change requires the board to be regenerated and this requirement is enforced by
*CMake*. Additionally `distortos_Memory_regions_..._text_vectors` *CMake* configuration option was renamed to
`distortos_Memory_regions_..._text_vectorTable`, which is related to the rename of `.text.vectors` linker section to
`.text.vectorTable`.
- Changed names of some interrupt vectors of *STM32F0*, *STM32F1*, *STM32L0* and *STM32L4* to be consistent with
`..._IRQn` names of `IRQn_Type` enum.
- Renamed all configuration `#define`s in `distortosConfiguration.h` from `CONFIG_...` to `DISTORTOS_...`. Some of them
were also shortened and simplified by removing redundant parts like architecture name
(e.g. `CONFIG_ARCHITECTURE_ARMV6_M_ARMV7_M_MAIN_STACK_SIZE` -> `DISTORTOS_ARCHITECTURE_MAIN_STACK_SIZE`), chip family
(e.g. `CONFIG_CHIP_STM32F0_RCC_HSE_FREQUENCY` -> `DISTORTOS_CHIP_RCC_HSE_FREQUENCY`) or peripheral version
(e.g. `CONFIG_CHIP_STM32_SDMMCV1_SDMMC1_DMA` -> `DISTORTOS_CHIP_SDMMC1_DMA`). This change requires the board to be
regenerated.
- Update *CMSIS* to version 5.5.1.
- Update *CMSIS-STM32F0* to version 1.9.0.
- Update *CMSIS-STM32F1* to version 1.7.0.
- Update *CMSIS-STM32F4* to version 1.24.0.
- Update *CMSIS-STM32F7* to version 1.15.0.
- Update *CMSIS-STM32L0* to version 1.11.0.
- Update *CMSIS-STM32L4* to version 1.14.0.

### Fixed

- Fixed `estd::ContiguousRange` to allow construction of `estd::ContiguousRange<const T>` from
`const std::array<const T, N>&`.
- Fixed `generateBoard.py` for *ruamel.yaml* 0.15.52 or later.
- Fixed frequent overrun errors appearing in *STM32's* *SPIv1* and *SPIv2* drivers.
- Fix critical linker script bug which affects *STM32F7* chips. If program contained and object with big alignment in
`.text` section, *LMA* and *VMA* were aligned differently. This resulted in undefined behaviour right from the start,
usually causing a *Hard Fault* exception within several cycles.

### Removed

- Removed support for configuring with *Kconfig* and building with *make*. Both of these tasks are now handled by
*CMake*.
- Removed some nonexistent or problematic interrupt vectors for some of *STM32F1* chips. Only *high density*,
*XL density* and *connectivity line* *STM32F1* chips have *DMA2*. Some of them map *DMA2 channel 5* interrupt to either
shared interrupt vector (with *DMA2 channel 4*) or to a separate interrupt vector, but enabling the latter has multiple
unrelated side effects, so use the former one only.
- Removed deprecated files and aliases: `lowLevelInitialization.hpp`, `StaticRawFifoQueue2` and
`StaticRawMessageQueue2`.

[0.6.0](https://github.com/DISTORTEC/distortos/compare/v0.5.0...v0.6.0) - 2018-07-01
------------------------------------------------------------------------------------

### Added

- Support for all 81 [STM32L4](http://www.st.com/stm32l4) devices.
- Support and test configurations for [NUCLEO-L432KC](http://www.st.com/en/evaluation-tools/nucleo-l432kc.html) and
[NUCLEO-L476RG](http://www.st.com/en/evaluation-tools/nucleo-l476rg.html) boards with *STM32L4* chips.
- Support and test configuration for [NUCLEO-F446RE](http://www.st.com/en/evaluation-tools/nucleo-f446re.html) board
with *STM32F4* chip.
- `distortos/C-API/ConditionVariable.h` - C-API for condition variables, implemented as wrappers for
`distortos::ConditionVariable` member functions.
- `distortos/C-API/Mutex.h` - C-API for mutexes, implemented as wrappers for `distortos::Mutex` member functions.
- `distortos/C-API/Semaphore.h` - C-API for semaphores, implemented as wrappers for `distortos::Semaphore` member
functions.
- `distortos::fromCApi()` which can be used to cast references to C-API objects (like `distortos_Semaphore`) into
references to regular C++ API objects (like `distortos::Semaphore`).
- `distortos::DynamicSoftwareTimer` class - a dynamic and (mostly) non-templated counterpart of
`distortos::StaticSoftwareTimer`.
- `distortos::ThisThread::exit()`, similar to `pthread_exit()`, which can be used to cause early exit of the current
thread.
- `distortos::ThreadIdentifier` type which can be used to uniquely identify and access thread's instance. Identifier of
the thread can be obtained using `distortos::Thread::getIdentifier()` and `distortos::ThisThread::getIdentifier()`.
- Unit tests of C-API for condition variables, mutexes and semaphores, using
[Catch](https://github.com/philsquared/Catch) unit test framework and
[Trompeloeil](https://github.com/rollbear/trompeloeil) mocking framework. Build system of unit tests uses
[CMake](https://cmake.org/).
- New overload of `distortos::Mutex`'s constructor for "normal" type.
- `BIND_LOW_LEVEL_PREINITIALIZER()` and `BIND_LOW_LEVEL_INITIALIZER()` macros, which can be used to bind (at
compile/link time) any function as low-level preinitializer (executed before *.bss* and *.data* sections'
initialization, before constructors for global and static objects) or low-level initializer (executed after *.bss* and
*.data* sections have been initialized, but before constructors for global and static objects). Each
preinitializer/initializer has its own order of execution from 0 to 99.
- `include/CONCATENATE.h` and `include/STRINGIFY.h` with useful macros for token concatenation and stringification.
- *CMake*-based build system. At this moment all configuration is still done with *Kconfig* - *CMake* loads selected
`distortosConfiguration.mk` and only deals with compilation. Typical use case involves following steps: select
configuration with `make configure CONFIG_PATH=...`, create output folder of your choice (`mkdir output`) and enter it
(`cd output`), configure compilation with `cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/Toolchain-arm-none-eabi.cmake` and
finally start the build with `make`. You can obviously use other
[*CMake* generators](https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html), e.g. *Ninja*, *Eclipse CDT4*
project, ... *CMake*-based build system will only support configurations with proper board, i.e. "Custom board" choice
for "Board" in *Kconfig* is not supported.
- Added `rbegin()`, `rend()`, `crbegin()` and `crend()` functions to `IntrusiveList` and `SortedIntrusiveList` classes,
making them usable with `estd::ReverseAdaptor`.

### Changed

- Reduced size of `distortos::Mutex` from 28 bytes to 24 bytes (5 pointers + 4 bytes).
- `distortos::ThreadCommon` was moved to `distortos::internal` namespace. `distortos/ThreadCommon.hpp` was moved to
`distortos/internal/scheduler/ThreadCommon.hpp`. There's no need for this class to be available in the public API.
- All low-level initializers (architecture, chip, peripherals, scheduler, threads, ...) use
`BIND_LOW_LEVEL_INITIALIZER()` instead of being called via `distortosPreinitArray[]` from newlib's
`__libc_init_array()`.
- Changed linker script symbols related to *.bss* and *.data* section initializers - `__{bss,data}_array_{end,start}`
becomes `__{bss,data}_initializers_{end,start}`.
- Replaced `generateChipDtsi.py` with `generateChipYaml.py`. New script reads all input data and its hierarchy from
`*.csv` files - no templates or external files are used. Output `*.yaml` files for described chips are generated by
*ruamel.yaml* parser/emitter module, which is the only dependency of this script.
- Replaced *devicetree*-based board generator with one using `*.yaml` files as input. Model of data in `*.yaml` files
resembles devicetree, but is more suited for requirements of *distortos* (static code generation instead of dynamic run
time configuration). New version of `generateBoard.py` requires *ruamel.yaml* parser/emitter module and *jinja2*
template engine. It is also possible to use chip `*.yaml` files directly to generate so-called "raw" board, which can
then be used by CMake. Basic documentation of bindings for chip and board `*.yaml` files is included in
`documentation/yaml-bindings/`.
- Indexes of board buttons and LEDs - generated automatically by `generateBoard.py` - were changed from
`<name>ButtonIndex` and `<name>LedIndex` to `<group><Name>Index`. For example `b1ButtonIndex` and `ld3LedIndex` were
changed to `buttonsB1Index` and `ledsLd3Index`. Similar change was done for counts of available board buttons and LEDs -
they were changed from `total<Name>` to `<group>Count`. For example `totalButtons` and `totalLeds` were changed to
`buttonsCount` and `ledsCount`.
- Renamed `StaticRawFifoQueue2` to `StaticRawFifoQueue` and `StaticRawMessageQueue2` to `StaticRawMessageQueue`. Aliases
for old names were added and marked as deprecated and are scheduled to be removed after v0.6.0
- Update *CMSIS* to version 5.3.0.

### Fixed

- Fixed *GDB* pretty-printers of *distortos* queues when using *GCC 7*. New version of *libstdc++v3* from *GCC* changed
implementation of `std::unique_ptr`, which is used internally by queues for managing storage. Fixed pretty-printers from
`distortos.py` now handle both variants of `std::unique_ptr`.
- Mark all `.bss` sections in generated linker scripts as `(NOLOAD)`. Without this change `.bin` file for a project
containing zero-initialized data in additional memories would be extremely large - for example ~134 MB in case of
*STM32F4* when anything is placed in `.ccm.bss`.
- Fixed interrupt priorities for *STM32's* *SPIv2* and *USARTv2* for cases when
`CONFIG_ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI` is defined and non-zero. Previously interrupt priorities of these
peripherals were always set to 0 (highest possible).
- Restore 4-byte alignment for `.bss` and `.data` sections in linker scripts generated for *ARMv6-M* and *ARMv7-M*
architectures. Previously when beginning/end of these sections were not 4-byte aligned (for example there's just one
byte in such section), the `__..._start` / `__..._end` symbols would not be properly aligned for use by
`Reset_Handler()`'s initialization code, which initializes 4 bytes at a time.
- `#include ...` examples in documentation generated by *doxygen* use proper paths - e.g.
`#include "distortos/Semaphore.hpp"` instead of `#include "Semaphore.hpp"`.
- Fixes necessary for compilation with *GCC 8*.

### Removed

- Removed call to `lowLevelInitialization0()` from `Reset_Handler()` for *ARMv6-M* and *ARMv7-M*. All low-level
preinitializers should use `BIND_LOW_LEVEL_PREINITIALIZER()` macro.
- Removed `board::lowLevelInitialization()` declaration. Low-level initializers of board should use
`BIND_LOW_LEVEL_INITIALIZER(60, ...);`.
- Removed `pydts.py`, as new version of board generator uses `*.yaml` files as input.
- Removed *tup* build infrastructure - *tup* is no longer supported for building *distortos*.
- Removed deprecated functions and aliases: `StaticRawFifoQueueFromSize`, old `StaticRawFifoQueue`,
`StaticRawMessageQueueFromSize`, old `StaticRawMessageQueue`, old variant of STM32's GPIOv2
`configureAlternateFunctionPin()`.

[0.5.0](https://github.com/DISTORTEC/distortos/compare/v0.4.0...v0.5.0) - 2017-09-14
------------------------------------------------------------------------------------

### Added

- Support for whole [STM32L0](http://www.st.com/stm32l0) chip family.
- Support and test configuration for [NUCLEO-L073RZ](http://www.st.com/en/evaluation-tools/nucleo-l073rz.html) board
with *STM32L0* chip.
- Support and test configuration for [32F769IDISCOVERY](http://www.st.com/en/evaluation-tools/32f769idiscovery.html)
board with *STM32F7* chip.
- Test of `Thread::generateSignal()` and `Thread::queueSignal()` returning `ENOSPC` when the amount of target thread's
free stack is too small to request signal delivery.
- *GDB* pretty-printers for `estd` containers (`estd::ContiguousRange`, `estd::IntrusiveForwardList`,
`estd::IntrusiveList`, `estd::SortedIntrusiveForwardList` and `estd::SortedIntrusiveList`), `internal::ThreadList` and
all variants of FIFO and message queues (`internal::FifoQueueBase`, `FifoQueue`, `DynamicFifoQueue`, `StaticFifoQueue`,
`RawFifoQueue`, `DynamicRawFifoQueue`, `StaticRawFifoQueue`, `internal::MessageQueueBase`, `MessageQueue`,
`DynamicMessageQueue`, `StaticMessageQueue`, `RawMessageQueue`, `DynamicRawMessageQueue` and `StaticRawMessageQueue`).
This greatly simplifies system debugging by allowing easy debugger access to thread lists (inside scheduler or
synchronization objects), software timer lists (inside scheduler) and by allowing live examination of queue contents.
- `ThisThread::getSchedulingPolicy()` and `ThisThread::setSchedulingPolicy()` - counterparts of
`Thread::getSchedulingPolicy()` and `Thread::setSchedulingPolicy()`.
- Parser of devicetree `*.dts` files written in *Python* with *ply* - `pydts.py`. The script can parse a `*.dts` file
into a Python dictionary. As this is a proof-of-concept for now, only limited set of devicetree syntax is supported.
Following features are currently not handled: path references, bytestrings, any integer arithmetic other than addition
and multiplication, labels for anything else than a node, multiple labels for single node, memory reservations, any
non-standard extensions, `/include/` statements (`#include` statements should be used exclusively), C or C++ comments
(should be handled by C preprocessor). The features which are currently handled are enough to parse all STM32 `*.dts`
files from *Linux* or from *Zephyr*. The script relies on *C preprocessor* to resolve `#define` macros, handle
`#include` statements and remove C/C++ comments. Before you feed the file into this script, you should first do
something like `cpp -nostdinc -undef -E -x assembler-with-cpp in.dts -o out.dts`. Linemarkers generated by current *cpp*
versions are supported.
- Support for cores which don't support bit-banding (*ARM Cortex-M0(+)*, *ARM Cortex-M1*, *ARM Cortex-M7*) or don't have
`BASEPRI` register (*ARMv6-M*) to *STM32's* *SPIv1* low-level driver.
- Devicetree fragments (`*.dtsi` files) for architectures.
- `*.csv` files which describe memory and selected peripherals of all supported chips.
- `generateChipDtsi.py` *Python* script (using *Jinja2* template engine) which can generate chip's devicetree fragments
(`*.dtsi` files) from `*.csv` input file.
- Basic devicetree `*.dts` files for all supported boards.
- Board generator *Python* script using `.dts` files as input (parsed with `pydts.py` module) and *Jinja2* template
engine for rendering output. This new feature can be used with `make board CONFIG_FILE=path/to/config.dts` command.
- "Enable support for signals" option in *Kconfig* menus. When this option is enabled, all namespaces, functions and
classes required for signals are available. Otherwise they are completely disabled, which reduces the size of compiled
application. This affects `ThisThread::Signals` namespace, `Thread::generateSignal()`, `Thread::getPendingSignalSet()`,
`Thread::queueSignal()` and `DynamicSignalsReceiver`, `SignalInformationQueueWrapper`, `SignalsCatcher`,
`SignalsReceiver`, `StaticSignalsReceiver` classes.
- Experimental "Enable link-time optimization" option in *Kconfig* menus.
- "Enable static destructors" and "Register static destructors in run time" options in *Kconfig* menus. These can be
used to completely disable destructors for objects with static storage duration, which reduces ROM usage of application.
By default both options are disabled, as this is the most common choice for an embedded application which never exits.
- Support for newlib's ratargetable locking, which makes all newlib functions with shared state - like the ones from
`<stdio.h>`, functions which manipulate time zone, modify environment variables and so on - thread-safe. "Retargetable
locking" has to be enabled during toolchain compilation - toolchains compiled in March 2017 or later should support that
feature.

### Changed

- `architecture::requestFunctionExecution()` no longer handles the case of current thread sending the request to
itself.
- Size of stack passed as argument to `DynamicThread` or as template argument to `StaticThread` is adjusted to alignment
requirements of architecture. This way "usable" size of stack will never be less than the requested value.
- Generated `distortosConfiguration.h` file no longer "undefines" *Kconfig* symbols which are not set.
- All boolean *Kconfig* symbols are now converted to `#define CONFIG_... 1` in generated `distortosConfiguration.h`
file. Previously they were converted to a macro without value (`#define CONFIG_...`), which made building complex
conditions harder than necessary.
- All supported boards were regenerated from `.dts` files.
- All folders in `source/board/STM32/STM32*/` and in `configurations/` were renamed to be in sync with filenames of
their generated contents.
- `chip::configurePin()`, `chip::configureInputPin()` and `chip::configureOutputPin()` for STM32's GPIOv1 now have
default values for all the arguments which are less important.
- `chip::configurePin()`, `chip::configureAlternateFunctionPin()`, `chip::configureInputPin()` and
`chip::configureOutputPin()` for STM32's GPIOv2 now have default values for all the arguments which are less important.
- Generated linker scripts (either from `.dts` file by `make board` or during compilation when using custom board) are
more similar to GNU's default linker script.
- Type of `TickClock` counter - `TickClock::rep` was changed from `uint64_t` to `int64_t`. This way
`TickClock::duration` can be negative and `TickClock::time_point` may represent time points before bootup.
- Update *CMSIS* to version 5.1.0.
- Update *CMSIS-STM32F0* to version 1.8.0.
- Update *CMSIS-STM32F1* to version 1.6.0.
- Update *CMSIS-STM32F4* to version 1.16.0.

### Deprecated

- `StaticRawFifoQueue<T, QueueSize>`, `StaticRawFifoQueueFromSize<ElementSize, QueueSize>`,
`StaticRawMessageQueue<T, QueueSize>` and `StaticRawMessageQueueFromSize<ElementSize, QueueSize>` were marked as
deprecated and are scheduled to be removed after v0.5.0. The classes are deprecated because they don't match their
"dynamic" counterparts (which use size in bytes, not type) and because
`StaticRaw...QueueFromSize<ElementSize, QueueSize>` aliases work only for `ElementSize` which is a positive power of 2.
New versions of these classes which were added - `StaticRawFifoQueue2<ElementSize, QueueSize>` and
`StaticRawMessageQueue2<ElementSize, QueueSize>` - don't have these flaws. To upgrade your code replace
`StaticRaw...Queue<T, QueueSize>` with `StaticRaw...Queue2<sizeof(T), QueueSize>` and
`StaticRaw...QueueFromSize<ElementSize, QueueSize>` with `StaticRaw...Queue2<ElementSize, QueueSize>`.
- `chip::configureAlternateFunctionPin(chip::Pin, bool, chip::PinOutputSpeed, chip::PinPull, chip::PinAlternateFunction)`
for STM32's GPIOv2 was marked as deprecated and is scheduled to be removed after v0.5.0. This function is deprecated
because one of the most important arguments - `chip::PinAlternateFunction` - is the last one, which makes it impossible
to have default values for the less important arguments. New overload
`chip::configureAlternateFunctionPin(chip::Pin, chip::PinAlternateFunction, bool, chip::PinOutputSpeed, chip::PinPull)`
has `chip::PinAlternateFunction` passed on the second position, not the last one, thus allowing to have a default value
for all the arguments that follow.

### Fixed

- `ThisThread::Signals::setSignalMask()` may never return `ENOSPC`, as it delivers signals directly, not via
`architecture::requestFunctionExecution()`.
- Failure during generation or queuing of signal - for example due to free stack size being insufficient, which is
reported with `ENOSPC` error code - won't leave the generated/queued signal pending.
- Signals are never delived from within critical section.
- Fixed compilation of *USARTv2* low-level driver for some *STM32F0* chips, for which `USART_CR1_M0` is not defined.
- Minor fixes necessary for compilation and proper behaviour in *GCC 7*.
- Fixes required for compilation and proper execution when link-time optimization is enabled.
- Fix *tup*-based build when `Tupfile.lua` from folder *A* is used to compile files from folder *B*. Previously proper
folder hierarchy would not be created in the output directory.
- Fix stack overflow in idle thread for *ARMv6-M* when `Thread::detach()` is enabled, "Optimize for size (-Os)" is
selected and *GCC 7* is used. In that case idle thread needs 264 bytes of stack, so increase its size to 320 bytes.

### Removed

- All support for *GCC 4.x* was removed, *GCC 5.1* is the minimum supported version. *GCC 5.1* - released more than 2
years ago - is the first version to fully support `constexpr` constructors. Dropping support for earlier versions
finally allows global objects' initialization to be greatly simplified. This project aims to support 3 most recent
"major" versions of *GCC*, which - currently - means *GCC 5*, *GCC 6* and *GCC 7*.

[0.4.0](https://github.com/DISTORTEC/distortos/compare/v0.3.0...v0.4.0) - 2017-03-11
------------------------------------------------------------------------------------

### Added

- Support for all 78 [STM32F7](http://www.st.com/stm32f7) chips.
- Support and test configuration for [32F746GDISCOVERY](http://www.st.com/en/evaluation-tools/32f746gdiscovery.html)
board with *STM32F7* chip.
- Set `STKALIGN` bit in `SCB->CCR` for *ARM Cortex-M3 r1p1* (like *STM32F1*). Thanks to that, stack will be
automatically aligned to 8-bytes on exception entry, which is required by *AAPCS*. This bit is set by default on all
other *ARM Cortex-M* cores.
- `FATAL_ERROR()` macro with weak `fatalErrorHook()`.
- "Check context of functions" option in *Kconfig* menus. When this option is selected, the context of functions which
must not be used from interrupt context (all blocking functions, all `Mutex` functions and all functions from
`ThisThread` namespace) is checked during run-time. If a violation is detected, `FATAL_ERROR()` is called with
appropriate message. See help of the new option for more info.
- "Check stack pointer range during context switch" option in *Kconfig* menus. Selecting this option enables simple
range checking of preempted thread's stack pointer during context switches. Such check is relatively fast, but cannot
detect all stack overflows. `FATAL_ERROR()` is called with appropriate message when an overflow is detected. Check new
option's help for more info.
- "Check stack guard contents during context switch" option in *Kconfig* menus. When enabled, this option extends stacks
for all threads (including `main()` thread) with a "stack guard" at the overflow end. This "stack guard" - just as the
whole stack - is filled with a sentinel value during thread initialization. During each context switch contents of
preempted thread's "stack guard" are checked - if any byte has changed, FATAL_ERROR() will be called with appropriate
message. This method is able to detect stack overflows much more reliably than simple stack pointer range checking and
is still sufficiently fast, assuming that the size of "stack guard" is reasonable. For more info, check new option's
help.
- "Check stack pointer range during system tick" and "Check stack guard contents during system tick" options in
*Kconfig* menus, which are similar to "Check stack pointer range during context switch" and "Check stack guard contents
during context switch" respectively, but executed during every system tick.
- `Thread::getStackSize()` and `ThisThread::getStackSize()` which can be used to get thread's stack size.
- `Thread::getStackHighWaterMark()` and `ThisThread::getStackHighWaterMark()` which can be used to get "high water mark"
(max usage) of thread's stack.
- Test of thread's `start()` returning `ENOSPC` when stack is too small.
- `STM32-bit-banding.h` header with `STM32_BITBAND_ADDRESS()` and `STM32_BITBAND()` macros, which are more suited for
STM32's CMSIS headers - it's enough to write `STM32_BITBAND(RCC, CR, PLLON)` instead of
`BITBAND(&RCC->CR, RCC_CR_PLLON_bit)`. Note that these new macros must not be used with `FLASH` registers - use
`STM32_BITBAND_FLASH()` and `STM32_BITBAND_FLASH_ADDRESS()` for that peripheral.
- Options to enable STM32's GPIO to *Kconfig* menu.
- New chips: 10 *STM32F413* chips and 5 *STM32F423* chips.
- `README.md` files for all supported boards.

### Changed

- Changed placement of fixed stacks for *ARMv6-M* and *ARMv7-M* in generated linker script. Stack for interrupts ("main"
stack) is located at the beginning of RAM, so any stack overflow during interrupt handling will cause a HardFault
exception. Stack for `main()` thread ("process" stack) is placed at the end or RAM, after heap, which introduces a
potential safety margin for any stack overflows in this thread - as long as heap's last block is not allocated, the
memory below this stack is not used.
- Stacks are filled with 0xed419f25 instead of 0. Using 0 is not a reliable method to detect stack usage/overflow, as 0
is very likely to be used in the application (for example to zero-initialize variables).
- Reduced default size of stack for interrupts to 1 kB.
- Reduced size of stack for interrupts to 1 kB in all test configurations.
- Reduced size of stack for idle thread to 256 bytes when support for thread detachment is enabled.
- `architecture::requestFunctionExecution()` checks for amount of free stack before doing any stack modifications. If
there's not enough free stack available, it returns an `ENOSPC` error code. Modify all callers of this function (which
includes functions to generate/queue signals and set signal mask) to handle this error code.
- Changed number of threads, software timers, queued signals and signal actions used by test application to 8 (was 10)
to reduce its RAM requirements. This fixes a crash of test application for *NUCLEO-F103RB* board caused by insufficient
memory available for `_sbrk_r()` and crashes of other configurations at lower optimization levels due to stack
overflow.
- Reimplemented `callOnce()` with a mutex. This increases the size of `OnceFlag` object (32 bytes vs 8 bytes), but at
the same time reduces stack requirements of any thread using `callOnce()` function. Removed
`ThreadState::blockedOnOnceFlag` enum value.
- Cleaned up `architecture` namespace, leaving only architecture-specific elements. Moved `Stack`,
`InterruptMaskingUnmaskingLock` and `InterruptUnmaskingLock` to `internal` namespace. Moved `InterruptMaskingLock` to
`distortos` namespace.
- Removed virtual `Thread::start()` and convert overrides available via `DynamicThread` and `StaticThread` to
non-virtual functions.
- `architecture::initializeStack()` checks for buffer overflow before actually doing any memory operations. If stack
is too small for stack frame, `ENOSPC` error code is returned. Modify all call paths - starting at
`DynamicThread::start()` and `StaticThread::start()` - to handle this error.
- Moved `lowLevelInitialization0()` to the very beginning of reset handler.
- Renamed `BITBAND_PERIPH()` macro to `BITBAND_PERIPHERAL()`.
- Moved remaining *ARMv6-M* and *ARMv7-M* assembly functions (`Reset_Handler()` and `SVC_Handler`) to C++ source files.
- `ARMv6-M-ARMv7-M-Reset_Handler.cpp` no longer requires `__USES_CXX` and `__USES_TWO_STACKS` to be defined in
compilation flags.
- Moved enabling of RCC clocks for STM32's GPIO from `board::lowLevelInitialization()` to
`chip::lowLevelInitialization()`.
- Buttons and LEDs for boards depend on enabling the GPIO port to which they are connected. "Enable buttons" and "Enable
LEDs" options in *Kconfig* menu are available only if at least one GPIO port of buttons / LEDs is enabled. Preprocessor
macros with total number of buttons and LEDs which were previously provided by *Kconfig* - `CONFIG_BOARD_TOTAL_BUTTONS`
and `CONFIG_BOARD_TOTAL_LEDS` - were renamed to `DISTORTOS_BOARD_TOTAL_BUTTONS` and `DISTORTOS_BOARD_TOTAL_LEDS`. These
macros are now generated automatically by `buttons.hpp` and `leds.hpp` respectively.
- Update *CMSIS-STM32F0* to version 1.7.0.
- Update *CMSIS-STM32F4* to version 1.14.0.

### Fixed

- Added `ChipSpiMasterLowLevel` and `ChipUartLowLevel` classes to "devices" group in *API* reference generated by
*doxygen*.
- Compiler's built-in defines are passed automatically to *doxygen*, which fixes some missing documentation entries (for
example bit-banding macros).
- Fixed failures in several test cases (`ThreadSleepForTestCase`, `SignalsInterruptionTestCase` and
`CallOnceOperationsTestCase`) that occurred only with low core frequency due to very strict timing requirements.
- Fixed stack overflow in `CallOnceOperationsTestCase` which occurred only on *ARM Cortex-M0* cores with `-Og`
optimization level.
- Maximum values of APB1 and APB2 frequencies for *STM32F4* take into account whether over-drive is enabled or not.

### Removed

- `lowLevelInitialization1()` - which was executed right before calling `main()` - from reset handler.
- All `...-bits.h` headers files, which are superseded by recent versions of CMSIS headers.
- Configuration of chip package from *Kconfig* menu.

[0.3.0](https://github.com/DISTORTEC/distortos/compare/v0.2.0...v0.3.0) - 2016-11-24
------------------------------------------------------------------------------------

### Added

- Support for periodic execution of software timers. All overloads of `SoftwareTimer::start()` were extended with
optional second argument with period. If the period is 0 (which is the default value), software timer's function is
executed only once. Otherwise it will be executed periodically until the software timer is stopped.
- `devices::UartBase` interface class, which can be used as a private base for devices using UART interface.
- `devices::UartLowLevel` interface class, which is a low-level interface to hardware-dependent UART driver.
- `chip::ChipUartLowLevel` class - interrupt-driven implementation of `devices::UartLowLevel` interface for *STM32F0*,
*STM32F1* and *STM32F4*.
- `devices::SpiMasterBase` interface class, which can be used as a private base for devices using SPI as bus master.
- `devices::SpiMasterLowLevel` interface class, which is a low-level interface to hardware-dependent SPI bus master
driver.
- `chip::ChipSpiMasterLowLevel` class - interrupt-driven implementation of `devices::SpiMasterLowLevel` interface for
*STM32F0*, *STM32F1* and *STM32F4*.
- "Peripherals configuration" *Kconfig* menu, where low-level drivers for SPI bus master and U[S]ART can
be selected.
- `uarts.hpp` header for *STM32F0*, *STM32F1* and *STM32F4* with declarations of all enabled low-level U[S]ART drivers.
- `spis.hpp` header for *STM32F0*, *STM32F1* and *STM32F4* with declarations of all enabled low-level SPI master
drivers.
- `devices::SerialPort` class - universal serial port device with an interface similar to standard files (`open()`,
`close()`, `read()`, `write()`). `read()` and `write()` member functions of this class support both blocking (with
or without timeout) and non-blocking behaviour. Convenient wrappers for reading/writing with timeout are also provided:
`tryReadFor()`, `tryReadUntil()`, `tryWriteFor()` and `tryWriteUntil()`.
- `estd::ScopeGuard` template class, which can be used to execute bound function on scope exit using RAII pattern.
- `devices::Rs485` class - *RS-485* device with all features of `devices::SerialPort` and with automatic management
of "driver enable" output pin.
- `devices::SpiMaster` and `devices::SpiDevice` classes, which enable easy communication with multiple SPI slave devices
connected to the same SPI bus master.
- `devices::SpiEeprom` class which is a driver for common SPI EEPROM chips: *Atmel AT25xxx*,
*ON Semiconductor CAT25xxx*, *ST M95xxx*, *Microchip 25xxxxx* or similar.
- Support for `assert()` - override of newlib's `__assert_func()`, weak `assertHook()` for application and option to
enable/disable assertions in *Kconfig* menus.
- Extend `estd::ContiguousRange` with support for `std::array`, constructor for non-const -> const conversions and
various useful functions: `cbegin()`, `cend()`, `rbegin()`, `rend()`, `crbegin()`, `crend()`.
- Option in *Kconfig* menus for manual configuration of ROM's offset and size in generated linker script. This can be
useful when the device has a bootloader at the beginning of flash, when the application is a bootloader and/or when the
application uses a few last ROM pages for its own purposes (e.g. emulated EEPROM, storing configuration, logging, ...).
- `architecture::isInInterruptContext()` which checks whether thread or interrupt context is currently active. An
example use is when the application needs to decide between blocking or non-blocking behaviour, as blocking is not
possible in interrupt context.
- `protected` `ChipInputPin::getInvertedMode()`, `ChipInputPin::setInvertedMode()`, `ChipOutputPin::getInvertedMode()`
and `ChipOutputPin::setInvertedMode()` functions, which - if needed - can be made `public` by deriving from these
classes.
- Missing tests of timers: basic operations of periodic timers, synchronous and asynchronous stops and restarts, reading
of state in timer's function.
- New chips: *STM32F071C8*, *STM32F101C4*, 8 *STM32F412* chips, 3 *STM32F469Vx* chips, 3 *STM32F469Zx* chips, 2
*STM32F479Vx* chips and 2 *STM32F479Zx* chips.
- New package types: *UFQFPN48* for *STM32F101C8*, *LQFP48* for *STM32F410CB*, *UFBGA64* for *STM32F410RB*, *LQFP176*
for *STM32F469IG*.

### Changed

- Replace generator of `distortosConfiguration.h` that uses *AWK* (`makeDistortosConfiguration.awk`) with the one using
shell, *cat* and *sed* (`makeDistortosConfiguration.sh`). With this change *AWK* is no longer needed to configure &
build this project.
- Merge GPIO drivers for *STM32*.
- Update *CMSIS-STM32F0* to version 1.6.0.
- Update *CMSIS-STM32F1* to version 1.4.0.
- Update *CMSIS-STM32F4* to version 1.13.0.

### Fixed

- Fix generated linker scripts for older versions of *ld* (like 2.24.0.20141128), where hidden symbols cannot be used in
expressions.
- Fix failure of test application for new versions of "official" ARM toolchain (5.x).
- Fixes necessary for compilation and proper behaviour in *GCC 6.2.0* - mainly related to `reinterpret_cast<>()` in
constant expressions.
- *ST's* website no longer lists *WLCSP49* as package type for *STM32F401CB*.

[0.2.0](https://github.com/DISTORTEC/distortos/compare/v0.1.0...v0.2.0) - 2016-05-03
------------------------------------------------------------------------------------

### Added

- Support for *ARMv6-M* (*ARM Cortex-M0*, *ARM Cortex-M0+* and *ARM Cortex-M1*) architecture.
- Support for all 72 [STM32F0](http://www.st.com/stm32f0) chips.
- Support for all 94 [STM32F1](http://www.st.com/stm32f1) chips.
- Support and test configurations for [NUCLEO-F091RC](http://www.st.com/web/catalog/tools/PF260944) board with *STM32F0*
chip, [NUCLEO-F103RB](http://www.st.com/nucleoF103RB-pr) board with *STM32F1* chip and
[NUCLEO-F429ZI](http://www.st.com/web/catalog/tools/PF262637) board with *STM32F4* chip.
- `make oldconfig` target, which can be used to update currently selected configuration asking about new options.
- `make olddefconfig` target, which can be used to update currently selected configuration with default values of new
options.
- `forAllConfigurations.sh` script, which can be used to run any command - passed in first argument - for all
configurations found in provided (optional) search path - passed in second argument, default - current path ("."). One
use case is updating all configurations - `./scripts/forAllConfigurations.sh "make oldconfig"` or
`./scripts/forAllConfigurations.sh "make olddefconfig"`.
- Ability to delete some folders - if you don't use their contents - without breaking *make*/*tup* build and *Kconfig*
menus. You can obviously delete anything from `configurations/`, including the entire folder. The same is true for
`documentation/` directory if you don't plan to generate *API* reference with *doxygen*. In case of `external/`,
`source/architecture/`, `source/board/` and `source/chip/` you can delete any number of complete components or group of
components - e.g. you can delete `source/board/STM32/STM32F4/STM32F4DISCOVERY/` (port for single board) and/or
`source/board/STM32/STM32F4/` (group of ports for boards), but you shouldn't delete just
`source/board/STM32/STM32F4/STM32F4DISCOVERY/include/` (part of port for single board). Additionally you can delete
entire `source/board/` and/or `test/` folders.
- Ability to add configurations of custom applications to *Kconfig* menus. User can add any number of files named
`Kconfig-applicationOptions` anywhere in the project, and they will all be automatically included in the
"Applications configuration" menu. The same is possible for custom boards, chips and architectures, but these are
slightly more complicated, as multiple files are needed.
- Configuration of *SRAM1*, *SRAM2* and *SRAM3* regions' unification for *STM32F4* chips.
- Support for additional memory regions of *STM32F4* chips (*BKPSRAM*, *CCM*, *SRAM2*, *SRAM3*) in generated linker
scripts.
- Ability to use *ARMv7-M*'s *PRIMASK* to disable all interrupts during critical sections when "Interrupt priority
disabled in critical sections" (*ARCHITECTURE_ARMV7_M_KERNEL_BASEPRI*) options is set to 0. This is the new default
setting, as this is a more conservative (and thus safer for a beginner) approach, which doesn't require any special
configuration of interrupt priorities (default value is 0).
- Support for optional inversion to all implementations of `ChipInputPin` and `ChipOutputPin`.
- `buttons.hpp` header with board buttons for all supported boards.
- `clocks.hpp` header with definition of clocks for selected chip.
- New package type for *STM32F411C* chips - the website lists *WLCSP49* variant now.

### Changed

- Change names of elements of all `enum class` from `ALL_CAPS` or `PascalCase` to `camelCase` for consistency with
their use as constants and with other enums.
- Replace individual linker script generators for *STM32F1* and *STM32F4* families with generic script for
*ARMv7-M*-based chips. The new generator also supports arbitrary number of additional memories (like *BKPSRAM*, *CCM*,
...), which can be defined with command line arguments.
- Reorganize and simplify *Kconfig* menus for *STM32F1* and *STM32F4*.
- Improve configuration and initialization of chip clocks for *STM32F1* and *STM32F4*.
- Move configuration of *SysTick* timer from `chip::lowLevelInitialization()` to `architecture::startScheduling()` for
all *ARMv6-M* and *ARMv7-M* targets.
- Change allowed range of *PLLN* for *STM32F446* chips from \[192; 432\] (first revision of the reference manual) to
\[50; 432\] (second revision of the reference manual).
- API reference generated by *doxygen* no longer includes code from disabled folders. This way documentation for
particular configuration has entries only for single selected chip & architecture, single selected board (if any) and
only for enabled applications - sources that are excluded from compilation are not parsed by *doxygen*.

### Fixed

- Fix insufficient dependencies for "PLLR" option in system clock source configuration for *STM32F4*. Main PLL's "/R"
output cannot be used as system clock if PLL is disabled.
- Fix invalid definition of templated variants of `Semaphore::tryWaitFor()` and `Semaphore::tryWaitUntil()`.
- Added missing *STM32F429NG* chip to *Kconfig* configuration menus.
- Fix *make* build for constructs like `CXXFLAGS_$(d)someFileName.cpp := ...` not working correctly in top-level folder
of the project (where `$(d)` is `./`).

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
- Support for [STM32F4](http://www.st.com/stm32f4) chips.
- Support and test configurations for 3 boards with *STM32F4* chips:
[32F429IDISCOVERY](http://www.st.com/web/catalog/tools/PF259090), [NUCLEO-F401RE](http://www.st.com/nucleoF401RE-pr) and
[STM32F4DISCOVERY](http://www.st.com/web/catalog/tools/PF252419).
- Extensive functional tests for all implemented functionalities.
- Configuration with mconf tool from [kconfig-frontends](http://ymorin.is-a-geek.org/projects/kconfig-frontends).
- `README.md` file with introduction and information about configuration & building.
- Integration with [Travis CI](https://travis-ci.org/DISTORTEC/distortos) service.
