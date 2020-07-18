/**
 * \file
 * \brief CopyConstructQueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_COPYCONSTRUCTQUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_COPYCONSTRUCTQUEUEFUNCTOR_HPP_

#include "distortos/internal/synchronization/QueueFunctor.hpp"

namespace distortos
{

namespace internal
{

/**
 * \brief CopyConstructQueueFunctor is a functor used for pushing of data to the queue using copy-construction.
 *
 * \tparam T is the type of data pushed to the queue
 */

template<typename T>
class CopyConstructQueueFunctor : public QueueFunctor
{
public:

	/**
	 * \brief CopyConstructQueueFunctor's constructor
	 *
	 * \param [in] value is a reference to object that will be used as argument of copy constructor
	 */

	constexpr explicit CopyConstructQueueFunctor(const T& value) :
			value_{value}
	{

	}

	/**
	 * \brief Copy-constructs the element in the queue's storage
	 *
	 * \param [in,out] storage is a pointer to storage for element
	 */

	void operator()(void* const storage) const override
	{
		new (storage) T{value_};
	}

private:

	/// reference to object that will be used as argument of copy constructor
	const T& value_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_COPYCONSTRUCTQUEUEFUNCTOR_HPP_
