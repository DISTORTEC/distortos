/**
 * \file
 * \brief SpiEeprom class implementation
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "distortos/devices/memory/SpiEeprom.hpp"

#include "distortos/devices/memory/SpiEepromProxy.hpp"

namespace distortos
{

namespace devices
{

/*---------------------------------------------------------------------------------------------------------------------+
| public functions
+---------------------------------------------------------------------------------------------------------------------*/

SpiEeprom::~SpiEeprom()
{

}

int SpiEeprom::close()
{
	return spiDevice_.close();
}

int SpiEeprom::erase(const uint64_t address, const uint64_t size)
{
	return Proxy{*this}.erase(address, size);
}

size_t SpiEeprom::getEraseBlockSize() const
{
	return 1;
}

std::pair<bool, uint8_t> SpiEeprom::getErasedValue() const
{
	return {true, UINT8_MAX};
}

size_t SpiEeprom::getProgramBlockSize() const
{
	return 1;
}

size_t SpiEeprom::getReadBlockSize() const
{
	return 1;
}

uint64_t SpiEeprom::getSize() const
{
	return 128 * (1 << ((static_cast<uint8_t>(type_) & sizeMask_) >> sizeShift_));
}

std::pair<int, bool> SpiEeprom::isWriteInProgress()
{
	return Proxy{*this}.isWriteInProgress();
}

int SpiEeprom::lock()
{
	return spiDevice_.lock();
}

int SpiEeprom::open()
{
	return spiDevice_.open();
}

std::pair<int, size_t> SpiEeprom::program(const uint64_t address, const void* const buffer, const size_t size)
{
	return Proxy{*this}.program(address, buffer, size);
}

std::pair<int, size_t> SpiEeprom::read(const uint64_t address, void* const buffer, const size_t size)
{
	return Proxy{*this}.read(address, buffer, size);
}

int SpiEeprom::synchronize()
{
	return Proxy{*this}.synchronize();
}

int SpiEeprom::trim(uint64_t, uint64_t)
{
	return {};
}

int SpiEeprom::unlock()
{
	return spiDevice_.unlock();
}

}	// namespace devices

}	// namespace distortos
