/**
 * \file
 * \brief SwapPopQueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SWAPPOPQUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SWAPPOPQUEUEFUNCTOR_HPP_

#include "distortos/internal/synchronization/QueueFunctor.hpp"

#include <utility>

namespace distortos
{

namespace internal
{

/**
 * \brief SwapPopQueueFunctor is a functor used for popping of data from the queue using swap.
 *
 * \tparam T is the type of data popped from the queue
 */

template<typename T>
class SwapPopQueueFunctor : public QueueFunctor
{
public:

	/**
	 * \brief SwapPopQueueFunctor's constructor
	 *
	 * \param [out] value is a reference to object that will be used to return popped value, its contents are swapped
	 * with the value in the queue's storage and destructed when no longer needed
	 */

	constexpr explicit SwapPopQueueFunctor(T& value) :
			value_{value}
	{

	}

	/**
	 * \brief Swaps the element in the queue's storage with the value provided by user and destroys this value when no
	 * longer needed.
	 *
	 * \param [in,out] storage is a pointer to storage with element
	 */

	void operator()(void* const storage) const override
	{
		auto& swappedValue = *reinterpret_cast<T*>(storage);
		using std::swap;
		swap(value_, swappedValue);
		swappedValue.~T();
	}

private:

	/// reference to object that will be used to return popped value
	T& value_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_SWAPPOPQUEUEFUNCTOR_HPP_
