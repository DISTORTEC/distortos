#
# \file
# \brief distortos configuration
#
# \warning
# Automatically generated file - do not edit!
#

if(DEFINED ENV{DISTORTOS_PATH})
	set(DISTORTOS_PATH "$ENV{DISTORTOS_PATH}")
else()
	set(DISTORTOS_PATH "../")
endif()

set("distortos_Build_00_Static_destructors"
		"OFF"
		CACHE
		"BOOL"
		"Enable static destructors.\n\nEnable destructors for objects with static storage duration. As embedded applications almost never \"exit\", these destructors are usually never executed, wasting ROM.")
set("distortos_Scheduler_00_Tick_frequency"
		"1000"
		CACHE
		"STRING"
		"System's tick frequency, Hz.\n\nAllowed range: [1; 2147483647]")
set("distortos_Scheduler_01_Round_robin_frequency"
		"10"
		CACHE
		"STRING"
		"Round-robin frequency, Hz.\n\nAllowed range: [1; 1000]")
set("distortos_Scheduler_02_Support_for_signals"
		"ON"
		CACHE
		"BOOL"
		"Enable support for signals.\n\nEnable namespaces, functions and classes related to signals:\n- ThisThread::Signals namespace;\n- Thread::generateSignal();\n- Thread::getPendingSignalSet();\n- Thread::queueSignal();\n- DynamicSignalsReceiver class;\n- SignalInformationQueueWrapper class;\n- SignalsCatcher class;\n- SignalsReceiver class;\n- StaticSignalsReceiver class;\n\nWhen this options is not selected, these namespaces, functions and classes are not available at all.")
set("distortos_Scheduler_03_Support_for_thread_detachment"
		"ON"
		CACHE
		"BOOL"
		"Enable support for thread detachment.\n\nEnable functions that \"detach\" dynamic threads:\n- ThisThread::detach();\n- Thread::detach();\n\nWhen this options is not selected, these functions are not available at all.\n\nWhen dynamic and detached thread terminates, it will be added to the global list of threads pending for deferred deletion. The thread will actually be deleted in idle thread, but only when two mutexes are successfully locked:\n- mutex that protects dynamic memory allocator;\n- mutex that synchronizes access to the list of threads pending for deferred deletion;")
set("distortos_Scheduler_04_Main_thread_stack_size"
		"4096"
		CACHE
		"STRING"
		"Size (in bytes) of stack used by thread with main() function.\n\nAllowed range: [1; 2147483647]")
set("distortos_Scheduler_05_Main_thread_priority"
		"127"
		CACHE
		"STRING"
		"Initial priority of main thread.\n\nAllowed range: [1; 255]")
set("distortos_Scheduler_06_Reception_of_signals_by_main_thread"
		"ON"
		CACHE
		"BOOL"
		"Enable reception of signals for main thread.")
set("distortos_Scheduler_07_Queued_signals_for_main_thread"
		"8"
		CACHE
		"STRING"
		"Maximal number of queued signals for main thread. 0 disables queuing of signals for main thread.\n\nAllowed range: [0; 2147483647]")
set("distortos_Scheduler_08_SignalAction_objects_for_main_thread"
		"8"
		CACHE
		"STRING"
		"Maximal number of different SignalAction objects for main thread. 0 disables catching of signals for main thread.\n\nAllowed range: [0; 32]")
set("distortos_Checks_00_Context_of_functions"
		"ON"
		CACHE
		"BOOL"
		"Check context of functions.\n\nSome functions may only be used from thread context, as using them from interrupt context results in undefined behaviour. There are several groups of functions to which this restriction applies (some functions fall into several categories at once):\n- all blocking functions, like callOnce(), FifoQueue::push(), Semaphore::wait(), ..., as an attempt to block current thread of execution (not to be confused with current thread) is not possible in interrupt context;\n- all mutex functions, as the concept of ownership by a thread - core feature of mutex - cannot be fulfilled in interrupt context;\n- all functions from ThisThread namespace (including ThisThread::Signals namespace), as in interrupt context they would access a random thread that happened to be executing at that particular moment;\n\nUsing such functions from interrupt context is a common bug in applications which can be easily introduced and very hard to find, as the symptoms may appear only under certain circumstances.\n\nSelecting this option enables context checks in all functions with such requirements. If any of them is used from interrupt context, FATAL_ERROR() will be called.")
set("distortos_Checks_01_Stack_pointer_range_during_context_switch"
		"ON"
		CACHE
		"BOOL"
		"Check stack pointer range during context switch.\n\nSimple range checking of preempted thread's stack pointer can be performed during context switches. It is relatively fast, but cannot detect all stack overflows. The check is done before the software stack frame is pushed on thread's stack, but the size of this pending stack frame is accounted for - the intent is to detect a stack overflow which is about to happen, before it can cause (further) data corrution. FATAL_ERROR() will be called if the stack pointer is outside valid range.")
set("distortos_Checks_02_Stack_pointer_range_during_system_tick"
		"ON"
		CACHE
		"BOOL"
		"Check stack pointer range during system tick.\n\nSimilar to \"distortos_Checks_01_Stack_pointer_range_during_context_switch\", but executed during every system tick.")
set("distortos_Checks_03_Stack_guard_contents_during_context_switch"
		"ON"
		CACHE
		"BOOL"
		"Check stack guard contents during context switch.\n\nSelecting this option extends stacks for all threads (including main() thread) with a \"stack guard\" at the overflow end. This \"stack guard\" - just as the whole stack - is filled with a sentinel value 0xed419f25 during thread initialization. The contents of \"stack guard\" of preempted thread are checked during each context switch and if any byte has changed, FATAL_ERROR() will be called.\n\nThis method is slower than simple stack pointer range checking, but is able to detect stack overflows much more reliably. It is still sufficiently fast, assuming that the size of \"stack guard\" is reasonable.\n\nBe advised that uninitialized variables on stack which are larger than size of \"stack guard\" can create \"holes\" in the stack, thus circumventing this detection mechanism. This especially applies to arrays used as buffers.")
set("distortos_Checks_04_Stack_guard_contents_during_system_tick"
		"ON"
		CACHE
		"BOOL"
		"Check stack guard contents during system tick.\n\nSimilar to \"distortos_Checks_03_Stack_guard_contents_during_context_switch\", but executed during every system tick.")
set("distortos_Checks_05_Stack_guard_size"
		"32"
		CACHE
		"STRING"
		"Size (in bytes) of \"stack guard\".\n\nAny value which is not a multiple of stack alignment required by architecture, will be rounded up.\n\nAllowed range: [1; 2147483647]")
set("distortos_Checks_06_Asserts"
		"ON"
		CACHE
		"BOOL"
		"Enable asserts.\n\nSome errors, which are clearly program bugs, are never reported using error codes. When this option is enabled, these preconditions, postconditions, invariants and assertions are checked with assert() macro. On the other hand - with this option disabled, they are completely ignored.\n\nIt is highly recommended to keep this option enabled until the application is thoroughly tested.")
set("distortos_buttons"
		"ON"
		CACHE
		"BOOL"
		"Enable buttons")
set("distortos_buttons_B1"
		"ON"
		CACHE
		"BOOL"
		"Enable B1 (User)")
set("distortos_leds"
		"ON"
		CACHE
		"BOOL"
		"Enable leds")
set("distortos_leds_Ld2"
		"ON"
		CACHE
		"BOOL"
		"Enable Ld2 (Green)")
set("distortos_Peripherals_SPI1"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI1 low-level driver.")
set("distortos_Peripherals_SPI2"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI2 low-level driver.")
set("distortos_Peripherals_USART1"
		"OFF"
		CACHE
		"BOOL"
		"Enable USART1 low-level driver.")
set("distortos_Peripherals_USART2"
		"OFF"
		CACHE
		"BOOL"
		"Enable USART2 low-level driver.")
set("distortos_Peripherals_USART3"
		"OFF"
		CACHE
		"BOOL"
		"Enable USART3 low-level driver.")
set("distortos_Peripherals_GPIOA"
		"ON"
		CACHE
		"BOOL"
		"Enable GPIOA.")
set("distortos_Peripherals_GPIOB"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOB.")
set("distortos_Peripherals_GPIOC"
		"ON"
		CACHE
		"BOOL"
		"Enable GPIOC.")
set("distortos_Peripherals_GPIOD"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOD.")
set("distortos_Peripherals_DMA1"
		"OFF"
		CACHE
		"BOOL"
		"Enable DMA1 low-level driver.")
set("distortos_Clocks_00_Standard_configuration_of_clocks"
		"ON"
		CACHE
		"BOOL"
		"Enable standard configuration of clocks.\n\nThis will set values selected below and additionally configure appropriate FLASH latency before switching system clock to selected source.\n\nIf disabled, no clock configuration will be done during chip initialization. The values entered below (frequencies, dividers, ...) will only be used to determine chip clocks. The user must configure the chip manually to match these settings.")
set("distortos_Clocks_08_PLL"
		"ON"
		CACHE
		"BOOL"
		"Enable PLL.")
set("distortos_Clocks_09_Clock_source_of_PLL"
		"HSIDIV2"
		CACHE
		"STRING"
		"Select clock source of main PLL.\n\nPLL input frequency (PLLin) must be in the range:\n- [1 MHz; 24 MHz] for STM32F100 chips,\n- [3 MHz; 12 MHz] for STM32F105 and STM32F107 chips,\n- [1 MHz; 25 MHz] for other chips.")
set("distortos_Clocks_12_PLLMUL"
		"16"
		CACHE
		"STRING"
		"PLLMUL value for main PLL.\n\nIt is used to multiply main PLL input frequency (PLLin). Resulting PLL output frequency (PLLout) must be in the range:\n- [16 MHz; 24 MHz] for STM32F100 chips,\n- [16 MHz; 36 MHz] for STM32F101 chips,\n- [16 MHz; 48 MHz] for STM32F102 chips,\n- [18 MHz; 72 MHz] for STM32F105 and STM32F107 chips,\n- [16 MHz; 72 MHz] for other chips.\n\nPLLout = PLLin * PLLMUL")
set("distortos_Clocks_13_System_clock_source"
		"PLL"
		CACHE
		"STRING"
		"Select system clock source.")
set("distortos_Clocks_14_HPRE"
		"1"
		CACHE
		"STRING"
		"AHB clock division factor.\n\nAHBclk = SYSclk / AHBdivider")
set("distortos_Clocks_15_PPRE1"
		"2"
		CACHE
		"STRING"
		"APB1 (low speed) clock division factor.\n\nAPB1 clock must not exceed 36 MHz.\n\nAPB1clk = AHBclk / APB1divider")
set("distortos_Clocks_16_PPRE2"
		"1"
		CACHE
		"STRING"
		"APB2 (high speed) clock division factor.\n\nAPB2 clock must not exceed 72 MHz.\n\nAPB2clk = AHBclk / APB2divider")
set("distortos_Memory_00_Flash_prefetch"
		"ON"
		CACHE
		"BOOL"
		"Enable flash prefetch option in FLASH->ACR register. Has to be enabled when HPRE != 1.")
set("distortos_Memory_01_Flash_half_cycle_access"
		"OFF"
		CACHE
		"BOOL"
		"Enable flash half cycle access option in FLASH->ACR register. Has to be disabled when HPRE != 1.")
set("distortos_Architecture_00_Interrupt_stack_size"
		"1024"
		CACHE
		"STRING"
		"Size (in bytes) of \"main\" stack used by core exceptions and interrupts in Handler mode.\n\nAllowed range: [8; 2147483647]")
set("distortos_Architecture_01_Interrupt_priority_disabled_in_critical_sections"
		"0"
		CACHE
		"STRING"
		"Interrupt priority disabled in critical sections.\n\nMinimal numerical priority (inclusive) of interrupt handlers that can use system's functions.\n\nDuring critical sections all interrupts with numerical priority above or equal to this value will be disabled. Interrupts with numerical priority below this value are never disabled, but they may not use any system's functions.\n\nNote - \"lower\" logical priority has \"higher\" numeric value! If this option is set to \"x\", then interrupts with priorities between \"x\" and 255 (both inclusive) may use system's functions, while interrupts with priorities between 0 and \"x - 1\" (both inclusive) may not. If 0 is chosen, then all interrupts (except HardFault and NMI) are disabled during critical sections, so they may use system's functions.\n\nAllowed range: [0; 15]")
set("distortos_Memory_regions_00_text_vectorTable"
		"flash"
		CACHE
		"STRING"
		"Memory region for .text.vectorTable section in linker script")
set("distortos_Memory_regions_01_text"
		"flash"
		CACHE
		"STRING"
		"Memory region for .text section in linker script")
set("distortos_Memory_regions_02_ARM_exidx"
		"flash"
		CACHE
		"STRING"
		"Memory region for .ARM.exidx section in linker script")
set("distortos_Memory_regions_03_Main_stack"
		"SRAM"
		CACHE
		"STRING"
		"Memory region for main stack in linker script")
set("distortos_Memory_regions_04_bss"
		"SRAM"
		CACHE
		"STRING"
		"Memory region for .bss section in linker script")
set("distortos_Memory_regions_05_data_VMA"
		"SRAM"
		CACHE
		"STRING"
		"VMA memory region for .data section in linker script")
set("distortos_Memory_regions_06_data_LMA"
		"flash"
		CACHE
		"STRING"
		"LMA memory region for .data section in linker script")
set("distortos_Memory_regions_07_noinit"
		"SRAM"
		CACHE
		"STRING"
		"Memory region for .noinit section in linker script")
set("distortos_Memory_regions_08_SRAM_data_LMA"
		"flash"
		CACHE
		"STRING"
		"LMA memory region for .SRAM.data section in linker script")
set("distortos_Memory_regions_09_Process_stack"
		"SRAM"
		CACHE
		"STRING"
		"Memory region for process stack in linker script")
set("distortos_Memory_regions_10_Heap"
		"SRAM"
		CACHE
		"STRING"
		"Memory region for heap in linker script")
set("distortos_FileSystems_00_Integration_with_standard_library"
		"ON"
		CACHE
		"BOOL"
		"Enable integration of file systems with standard library.\n\nEnables functionality for accessing multiple distortos::FileSystem objects via functions from standard library headers. When this option is enabled, following features are enabled:\n- global functions distortos::mount() and distortos::unmount() (which supports deferred unmount of busy file system);\n- support for (most likely) all functions from <stdio.h> header, like fopen(), fclose(), fread(), fwrite(), fprintf(), fscanf() and so on;\n- support for selected I/O-related functions from <fcntl.h>, <unistd.h> and <sys/stat.h> headers: open(), close(), read(), write(), isatty(), lseek(), fstat(), mkdir(), stat() and unlink() (which supports both files and directories);\n- support for selected functions from <dirent.h> header: opendir(), closedir(), readdir_r(), rewinddir(), seekdir() and telldir();\n- support for statvfs() function from <sys/statvfs.h> header;")
set("DISTORTOS_CONFIGURATION_VERSION"
		"4"
		CACHE
		"INTERNAL"
		"")
set("CMAKE_BUILD_TYPE"
		"RelWithDebInfo"
		CACHE
		"STRING"
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ...")
set("CMAKE_CXX_FLAGS"
		"-fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -Wno-psabi -mcpu=cortex-m3 -mthumb -fno-use-cxa-atexit"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during all build types.")
set("CMAKE_CXX_FLAGS_DEBUG"
		"-Og -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during DEBUG builds.")
set("CMAKE_CXX_FLAGS_MINSIZEREL"
		"-Os"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during MINSIZEREL builds.")
set("CMAKE_CXX_FLAGS_RELEASE"
		"-O2"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during RELEASE builds.")
set("CMAKE_CXX_FLAGS_RELWITHDEBINFO"
		"-O2 -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during RELWITHDEBINFO builds.")
set("CMAKE_C_FLAGS"
		"-ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -mcpu=cortex-m3 -mthumb"
		CACHE
		"STRING"
		"Flags used by the C compiler during all build types.")
set("CMAKE_C_FLAGS_DEBUG"
		"-Og -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the C compiler during DEBUG builds.")
set("CMAKE_C_FLAGS_MINSIZEREL"
		"-Os"
		CACHE
		"STRING"
		"Flags used by the C compiler during MINSIZEREL builds.")
set("CMAKE_C_FLAGS_RELEASE"
		"-O2"
		CACHE
		"STRING"
		"Flags used by the C compiler during RELEASE builds.")
set("CMAKE_C_FLAGS_RELWITHDEBINFO"
		"-O2 -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the C compiler during RELWITHDEBINFO builds.")
set("CMAKE_EXE_LINKER_FLAGS"
		"-Wl,--gc-sections"
		CACHE
		"STRING"
		"Flags used by the linker during all build types.")
set("CMAKE_EXE_LINKER_FLAGS_DEBUG"
		""
		CACHE
		"STRING"
		"Flags used by the linker during DEBUG builds.")
set("CMAKE_EXE_LINKER_FLAGS_MINSIZEREL"
		""
		CACHE
		"STRING"
		"Flags used by the linker during MINSIZEREL builds.")
set("CMAKE_EXE_LINKER_FLAGS_RELEASE"
		""
		CACHE
		"STRING"
		"Flags used by the linker during RELEASE builds.")
set("CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO"
		""
		CACHE
		"STRING"
		"Flags used by the linker during RELWITHDEBINFO builds.")
set("CMAKE_EXPORT_COMPILE_COMMANDS"
		"ON"
		CACHE
		"BOOL"
		"Enable/Disable output of compile commands during generation.")
set("CMAKE_MODULE_LINKER_FLAGS"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of modules during all build types.")
set("CMAKE_MODULE_LINKER_FLAGS_DEBUG"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of modules during DEBUG builds.")
set("CMAKE_MODULE_LINKER_FLAGS_MINSIZEREL"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of modules during MINSIZEREL builds.")
set("CMAKE_MODULE_LINKER_FLAGS_RELEASE"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of modules during RELEASE builds.")
set("CMAKE_MODULE_LINKER_FLAGS_RELWITHDEBINFO"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of modules during RELWITHDEBINFO builds.")
set("CMAKE_SHARED_LINKER_FLAGS"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of shared libraries during all build types.")
set("CMAKE_SHARED_LINKER_FLAGS_DEBUG"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of shared libraries during DEBUG builds.")
set("CMAKE_SHARED_LINKER_FLAGS_MINSIZEREL"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of shared libraries during MINSIZEREL builds.")
set("CMAKE_SHARED_LINKER_FLAGS_RELEASE"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of shared libraries during RELEASE builds.")
set("CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of shared libraries during RELWITHDEBINFO builds.")
set("CMAKE_STATIC_LINKER_FLAGS"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of static libraries during all build types.")
set("CMAKE_STATIC_LINKER_FLAGS_DEBUG"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of static libraries during DEBUG builds.")
set("CMAKE_STATIC_LINKER_FLAGS_MINSIZEREL"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of static libraries during MINSIZEREL builds.")
set("CMAKE_STATIC_LINKER_FLAGS_RELEASE"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of static libraries during RELEASE builds.")
set("CMAKE_STATIC_LINKER_FLAGS_RELWITHDEBINFO"
		""
		CACHE
		"STRING"
		"Flags used by the linker during the creation of static libraries during RELWITHDEBINFO builds.")
set("CMAKE_TOOLCHAIN_FILE"
		"${DISTORTOS_PATH}/source/board/ST_NUCLEO-F103RB/Toolchain-ST_NUCLEO-F103RB.cmake"
		CACHE
		"FILEPATH"
		"The CMake toolchain file")
set("CMAKE_VERBOSE_MAKEFILE"
		"OFF"
		CACHE
		"BOOL"
		"If this value is on, makefiles will be generated without the .SILENT directive, and all commands will be echoed to the console during the make.  This is useful for debugging only. With Visual Studio IDE projects all commands are done without /nologo.")
