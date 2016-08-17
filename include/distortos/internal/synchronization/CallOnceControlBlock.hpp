/**
 * \file
 * \brief CallOnceControlBlock class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_CALLONCECONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_CALLONCECONTROLBLOCK_HPP_

#include "estd/invoke.hpp"
#include "estd/TypeErasedFunctor.hpp"

#include <sys/features.h>

namespace distortos
{

/// GCC 4.9 is needed for CallOnceControlBlock::operator()() function - and thus for OnceFlag and callOnce() - earlier
/// versions don't support parameter pack expansion in lambdas
#define DISTORTOS_CALLONCE_SUPPORTED	__GNUC_PREREQ(4, 9)

#if DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1

namespace internal
{

class ThreadList;

/// CallOnceControlBlock class implements functionality of OnceFlag class and callOnce()
/// \note This class requires GCC 4.9.
class CallOnceControlBlock
{
public:

	/**
	 * \brief CallOnceControlBlock's constructor
	 */

	constexpr CallOnceControlBlock() :
			blockedList_{},
			done_{}
	{

	}

	/**
	 * \brief CallOnceControlBlock's function call operator
	 *
	 * Does nothing if any function was already called for this object. In other case provided function and arguments
	 * are wrapped in a type-erased functor and passed to callOnceImplementation().
	 *
	 * \tparam Function is the function object that will be executed
	 * \tparam Args are the arguments for \a Function
	 *
	 * \param [in] function is the function object that will be executed
	 * \param [in] args are arguments for \a function
	 */

	template<typename Function, typename... Args>
	void operator()(Function&& function, Args&&... args);

private:

	/// Functor is a type-erased interface for functors which execute bound function with bound arguments
	class Functor : public estd::TypeErasedFunctor<void()>
	{

	};

	/**
	 * \brief BoundFunctor is a type-erased Functor which calls its bound functor
	 *
	 * \tparam F is the type of bound functor
	 */

	template<typename F>
	class BoundFunctor : public Functor
	{
	public:

		/**
		 * \brief BoundFunctor's constructor
		 *
		 * \param [in] boundFunctor is a rvalue reference to bound functor which will be used to move-construct internal
		 * bound functor
		 */

		constexpr explicit BoundFunctor(F&& boundFunctor) :
				boundFunctor_{std::move(boundFunctor)}
		{

		}

		/**
		 * \brief BoundFunctor's function call operator
		 *
		 * Calls the bound functor.
		 */

		void operator()() const override
		{
			boundFunctor_();
		}

	private:

		/// bound functor
		F boundFunctor_;
	};

	/**
	 * \brief Helper factory function to make BoundFunctor object with deduced template arguments
	 *
	 * \tparam F is the type of bound functor
	 *
	 * \param [in] boundFunctor is a rvalue reference to bound functor which will be used to move-construct internal
	 * bound functor
	 *
	 * \return BoundFunctor object with deduced template arguments
	 */

	template<typename F>
	constexpr static BoundFunctor<F> makeBoundFunctor(F&& boundFunctor)
	{
		return BoundFunctor<F>{std::move(boundFunctor)};
	}

	/**
	 * \brief Implements callOnce() using type-erased functor.
	 *
	 * Does nothing if any function was already called for this object. If the function is currently being executed, but
	 * not yet done, then the calling thread is blocked. In other case the function is executed and - after it is done -
	 * all blocked threads are unblocked.
	 *
	 * \param [in] functor is a reference to functor which will execute bound function with bound arguments
	 */

	void callOnceImplementation(const Functor& functor);

	/// pointer to stack-allocated list of ThreadControlBlock objects blocked on associated OnceFlag
	ThreadList* blockedList_;

	/// tells whether any function was already called for this object (true) or not (false)
	bool done_;
};

template<typename Function, typename... Args>
void CallOnceControlBlock::operator()(Function&& function, Args&&... args)
{
	if (done_ == true)	// function already executed?
		return;

	const auto functor = makeBoundFunctor(
			[&function, &args...]()
			{
				estd::invoke(std::forward<Function>(function), std::forward<Args>(args)...);
			});
	callOnceImplementation(functor);
}

}	// namespace internal

#endif	// DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_SYNCHRONIZATION_CALLONCECONTROLBLOCK_HPP_
