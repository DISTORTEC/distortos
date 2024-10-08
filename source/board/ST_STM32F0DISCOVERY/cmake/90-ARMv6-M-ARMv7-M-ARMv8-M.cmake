#
# file: cmake/90-ARMv6-M-ARMv7-M-ARMv8-M.cmake
#
# author: Copyright (C) 2018-2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
#
# Automatically generated file - do not edit!
#

set(DISTORTOS_RAW_LINKER_SCRIPT "source/board/ST_STM32F0DISCOVERY/ST_STM32F0DISCOVERY.ld")

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_ARCHITECTURE_ASCENDING_STACK
		OFF)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_ARCHITECTURE_EMPTY_STACK
		OFF)

distortosSetFixedConfiguration(INTEGER
		DISTORTOS_ARCHITECTURE_STACK_ALIGNMENT
		8)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_ARCHITECTURE_ARM
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_ARCHITECTURE_ARMV6_M
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_ARCHITECTURE_ARM_CORTEX_M0
		ON)

distortosSetFixedConfiguration(BOOLEAN
		DISTORTOS_ARCHITECTURE_ARM_CORTEX_M0_R0P0
		ON)

distortosSetConfiguration(INTEGER
		distortos_Architecture_00_Interrupt_stack_size
		1024
		MIN 8
		HELP "Size (in bytes) of \"main\" stack used by core exceptions and interrupts in Handler mode."
		OUTPUT_NAME DISTORTOS_ARCHITECTURE_MAIN_STACK_SIZE)

distortosSetConfiguration(STRING
		distortos_Memory_regions_00_text_vectorTable
		"flash"
		HELP "Memory region for .text.vectorTable section in linker script"
		OUTPUT_NAME DISTORTOS_LD_TEXT_VECTOR_TABLE_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_01_text
		"flash"
		HELP "Memory region for .text section in linker script"
		OUTPUT_NAME DISTORTOS_LD_TEXT_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_02_ARM_exidx
		"flash"
		HELP "Memory region for .ARM.exidx section in linker script"
		OUTPUT_NAME DISTORTOS_LD_ARM_EXIDX_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_03_Main_stack
		"SRAM"
		HELP "Memory region for main stack in linker script"
		OUTPUT_NAME DISTORTOS_LD_MAIN_STACK_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_04_bss
		"SRAM"
		HELP "Memory region for .bss section in linker script"
		OUTPUT_NAME DISTORTOS_LD_BSS_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_05_data_VMA
		"SRAM"
		HELP "VMA memory region for .data section in linker script"
		OUTPUT_NAME DISTORTOS_LD_DATA_VMA_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_06_data_LMA
		"flash"
		HELP "LMA memory region for .data section in linker script"
		OUTPUT_NAME DISTORTOS_LD_DATA_LMA_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_07_noinit
		"SRAM"
		HELP "Memory region for .noinit section in linker script"
		OUTPUT_NAME DISTORTOS_LD_NOINIT_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_08_SRAM_data_LMA
		"flash"
		HELP "LMA memory region for .SRAM.data section in linker script"
		OUTPUT_NAME DISTORTOS_LD_SRAM_DATA_LMA_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_09_Process_stack
		"SRAM"
		HELP "Memory region for process stack in linker script"
		OUTPUT_NAME DISTORTOS_LD_PROCESS_STACK_REGION
		OUTPUT_TYPES BOOLEAN)

distortosSetConfiguration(STRING
		distortos_Memory_regions_10_Heap
		"SRAM"
		HELP "Memory region for heap in linker script"
		OUTPUT_NAME DISTORTOS_LD_HEAP_REGION
		OUTPUT_TYPES BOOLEAN)

if(ARCHITECTURE_NVIC_WWDG_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_WWDG_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_WWDG_ENABLE)

if(ARCHITECTURE_NVIC_PVD_VDDIO2_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_PVD_VDDIO2_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_PVD_VDDIO2_ENABLE)

if(ARCHITECTURE_NVIC_RTC_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_RTC_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_RTC_ENABLE)

if(ARCHITECTURE_NVIC_FLASH_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_FLASH_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_FLASH_ENABLE)

if(ARCHITECTURE_NVIC_RCC_CRS_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_RCC_CRS_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_RCC_CRS_ENABLE)

if(ARCHITECTURE_NVIC_EXTI0_1_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_EXTI0_1_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_EXTI0_1_ENABLE)

if(ARCHITECTURE_NVIC_EXTI2_3_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_EXTI2_3_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_EXTI2_3_ENABLE)

if(ARCHITECTURE_NVIC_EXTI4_15_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_EXTI4_15_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_EXTI4_15_ENABLE)

if(ARCHITECTURE_NVIC_TSC_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TSC_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TSC_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH1_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_DMA1_CH1_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_DMA1_CH1_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH2_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH2_3_DMA2_CH1_2_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA1_CH2_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH3_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH2_3_DMA2_CH1_2_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA1_CH3_ENABLE)

if(ARCHITECTURE_NVIC_DMA2_CH1_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH2_3_DMA2_CH1_2_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA2_CH1_ENABLE)

if(ARCHITECTURE_NVIC_DMA2_CH2_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH2_3_DMA2_CH1_2_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA2_CH2_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH2_3_DMA2_CH1_2_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_DMA1_CH2_3_DMA2_CH1_2_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_DMA1_CH2_3_DMA2_CH1_2_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH4_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA1_CH4_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH5_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA1_CH5_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH6_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA1_CH6_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH7_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA1_CH7_ENABLE)

if(ARCHITECTURE_NVIC_DMA2_CH3_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA2_CH3_ENABLE)

if(ARCHITECTURE_NVIC_DMA2_CH4_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA2_CH4_ENABLE)

if(ARCHITECTURE_NVIC_DMA2_CH5_ENABLE)
	set(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE ON)
endif(ARCHITECTURE_NVIC_DMA2_CH5_ENABLE)

if(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_DMA1_CH4_7_DMA2_CH3_5_ENABLE)

if(ARCHITECTURE_NVIC_ADC1_COMP_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_ADC1_COMP_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_ADC1_COMP_ENABLE)

if(ARCHITECTURE_NVIC_TIM1_BRK_UP_TRG_COM_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM1_BRK_UP_TRG_COM_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM1_BRK_UP_TRG_COM_ENABLE)

if(ARCHITECTURE_NVIC_TIM1_CC_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM1_CC_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM1_CC_ENABLE)

if(ARCHITECTURE_NVIC_TIM2_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM2_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM2_ENABLE)

if(ARCHITECTURE_NVIC_TIM3_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM3_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM3_ENABLE)

if(ARCHITECTURE_NVIC_TIM6_DAC_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM6_DAC_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM6_DAC_ENABLE)

if(ARCHITECTURE_NVIC_TIM7_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM7_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM7_ENABLE)

if(ARCHITECTURE_NVIC_TIM14_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM14_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM14_ENABLE)

if(ARCHITECTURE_NVIC_TIM15_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM15_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM15_ENABLE)

if(ARCHITECTURE_NVIC_TIM16_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM16_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM16_ENABLE)

if(ARCHITECTURE_NVIC_TIM17_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_TIM17_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_TIM17_ENABLE)

if(ARCHITECTURE_NVIC_I2C1_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_I2C1_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_I2C1_ENABLE)

if(ARCHITECTURE_NVIC_I2C2_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_I2C2_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_I2C2_ENABLE)

if(ARCHITECTURE_NVIC_SPI1_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_SPI1_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_SPI1_ENABLE)

if(ARCHITECTURE_NVIC_SPI2_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_SPI2_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_SPI2_ENABLE)

if(ARCHITECTURE_NVIC_USART1_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_USART1_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_USART1_ENABLE)

if(ARCHITECTURE_NVIC_USART2_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_USART2_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_USART2_ENABLE)

if(ARCHITECTURE_NVIC_USART3_ENABLE)
	set(ARCHITECTURE_NVIC_USART3_8_ENABLE ON)
endif(ARCHITECTURE_NVIC_USART3_ENABLE)

if(ARCHITECTURE_NVIC_USART4_ENABLE)
	set(ARCHITECTURE_NVIC_USART3_8_ENABLE ON)
endif(ARCHITECTURE_NVIC_USART4_ENABLE)

if(ARCHITECTURE_NVIC_USART5_ENABLE)
	set(ARCHITECTURE_NVIC_USART3_8_ENABLE ON)
endif(ARCHITECTURE_NVIC_USART5_ENABLE)

if(ARCHITECTURE_NVIC_USART6_ENABLE)
	set(ARCHITECTURE_NVIC_USART3_8_ENABLE ON)
endif(ARCHITECTURE_NVIC_USART6_ENABLE)

if(ARCHITECTURE_NVIC_USART7_ENABLE)
	set(ARCHITECTURE_NVIC_USART3_8_ENABLE ON)
endif(ARCHITECTURE_NVIC_USART7_ENABLE)

if(ARCHITECTURE_NVIC_USART8_ENABLE)
	set(ARCHITECTURE_NVIC_USART3_8_ENABLE ON)
endif(ARCHITECTURE_NVIC_USART8_ENABLE)

if(ARCHITECTURE_NVIC_USART3_8_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_USART3_8_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_USART3_8_ENABLE)

if(ARCHITECTURE_NVIC_CEC_CAN_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_CEC_CAN_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_CEC_CAN_ENABLE)

if(ARCHITECTURE_NVIC_USB_ENABLE)
	distortosSetFixedConfiguration(BOOLEAN
			DISTORTOS_ARCHITECTURE_NVIC_USB_ENABLE
			ON)
endif(ARCHITECTURE_NVIC_USB_ENABLE)

include("${CMAKE_CURRENT_SOURCE_DIR}/source/architecture/ARM/ARMv6-M-ARMv7-M-ARMv8-M/distortos-sources.cmake")
