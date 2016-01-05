/**
 * \file
 * \brief OnceFlag class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ONCEFLAG_HPP_
#define INCLUDE_DISTORTOS_ONCEFLAG_HPP_

#include "distortos/internal/synchronization/CallOnceControlBlock.hpp"

#if DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1

namespace distortos
{

/**
 * \brief OnceFlag is a helper class for callOnce().
 *
 * Similar to std::once_flag - http://en.cppreference.com/w/cpp/thread/once_flag
 * Similar to POSIX pthread_once_t - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_once.html#
 *
 * \note This class requires GCC 4.9.
 *
 * \ingroup synchronization
 */

class OnceFlag
{
	template<typename Function, typename... Args>
	friend void callOnce(OnceFlag& onceFlag, Function&& function, Args&&... args);

public:

	/**
	 * \brief OnceFlag's constructor
	 */

	constexpr OnceFlag() :
			callOnceControlBlock_{}
	{

	}

private:

	/// internal internal::CallOnceControlBlock object used by callOnce()
	internal::CallOnceControlBlock callOnceControlBlock_;
};

}	// namespace distortos

#endif	// DISTORTOS_CALLONCE_SUPPORTED == 1 || DOXYGEN == 1

#endif	// INCLUDE_DISTORTOS_ONCEFLAG_HPP_
