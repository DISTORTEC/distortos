/**
 * \file
 * \brief ARMv7-M (Cortex-M3 / Cortex-M4) core vector table and default weak handlers.
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-04-28
 */

/*---------------------------------------------------------------------------------------------------------------------+
| provide default weak handlers
+---------------------------------------------------------------------------------------------------------------------*/

// Non maskable interrupt
__attribute__ ((weak)) void NMI_Handler(void)
{
	while (1);
}

// All class of fault
__attribute__ ((weak)) void HardFault_Handler(void)
{
	while (1);
}

// Memory management
__attribute__ ((weak)) void MemManage_Handler(void)
{
	while (1);
}

// Pre-fetch fault, memory access fault
__attribute__ ((weak)) void BusFault_Handler(void)
{
	while (1);
}

// Undefined instruction or illegal state
__attribute__ ((weak)) void UsageFault_Handler(void)
{
	while (1);
}

// Reserved 0x1C
__attribute__ ((weak)) void __Reserved_0x1C_Handler(void)
{
	while (1);
}

// Reserved 0x20
__attribute__ ((weak)) void __Reserved_0x20_Handler(void)
{
	while (1);
}

// Reserved 0x24
__attribute__ ((weak)) void __Reserved_0x24_Handler(void)
{
	while (1);
}

// Reserved 0x28
__attribute__ ((weak)) void __Reserved_0x28_Handler(void)
{
	while (1);
}

// System service call via SWI instruction
__attribute__ ((weak)) void SVC_Handler(void)
{
	while (1);
}

// Debug Monitor
__attribute__ ((weak)) void DebugMon_Handler(void)
{
	while (1);
}

// Reserved 0x34
__attribute__ ((weak)) void __Reserved_0x34_Handler(void)
{
	while (1);
}

// Pendable request for system service
__attribute__ ((weak)) void PendSV_Handler(void)
{
	while (1);
}

// System tick timer
__attribute__ ((weak)) void SysTick_Handler(void)
{
	while (1);
}

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// single interrupt vector - pointer to function with no arguments and no return value
typedef void (*InterruptVector)(void);

/*---------------------------------------------------------------------------------------------------------------------+
| global symbols' declarations
+---------------------------------------------------------------------------------------------------------------------*/

extern const char __main_stack_end[];	// main stack end - imported from linker script

void Reset_Handler(void);				// import the address of Reset_Handler()

/*---------------------------------------------------------------------------------------------------------------------+
| global variables
+---------------------------------------------------------------------------------------------------------------------*/

/// core vector table
const InterruptVector coreVectors[] __attribute__ ((section(".coreVectors"))) =
{
		(InterruptVector)__main_stack_end,		// Main stack end address
		Reset_Handler,							// Reset
		NMI_Handler,							// Non maskable interrupt
		HardFault_Handler,						// All class of fault
		MemManage_Handler,						// Memory management
		BusFault_Handler,						// Pre-fetch fault, memory access fault
		UsageFault_Handler,						// Undefined instruction or illegal state
		__Reserved_0x1C_Handler,				// Reserved 0x1C
		__Reserved_0x20_Handler,				// Reserved 0x20
		__Reserved_0x24_Handler,				// Reserved 0x24
		__Reserved_0x28_Handler,				// Reserved 0x28
		SVC_Handler,							// System service call via SVC instruction
		DebugMon_Handler,						// Debug Monitor
		__Reserved_0x34_Handler,				// Reserved 0x34
		PendSV_Handler,							// Pendable request for system service
		SysTick_Handler,						// System tick timer
};
