/**
 * \file
 * \brief SpiMasterProxy class header
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERPROXY_HPP_
#define INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERPROXY_HPP_

#include "distortos/devices/communication/SpiMode.hpp"

#include <utility>

namespace distortos
{

namespace devices
{

class SpiDevice;
class SpiDeviceProxy;
class SpiMaster;

/**
 * SpiMasterProxy is a [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard)-like class for locking
 * SpiMaster and for accessing its core functionality.
 *
 * \note Locks are recursive.
 *
 * \ingroup devices
 */

class SpiMasterProxy
{
public:

	/**
	 * \brief SpiMasterProxy's constructor
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \param [in] spiDeviceProxy is a reference to SpiDeviceProxy associated with this proxy
	 */

	explicit SpiMasterProxy(const SpiDeviceProxy& spiDeviceProxy);

	/**
	 * \brief SpiMasterProxy's destructor
	 *
	 * \warning This function must not be called from interrupt context!
	 */

	~SpiMasterProxy();

	/**
	 * \brief Configures parameters of associated SPI master.
	 *
	 * \param [in] mode is the desired SPI mode
	 * \param [in] clockFrequency is the desired clock frequency, Hz
	 * \param [in] wordLength selects word length, bits, [1; 32]
	 * \param [in] lsbFirst selects whether MSB (false) or LSB (true) is transmitted first
	 *
	 * \return pair with return code (0 on success, error code otherwise) and real clock frequency; error codes:
	 * - EBADF - associated SPI device or associated SPI master are not opened;
	 * - error codes returned by SpiMasterLowLevel::configure();
	 */

	std::pair<int, uint32_t> configure(SpiMode mode, uint32_t clockFrequency, uint8_t wordLength, bool lsbFirst) const;

	SpiMasterProxy(const SpiMasterProxy&) = delete;
	SpiMasterProxy& operator=(const SpiMasterProxy&) = delete;

private:

	/**
	 * \return reference to SpiDevice associated with this proxy
	 */

	SpiDevice& getSpiDevice() const;

	/**
	 * \return reference to SpiMaster associated with this proxy
	 */

    SpiMaster& getSpiMaster() const;

	/// reference to SpiDeviceProxy associated with this proxy
	const SpiDeviceProxy& spiDeviceProxy_;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_COMMUNICATION_SPIMASTERPROXY_HPP_
