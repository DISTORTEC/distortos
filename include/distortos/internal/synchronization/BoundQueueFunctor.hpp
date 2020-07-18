/**
 * \file
 * \brief BoundQueueFunctor class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_BOUNDQUEUEFUNCTOR_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_BOUNDQUEUEFUNCTOR_HPP_

#include "distortos/internal/synchronization/QueueFunctor.hpp"

#include <utility>

namespace distortos
{

namespace internal
{

/**
 * \brief BoundQueueFunctor is a type-erased QueueFunctor which calls its bound functor to execute actions on queue's
 * storage
 *
 * \tparam F is the type of bound functor, it will be called with <em>void*</em> as only argument
 */

template<typename F>
class BoundQueueFunctor : public QueueFunctor
{
public:

	/**
	 * \brief BoundQueueFunctor's constructor
	 *
	 * \param [in] boundFunctor is a rvalue reference to bound functor which will be used to move-construct internal
	 * bound functor
	 */

	constexpr explicit BoundQueueFunctor(F&& boundFunctor) :
			boundFunctor_{std::move(boundFunctor)}
	{

	}

	/**
	 * \brief Calls the bound functor which will execute some action on queue's storage (like copy-constructing,
	 * swapping, destroying, emplacing, ...)
	 *
	 * \param [in,out] storage is a pointer to storage with/for element
	 */

	void operator()(void* const storage) const override
	{
		boundFunctor_(storage);
	}

private:

	/// bound functor
	F boundFunctor_;
};

/**
 * \brief Helper factory function to make BoundQueueFunctor object with deduced template arguments
 *
 * \tparam F is the type of bound functor, it will be called with <em>void*</em> as only argument
 *
 * \param [in] boundFunctor is a rvalue reference to bound functor which will be used to move-construct returned object
 *
 * \return BoundQueueFunctor object with deduced template arguments
 */

template<typename F>
constexpr BoundQueueFunctor<F> makeBoundQueueFunctor(F&& boundFunctor)
{
	return BoundQueueFunctor<F>{std::move(boundFunctor)};
}

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_BOUNDQUEUEFUNCTOR_HPP_
