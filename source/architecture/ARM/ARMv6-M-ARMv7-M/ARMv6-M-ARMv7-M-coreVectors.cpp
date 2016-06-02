/**
 * \file
 * \brief ARMv6-M and ARMv7-M core vector table and default weak handlers
 *
 * \author Copyright (C) 2014-2016 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| provide default weak handlers
+---------------------------------------------------------------------------------------------------------------------*/

// 0x08: Non maskable interrupt
__attribute__ ((weak)) void NMI_Handler()
{
	while (1);
}

// 0x0c: All class of fault
__attribute__ ((weak)) void HardFault_Handler()
{
	while (1);
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

// 0x10: Memory management
__attribute__ ((weak)) void MemManage_Handler()
{
	while (1);
}

// 0x14: Pre-fetch fault, memory access fault
__attribute__ ((weak)) void BusFault_Handler()
{
	while (1);
}

// 0x18: Undefined instruction or illegal state
__attribute__ ((weak)) void UsageFault_Handler()
{
	while (1);
}

#else	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

// 0x10: Reserved
__attribute__ ((weak)) void Reserved_0x10_Handler()
{
	while (1);
}

// 0x14: Reserved
__attribute__ ((weak)) void Reserved_0x14_Handler()
{
	while (1);
}

// 0x18: Reserved
__attribute__ ((weak)) void Reserved_0x18_Handler()
{
	while (1);
}

#endif	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

// 0x1c: Reserved
__attribute__ ((weak)) void Reserved_0x1c_Handler()
{
	while (1);
}

// 0x20: Reserved
__attribute__ ((weak)) void Reserved_0x20_Handler()
{
	while (1);
}

// 0x24: Reserved
__attribute__ ((weak)) void Reserved_0x24_Handler()
{
	while (1);
}

// 0x28: Reserved
__attribute__ ((weak)) void Reserved_0x28_Handler()
{
	while (1);
}

// 0x2c: System service call via SVC instruction
__attribute__ ((weak)) void SVC_Handler()
{
	while (1);
}

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

// 0x30: Debug Monitor
__attribute__ ((weak)) void DebugMon_Handler()
{
	while (1);
}

#else	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

// 0x30: Reserved
__attribute__ ((weak)) void Reserved_0x30_Handler()
{
	while (1);
}

#endif	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

// 0x34: Reserved
__attribute__ ((weak)) void Reserved_0x34_Handler()
{
	while (1);
}

// 0x38: Pendable request for system service
__attribute__ ((weak)) void PendSV_Handler()
{
	while (1);
}

// 0x3c: System tick timer
__attribute__ ((weak)) void SysTick_Handler()
{
	while (1);
}

/*---------------------------------------------------------------------------------------------------------------------+
| global symbols' declarations
+---------------------------------------------------------------------------------------------------------------------*/

extern const char __main_stack_end[];	// main stack end - imported from linker script

void Reset_Handler();					// import the address of Reset_Handler()

}	// extern "C"

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// single interrupt vector - pointer to function with no arguments and no return value
using InterruptVector = void(*)();

/*---------------------------------------------------------------------------------------------------------------------+
| global variables
+---------------------------------------------------------------------------------------------------------------------*/

/// core vector table
extern "C" const InterruptVector coreVectors[] __attribute__ ((section(".coreVectors")))
{
		reinterpret_cast<InterruptVector>(__main_stack_end),	// 0x00: Main stack end address
		Reset_Handler,											// 0x04: Reset
		NMI_Handler,											// 0x08: Non maskable interrupt
		HardFault_Handler,										// 0x0c: All class of fault

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

		MemManage_Handler,										// 0x10: Memory management
		BusFault_Handler,										// 0x14: Pre-fetch fault, memory access fault
		UsageFault_Handler,										// 0x18: Undefined instruction or illegal state

#else	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

		Reserved_0x10_Handler,									// 0x10: Reserved
		Reserved_0x14_Handler,									// 0x14: Reserved
		Reserved_0x18_Handler,									// 0x18: Reserved

#endif	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

		Reserved_0x1c_Handler,									// 0x1c: Reserved
		Reserved_0x20_Handler,									// 0x20: Reserved
		Reserved_0x24_Handler,									// 0x24: Reserved
		Reserved_0x28_Handler,									// 0x28: Reserved
		SVC_Handler,											// 0x2c: System service call via SVC instruction

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

		DebugMon_Handler,										// 0x30: Debug Monitor

#else	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

		Reserved_0x30_Handler,									// 0x30: Reserved

#endif	// !defined(__ARM_ARCH_7M__) && !defined(__ARM_ARCH_7EM__)

		Reserved_0x34_Handler,									// 0x34: Reserved
		PendSV_Handler,											// 0x38: Pendable request for system service
		SysTick_Handler,										// 0x3c: System tick timer
};

static_assert(sizeof(coreVectors) / sizeof(*coreVectors) == 16, "Invalid size of coreVectors[]!");
