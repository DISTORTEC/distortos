/**
 * \file
 * \brief invoke() header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_INVOKE_HPP_
#define ESTD_INVOKE_HPP_

#include <utility>

namespace estd
{

namespace internal
{

/**
 * \brief Implementation of invoke() for function objects
 *
 * \tparam Function is the function object that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] function is the function object that will be executed
 * \param [in] args are arguments for \a function
 *
 * \return value returned by call to \a function with \a args
 */

template<typename Function, typename... Args>
inline auto invoke(Function&& function, Args&&... args) ->
		decltype(std::forward<Function>(function)(std::forward<Args>(args)...))
{
	return std::forward<Function>(function)(std::forward<Args>(args)...);
}

/**
 * \brief Implementation of invoke() for member functions
 *
 * \tparam T is the type returned by call to member function
 * \tparam Base is the type of base class
 * \tparam Derived is the type of derived class
 * \tparam Args are the arguments for member function
 *
 * \param [in] memberFunction is a pointer to member function of \a object that will be executed
 * \param [in] object is an object or a reference to object on which \a memberFunction will be executed
 * \param [in] args are arguments for \a memberFunction
 *
 * \return value returned by call to \a memberFunction on \a object with \a args
 */

template<typename T, typename Base, typename Derived, typename... Args>
inline auto invoke(T Base::* memberFunction, Derived&& object, Args&&... args) ->
		decltype((std::forward<Derived>(object).*memberFunction)(std::forward<Args>(args)...))
{
	return (std::forward<Derived>(object).*memberFunction)(std::forward<Args>(args)...);
}

/**
 * \brief Implementation of invoke() for member functions
 *
 * \tparam MemberFunction is the type or pointer to member function
 * \tparam Pointer is the type of pointer to object
 * \tparam Args are the arguments for member function
 *
 * \param [in] memberFunction is a pointer to member function of object that will be executed
 * \param [in] pointer is a pointer to object on which \a memberFunction will be executed
 * \param [in] args are arguments for \a memberFunction
 *
 * \return value returned by call to \a memberFunction on \a (*pointer) with \a args
 */

template<typename MemberFunction, typename Pointer, typename... Args>
inline auto invoke(MemberFunction memberFunction, Pointer&& pointer, Args&&... args) ->
		decltype(((*std::forward<Pointer>(pointer)).*memberFunction)(std::forward<Args>(args)...))
{
	return ((*std::forward<Pointer>(pointer)).*memberFunction)(std::forward<Args>(args)...);
}

/**
 * \brief Implementation of invoke() for data members
 *
 * \tparam T is the type of data member
 * \tparam Base is the type of base class
 * \tparam Derived is the type of derived class
 *
 * \param [in] dataMember is a pointer to data member of \a object that will be accessed
 * \param [in] object is an object or a reference to object in which \a dataMember will be accessed
 *
 * \return value returned by access to \a dataMember in \a object
 */

template<typename T, typename Base, typename Derived>
inline auto invoke(T Base::* dataMember, Derived&& object) -> decltype(std::forward<Derived>(object).*dataMember)
{
	return std::forward<Derived>(object).*dataMember;
}

/**
 * \brief Implementation of invoke() for data members
 *
 * \tparam DataMember is the type or pointer to data member
 * \tparam Pointer is the type of pointer to object
 *
 * \param [in] dataMember is a pointer to data member of object that will be accessed
 * \param [in] pointer is a pointer to object in which \a dataMember will be accessed
 *
 * \return value returned by access to \a dataMember in \a (*pointer)
 */

template<typename DataMember, typename Pointer>
inline auto invoke(DataMember dataMember, Pointer&& pointer) -> decltype((*std::forward<Pointer>(pointer)).*dataMember)
{
	return (*std::forward<Pointer>(pointer)).*dataMember;
}

}	// namespace internal

/**
 * \brief Invokes callable object in an appropriate way.
 *
 * Implementation inspired by https://en.cppreference.com/w/cpp/utility/functional/invoke
 *
 * \tparam Function is the function object that will be executed
 * \tparam Args are the arguments for \a Function
 *
 * \param [in] function is the function object that will be executed
 * \param [in] args are arguments for \a function
 *
 * \return value returned by call to \a function with \a args
 */

template<typename Function, typename... Args>
inline auto invoke(Function&& function, Args&&... args) ->
		decltype(internal::invoke(std::forward<Function>(function), std::forward<Args>(args)...))
{
	return internal::invoke(std::forward<Function>(function), std::forward<Args>(args)...);
}

}	// namespace estd

#endif	// ESTD_INVOKE_HPP_
