/**
 * \file
 * \brief DmaChannelFunctorCommon class header for DMA in STM32
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELFUNCTORCOMMON_HPP_
#define SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELFUNCTORCOMMON_HPP_

#include "distortos/chip/DmaChannelFunctor.hpp"

namespace distortos
{

namespace chip
{

/**
 * DmaChannelFunctorCommon class is a DmaChannelFunctor with empty implementations for optional event handlers.
 *
 * \ingroup devices
 */

class DmaChannelFunctorCommon : public DmaChannelFunctor
{
public:

	/**
	 * \brief "Transfer complete" event
	 *
	 * Called by low-level DMA channel driver when the transfer is physically finished.
	 *
	 * Empty default implementation - does nothing.
	 */

	void transferCompleteEvent() override
	{

	}
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELFUNCTORCOMMON_HPP_
