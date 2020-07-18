/**
 * \file
 * \brief MoveConstructQueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MOVECONSTRUCTQUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MOVECONSTRUCTQUEUEFUNCTOR_HPP_

#include "distortos/internal/synchronization/QueueFunctor.hpp"

#include <utility>

namespace distortos
{

namespace internal
{

/**
 * \brief MoveConstructQueueFunctor is a functor used for pushing of data to the queue using move-construction.
 *
 * \tparam T is the type of data pushed to the queue
 */

template<typename T>
class MoveConstructQueueFunctor : public QueueFunctor
{
public:

	/**
	 * \brief MoveConstructQueueFunctor's constructor
	 *
	 * \param [in] value is a rvalue reference to object that will be used as argument of move constructor
	 */

	constexpr explicit MoveConstructQueueFunctor(T&& value) :
			value_{std::move(value)}
	{

	}

	/**
	 * \brief Move-constructs the element in the queue's storage
	 *
	 * \param [in,out] storage is a pointer to storage for element
	 */

	void operator()(void* const storage) const override
	{
		new (storage) T{std::move(value_)};
	}

private:

	/// rvalue reference to object that will be used as argument of move constructor
	T&& value_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_MOVECONSTRUCTQUEUEFUNCTOR_HPP_
