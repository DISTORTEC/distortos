/**
 * \file
 * \brief SimpleFeedablePool class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-07
 */

#ifndef INCLUDE_DISTORTOS_ALLOCATORS_SIMPLEFEEDABLEPOOL_HPP_
#define INCLUDE_DISTORTOS_ALLOCATORS_SIMPLEFEEDABLEPOOL_HPP_

#include <cstddef>

namespace distortos
{

namespace allocators
{

/**
 * \brief SimpleFeedablePool is a very simple "feedable" pool of memory.
 *
 * It's meant to be used as a pool of memory for single object in the container. It should be fed just before something
 * is inserted to the container. Deallocation is a "no-op" - there's nothing to be done.
 */

class SimpleFeedablePool
{
public:

	/**
	 * \brief SimpleFeedablePool's constructor
	 */

	constexpr SimpleFeedablePool() :
			storage_{},
			size_{}
	{

	}

	/**
	 * \brief Allocates chunk of raw memory.
	 *
	 * If no storage is available in the pool, abort() is called.
	 *
	 * \param [in] size is the size of requested storage, bytes
	 *
	 * \return pointer to raw allocated storage
	 */

    void* allocate(size_t size);

	/**
	 * \brief Deallocates chunk of raw memory.
	 *
	 * \param [in] storage is a pointer to raw storage
	 * \param [in] size is the size of storage, bytes
	 */

    void deallocate(void*, size_t) const
    {

    }

	/**
	 * \brief Feeds chunk of raw storage to pool.
	 *
	 * \param [in] storage is a pointer to raw storage
	 * \param [in] size is the size of storage, bytes
	 */

	void feed(void* const storage, const size_t size)
	{
		storage_ = storage;
		size_ = size;
	}

	/**
	 * \brief Template interface for feed().
	 *
	 * \param T is the type of object fed into the pool as storage
	 *
	 * \param [in] storage is a reference to raw storage
	 */

	template<typename T>
	void feed(T& storage)
	{
		feed(&storage, sizeof(storage));
	}

	SimpleFeedablePool(const SimpleFeedablePool&) = delete;
	SimpleFeedablePool& operator=(const SimpleFeedablePool&) = delete;

private:

	/// pointer to raw storage
	void* storage_;

	/// size of storage, bytes
	size_t size_;
};

}	// namespace allocators

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ALLOCATORS_SIMPLEFEEDABLEPOOL_HPP_
