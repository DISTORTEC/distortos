/**
 * \file
 * \brief apply() header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_APPLY_HPP_
#define ESTD_APPLY_HPP_

#include "estd/IntegerSequence.hpp"
#include "estd/invoke.hpp"

#include <tuple>

namespace estd
{

namespace internal
{

/**
 * \brief Implementation of apply()
 *
 * \tparam Function is the function object that will be invoked
 * \tparam Tuple is the type of tuple of arguments
 * \tparam Indexes is a sequence of std::size_t indexes for \a Tuple
 *
 * \param [in] function is the function object that will be executed
 * \param [in] tuple is the tuple of arguments
 *
 * \return value returned by call to \a function with arguments from \a tuple
 */

template<typename Function, typename Tuple, std::size_t... Indexes>
constexpr auto apply(Function&& function, Tuple&& tuple, estd::IndexSequence<Indexes...>) ->
		decltype(estd::invoke(std::forward<Function>(function), std::get<Indexes>(std::forward<Tuple>(tuple))...))
{
	return estd::invoke(std::forward<Function>(function), std::get<Indexes>(std::forward<Tuple>(tuple))...);
}

} // namespace internal

/**
 * \brief Invokes callable object with a tuple of arguments.
 *
 * Implementation inspired by http://en.cppreference.com/w/cpp/experimental/apply
 *
 * \tparam Function is the function object that will be invoked
 * \tparam Tuple is the type of tuple of arguments
 *
 * \param [in] function is the function object that will be executed
 * \param [in] tuple is the tuple of arguments
 *
 * \return value returned by call to \a function with arguments from \a tuple
 */

template <typename Function, typename Tuple>
constexpr auto apply(Function&& function, Tuple&& tuple) ->
		decltype(internal::apply(std::forward<Function>(function), std::forward<Tuple>(tuple),
				estd::MakeIndexSequence<std::tuple_size<typename std::decay<Tuple>::type>{}>{}))
{
	return internal::apply(std::forward<Function>(function), std::forward<Tuple>(tuple),
			estd::MakeIndexSequence<std::tuple_size<typename std::decay<Tuple>::type>{}>{});
}

}	// namespace estd

#endif	// ESTD_APPLY_HPP_
