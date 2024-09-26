/**
 * \file
 * \brief durationCastCeil() header
 *
 * \author Copyright (C) 2024 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_DURATIONCASTCEIL_HPP_
#define ESTD_DURATIONCASTCEIL_HPP_

#include <chrono>

namespace estd
{

/**
 * \brief Tag struct used to enable durationCastCeil() for std::chrono::duration types only
 */

template<typename>
struct isDuration : std::false_type
{

};

/**
 * \brief Tag struct used to enable durationCastCeil() for std::chrono::duration types only
 *
 * Specialization for std::chrono::duration type
 *
 * \tparam Rep is the "Rep" type of the duration
 * \tparam Period is the "Period" type (ratio) of the duration
 */

template<typename Rep, typename Period>
struct isDuration<std::chrono::duration<Rep, Period>> : std::true_type
{

};

/**
 * \brief Casts one duration type to another, rounding the result "up".
 *
 * Implementation inspired by https://en.cppreference.com/w/cpp/chrono/duration/ceil
 *
 * \tparam TargetDuration is the type of target duration
 * \tparam Rep is the "Rep" type of the source duration
 * \tparam Period is the "Period" type (ratio) of the source duration
 *
 * \param [in] duration is the source duration
 *
 * \return \a duration cast to \a TargetDuration and rounded "up"
 */

template<typename TargetDuration, typename Rep, typename Period,
		typename = typename std::enable_if<isDuration<TargetDuration>{}>::type>
constexpr TargetDuration durationCastCeil(const std::chrono::duration<Rep, Period>& duration)
{
	const auto targetDuration = std::chrono::duration_cast<TargetDuration>(duration);
	if (targetDuration < duration)
		return targetDuration + TargetDuration{1};

	return targetDuration;
}

}	// namespace estd

#endif	// ESTD_DURATIONCASTCEIL_HPP_
