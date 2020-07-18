/**
 * \file
 * \brief ScopeGuard template class header
 *
 * \author Copyright (C) 2016 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_SCOPEGUARD_HPP_
#define ESTD_SCOPEGUARD_HPP_

#include <utility>

namespace estd
{

/**
 * \brief ScopeGuard template class is a idiom introduced by Andrei Alexandrescu and proposed for C++14 in N3949.
 *
 * http://www.open-std.org/jtc1/sc22/WG21/docs/papers/2014/n3949.pdf
 * http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C
 *
 * \tparam Function is the type of function executed on scope exit
 */

template<typename Function>
class ScopeGuard
{
public:

	/**
	 * \brief ScopeGuard's constructor
	 *
	 * \param [in] function is a rvalue reference to function executed on scope exit
	 */

	constexpr explicit ScopeGuard(Function&& function) noexcept :
			function_{std::move(function)},
			released_{}
	{

	}

	/**
	 * \brief ScopeGuard's move constructor
	 *
	 * \param [in] other is a rvalue reference to ScopeGuard used as source of move
	 */

	ScopeGuard(ScopeGuard&& other) noexcept :
			function_(std::move(other.function_)),
			released_{other.released_}
	{
		other.release();
	}

	/**
	 * \brief ScopeGuard's destructor
	 *
	 * If not already "released", executes bound function.
	 */

	~ScopeGuard() noexcept
	{
		if (released_ == false)
			function_();
	}

	/**
	 * \brief Releases ScopeGuard.
	 *
	 * After this call, the bound function will not be executed when this object goes out of scope.
	 */

	void release() noexcept
	{
		released_ = true;
	}

	ScopeGuard(const ScopeGuard&) = delete;
	ScopeGuard& operator=(const ScopeGuard&) = delete;
	ScopeGuard& operator=(ScopeGuard&&) = delete;

private:

	/// function executed on scope exit
	Function function_;

	/// true if object is released (bound function will not be executed), false otherwise
	bool released_;
};

/**
 * \brief Helper factory function to make ScopeGuard object with deduced template arguments
 *
 * \tparam Function is the type of function executed on scope exit
 *
 * \param [in] function is a rvalue reference to function executed on scope exit
 *
 * \return ScopeGuard object with deduced template arguments
 */

template <typename Function>
ScopeGuard<Function> makeScopeGuard(Function&& function) noexcept
{
	return ScopeGuard<Function>{std::move(function)};
}

}	// namespace estd

#endif	// ESTD_SCOPEGUARD_HPP_
