#
# \file
# \brief distortos configuration
#
# \warning
# Automatically generated file - do not edit!
#

set("CMAKE_BUILD_TYPE"
		"RelWithDebInfo"
		CACHE
		"STRING"
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ...")
set("CMAKE_CXX_FLAGS"
		"-fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard -fno-use-cxa-atexit"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during all build types.")
set("CMAKE_CXX_FLAGS_DEBUG"
		"-Og -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during DEBUG builds.")
set("CMAKE_CXX_FLAGS_MINSIZEREL"
		"-Os -DNDEBUG"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during MINSIZEREL builds.")
set("CMAKE_CXX_FLAGS_RELEASE"
		"-O2 -DNDEBUG"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during RELEASE builds.")
set("CMAKE_CXX_FLAGS_RELWITHDEBINFO"
		"-O2 -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the CXX compiler during RELWITHDEBINFO builds.")
set("CMAKE_C_FLAGS"
		"-ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard"
		CACHE
		"STRING"
		"Flags used by the C compiler during all build types.")
set("CMAKE_C_FLAGS_DEBUG"
		"-Og -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the C compiler during DEBUG builds.")
set("CMAKE_C_FLAGS_MINSIZEREL"
		"-Os -DNDEBUG"
		CACHE
		"STRING"
		"Flags used by the C compiler during MINSIZEREL builds.")
set("CMAKE_C_FLAGS_RELEASE"
		"-O2 -DNDEBUG"
		CACHE
		"STRING"
		"Flags used by the C compiler during RELEASE builds.")
set("CMAKE_C_FLAGS_RELWITHDEBINFO"
		"-O2 -g -ggdb3"
		CACHE
		"STRING"
		"Flags used by the C compiler during RELWITHDEBINFO builds.")
set("CMAKE_EXE_LINKER_FLAGS"
		"-Wl,--gc-sections -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard"
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
		"../source/board/ST_NUCLEO-L432KC/Toolchain-ST_NUCLEO-L432KC.cmake"
		CACHE
		"FILEPATH"
		"The CMake toolchain file")
set("CMAKE_VERBOSE_MAKEFILE"
		"OFF"
		CACHE
		"BOOL"
		"If this value is on, makefiles will be generated without the .SILENT directive, and all commands will be echoed to the console during the make.  This is useful for debugging only. With Visual Studio IDE projects all commands are done without /nologo.")
set("CONFIG_ARCHITECTURE_ARM"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_ARCHITECTURE_ARMV7_M"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_ARCHITECTURE_ARM_CORTEX_M4"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_ARCHITECTURE_ARM_CORTEX_M4_R0P1"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_ARCHITECTURE_ASCENDING_STACK"
		"OFF"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_ARCHITECTURE_EMPTY_STACK"
		"OFF"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_ARCHITECTURE_STACK_ALIGNMENT"
		"8"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_BOARD"
		"ST,NUCLEO-L432KC"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP"
		"STM32L432KC"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32L4"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32L43"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32L432"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32L432K"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32L4_RCC_HSE_CLOCK_BYPASS"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32_GPIOV2_HAS_4_AF_BITS"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32_GPIOV2_HAS_HIGH_SPEED"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("CONFIG_CHIP_STM32_USARTV2_HAS_CR1_M1_BIT"
		"ON"
		CACHE
		"INTERNAL"
		"")
set("distortos_Architecture_00_Interrupt_stack_size"
		"1024"
		CACHE
		"STRING"
		"Size (in bytes) of \"main\" stack used by core exceptions and interrupts in Handler mode.\n\nAllowed range: [8; 2147483647]")
set("distortos_Architecture_01_Interrupt_priority_disabled_in_critical_sections"
		"0"
		CACHE
		"STRING"
		"Interrupt priority disabled in critical sections.\n\nMinimal numerical priority (inclusive) of interrupt handlers that can use system's functions.\n\nDuring critical sections all interrupts with numerical priority above or equal to this value will be disabled.\nInterrupts with numerical priority below this value are never disabled, but they may not use any system's\nfunctions.\n\nNote - \"lower\" logical priority has \"higher\" numeric value! If this option is set to \"x\", then interrupts\nwith priorities between \"x\" and 255 (both inclusive) may use system's functions, while interrupts with\npriorities between 0 and \"x - 1\" (both inclusive) may not. If 0 is chosen, then all interrupts (except\nHardFault and NMI) are disabled during critical sections, so they may use system's functions.\n\nAllowed range: [-2147483648; 15]")
set("distortos_Build_00_Static_destructors"
		"OFF"
		CACHE
		"BOOL"
		"Enable static destructors.\n\nEnable destructors for objects with static storage duration. As embedded applications almost never \"exit\",\nthese destructors are usually never executed, wasting ROM.")
set("distortos_Build_02_Floating_point_ABI"
		"hard"
		CACHE
		"STRING"
		"Select floating-point ABI.")
set("distortos_Checks_00_Context_of_functions"
		"ON"
		CACHE
		"BOOL"
		"Check context of functions.\n\nSome functions may only be used from thread context, as using them from interrupt context results in undefined\nbehaviour. There are several groups of functions to which this restriction applies (some functions fall into\nseveral categories at once):\n1. all blocking functions, like callOnce(), FifoQueue::push(), Semaphore::wait(), ..., as an attempt to block\ncurrent thread of execution (not to be confused with current thread) is not possible in interrupt context;\n2. all mutex functions, as the concept of ownership by a thread - core feature of mutex - cannot be fulfilled in\ninterrupt context;\n3. all functions from ThisThread namespace (including ThisThread::Signals namespace), as in interrupt context\nthey would access a random thread that happened to be executing at that particular moment;\n\nUsing such functions from interrupt context is a common bug in applications which can be easily introduced and\nvery hard to find, as the symptoms may appear only under certain circumstances.\n\nSelecting this option enables context checks in all functions with such requirements. If any of them is used\nfrom interrupt context, FATAL_ERROR() will be called.")
set("distortos_Checks_01_Stack_pointer_range_during_context_switch"
		"ON"
		CACHE
		"BOOL"
		"Check stack pointer range during context switch.\n\nSimple range checking of preempted thread's stack pointer can be performed during context switches. It is\nrelatively fast, but cannot detect all stack overflows. The check is done before the software stack frame is\npushed on thread's stack, but the size of this pending stack frame is accounted for - the intent is to detect a\nstack overflow which is about to happen, before it can cause (further) data corrution. FATAL_ERROR() will be\ncalled if the stack pointer is outside valid range.")
set("distortos_Checks_02_Stack_pointer_range_during_system_tick"
		"ON"
		CACHE
		"BOOL"
		"Check stack pointer range during system tick.\n\nSimilar to \"Check stack pointer range during context switch\", but executed during every system tick.")
set("distortos_Checks_03_Stack_guard_contents_during_context_switch"
		"ON"
		CACHE
		"BOOL"
		"Check stack guard contents during context switch.\n\nSelecting this option extends stacks for all threads (including main() thread) with a \"stack guard\" at the\noverflow end. This \"stack guard\" - just as the whole stack - is filled with a sentinel value 0xed419f25 during\nthread initialization. The contents of \"stack guard\" of preempted thread are checked during each context\nswitch and if any byte has changed, FATAL_ERROR() will be called.\n\nThis method is slower than simple stack pointer range checking, but is able to detect stack overflows much more\nreliably. It is still sufficiently fast, assuming that the size of \"stack guard\" is reasonable.\n\nBe advised that uninitialized variables on stack which are larger than size of \"stack guard\" can create\n\"holes\" in the stack, thus circumventing this detection mechanism. This especially applies to arrays used as\nbuffers.")
set("distortos_Checks_04_Stack_guard_contents_during_system_tick"
		"ON"
		CACHE
		"BOOL"
		"Check stack guard contents during system tick.\n\nSimilar to \"Check stack guard contents during context switch\", but executed during every system tick.")
set("distortos_Checks_05_Stack_guard_size"
		"32"
		CACHE
		"STRING"
		"Size (in bytes) of \"stack guard\".\n\nAny value which is not a multiple of stack alignment required by architecture will be rounded up.\n\nAllowed range: [1; 2147483647]")
set("distortos_Clocks_00_Standard_configuration_of_clocks"
		"ON"
		CACHE
		"BOOL"
		"Enable standard configuration of clocks.\n\nThis will set values selected below and additionally configure appropriate FLASH latency before switching system\nclock to selected source.\n\nIf disabled, no clock configuration will be done during chip initialization. The values entered below\n(frequencies, dividers, ...) will only be used to determine chip clocks. The user must configure the chip\nmanually to match these settings.")
set("distortos_Clocks_01_Voltage_scaling_range"
		"1"
		CACHE
		"STRING"
		"Select voltage scaling range.\n\nAllowed range: [1; 2]")
set("distortos_Clocks_03_HSI16"
		"ON"
		CACHE
		"BOOL"
		"Enable HSI16.")
set("distortos_Clocks_04_MSI"
		"OFF"
		CACHE
		"BOOL"
		"Enable MSI.")
set("distortos_Clocks_06_PLL"
		"ON"
		CACHE
		"BOOL"
		"Enable PLL.")
set("distortos_Clocks_07_Clock_source_of_PLLs"
		"HSI16"
		CACHE
		"STRING"
		"Select clock source of PLLs.")
set("distortos_Clocks_08_PLLM"
		"4"
		CACHE
		"STRING"
		"PLLM value for PLLs.\n\nIt is used to divide PLL input clock (PLLin) before it is fed to VCO. VCO input frequency (VCOin)\nmust be in the range [4 MHz; 16 MHz].\n\nVCOin = PLLin / PLLM\n\nAllowed range: [1; 8]")
set("distortos_Clocks_09_PLLN"
		"40"
		CACHE
		"STRING"
		"PLLN value for main PLL.\n\nIt is used to multiply VCO input frequency (VCOin). Resulting VCO output frequency (VCOout) must be\nin the range [64 MHz; 344 MHz] in voltage scaling range 1 or [64 MHz; 128 MHz] in voltage scaling\nrange 2.\n\nVCOout = VCOin * PLLN = PLLin / PLLM * PLLN\n\nAllowed range: [8; 86]")
set("distortos_Clocks_10_PLLP"
		"2"
		CACHE
		"STRING"
		"PLLP value for main PLL.\n\nIt is used to divide VCO output frequency (VCOout) to produce clock for SAI1 and SAI2 (PLLPout).\nPLL \"P\" output frequency (PLLPout) must be in the range [2.0645 MHz; 80 MHz] in voltage scaling\nrange 1 or [2.0645 MHz; 26 MHz] in voltage scaling range 2.\n\nPLLPout = VCOout / PLLP = PLLin / PLLM * PLLN / PLLP")
set("distortos_Clocks_11_PLLQ"
		"2"
		CACHE
		"STRING"
		"PLLQ value for main PLL.\n\nIt is used to divide VCO output frequency (VCOout) to produce clock for RNG, SDMMC and USB\n(PLLQout). PLL \"Q\" output frequency (PLLQout) must be in the range [8 MHz; 80 MHz] in voltage\nscaling range 1 or [8 MHz; 26 MHz] in voltage scaling range 2.\n\nPLLQout = VCOout / PLLQ = PLLin / PLLM * PLLN / PLLQ")
set("distortos_Clocks_12_PLLR"
		"2"
		CACHE
		"STRING"
		"PLLR value for main PLL.\n\nIt is used to divide VCO output frequency (VCOout) to produce system clock (PLLRout). PLL \"R\"\noutput frequency (PLLRout) must be in the range [8 MHz; 80 MHz] in voltage scaling range 1 or\n[8 MHz; 26 MHz] in voltage scaling range 2.\n\nPLLRout = VCOout / PLLR = PLLin / PLLM * PLLN / PLLR")
set("distortos_Clocks_13_PLLP_output"
		"OFF"
		CACHE
		"BOOL"
		"Enable PLL's \"P\" output.")
set("distortos_Clocks_14_PLLQ_output"
		"OFF"
		CACHE
		"BOOL"
		"Enable PLL's \"Q\" output.")
set("distortos_Clocks_15_PLLR_output"
		"ON"
		CACHE
		"BOOL"
		"Enable PLL's \"R\" output.")
set("distortos_Clocks_16_System_clock_source"
		"PLL"
		CACHE
		"STRING"
		"Select system clock source.")
set("distortos_Clocks_17_HPRE"
		"1"
		CACHE
		"STRING"
		"AHB clock division factor.\n\nAHBclk = SYSclk / AHBdivider")
set("distortos_Clocks_18_PPRE1"
		"1"
		CACHE
		"STRING"
		"APB1 (low speed) clock division factor.\n\nAPB1clk = AHBclk / APB1divider")
set("distortos_Clocks_19_PPRE2"
		"1"
		CACHE
		"STRING"
		"APB2 (high speed) clock division factor.\n\nAPB2clk = AHBclk / APB2divider")
set("distortos_Memory_00_Flash_prefetch"
		"ON"
		CACHE
		"BOOL"
		"Enable flash prefetch option in FLASH->ACR register.")
set("distortos_Memory_01_Flash_data_cache"
		"ON"
		CACHE
		"BOOL"
		"Enable flash data cache option in FLASH->ACR register.")
set("distortos_Memory_02_Flash_instruction_cache"
		"ON"
		CACHE
		"BOOL"
		"Enable flash instruction cache option in FLASH->ACR register.")
set("distortos_Memory_regions_00_text_vectors"
		"flash"
		CACHE
		"STRING"
		"Memory region for .text.vectors section in linker script")
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
		"SRAM1"
		CACHE
		"STRING"
		"Memory region for main stack in linker script")
set("distortos_Memory_regions_04_bss"
		"SRAM1"
		CACHE
		"STRING"
		"Memory region for .bss section in linker script")
set("distortos_Memory_regions_05_data_VMA"
		"SRAM1"
		CACHE
		"STRING"
		"VMA memory region for .data section in linker script")
set("distortos_Memory_regions_06_data_LMA"
		"flash"
		CACHE
		"STRING"
		"LMA memory region for .data section in linker script")
set("distortos_Memory_regions_07_noinit"
		"SRAM1"
		CACHE
		"STRING"
		"Memory region for .noinit section in linker script")
set("distortos_Memory_regions_08_SRAM1_data_LMA"
		"flash"
		CACHE
		"STRING"
		"LMA memory region for .SRAM1.data section in linker script")
set("distortos_Memory_regions_09_Process_stack"
		"SRAM1"
		CACHE
		"STRING"
		"Memory region for process stack in linker script")
set("distortos_Memory_regions_10_Heap"
		"SRAM1"
		CACHE
		"STRING"
		"Memory region for heap in linker script")
set("distortos_Peripherals_GPIOA"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOA.")
set("distortos_Peripherals_GPIOB"
		"ON"
		CACHE
		"INTERNAL"
		"Enable GPIOB.")
set("distortos_Peripherals_GPIOC"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOC.")
set("distortos_Peripherals_GPIOH"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOH.")
set("distortos_Peripherals_SPI1"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI1 low-level driver.")
set("distortos_Peripherals_SPI3"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI3 low-level driver.")
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
		"Enable support for thread detachment.\n\nEnable functions that \"detach\" dynamic threads:\n- ThisThread::detach();\n- Thread::detach();\n\nWhen this options is not selected, these functions are not available at all.\n\nWhen dynamic and detached thread terminates, it will be added to the global list of threads pending for deferred\ndeletion. The thread will actually be deleted in idle thread, but only when two mutexes are successfully locked:\n- mutex that protects dynamic memory allocator;\n- mutex that synchronizes access to the list of threads pending for deferred deletion;")
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
		"Maximal number of queued signals for main thread. 0 disables queuing of signals for main thread.\n\nAllowed range: [-2147483648; 2147483647]")
set("distortos_Scheduler_08_SignalAction_objects_for_main_thread"
		"8"
		CACHE
		"STRING"
		"Maximal number of different SignalAction objects for main thread. 0 disables catching of signals for main thread.\n\nAllowed range: [-2147483648; 32]")
set("distortos_leds"
		"ON"
		CACHE
		"BOOL"
		"Enable leds")
set("distortos_leds_Ld3"
		"ON"
		CACHE
		"BOOL"
		"Enable Ld3 (Green)")
