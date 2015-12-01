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
 * \date 2015-12-01
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_CONTAINERS_SORTEDCONTAINER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_CONTAINERS_SORTEDCONTAINER_HPP_

#include <algorithm>

namespace distortos
{

namespace internal
{

/**
 * \brief SortedContainerBase class is a wrapper around Container which can be used as base of SortedContainer
 *
 * \param Container is the underlying container, it must provide following functions: begin(), emplace(), empty(),
 * end(), pop_front() and splice(). It must contain following types: allocator_type, const_iterator, iterator and
 * value_type. Optionally functions erase() and size() of Container are forwarded if they exist.
 */

template<typename Container>
class SortedContainerBase
{
public:

	/// iterator
	using iterator = typename Container::iterator;

	/// const_iterator
	using const_iterator = typename Container::const_iterator;

	/// value_type
	using value_type = typename Container::value_type;

	/// allocator_type
	using allocator_type = typename Container::allocator_type;

	/**
	 * \brief Primary template used to test presence of C::splice()
	 *
	 * \param C is the type of container that will be tested
	 * \param Signature is the signature of C::splice() that will be tested
	 */

	template<typename C, typename Signature>
	struct TestHasSplice;

	/**
	 * \brief Template used to test presence of R C::splice(Args...)
	 *
	 * \param C is the type of container that will be tested
	 * \param R is the type returned by tested C::splice()
	 * \param Args are the types of arguments for tested C::splice()
	 */

	template<typename C, typename R, typename... Args>
	struct TestHasSplice<C, R(Args...)>
	{
		/**
		* \brief Overload selected when R C::splice(Args...) function is present.
		*
		* \return std::true_type
		*/

		template<typename CC = C>
		constexpr static auto test(CC* p) ->
				typename std::is_same<decltype(p->splice(std::declval<Args>()...)), R>::type;

		/**
		* \brief Overload selected when R C::splice(Args...) function is not present.
		*
		* \return std::false_type
		*/

		constexpr static auto test(...) -> std::false_type;
	};

	/**
	 * \brief Predicate telling whether void Container::splice(iterator, Container&, iterator) function is present (it
	 * inherits from std::true_type in that case) or not (inherits from std::false_type).
	 *
	 * \note Correct prototype should be <em>void Container::splice(const_iterator, Container&, const_iterator)</em>,
	 * but libstdc++v3 used \a iterator instead of \a const_iterator until release of GCC 4.9. The prototype used below
	 * works in any case, as \a iterator is convertible to \a const_iterator, but not the other way around.
	 */

	using HasSplice = decltype(TestHasSplice<Container, void(iterator, Container&, iterator)>::
			test(std::declval<Container*>()));

	/**
	 * \brief SortedContainerBase's constructor
	 *
	 * \param [in] allocator is a reference to allocator_type object used to copy-construct allocator of container
	 */

	constexpr explicit SortedContainerBase(const allocator_type& allocator = allocator_type{}) :
			container_{allocator}
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
	 * \brief Forwarding of Container::pop_front()
	 */

	decltype(std::declval<Container>().pop_front()) pop_front()
	{
		return container_.pop_front();
	}

protected:

	/// container used for keeping elements
	Container container_;
};

/**
 * \brief SortedContainer class is a container that keeps the elements sorted during emplace of transfer.
 *
 * \note The elements are sorted as long as the user does not modify the contents via iterators.
 *
 * \param Container is the underlying container, it must provide following functions: begin(), emplace(), empty(),
 * end(), pop_front() and splice(). It must contain following types: allocator_type, const_iterator, iterator and
 * value_type. Optionally functions erase() and size() of Container are forwarded if they exist.
 * \param Compare is a type of functor used for comparison, std::less results in descending order, std::greater - in
 * ascending order.
 * \param HasSplice selects implementation - std::true_type to use Container::emplace() and Container::splice();
 * std::false_type to use Container::emplace_front(), Container::splice_after() and Container::before_begin()
 */

template<typename Container, typename Compare, typename HasSplice = typename SortedContainerBase<Container>::HasSplice>
class SortedContainer : public SortedContainerBase<Container>
{
public:

	/// base of SortedContainer
	using Base = SortedContainerBase<Container>;

	/// import iterator type alias from Base
	using typename Base::iterator;

	/// import const_iterator type alias from Base
	using typename Base::const_iterator;

	/// import value_type type alias from Base
	using typename Base::value_type;

	/// import allocator_type type alias from Base
	using typename Base::allocator_type;

	/// import Base's constructor
	using Base::Base;

	/**
	 * \brief SortedContainer's constructor
	 *
	 * \param [in] compare is a reference to Compare object used to copy-construct comparison functor
	 * \param [in] allocator is a reference to allocator_type object used to copy-construct allocator of container
	 */

	constexpr explicit SortedContainer(const Compare& compare = Compare{},
			const allocator_type& allocator = allocator_type{}) :
			Base{allocator},
			compare_{compare}
	{

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
		const auto it = Base::container_.emplace(Base::begin(), std::forward<Args>(args)...);
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
		Base::container_.splice(insertPosition, other.container_, otherPosition);
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
		return std::find_if(Base::begin(), Base::end(),
				[this, &value](const value_type& element) -> bool
				{
					return compare_(element, value);
				}
		);
	}

	/// instance of functor used for comparison
	Compare compare_;
};

/**
 * \brief SortedContainer specialization for Containers that don't have Container::emplace() and Container::splice() -
 * like std::forward_list. In this case Container::emplace_front(), Container::splice_after() and
 * Container::before_begin() are used.
 */

template<typename Container, typename Compare>
class SortedContainer<Container, Compare, std::false_type> : public SortedContainerBase<Container>
{
public:

	/// base of SortedContainer
	using Base = SortedContainerBase<Container>;

	/// import iterator type alias from Base
	using typename Base::iterator;

	/// import const_iterator type alias from Base
	using typename Base::const_iterator;

	/// import value_type type alias from Base
	using typename Base::value_type;

	/// import allocator_type type alias from Base
	using typename Base::allocator_type;

	/// import Base's constructor
	using Base::Base;

	/**
	 * \brief SortedContainer's constructor
	 *
	 * \param [in] compare is a reference to Compare object used to copy-construct comparison functor
	 * \param [in] allocator is a reference to allocator_type object used to copy-construct allocator of container
	 */

	constexpr explicit SortedContainer(const Compare& compare = Compare{},
			const allocator_type& allocator = allocator_type{}) :
			Base{allocator},
			compare_{compare}
	{

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
		Base::container_.emplace_front(std::forward<Args>(args)...);
		const auto it = Base::container_.begin();
		sortedSpliceAfter(*this, Base::container_.before_begin());
		return it;
	}

	/**
	 * \brief Sorted splice_after()
	 *
	 * \param [in] other is the container from which the object is transfered
	 * \param [in] otherPositionBefore is the position before the transfered object in the other container
	 */

	void sortedSpliceAfter(SortedContainer& other, const iterator otherPositionBefore)
	{
		const auto next = std::next(otherPositionBefore);
		const auto insertPositionBefore = findInsertPositionBefore(*next);
		Base::container_.splice_after(insertPositionBefore, other.container_, otherPositionBefore);
	}

private:

	/**
	 * \brief Finds insert position "before" the position that satisfies sorting criteria.
	 *
	 * Finds the insert position "before" the position where Compare of current element and provided value returns true.
	 *
	 * \param [in] value is the value that is going to be emplaced/transfered
	 */

	iterator findInsertPositionBefore(const value_type& value)
	{
		auto it = Base::container_.before_begin();
		auto next = Base::container_.begin();
		const auto last = Base::container_.end();

		while (next != last)
		{
			if (compare_(*next, value) == true)
				return it;

			it = next;
			++next;
		}
		return it;
	}

	/// instance of functor used for comparison
	Compare compare_;
};

}	// namespace internal

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_INTERNAL_CONTAINERS_SORTEDCONTAINER_HPP_
