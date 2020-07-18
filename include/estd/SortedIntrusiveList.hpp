/**
 * \file
 * \brief SortedIntrusiveList template class header
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_SORTEDINTRUSIVELIST_HPP_
#define ESTD_SORTEDINTRUSIVELIST_HPP_

#include "estd/IntrusiveList.hpp"

#include <algorithm>

namespace estd
{

/**
 * \brief SortedIntrusiveList class is an IntrusiveList with sorted elements
 *
 * This class tries to provide an interface similar to std::list.
 *
 * \note The elements are sorted as long as the user does not modify the contents.
 *
 * \tparam Compare is a type of functor used for comparison, std::less results in descending order, std::greater - in
 * ascending order
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T; using different type than \a T can be
 * used to break circular dependencies, because \a T must be fully defined to instantiate this class, but it is enough
 * to forward declare \a U - it only needs to be fully defined to use member functions
 */

template<typename Compare, typename T, IntrusiveListNode T::* NodePointer, typename U = T>
class SortedIntrusiveList
{
public:

	/// unsorted intrusive list used internally
	using UnsortedIntrusiveList = IntrusiveList<T, NodePointer, U>;

	/// const iterator of elements on the list
	using const_iterator = typename UnsortedIntrusiveList::const_iterator;

	/// const reverse iterator of elements on the list
	using const_reverse_iterator = typename UnsortedIntrusiveList::const_reverse_iterator;

	/// const pointer to value linked in the list
	using const_pointer = typename UnsortedIntrusiveList::const_pointer;

	/// const reference to value linked in the list
	using const_reference = typename UnsortedIntrusiveList::const_reference;

	/// iterator of elements on the list
	using iterator = typename UnsortedIntrusiveList::iterator;

	/// reverse iterator of elements on the list
	using reverse_iterator = typename UnsortedIntrusiveList::reverse_iterator;

	/// pointer to value linked in the list
	using pointer = typename UnsortedIntrusiveList::pointer;

	/// reference to value linked in the list
	using reference = typename UnsortedIntrusiveList::reference;

	/// value linked in the list
	using value_type = typename UnsortedIntrusiveList::value_type;

	/**
	 * \brief SortedIntrusiveList's constructor
	 *
	 * \param [in] compare is a reference to Compare object used to copy-construct internal comparison functor
	 */

	constexpr explicit SortedIntrusiveList(const Compare& compare = Compare{}) :
			implementation_{compare}
	{

	}

	/**
	 * \return reference to last element on the list
	 */

	reference back()
	{
		return implementation_.intrusiveList.back();
	}

	/**
	 * \return const reference to last element on the list
	 */

	const_reference back() const
	{
		return implementation_.intrusiveList.back();
	}

	/**
	 * \return iterator of first element on the list
	 */

	iterator begin()
	{
		return implementation_.intrusiveList.begin();
	}

	/**
	 * \return const iterator of first element on the list
	 */

	const_iterator begin() const
	{
		return implementation_.intrusiveList.begin();
	}

	/**
	 * \return const iterator of first element on the list
	 */

	const_iterator cbegin() const
	{
		return implementation_.intrusiveList.cbegin();
	}

	/**
	 * \return const iterator of "one past the last" element on the list
	 */

	const_iterator cend() const
	{
		return implementation_.intrusiveList.cend();
	}

	/**
	 * \brief Unlinks all elements from the list.
	 */

	void clear()
	{
		implementation_.intrusiveList.clear();
	}

	/**
	 * \return const reverse iterator to first element in the reversed list (last element of the non-reversed list)
	 */

	const_reverse_iterator crbegin() const
	{
		return implementation_.intrusiveList.crbegin();
	}

	/**
	 * \return const reverse iterator to "one past the last" element in the reversed list ("one before the first"
	 * element of the non-reversed list)
	 */

	const_reverse_iterator crend() const
	{
		return implementation_.intrusiveList.crend();
	}

	/**
	 * \return true is the list is empty, false otherwise
	 */

	bool empty() const
	{
		return implementation_.intrusiveList.empty();
	}

	/**
	 * \return iterator of "one past the last" element on the list
	 */

	iterator end()
	{
		return implementation_.intrusiveList.end();
	}

	/**
	 * \return const iterator of "one past the last" element on the list
	 */

	const_iterator end() const
	{
		return implementation_.intrusiveList.end();
	}

	/**
	 * \return reference to first element on the list
	 */

	reference front()
	{
		return implementation_.intrusiveList.front();
	}

	/**
	 * \return const reference to first element on the list
	 */

	const_reference front() const
	{
		return implementation_.intrusiveList.front();
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
		return UnsortedIntrusiveList::insert(implementation_.findInsertPosition(newElement), newElement);
	}

	/**
	 * \brief Unlinks the last element from the list.
	 */

	void pop_back()
	{
		implementation_.intrusiveList.pop_back();
	}

	/**
	 * \brief Unlinks the first element from the list.
	 */

	void pop_front()
	{
		implementation_.intrusiveList.pop_front();
	}

	/**
	 * \return reverse iterator to first element in the reversed list (last element of the non-reversed list)
	 */

	reverse_iterator rbegin()
	{
		return implementation_.intrusiveList.rbegin();
	}

	/**
	 * \return const reverse iterator to first element in the reversed list (last element of the non-reversed list)
	 */

	const_reverse_iterator rbegin() const
	{
		return implementation_.intrusiveList.rbegin();
	}

	/**
	 * \return reverse iterator to "one past the last" element in the reversed list ("one before the first" element of
	 * the non-reversed list)
	 */

	reverse_iterator rend()
	{
		return implementation_.intrusiveList.rend();
	}

	/**
	 * \return const reverse iterator to "one past the last" element in the reversed list ("one before the first"
	 * element of the non-reversed list)
	 */

	const_reverse_iterator rend() const
	{
		return implementation_.intrusiveList.rend();
	}

	/**
	 * \brief Transfers the element from another list to this one, keeping it sorted.
	 *
	 * \param [in] splicedElement is an iterator of the element that will be spliced from another list to this one
	 */

	void splice(const iterator splicedElement)
	{
		UnsortedIntrusiveList::splice(implementation_.findInsertPosition(*splicedElement), splicedElement);
	}

	/**
	 * \brief Swaps contents with another list.
	 *
	 * \param [in] other is a reference to SortedIntrusiveList with which contents of this list will be swapped
	 */

	void swap(SortedIntrusiveList& other)
	{
		implementation_.swap(other.implementation_);
	}

	/**
	 * \brief Unlinks the element at \a position from the list.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is an iterator of the element that will be unlinked from the list
	 *
	 * \return iterator of the element that was following the element which was unlinked
	 */

	static iterator erase(const iterator position)
	{
		return UnsortedIntrusiveList::erase(position);
	}

	SortedIntrusiveList(const SortedIntrusiveList&) = delete;
	SortedIntrusiveList(SortedIntrusiveList&&) = default;
	const SortedIntrusiveList& operator=(const SortedIntrusiveList&) = delete;
	SortedIntrusiveList& operator=(SortedIntrusiveList&&) = delete;

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
				intrusiveList{}
		{

		}

		/**
		 * \brief Finds insert position that satisfies sorting criteria.
		 *
		 * \param [in] newElement is a const reference to new element that is going to be inserted/spliced
		 *
		 * \return iterator for which Compare's function call operator of dereferenced value and \a newElement returns
		 * true.
		 */

		iterator findInsertPosition(const_reference newElement)
		{
			return std::find_if(intrusiveList.begin(), intrusiveList.end(),
					[this, &newElement](const_reference& element) -> bool
					{
						return this->Compare::operator()(element, newElement);
					}
			);
		}

		/**
		 * \brief Swaps contents with another instance.
		 *
		 * \param [in] other is a reference to Implementation with which contents of this instance will be swapped
		 */

		void swap(Implementation& other)
		{
			intrusiveList.swap(other.intrusiveList);
			using std::swap;
			swap(static_cast<Compare&>(*this), static_cast<Compare&>(other));
		}

		Implementation(const Implementation&) = delete;
		Implementation(Implementation&&) = default;
		const Implementation& operator=(const Implementation&) = delete;
		Implementation& operator=(Implementation&&) = delete;

		/// internal unsorted IntrusiveList
		UnsortedIntrusiveList intrusiveList;
	};

	/// internal Implementation object - unsorted IntrusiveList and Compare instance
	Implementation implementation_;
};

/**
 * \brief Swaps contents of two lists.
 *
 * \tparam Compare is a type of functor used for comparison, std::less results in descending order, std::greater - in
 * ascending order
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a reference to SortedIntrusiveList with which contents of \a right will be swapped
 * \param [in] right is a reference to SortedIntrusiveList with which contents of \a left will be swapped
 */

template<typename Compare, typename T, IntrusiveListNode T::* NodePointer, typename U = T>
inline void swap(SortedIntrusiveList<Compare, T, NodePointer, U>& left,
		SortedIntrusiveList<Compare, T, NodePointer, U>& right)
{
	left.swap(right);
}

}	// namespace estd

#endif	// ESTD_SORTEDINTRUSIVELIST_HPP_
