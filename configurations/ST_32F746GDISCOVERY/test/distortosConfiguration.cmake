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

set("CMAKE_BUILD_TYPE"
		"RelWithDebInfo"
		CACHE
		"STRING"
		"Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel ...")
set("CMAKE_CXX_FLAGS"
		"-fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mthumb -mfloat-abi=hard -fno-use-cxa-atexit"
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
		"-ffunction-sections -fdata-sections -Wall -Wextra -Wshadow -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mthumb -mfloat-abi=hard"
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
		"-Wl,--gc-sections -mcpu=cortex-m7 -mfpu=fpv5-sp-d16 -mthumb -mfloat-abi=hard"
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
		"${DISTORTOS_PATH}/source/board/ST_32F746GDISCOVERY/Toolchain-ST_32F746GDISCOVERY.cmake"
		CACHE
		"FILEPATH"
		"The CMake toolchain file")
set("CMAKE_VERBOSE_MAKEFILE"
		"OFF"
		CACHE
		"BOOL"
		"If this value is on, makefiles will be generated without the .SILENT directive, and all commands will be echoed to the console during the make.  This is useful for debugging only. With Visual Studio IDE projects all commands are done without /nologo.")
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
set("distortos_Build_00_Static_destructors"
		"OFF"
		CACHE
		"BOOL"
		"Enable static destructors.\n\nEnable destructors for objects with static storage duration. As embedded applications almost never \"exit\", these destructors are usually never executed, wasting ROM.")
set("distortos_Build_02_Floating_point_ABI"
		"hard"
		CACHE
		"STRING"
		"Select floating-point ABI.")
set("distortos_Checks_00_Context_of_functions"
		"ON"
		CACHE
		"BOOL"
		"Check context of functions.\n\nSome functions may only be used from thread context, as using them from interrupt context results in undefined behaviour. There are several groups of functions to which this restriction applies (some functions fall into several categories at once): 1. all blocking functions, like callOnce(), FifoQueue::push(), Semaphore::wait(), ..., as an attempt to block current thread of execution (not to be confused with current thread) is not possible in interrupt context; 2. all mutex functions, as the concept of ownership by a thread - core feature of mutex - cannot be fulfilled in interrupt context; 3. all functions from ThisThread namespace (including ThisThread::Signals namespace), as in interrupt context they would access a random thread that happened to be executing at that particular moment;\n\nUsing such functions from interrupt context is a common bug in applications which can be easily introduced and very hard to find, as the symptoms may appear only under certain circumstances.\n\nSelecting this option enables context checks in all functions with such requirements. If any of them is used from interrupt context, FATAL_ERROR() will be called.")
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
set("distortos_Clocks_00_Standard_configuration_of_clocks"
		"ON"
		CACHE
		"BOOL"
		"Enable standard configuration of clocks.\n\nThis will set values selected below and additionally configure appropriate FLASH latency before switching system clock to selected source.\n\nIf disabled, no clock configuration will be done during chip initialization. The values entered below (frequencies, dividers, ...) will only be used to determine chip clocks. The user must configure the chip manually to match these settings.")
set("distortos_Clocks_01_Voltage_scale_mode"
		"1"
		CACHE
		"STRING"
		"Select voltage scale mode.\n\nAllowed range: [1; 3]")
set("distortos_Clocks_02_Over_drive_mode"
		"ON"
		CACHE
		"BOOL"
		"Enable over-drive mode.")
set("distortos_Clocks_03_HSE"
		"ON"
		CACHE
		"BOOL"
		"Enable HSE external user clock, 25000000 Hz.")
set("distortos_Clocks_04_PLLs"
		"ON"
		CACHE
		"BOOL"
		"Enable PLLs.")
set("distortos_Clocks_05_Clock_source_of_PLLs"
		"HSE"
		CACHE
		"STRING"
		"Select clock source of main and audio PLLs.")
set("distortos_Clocks_06_PLLM"
		"25"
		CACHE
		"STRING"
		"PLLM value for main and audio PLLs.\n\nIt is used to divide PLL input clock (PLLin) before it is fed to VCO of each PLL. VCO input frequency (VCOin) must be in the range [1 MHz; 2 MHz], 2 MHz is recommended to limit PLL jitter.\n\nVCOin = PLLin / PLLM\n\nAllowed range: [2; 63]")
set("distortos_Clocks_07_PLL"
		"ON"
		CACHE
		"BOOL"
		"Enable main PLL.")
set("distortos_Clocks_08_PLLN"
		"432"
		CACHE
		"STRING"
		"PLLN value for main PLL.\n\nIt is used to multiply VCO input frequency (VCOin). Resulting VCO output frequency (VCOout) must be in the range [100 MHz; 432 MHz].\n\nVCOout = VCOin * PLLN = PLLin / PLLM * PLLN\n\nAllowed range: [50; 432]")
set("distortos_Clocks_09_PLLP"
		"2"
		CACHE
		"STRING"
		"PLLP value for main PLL.\n\nIt is used to divide VCO output frequency (VCOout). PLL output frequency (PLLout) must not exceed:\n- 216 MHz (with over-drive mode) or 180 MHz (otherwise) in voltage scale 1 mode,\n- 180 MHz (with over-drive mode) or 168 MHz (otherwise) in voltage scale 2 mode,\n- 144 MHz in voltage scale 3 mode.\n\nPLLout = VCOout / PLLP = PLLin / PLLM * PLLN / PLLP")
set("distortos_Clocks_10_PLLQ"
		"9"
		CACHE
		"STRING"
		"PLLQ value for main PLL.\n\nIt is used to divide VCO output frequency (VCOout) to produce clock for USB OTG FS, SDMMC and RNG (PLLQout). USB OTG FS requires a 48 MHz clock for correct operation, while SDMMC and RNG need a frequency lower than or equal to 48 MHz to work correctly.\n\nPLLQout = VCOout / PLLQ = PLLin / PLLM * PLLN / PLLQ\n\nAllowed range: [2; 15]")
set("distortos_Clocks_12_PLLI2S"
		"OFF"
		CACHE
		"BOOL"
		"Enable PLLI2S.")
set("distortos_Clocks_17_PLLSAI"
		"OFF"
		CACHE
		"BOOL"
		"Enable PLLSAI.")
set("distortos_Clocks_22_PLL48_clock_source"
		"PLLQ"
		CACHE
		"STRING"
		"Select PLL48 clock source.")
set("distortos_Clocks_23_System_clock_source"
		"PLL"
		CACHE
		"STRING"
		"Select system clock source.")
set("distortos_Clocks_24_HPRE"
		"1"
		CACHE
		"STRING"
		"AHB clock division factor.\n\nAHBclk = SYSclk / AHBdivider")
set("distortos_Clocks_25_PPRE1"
		"4"
		CACHE
		"STRING"
		"APB1 (low speed) clock division factor.\n\nAPB1 clock must not exceed 54 MHz (with over-drive mode) or 45 MHz (otherwise).\n\nAPB1clk = AHBclk / APB1divider")
set("distortos_Clocks_26_PPRE2"
		"2"
		CACHE
		"STRING"
		"APB2 (high speed) clock division factor.\n\nAPB2 clock must not exceed 108 MHz (with over-drive mode) or 90 MHz (otherwise).\n\nAPB2clk = AHBclk / APB2divider")
set("distortos_Clocks_27_SDMMC1_clock_source"
		"PLL48CLK"
		CACHE
		"STRING"
		"Select SDMMC1 adapter clock source.")
set("distortos_Memory_00_Flash_prefetch"
		"ON"
		CACHE
		"BOOL"
		"Enable flash prefetch option in FLASH->ACR register.")
set("distortos_Memory_01_Flash_ART_accelerator"
		"ON"
		CACHE
		"BOOL"
		"Enable flash ART accelerator in FLASH->ACR register.")
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
set("distortos_Memory_regions_09_ITCM_data_LMA"
		"flash"
		CACHE
		"STRING"
		"LMA memory region for .ITCM.data section in linker script")
set("distortos_Memory_regions_10_Process_stack"
		"SRAM"
		CACHE
		"STRING"
		"Memory region for process stack in linker script")
set("distortos_Memory_regions_11_Heap"
		"SRAM"
		CACHE
		"STRING"
		"Memory region for heap in linker script")
set("distortos_Peripherals_DMA1"
		"OFF"
		CACHE
		"BOOL"
		"Enable DMA1 low-level driver.")
set("distortos_Peripherals_DMA2"
		"OFF"
		CACHE
		"BOOL"
		"Enable DMA2 low-level driver.")
set("distortos_Peripherals_GPIOA"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOA.")
set("distortos_Peripherals_GPIOB"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOB.")
set("distortos_Peripherals_GPIOC"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOC.")
set("distortos_Peripherals_GPIOD"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOD.")
set("distortos_Peripherals_GPIOE"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOE.")
set("distortos_Peripherals_GPIOF"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOF.")
set("distortos_Peripherals_GPIOG"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOG.")
set("distortos_Peripherals_GPIOH"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOH.")
set("distortos_Peripherals_GPIOI"
		"ON"
		CACHE
		"BOOL"
		"Enable GPIOI.")
set("distortos_Peripherals_GPIOJ"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOJ.")
set("distortos_Peripherals_GPIOK"
		"OFF"
		CACHE
		"BOOL"
		"Enable GPIOK.")
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
set("distortos_Peripherals_SPI3"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI3 low-level driver.")
set("distortos_Peripherals_SPI4"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI4 low-level driver.")
set("distortos_Peripherals_SPI5"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI5 low-level driver.")
set("distortos_Peripherals_SPI6"
		"OFF"
		CACHE
		"BOOL"
		"Enable SPI6 low-level driver.")
set("distortos_Peripherals_UART4"
		"OFF"
		CACHE
		"BOOL"
		"Enable UART4 low-level driver.")
set("distortos_Peripherals_UART5"
		"OFF"
		CACHE
		"BOOL"
		"Enable UART5 low-level driver.")
set("distortos_Peripherals_UART7"
		"OFF"
		CACHE
		"BOOL"
		"Enable UART7 low-level driver.")
set("distortos_Peripherals_UART8"
		"OFF"
		CACHE
		"BOOL"
		"Enable UART8 low-level driver.")
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
set("distortos_Peripherals_USART6"
		"OFF"
		CACHE
		"BOOL"
		"Enable USART6 low-level driver.")
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
set("distortos_leds_Ld1"
		"ON"
		CACHE
		"BOOL"
		"Enable Ld1 (Green)")
set("DISTORTOS_CONFIGURATION_NAMES"
		"distortos_Architecture_00_Interrupt_stack_size;distortos_Architecture_01_Interrupt_priority_disabled_in_critical_sections;distortos_Build_00_Static_destructors;distortos_Build_02_Floating_point_ABI;distortos_Checks_00_Context_of_functions;distortos_Checks_01_Stack_pointer_range_during_context_switch;distortos_Checks_02_Stack_pointer_range_during_system_tick;distortos_Checks_03_Stack_guard_contents_during_context_switch;distortos_Checks_04_Stack_guard_contents_during_system_tick;distortos_Checks_05_Stack_guard_size;distortos_Clocks_00_Standard_configuration_of_clocks;distortos_Clocks_01_Voltage_scale_mode;distortos_Clocks_02_Over_drive_mode;distortos_Clocks_03_HSE;distortos_Clocks_04_PLLs;distortos_Clocks_05_Clock_source_of_PLLs;distortos_Clocks_06_PLLM;distortos_Clocks_07_PLL;distortos_Clocks_08_PLLN;distortos_Clocks_09_PLLP;distortos_Clocks_10_PLLQ;distortos_Clocks_12_PLLI2S;distortos_Clocks_17_PLLSAI;distortos_Clocks_22_PLL48_clock_source;distortos_Clocks_23_System_clock_source;distortos_Clocks_24_HPRE;distortos_Clocks_25_PPRE1;distortos_Clocks_26_PPRE2;distortos_Clocks_27_SDMMC1_clock_source;distortos_Memory_00_Flash_prefetch;distortos_Memory_01_Flash_ART_accelerator;distortos_Memory_regions_00_text_vectorTable;distortos_Memory_regions_01_text;distortos_Memory_regions_02_ARM_exidx;distortos_Memory_regions_03_Main_stack;distortos_Memory_regions_04_bss;distortos_Memory_regions_05_data_VMA;distortos_Memory_regions_06_data_LMA;distortos_Memory_regions_07_noinit;distortos_Memory_regions_08_SRAM_data_LMA;distortos_Memory_regions_09_ITCM_data_LMA;distortos_Memory_regions_10_Process_stack;distortos_Memory_regions_11_Heap;distortos_Peripherals_DMA1;distortos_Peripherals_DMA2;distortos_Peripherals_GPIOA;distortos_Peripherals_GPIOB;distortos_Peripherals_GPIOC;distortos_Peripherals_GPIOD;distortos_Peripherals_GPIOE;distortos_Peripherals_GPIOF;distortos_Peripherals_GPIOG;distortos_Peripherals_GPIOH;distortos_Peripherals_GPIOI;distortos_Peripherals_GPIOJ;distortos_Peripherals_GPIOK;distortos_Peripherals_SPI1;distortos_Peripherals_SPI2;distortos_Peripherals_SPI3;distortos_Peripherals_SPI4;distortos_Peripherals_SPI5;distortos_Peripherals_SPI6;distortos_Peripherals_UART4;distortos_Peripherals_UART5;distortos_Peripherals_UART7;distortos_Peripherals_UART8;distortos_Peripherals_USART1;distortos_Peripherals_USART2;distortos_Peripherals_USART3;distortos_Peripherals_USART6;distortos_Scheduler_00_Tick_frequency;distortos_Scheduler_01_Round_robin_frequency;distortos_Scheduler_02_Support_for_signals;distortos_Scheduler_03_Support_for_thread_detachment;distortos_Scheduler_04_Main_thread_stack_size;distortos_Scheduler_05_Main_thread_priority;distortos_Scheduler_06_Reception_of_signals_by_main_thread;distortos_Scheduler_07_Queued_signals_for_main_thread;distortos_Scheduler_08_SignalAction_objects_for_main_thread;distortos_buttons;distortos_buttons_B1;distortos_leds;distortos_leds_Ld1"
		CACHE
		"INTERNAL"
		"")
