/**
 * \file
 * \brief OnceFlag class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_ONCEFLAG_HPP_
#define INCLUDE_DISTORTOS_ONCEFLAG_HPP_

#include "distortos/Mutex.hpp"

namespace distortos
{

/**
 * \brief OnceFlag is a helper class for callOnce().
 *
 * Similar to std::once_flag - https://en.cppreference.com/w/cpp/thread/once_flag
 * Similar to POSIX pthread_once_t - http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_once.html#
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
			mutex_{},
			done_{}
	{

	}

private:

	/// internal mutex used by callOnce()
	Mutex mutex_;

	/// tells whether any function was already called for this object (true) or not (false)
	volatile bool done_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ONCEFLAG_HPP_
