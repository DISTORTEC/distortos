/**
 * \file
 * \brief MemcpyPopQueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MEMCPYPOPQUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MEMCPYPOPQUEUEFUNCTOR_HPP_

#include "distortos/internal/synchronization/QueueFunctor.hpp"

#include <cstddef>

namespace distortos
{

namespace internal
{

/// MemcpyPopQueueFunctor is a functor used for popping of data from the raw queue with memcpy()
class MemcpyPopQueueFunctor : public QueueFunctor
{
public:

	/**
	 * \brief MemcpyPopQueueFunctor's constructor
	 *
	 * \param [out] buffer is a pointer to buffer for popped element
	 * \param [in] size is the size of \a buffer, bytes
	 */

	constexpr MemcpyPopQueueFunctor(void* const buffer, const size_t size) :
			buffer_{buffer},
			size_{size}
	{

	}

	/**
	 * \brief Copies the data from raw queue's storage (with memcpy()).
	 *
	 * \param [in,out] storage is a pointer to storage for element
	 */

	void operator()(void* storage) const override;

private:

	/// pointer to buffer for popped element
	void* const buffer_;

	/// size of \a buffer_, bytes
	const size_t size_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MEMCPYPOPQUEUEFUNCTOR_HPP_
