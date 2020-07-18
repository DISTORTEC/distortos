/**
 * \file
 * \brief MemcpyPushQueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MEMCPYPUSHQUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MEMCPYPUSHQUEUEFUNCTOR_HPP_

#include "distortos/internal/synchronization/QueueFunctor.hpp"

#include <cstddef>

namespace distortos
{

namespace internal
{

/// MemcpyPushQueueFunctor is a functor used for pushing of data to the raw queue with memcpy()
class MemcpyPushQueueFunctor : public QueueFunctor
{
public:

	/**
	 * \brief MemcpyPushQueueFunctor's constructor
	 *
	 * \param [in] data is a pointer to data that will be pushed to raw queue
	 * \param [in] size is the size of \a data, bytes
	 */

	constexpr MemcpyPushQueueFunctor(const void* const data, const size_t size) :
			data_{data},
			size_{size}
	{

	}

	/**
	 * \brief Copies the data to raw queue's storage (with memcpy()).
	 *
	 * \param [in,out] storage is a pointer to storage for element
	 */

	void operator()(void* storage) const override;

private:

	/// pointer to data that will be pushed to raw queue
	const void* const data_;

	/// size of \a data_, bytes
	const size_t size_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MEMCPYPUSHQUEUEFUNCTOR_HPP_
