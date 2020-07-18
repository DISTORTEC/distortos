/**
 * \file
 * \brief SortedIntrusiveForwardList template class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_SORTEDINTRUSIVEFORWARDLIST_HPP_
#define ESTD_SORTEDINTRUSIVEFORWARDLIST_HPP_

#include "estd/IntrusiveForwardList.hpp"

#include <algorithm>

namespace estd
{

/**
 * \brief SortedIntrusiveForwardList class is an IntrusiveForwardList with sorted elements
 *
 * This class tries to provide an interface similar to std::forward_list.
 *
 * \note The elements are sorted as long as the user does not modify the contents.
 *
 * \tparam Compare is a type of functor used for comparison, std::less results in descending order, std::greater - in
 * ascending order
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T; using different type than \a T can be
 * used to break circular dependencies, because \a T must be fully defined to instantiate this class, but it is enough
 * to forward declare \a U - it only needs to be fully defined to use member functions
 */

template<typename Compare, typename T, IntrusiveForwardListNode T::* NodePointer, typename U = T>
class SortedIntrusiveForwardList
{
public:

	/// unsorted intrusive forward list used internally
	using UnsortedIntrusiveForwardList = IntrusiveForwardList<T, NodePointer, U>;

	/// const iterator of elements on the list
	using const_iterator = typename UnsortedIntrusiveForwardList::const_iterator;

	/// const pointer to value linked in the list
	using const_pointer = typename UnsortedIntrusiveForwardList::const_pointer;

	/// const reference to value linked in the list
	using const_reference = typename UnsortedIntrusiveForwardList::const_reference;

	/// iterator of elements on the list
	using iterator = typename UnsortedIntrusiveForwardList::iterator;

	/// pointer to value linked in the list
	using pointer = typename UnsortedIntrusiveForwardList::pointer;

	/// reference to value linked in the list
	using reference = typename UnsortedIntrusiveForwardList::reference;

	/// value linked in the list
	using value_type = typename UnsortedIntrusiveForwardList::value_type;

	/**
	 * \brief SortedIntrusiveForwardList's constructor
	 *
	 * \param [in] compare is a reference to Compare object used to copy-construct internal comparison functor
	 */

	constexpr explicit SortedIntrusiveForwardList(const Compare& compare = Compare{}) :
			implementation_{compare}
	{

	}

	/**
	 * \return iterator of "one before the first" element on the list
	 */

	iterator before_begin()
	{
		return implementation_.intrusiveForwardList.before_begin();
	}

	/**
	 * \return const iterator of "one before the first" element on the list
	 */

	const_iterator before_begin() const
	{
		return implementation_.intrusiveForwardList.before_begin();
	}

	/**
	 * \return iterator of first element on the list
	 */

	iterator begin()
	{
		return implementation_.intrusiveForwardList.begin();
	}

	/**
	 * \return const iterator of first element on the list
	 */

	const_iterator begin() const
	{
		return implementation_.intrusiveForwardList.begin();
	}

	/**
	 * \return const iterator of "one before the first" element on the list
	 */

	const_iterator cbefore_begin() const
	{
		return implementation_.intrusiveForwardList.cbefore_begin();
	}

	/**
	 * \return const iterator of first element on the list
	 */

	const_iterator cbegin() const
	{
		return implementation_.intrusiveForwardList.cbegin();
	}

	/**
	 * \return const iterator of "one past the last" element on the list
	 */

	const_iterator cend() const
	{
		return implementation_.intrusiveForwardList.cend();
	}

	/**
	 * \brief Unlinks all elements from the list.
	 */

	void clear()
	{
		implementation_.intrusiveForwardList.clear();
	}

	/**
	 * \return true is the list is empty, false otherwise
	 */

	bool empty() const
	{
		return implementation_.intrusiveForwardList.empty();
	}

	/**
	 * \return iterator of "one past the last" element on the list
	 */

	iterator end()
	{
		return implementation_.intrusiveForwardList.end();
	}

	/**
	 * \return const iterator of "one past the last" element on the list
	 */

	const_iterator end() const
	{
		return implementation_.intrusiveForwardList.end();
	}

	/**
	 * \return reference to first element on the list
	 */

	reference front()
	{
		return implementation_.intrusiveForwardList.front();
	}

	/**
	 * \return const reference to first element on the list
	 */

	const_reference front() const
	{
		return implementation_.intrusiveForwardList.front();
	}

	/**
	 * \brief Links the element in the list, keeping it sorted.
	 *
	 * \param [in] newElement is a reference to the element that will be linked in the list
	 *
	 * \return iterator of \a newElement
	 */

	iterator insert(reference newElement)
	{
		return UnsortedIntrusiveForwardList::insert_after(implementation_.findInsertPositionBefore(newElement),
				newElement);
	}

	/**
	 * \brief Unlinks the first element from the list.
	 */

	void pop_front()
	{
		implementation_.intrusiveForwardList.pop_front();
	}

	/**
	 * \brief Transfers the element from another list to this one, keeping it sorted.
	 *
	 * \param [in] beforeSplicedElement is an iterator of the element preceding the one which will be spliced from
	 * another list to this one
	 */

	void splice_after(const iterator beforeSplicedElement)
	{
		const auto splicedElement = std::next(beforeSplicedElement);
		UnsortedIntrusiveForwardList::splice_after(implementation_.findInsertPositionBefore(*splicedElement),
				beforeSplicedElement);
	}

	/**
	 * \brief Swaps contents with another list.
	 *
	 * \param [in] other is a reference to SortedIntrusiveForwardList with which contents of this list will be swapped
	 */

	void swap(SortedIntrusiveForwardList& other)
	{
		implementation_.swap(other.implementation_);
	}

	/**
	 * \brief Unlinks the element following \a position from the list.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is an iterator preceding the element which will be unlinked from the list
	 *
	 * \return iterator of the element that was following the element which was unlinked
	 */

	static iterator erase_after(const iterator position)
	{
		return UnsortedIntrusiveForwardList::erase_after(position);
	}

	SortedIntrusiveForwardList(const SortedIntrusiveForwardList&) = delete;
	SortedIntrusiveForwardList(SortedIntrusiveForwardList&&) = default;
	const SortedIntrusiveForwardList& operator=(const SortedIntrusiveForwardList&) = delete;
	SortedIntrusiveForwardList& operator=(SortedIntrusiveForwardList&&) = delete;

private:

	/// Implementation struct is used primarily for "Empty Base Optimization" with \a Compare type
	struct Implementation : public Compare
	{
		/**
		 * \brief Implementation's constructor
		 *
		 * \param [in] comparee is a reference to Compare object used to copy-construct internal comparison functor
		 */

		constexpr explicit Implementation(const Compare& comparee) :
				Compare{comparee},
				intrusiveForwardList{}
		{

		}

		/**
		 * \brief Finds insert position "before" the position that satisfies sorting criteria.
		 *
		 * \param [in] newElement is a const reference to new element that is going to be inserted/spliced
		 *
		 * \return iterator for which Compare's function call operator of next value and \a newElement returns true.
		 */

		iterator findInsertPositionBefore(const_reference newElement)
		{
			auto it = intrusiveForwardList.before_begin();
			auto next = intrusiveForwardList.begin();
			const auto end = intrusiveForwardList.end();

			while (next != end && this->Compare::operator()(*next, newElement) == false)
			{
				it = next;
				++next;
			}

			return it;
		}

		/**
		 * \brief Swaps contents with another instance.
		 *
		 * \param [in] other is a reference to Implementation with which contents of this instance will be swapped
		 */

		void swap(Implementation& other)
		{
			intrusiveForwardList.swap(other.intrusiveForwardList);
			using std::swap;
			swap(static_cast<Compare&>(*this), static_cast<Compare&>(other));
		}

		Implementation(const Implementation&) = delete;
		Implementation(Implementation&&) = default;
		const Implementation& operator=(const Implementation&) = delete;
		Implementation& operator=(Implementation&&) = delete;

		/// internal unsorted IntrusiveForwardList
		UnsortedIntrusiveForwardList intrusiveForwardList;
	};

	/// internal Implementation object - unsorted IntrusiveForwardList and Compare instance
	Implementation implementation_;
};

/**
 * \brief Swaps contents of two lists.
 *
 * \tparam Compare is a type of functor used for comparison, std::less results in descending order, std::greater - in
 * ascending order
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a reference to SortedIntrusiveForwardList with which contents of \a right will be swapped
 * \param [in] right is a reference to SortedIntrusiveForwardList with which contents of \a left will be swapped
 */

template<typename Compare, typename T, IntrusiveForwardListNode T::* NodePointer, typename U = T>
inline void swap(SortedIntrusiveForwardList<Compare, T, NodePointer, U>& left,
		SortedIntrusiveForwardList<Compare, T, NodePointer, U>& right)
{
	left.swap(right);
}

}	// namespace estd

#endif	// ESTD_SORTEDINTRUSIVEFORWARDLIST_HPP_
