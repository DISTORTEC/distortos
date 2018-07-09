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

int SpiEeprom::close()
{
	return spiDevice_.close();
}

int SpiEeprom::erase(const uint64_t address, const uint64_t size)
{
	return Proxy{*this}.erase(address, size);
}

std::pair<int, bool> SpiEeprom::isWriteInProgress()
{
	return Proxy{*this}.isWriteInProgress();
}

int SpiEeprom::open()
{
	return spiDevice_.open();
}

std::pair<int, size_t> SpiEeprom::read(const uint32_t address, void* const buffer, const size_t size)
{
	return Proxy{*this}.read(address, buffer, size);
}

int SpiEeprom::waitWhileWriteInProgress()
{
	return Proxy{*this}.waitWhileWriteInProgress();
}

std::pair<int, size_t> SpiEeprom::write(const uint32_t address, const void* const buffer, const size_t size)
{
	return Proxy{*this}.write(address, buffer, size);
}

}	// namespace devices

}	// namespace distortos
