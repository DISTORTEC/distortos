/**
 * \file
 * \brief SdmmcPeripheral class header for SDMMCv1 in STM32
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SDMMCV1_INCLUDE_DISTORTOS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SDMMCV1_INCLUDE_DISTORTOS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_

#include "distortos/chip/getBusFrequency.hpp"

namespace distortos
{

namespace chip
{

/// SdmmcPeripheral class is a raw SDMMC peripheral for SDMMCv1 in STM32
class SdmmcPeripheral
{
public:

	/**
	 * \brief SdmmcPeripheral's constructor
	 *
	 * \param [in] sdmmcBase is a base address of SDMMC peripheral
	 * \param [in] adapterFrequency is the SDMMC adapter clock frequency, Hz
	 */

	constexpr SdmmcPeripheral(const uintptr_t sdmmcBase, const uint32_t adapterFrequency) :
			sdmmcBase_{sdmmcBase},
			adapterFrequency_{adapterFrequency},
			peripheralFrequency_{getBusFrequency(sdmmcBase)}
	{

	}

	/**
	 * \return adapter clock frequency, Hz
	 */

	uint32_t getAdapterFrequency() const
	{
		return adapterFrequency_;
	}

	/**
	 * \return address of FIFO register
	 */

	uintptr_t getFifoAddress() const
	{
		return reinterpret_cast<uintptr_t>(&getSdmmc().FIFO);
	}

	/**
	 * \return peripheral clock frequency, Hz
	 */

	uint32_t getPeripheralFrequency() const
	{
		return peripheralFrequency_;
	}

	/**
	 * \return current value of POWER register
	 */

	uint32_t readPower() const
	{
		return getSdmmc().POWER;
	}

	/**
	 * \return current value of CLKCR register
	 */

	uint32_t readClkcr() const
	{
		return getSdmmc().CLKCR;
	}

	/**
	 * \return current value of ARG register
	 */

	uint32_t readArg() const
	{
		return getSdmmc().ARG;
	}

	/**
	 * \return current value of CMD register
	 */

	uint32_t readCmd() const
	{
		return getSdmmc().CMD;
	}

	/**
	 * \return current value of RESPCMD register
	 */

	uint32_t readRespcmd() const
	{
		return getSdmmc().RESPCMD;
	}

	/**
	 * \return current value of RESP1 register
	 */

	uint32_t readResp1() const
	{
		return getSdmmc().RESP1;
	}

	/**
	 * \return current value of RESP2 register
	 */

	uint32_t readResp2() const
	{
		return getSdmmc().RESP2;
	}

	/**
	 * \return current value of RESP3 register
	 */

	uint32_t readResp3() const
	{
		return getSdmmc().RESP3;
	}

	/**
	 * \return current value of RESP4 register
	 */

	uint32_t readResp4() const
	{
		return getSdmmc().RESP4;
	}

	/**
	 * \return current value of DTIMER register
	 */

	uint32_t readDtimer() const
	{
		return getSdmmc().DTIMER;
	}

	/**
	 * \return current value of DLEN register
	 */

	uint32_t readDlen() const
	{
		return getSdmmc().DLEN;
	}

	/**
	 * \return current value of DCTRL register
	 */

	uint32_t readDctrl() const
	{
		return getSdmmc().DCTRL;
	}

	/**
	 * \return current value of DCOUNT register
	 */

	uint32_t readDcount() const
	{
		return getSdmmc().DCOUNT;
	}

	/**
	 * \return current value of STA register
	 */

	uint32_t readSta() const
	{
		return getSdmmc().STA;
	}

	/**
	 * \return current value of ICR register
	 */

	uint32_t readIcr() const
	{
		return getSdmmc().ICR;
	}

	/**
	 * \return current value of MASK register
	 */

	uint32_t readMask() const
	{
		return getSdmmc().MASK;
	}

	/**
	 * \return current value of FIFOCNT register
	 */

	uint32_t readFifocnt() const
	{
		return getSdmmc().FIFOCNT;
	}

	/**
	 * \return current value of FIFO register
	 */

	uint32_t readFifo() const
	{
		return getSdmmc().FIFO;
	}

	/**
	 * \brief Writes value to POWER register.
	 *
	 * \param [in] power is the value that will be written to POWER register
	 */

	void writePower(const uint32_t power) const
	{
		getSdmmc().POWER = power;
	}

	/**
	 * \brief Writes value to CLKCR register.
	 *
	 * \param [in] clkcr is the value that will be written to CLKCR register
	 */

	void writeClkcr(const uint32_t clkcr) const
	{
		getSdmmc().CLKCR = clkcr;
	}

	/**
	 * \brief Writes value to ARG register.
	 *
	 * \param [in] arg is the value that will be written to ARG register
	 */

	void writeArg(const uint32_t arg) const
	{
		getSdmmc().ARG = arg;
	}

	/**
	 * \brief Writes value to CMD register.
	 *
	 * \param [in] cmd is the value that will be written to CMD register
	 */

	void writeCmd(const uint32_t cmd) const
	{
		getSdmmc().CMD = cmd;
	}

	/**
	 * \brief Writes value to DTIMER register.
	 *
	 * \param [in] dtimer is the value that will be written to DTIMER register
	 */

	void writeDtimer(const uint32_t dtimer) const
	{
		getSdmmc().DTIMER = dtimer;
	}

	/**
	 * \brief Writes value to DLEN register.
	 *
	 * \param [in] dlen is the value that will be written to DLEN register
	 */

	void writeDlen(const uint32_t dlen) const
	{
		getSdmmc().DLEN = dlen;
	}

	/**
	 * \brief Writes value to DCTRL register.
	 *
	 * \param [in] dctrl is the value that will be written to DCTRL register
	 */

	void writeDctrl(const uint32_t dctrl) const
	{
		getSdmmc().DCTRL = dctrl;
	}

	/**
	 * \brief Writes value to ICR register.
	 *
	 * \param [in] icr is the value that will be written to ICR register
	 */

	void writeIcr(const uint32_t icr) const
	{
		getSdmmc().ICR = icr;
	}

	/**
	 * \brief Writes value to MASK register.
	 *
	 * \param [in] mask is the value that will be written to MASK register
	 */

	void writeMask(const uint32_t mask) const
	{
		getSdmmc().MASK = mask;
	}

	/**
	 * \brief Writes value to FIFO register.
	 *
	 * \param [in] fifo is the value that will be written to FIFO register
	 */

	void writeFifo(const uint32_t fifo) const
	{
		getSdmmc().FIFO = fifo;
	}

private:

	/**
	 * \return reference to SDMMC_TypeDef object
	 */

	SDMMC_TypeDef& getSdmmc() const
	{
		return *reinterpret_cast<SDMMC_TypeDef*>(sdmmcBase_);
	}

	/// base address of SDMMC peripheral
	uintptr_t sdmmcBase_;

	/// adapter clock frequency, Hz
	uint32_t adapterFrequency_;

	/// peripheral clock frequency, Hz
	uint32_t peripheralFrequency_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SDMMCV1_INCLUDE_DISTORTOS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_
