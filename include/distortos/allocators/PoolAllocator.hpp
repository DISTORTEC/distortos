/**
 * \file
 * \brief PoolAllocator class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-07
 */

#ifndef INCLUDE_DISTORTOS_ALLOCATORS_POOLALLOCATOR_HPP_
#define INCLUDE_DISTORTOS_ALLOCATORS_POOLALLOCATOR_HPP_

#include "distortos/allocators/AllocatorBase.hpp"

namespace distortos
{

namespace allocators
{

/**
 * \brief PoolAllocator class is a generic allocator which uses pool.
 *
 * \param T is the allocated type
 * \param PoolType is the type of pool used by allocator
 */

template<typename T, typename PoolType>
class PoolAllocator : public AllocatorBase<T>
{
public:

	/// alias of PoolType template parameter
	using Pool = PoolType;

	/// base of PoolAllocator
	using Base = AllocatorBase<T>;

	using typename Base::value_type;
	using typename Base::pointer;
	using typename Base::const_pointer;
	using typename Base::reference;
	using typename Base::const_reference;
	using typename Base::size_type;

	/**
	 * \brief Rebinds allocator to different type.
	 *
	 * \param U is the allocated type
	 */

	template<typename U>
	struct rebind
	{
		/// type of rebound allocator
		using other = PoolAllocator<U, Pool>;
	};

	/**
	 * \brief PoolAllocator's constructor
	 *
	 * \param [in] pool is a reference to pool used by allocator
	 */

	constexpr explicit PoolAllocator(Pool& pool) :
			pool_(pool)
	{

	}

	/**
	 * \brief PoolAllocator's copy constructor
	 *
	 * \param U is the allocated type
	 *
	 * \param [in] other is a reference to PoolAllocator which will be copied
	 */

	template<typename U>
	constexpr explicit PoolAllocator(const PoolAllocator<U, Pool>& other) :
			pool_(other.pool_)
	{

	}

	/**
	 * \brief Allocates raw storage.
	 *
	 * \param [in] size is the number of elements (each of size sizeof(value_type)) to be allocated
	 * \param [in] hint is the hint that may be used to improve locality
	 *
	 * \return pointer to allocated storage
	 */

	pointer allocate(size_type size, const void* = nullptr)
	{
		return static_cast<T*>(pool_.allocate(size * sizeof(T)));
	}

	/**
	 * \brief Deallocates raw storage.
	 *
	 * \param [in] storage is the pointer to deallocated storage
	 * \param [in] size is the number of elements (each of size sizeof(value_type)) to be deallocated
	 */

	void deallocate(pointer storage, size_type size)
	{
		pool_.deallocate(storage, size);
	}

private:

	/// reference to pool used by allocator
	Pool& pool_;

	template<typename U, typename OtherPoolType>
	friend class PoolAllocator;

	template<typename T1, typename T2, typename OtherPoolType>
	friend bool operator==(const PoolAllocator<T1, OtherPoolType>& left, const PoolAllocator<T2, OtherPoolType>& right);
};

/**
 * \brief PoolAllocator operator==
 *
 * \param T1 is the type allocated by left PoolAllocator
 * \param T2 is the type allocated by right PoolAllocator
 * \param OtherPoolType is the type of pool used by both allocators
 *
 * \param [in] left is a reference to left PoolAllocator
 * \param [in] right is a reference to right PoolAllocator
 *
 * \return true if the allocators are equal, false otherwise
 */

template<typename T1, typename T2, typename OtherPoolType>
inline bool operator==(const PoolAllocator<T1, OtherPoolType>& left, const PoolAllocator<T2, OtherPoolType>& right)
{
    return &left.pool_ == &right.pool_;
}

/**
 * \brief PoolAllocator operator!=
 *
 * \param T1 is the type allocated by left PoolAllocator
 * \param T2 is the type allocated by right PoolAllocator
 * \param OtherPoolType is the type of pool used by both allocators
 *
 * \param [in] left is a reference to left PoolAllocator
 * \param [in] right is a reference to right PoolAllocator
 *
 * \return true if the allocators are not equal, false otherwise
 */

template<typename T1, typename T2, typename OtherPoolType>
inline bool operator!=(const PoolAllocator<T1, OtherPoolType>& left, const PoolAllocator<T2, OtherPoolType>& right)
{
    return !(left == right);
}

}	// namespace allocators

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ALLOCATORS_POOLALLOCATOR_HPP_
