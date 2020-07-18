/**
 * \file
 * \brief TypeErasedFunctor template class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_TYPEERASEDFUNCTOR_HPP_
#define ESTD_TYPEERASEDFUNCTOR_HPP_

namespace estd
{

template<typename Signature, bool NonConst = {}>
class TypeErasedFunctor;

/**
 * \brief TypeErasedFunctor class is an interface for type-erased functors.
 *
 * Overload with const operator()().
 *
 * \tparam R is the type returned by <em>TypeErasedFunctor::operator()() const</em>
 * \tparam Args are the types of arguments for <em>TypeErasedFunctor::operator()() const</em>
 */

template<typename R, typename... Args>
class TypeErasedFunctor<R(Args...), false>
{
public:

	/**
	 * \brief Function call operator of TypeErasedFunctor
	 *
	 * \param [in,out] args are arguments for derived function
	 *
	 * \return value returned by derived function
	 */

	virtual R operator()(Args... args) const = 0;

protected:

	/**
	 * \brief TypeErasedFunctor's destructor
	 *
	 * \note Polymorphic objects of TypeErasedFunctor type must not be deleted via pointer/reference
	 */

	~TypeErasedFunctor() = default;
};

/**
 * \brief TypeErasedFunctor class is an interface for type-erased functors.
 *
 * Overload with non-const operator()().
 *
 * \tparam R is the type returned by <em>TypeErasedFunctor::operator()()</em>
 * \tparam Args are the types of arguments for <em>TypeErasedFunctor::operator()()</em>
 */

template<typename R, typename... Args>
class TypeErasedFunctor<R(Args...), true>
{
public:

	/**
	 * \brief Function call operator of TypeErasedFunctor
	 *
	 * \param [in,out] args are arguments for derived function
	 *
	 * \return value returned by derived function
	 */

	virtual R operator()(Args... args) = 0;

protected:

	/**
	 * \brief TypeErasedFunctor's destructor
	 *
	 * \note Polymorphic objects of TypeErasedFunctor type must not be deleted via pointer/reference
	 */

	~TypeErasedFunctor() = default;
};

}	// namespace estd

#endif	// ESTD_TYPEERASEDFUNCTOR_HPP_
