/**
 * \file
 * \brief Header with templated bitwise operators for enum class flags.
 *
 * Implementation based on [Using Enum Classes as Bitfields]
 * (https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html)
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_ENUMCLASSFLAGS_HPP_
#define ESTD_ENUMCLASSFLAGS_HPP_

#include <type_traits>

namespace estd
{

/**
 * \brief Tag struct used to enable bitwise operators for selected enum class flags.
 *
 * \tparam T is the type of enum class flags
 */

template<typename T>
struct isEnumClassFlags : std::false_type
{

};

}	// namespace estd

/**
 * \brief Bitwise AND operator
 *
 * \tparam T is the type of enum class flags for which there exists a specialization of estd::isEnumClassFlags which
 * inherits from std::true_type
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return result of bitwise AND of \a left and \a right
 */

template<typename T>
constexpr typename std::enable_if<estd::isEnumClassFlags<T>{}, T>::type operator&(const T& left, const T& right)
{
	using UnderlyingType = typename std::underlying_type<T>::type;
	return static_cast<T>(static_cast<UnderlyingType>(left) & static_cast<UnderlyingType>(right));
}

/**
 * \brief Bitwise OR operator
 *
 * \tparam T is the type of enum class flags for which there exists a specialization of estd::isEnumClassFlags which
 * inherits from std::true_type
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return result of bitwise OR of \a left and \a right
 */

template<typename T>
constexpr typename std::enable_if<estd::isEnumClassFlags<T>{}, T>::type operator|(const T& left, const T& right)
{
	using UnderlyingType = typename std::underlying_type<T>::type;
	return static_cast<T>(static_cast<UnderlyingType>(left) | static_cast<UnderlyingType>(right));
}

/**
 * \brief Bitwise XOR operator
 *
 * \tparam T is the type of enum class flags for which there exists a specialization of estd::isEnumClassFlags which
 * inherits from std::true_type
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return result of bitwise XOR of \a left and \a right
 */

template<typename T>
constexpr typename std::enable_if<estd::isEnumClassFlags<T>{}, T>::type operator^(const T& left, const T& right)
{
	using UnderlyingType = typename std::underlying_type<T>::type;
	return static_cast<T>(static_cast<UnderlyingType>(left) ^ static_cast<UnderlyingType>(right));
}

/**
 * \brief Bitwise NOT operator
 *
 * \tparam T is the type of enum class flags for which there exists a specialization of estd::isEnumClassFlags which
 * inherits from std::true_type
 *
 * \param [in] object is the object on which the operator will act
 *
 * \return result of bitwise NOT of \a object
 */

template<typename T>
constexpr typename std::enable_if<estd::isEnumClassFlags<T>{}, T>::type operator~(const T& object)
{
	using UnderlyingType = typename std::underlying_type<T>::type;
	return static_cast<T>(~static_cast<UnderlyingType>(object));
}

/**
 * \brief Bitwise AND assignment operator
 *
 * \tparam T is the type of enum class flags for which there exists a specialization of estd::isEnumClassFlags which
 * inherits from std::true_type
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return reference to \a left after it was assigned the result of bitwise AND of \a left and \a right
 */

template<typename T>
constexpr typename std::enable_if<estd::isEnumClassFlags<T>{}, T&>::type operator&=(T& left, const T& right)
{
	return (left = left & right);
}

/**
 * \brief Bitwise OR assignment operator
 *
 * \tparam T is the type of enum class flags for which there exists a specialization of estd::isEnumClassFlags which
 * inherits from std::true_type
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return reference to \a left after it was assigned the result of bitwise OR of \a left and \a right
 */

template<typename T>
constexpr typename std::enable_if<estd::isEnumClassFlags<T>{}, T&>::type operator|=(T& left, const T& right)
{
	return (left = left | right);
}

/**
 * \brief Bitwise XOR assignment operator
 *
 * \tparam T is the type of enum class flags for which there exists a specialization of estd::isEnumClassFlags which
 * inherits from std::true_type
 *
 * \param [in] left is the object on the left-hand side of operator
 * \param [in] right is the object on the right-hand side of operator
 *
 * \return reference to \a left after it was assigned the result of bitwise XOR of \a left and \a right
 */

template<typename T>
constexpr typename std::enable_if<estd::isEnumClassFlags<T>{}, T&>::type operator^=(T& left, const T& right)
{
	return (left = left ^ right);
}

#endif	// ESTD_ENUMCLASSFLAGS_HPP_
