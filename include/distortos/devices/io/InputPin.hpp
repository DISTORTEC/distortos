/**
 * \file
 * \brief InputPin class header
 *
 * \author Copyright (C) 2016-2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_DEVICES_IO_INPUTPIN_HPP_
#define INCLUDE_DISTORTOS_DEVICES_IO_INPUTPIN_HPP_

namespace distortos
{

namespace devices
{

/**
 * InputPin class is an interface for single input pin
 *
 * \ingroup devices
 */

class InputPin
{
public:

	/**
	 * \brief InputPin's destructor
	 */

	virtual ~InputPin() = default;

	/**
	 * \return current state of pin
	 */

	virtual bool get() const = 0;
};

}	// namespace devices

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_DEVICES_IO_INPUTPIN_HPP_
