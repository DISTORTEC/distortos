/**
 * \file
 * \brief ChipSpiMasterLowLevel class header for SPIv1 in STM32
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_CHIPSPIMASTERLOWLEVEL_HPP_
#define SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_CHIPSPIMASTERLOWLEVEL_HPP_

#include "distortos/devices/communication/SpiMasterLowLevel.hpp"

namespace distortos
{

namespace chip
{

class SpiPeripheral;

/**
 * ChipSpiMasterLowLevel class is a low-level SPI master driver for SPIv1 in STM32
 *
 * \ingroup devices
 */

class ChipSpiMasterLowLevel : public devices::SpiMasterLowLevel
{
public:

	/**
	 * \brief ChipSpiMasterLowLevel's constructor
	 *
	 * \param [in] spiPeripheral is a reference to raw SPI peripheral
	 */

	constexpr explicit ChipSpiMasterLowLevel(const SpiPeripheral& spiPeripheral) :
			spiPeripheral_{spiPeripheral},
			spiMasterBase_{},
			readBuffer_{},
			writeBuffer_{},
			size_{},
			readPosition_{},
			writePosition_{},
			dummyData_{},
			started_{}
	{

	}

	/**
	 * \brief SpiMasterLowLevel's destructor
	 *
	 * Does nothing if driver is already stopped. If it's not, performs forced stop of operation.
	 */

	~ChipSpiMasterLowLevel() override;

	/**
	 * \brief Configures parameters of low-level SPI master driver.
	 *
	 * \param [in] mode is the desired SPI mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz
	 * \param [in] wordLength selects word length, bits, {8, 16}
	 * \param [in] lsbFirst selects whether MSB (false) or LSB (true) is transmitted first
	 * \param [in] dummyData is the dummy data that will be sent if write buffer of transfer is nullptr
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real clock frequency; error codes:
	 * - EBADF - the driver is not started;
	 * - EBUSY - transfer is in progress;
	 * - EINVAL - selected SPI mode and/or clock frequency and/or format are invalid;
	 */

	std::pair<int, uint32_t> configure(devices::SpiMode mode, uint32_t clockFrequency, uint8_t wordLength,
			bool lsbFirst, uint32_t dummyData) override;

	/**
	 * \brief Interrupt handler
	 *
	 * \note this must not be called by user code
	 */

	void interruptHandler();

	/**
	 * \brief Starts low-level SPI master driver.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not stopped;
	 */

	int start() override;

	/**
	 * \brief Starts asynchronous transfer.
	 *
	 * This function returns immediately. When the transfer is physically finished (either expected number of bytes were
	 * written and read or an error was detected), SpiMasterBase::transferCompleteEvent() will be executed.
	 *
	 * \param [in] spiMasterBase is a reference to SpiMasterBase object that will be notified about completed transfer
	 * \param [in] writeBuffer is the buffer with data that will be written, nullptr to send dummy data
	 * \param [out] readBuffer is the buffer with data that will be read, nullptr to ignore received data
	 * \param [in] size is the size of transfer (size of \a writeBuffer and/or \a readBuffer), bytes, must be even if
	 * number of data bits is in range (8; 16], divisible by 3 if number of data bits is in range (16; 24] or divisible
	 * by 4 if number of data bits is in range (24; 32]
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not started;
	 * - EBUSY - transfer is in progress;
	 * - EINVAL - \a size is invalid;
	 */

	int startTransfer(devices::SpiMasterBase& spiMasterBase, const void* writeBuffer, void* readBuffer,
			size_t size) override;

	/**
	 * \brief Stops low-level SPI master driver.
	 *
	 * \return 0 on success, error code otherwise:
	 * - EBADF - the driver is not started;
	 * - EBUSY - transfer is in progress;
	 */

	int stop() override;

private:

	/**
	 * \return true if driver is started, false otherwise
	 */

	bool isStarted() const
	{
		return started_;
	}

	/**
	 * \return true if transfer is in progress, false otherwise
	 */

	bool isTransferInProgress() const
	{
		return size_ != 0;
	}

	/**
	 * \brief Writes next item to SPI peripheral.
	 *
	 * \param [in] wordLength selects word length, bits, {8, 16}
	 */

	void writeNextItem(uint8_t wordLength);

	/// reference to raw SPI peripheral
	const SpiPeripheral& spiPeripheral_;

	/// pointer to SpiMasterBase object associated with this one
	devices::SpiMasterBase* volatile spiMasterBase_;

	/// buffer to which the data is being written, nullptr to ignore received data
	uint8_t* volatile readBuffer_;

	/// buffer with data that is being transmitted, nullptr to send dummy data
	const uint8_t* volatile writeBuffer_;

	/// size of transfer (size of \a readBuffer_ and/or \a writeBuffer_), bytes
	volatile size_t size_;

	/// current position in \a readBuffer_
	volatile size_t readPosition_;

	/// current position in \a writeBuffer_
	volatile size_t writePosition_;

	/// dummy data that will be sent if write buffer of transfer is nullptr
	uint16_t dummyData_;

	/// true if driver is started, false otherwise
	bool started_;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_PERIPHERALS_SPIV1_INCLUDE_DISTORTOS_CHIP_CHIPSPIMASTERLOWLEVEL_HPP_
