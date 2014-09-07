/**
 * \file
 * \brief AllocatorBase class header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-09-07
 */

#ifndef INCLUDE_DISTORTOS_ALLOCATORS_ALLOCATORBASE_HPP_
#define INCLUDE_DISTORTOS_ALLOCATORS_ALLOCATORBASE_HPP_

#include <utility>

#include <cstddef>

namespace distortos
{

/// allocators namespace groups allocators
namespace allocators
{

/**
 * \brief AllocatorBase class is a common base for allocators.
 *
 * \param T is the allocated type
 */

template<typename T>
class AllocatorBase
{
public:

	/// allocated type
	using value_type = T;

	/// pointer to allocated type
	using pointer = value_type*;

	/// const pointer to allocated type
	using const_pointer = const value_type*;

	/// reference to allocated type
	using reference = value_type&;

	/// const reference to allocated type
	using const_reference = const value_type&;

	/// size_type
	using size_type = size_t;

	/**
	 * \brief Destroys allocated object, without any storage dallocation.
	 *
	 * \param U is the destroyed type
	 *
	 * \param [in] object is a pointer to destroyed object
	 */

	template<typename U>
	void destroy(U* const object)
	{
		object->~U();
	}

	/**
	 * \brief Constructs object in previously allocated storage.
	 *
	 * \param U is the constructed type
	 * \param Args are the arguments for object constructor
	 *
	 * \param [in] storage is a pointer to raw allocated storage
	 * \param [in] args are arguments for object constructor
	 */

	template<typename U, typename... Args>
	void construct(U* const storage, Args&&... args)
	{
		::new (static_cast<void*>(storage)) U{std::forward<Args>(args)...};
	}
};

}	// namespace allocators

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_ALLOCATORS_ALLOCATORBASE_HPP_
