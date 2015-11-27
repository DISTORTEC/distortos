/**
 * \file
 * \brief FeedablePool class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-11-27
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_ALLOCATORS_FEEDABLEPOOL_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_ALLOCATORS_FEEDABLEPOOL_HPP_

#include "distortos/internal/allocators/SimpleFeedablePool.hpp"

namespace distortos
{

namespace allocators
{

/// FeedablePool is a modification of SimpleFeedablePool that feeds deallocated storage back to pool.
class FeedablePool : public SimpleFeedablePool
{
public:

	/// import SimpleFeedablePool's constructor
	using SimpleFeedablePool::SimpleFeedablePool;

	/**
	 * \brief Deallocates chunk of raw memory.
	 *
	 * \param [in] storage is a pointer to raw storage
	 * \param [in] size is the size of storage, bytes
	 */

	void deallocate(void* const storage, const size_t size)
	{
		feed(storage, size);
	}
};

}	// namespace allocators

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_ALLOCATORS_FEEDABLEPOOL_HPP_
