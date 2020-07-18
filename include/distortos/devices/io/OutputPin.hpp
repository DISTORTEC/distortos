/**
 * \file
 * \brief OutputPin class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_IO_OUTPUTPIN_HPP_
#define INCLUDE_DISTORTOS_DEVICES_IO_OUTPUTPIN_HPP_

#include "distortos/devices/io/InputPin.hpp"

namespace distortos
{

namespace devices
{

/**
 * \brief OutputPin class is an interface for single output pin.
 *
 * \ingroup devices
 */

class OutputPin : public InputPin
{
public:

	/**
	 * \brief Sets state of pin.
	 *
	 * \param [in] state is the new state of pin
	 */

	virtual void set(bool state) = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_IO_OUTPUTPIN_HPP_
