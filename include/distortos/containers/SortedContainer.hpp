/**
 * \file
 * \brief SortedContainerBase and SortedContainer classes header
 *
 * \author Copyright (C) 2014 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2014-08-01
 */

#ifndef INCLUDE_DISTORTOS_CONTAINERS_SORTEDCONTAINER_HPP_
#define INCLUDE_DISTORTOS_CONTAINERS_SORTEDCONTAINER_HPP_

#include <algorithm>

namespace distortos
{

namespace containers
{


/**
 * \brief Base for SortedContainer - can be used to get common iterator.
 *
 * \param Container is the underlying container, it must provide following functions: begin(), emplace(), end(), size()
 * and splice(). It must contain following types: iterator and value_type.
 */

template<typename Container>
class SortedContainerBase : public Container
{
public:

	/// base of SortedContainerBase
	using Base = Container;
};

/**
 * \brief SortedContainer class is a container that keeps the elements sorted during emplace of transfer.
 *
 * \note The elements are sorted as long as the user does not modify the contents via iterators.
 *
 * \param Container is the underlying container, it must provide following functions: begin(), emplace(), end(), size()
 * and splice(). It must contain following types: iterator and value_type.
 * \param Compare is a type of functor used for comparison, std::less results in descending order, std::greater - in
 * ascending order.
 */

template<typename Container, typename Compare>
class SortedContainer : private SortedContainerBase<Container>
{
	template<typename OtherContainer, typename OtherCompare>
	friend class SortedContainer;

public:

	/**
	 * \brief SortedContainer's constructor
	 *
	 * \param Args are types of argument for Compare object
	 *
	 * \param [in] args are arguments for Compare object constructor
	 */

	template<typename... Args>
	SortedContainer(Args&&... args) :
            compare_{std::forward<Args>(args)...}
	{}

	/// base of SortedContainer
	using Base = SortedContainerBase<Container>;

	using typename Base::iterator;
	using typename Base::value_type;

	using Base::begin;
	using Base::empty;
	using Base::end;
	using Base::size;

	/**
	 * \brief Sorted emplace()
	 *
	 * \param Args are types of argument for value_type constructor
	 *
	 * \param [in] args are arguments for value_type constructor
	 */

	template<typename... Args>
	iterator sortedEmplace(Args&&... args)
	{
		Base::emplace(begin(), std::forward<Args>(args)...);
		const auto it = begin();
		sortedSplice(*this, it);
		return it;
	}

	/**
	 * \brief Sorted splice()
	 *
	 * \param OtherContainer is the type of container from which the object is transfered
	 * \param OtherIterator is the type of iterator in OtherContainer
	 *
	 * \param [in] other is the container from which the object is transfered
	 * \param [in] other_position is the position of the transfered object in the other container
	 */

	template<typename OtherContainer, typename OtherIterator>
	void sortedSplice(OtherContainer &other, OtherIterator other_position)
	{
		const auto insert_position = findInsertPosition_(*other_position);
		Base::splice(insert_position, other, other_position);
	}

private:

	/**
	 * \brief Finds insert position that satisfies sorting criteria.
	 *
	 * Finds the insert position where Compare of current element and provided value returns true.
	 *
	 * \param [in] value is the value that is going to be emplaced/transfered
	 */

	iterator findInsertPosition_(const value_type &value)
	{
		return std::find_if(begin(), end(),
				[this, &value](const value_type &element) -> bool
				{
					return compare_(element, value);
				}
		);
	}

	/// instance of functor used for comparison
	Compare compare_;
};

}	// namespace containers

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_CONTAINERS_SORTEDCONTAINER_HPP_
