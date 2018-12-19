/**
 * \file
 * \brief DmaChannelFunctor class header for DMA in STM32
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELFUNCTOR_HPP_
#define SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELFUNCTOR_HPP_

#include <cstddef>

namespace distortos
{

namespace chip
{

/**
 * DmaChannelFunctor class is a functor for low-level DMA channel driver in STM32
 *
 * \ingroup devices
 */

class DmaChannelFunctor
{
public:

	/**
	 * \brief DmaChannelFunctor's destructor
	 */

	virtual ~DmaChannelFunctor() = default;

	/**
	 * \brief "Transfer complete" event
	 *
	 * Called by low-level DMA channel driver when the transfer is physically finished.
	 */

	virtual void transferCompleteEvent() = 0;

	/**
	 * \brief "Transfer error" event
	 *
	 * Called by low-level DMA channel driver when transfer error is detected.
	 *
	 * \param [in] transactionsLeft is the number transactions left
	 */

	virtual void transferErrorEvent(size_t transactionsLeft) = 0;
};

}	// namespace chip

}	// namespace distortos

#endif	// SOURCE_CHIP_STM32_INCLUDE_DISTORTOS_CHIP_DMACHANNELFUNCTOR_HPP_
