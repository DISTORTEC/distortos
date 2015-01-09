/**
 * \file
 * \brief SortedContainer class header
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-09
 */

#ifndef INCLUDE_DISTORTOS_CONTAINERS_SORTEDCONTAINER_HPP_
#define INCLUDE_DISTORTOS_CONTAINERS_SORTEDCONTAINER_HPP_

#include <algorithm>

namespace distortos
{

namespace containers
{

/**
 * \brief SortedContainer class is a container that keeps the elements sorted during emplace of transfer.
 *
 * \note The elements are sorted as long as the user does not modify the contents via iterators.
 *
 * \param Container is the underlying container, it must provide following functions: begin(), emplace(), empty(),
 * end() and splice(). It must contain following types: allocator_type, const_iterator, iterator and value_type.
 * Optionally functions erase() and size() of Container are forwarded if they exist.
 * \param Compare is a type of functor used for comparison, std::less results in descending order, std::greater - in
 * ascending order.
 */

template<typename Container, typename Compare>
class SortedContainer
{
public:

	using iterator = typename Container::iterator;
	using const_iterator = typename Container::const_iterator;
	using value_type = typename Container::value_type;
	using allocator_type = typename Container::allocator_type;

	/**
	 * \brief SortedContainer's constructor
	 *
	 * \param [in] compare is a reference to Compare object used to copy-construct comparison functor
	 * \param [in] allocator is a reference to allocator_type object used to copy-construct allocator of container
	 */

	explicit SortedContainer(const Compare& compare = Compare{}, const allocator_type& allocator = allocator_type{}) :
			container_{allocator},
            compare_(compare)
	{

	}

	/**
	 * \brief SortedContainer's constructor
	 *
	 * \param [in] allocator is a reference to allocator_type object used to copy-construct allocator of container
	 */

	explicit SortedContainer(const allocator_type& allocator) :
			container_{allocator},
			compare_{}
	{

	}

	/**
	 * \brief Forwarding of Container::begin()
	 */

	decltype(std::declval<Container>().begin()) begin()
	{
		return container_.begin();
	}

	/**
	 * \brief Forwarding of Container::begin() const
	 */

	decltype(std::declval<const Container>().begin()) begin() const
	{
		return container_.begin();
	}

	/**
	 * \brief Forwarding of Container::empty() const
	 */

	decltype(std::declval<Container>().empty()) empty() const
	{
		return container_.empty();
	}

	/**
	 * \brief Forwarding of Container::end()
	 */

	decltype(std::declval<Container>().end()) end()
	{
		return container_.end();
	}

	/**
	 * \brief Forwarding of Container::end() const
	 */

	decltype(std::declval<const Container>().end()) end() const
	{
		return container_.end();
	}

	/**
	 * \brief Forwarding of Container::erase(...)
	 *
	 * \note forwarded only if Container::erase(Args&&...) exists
	 */

	template<typename... Args, typename C = Container>
	auto erase(Args&&... args) -> decltype(std::declval<C>().erase(std::forward<Args>(args)...))
	{
		return container_.erase(std::forward<Args>(args)...);
	}

	/**
	 * \brief Forwarding of Container::size() const
	 *
	 * \note forwarded only if Container::size() const exists
	 */

	template<typename C = Container>
	decltype(std::declval<const C>().size()) size() const
	{
		return container_.size();
	}

	/**
	 * \brief Sorted emplace()
	 *
	 * \param Args are types of argument for value_type constructor
	 *
	 * \param [in] args are arguments for value_type constructor
	 *
	 * \return iterator to emplaced element
	 */

	template<typename... Args>
	iterator sortedEmplace(Args&&... args)
	{
		container_.emplace(begin(), std::forward<Args>(args)...);
		const auto it = begin();
		sortedSplice(*this, it);
		return it;
	}

	/**
	 * \brief Sorted splice()
	 *
	 * \param [in] other is the container from which the object is transfered
	 * \param [in] otherPosition is the position of the transfered object in the other container
	 */

	void sortedSplice(SortedContainer& other, const iterator otherPosition)
	{
		const auto insertPosition = findInsertPosition(*otherPosition);
		container_.splice(insertPosition, other.container_, otherPosition);
	}

private:

	/**
	 * \brief Finds insert position that satisfies sorting criteria.
	 *
	 * Finds the insert position where Compare of current element and provided value returns true.
	 *
	 * \param [in] value is the value that is going to be emplaced/transfered
	 */

	iterator findInsertPosition(const value_type& value)
	{
		return std::find_if(begin(), end(),
				[this, &value](const value_type& element) -> bool
				{
					return compare_(element, value);
				}
		);
	}

	/// container used for keeping elements
	Container container_;

	/// instance of functor used for comparison
	Compare compare_;
};

}	// namespace containers

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_CONTAINERS_SORTEDCONTAINER_HPP_
