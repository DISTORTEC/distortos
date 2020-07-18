/**
 * \file
 * \brief IntrusiveList template class header
 *
 * \author Copyright (C) 2015-2018 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_INTRUSIVELIST_HPP_
#define ESTD_INTRUSIVELIST_HPP_

#include <iterator>

#include <cstddef>

namespace estd
{

namespace internal
{

class IntrusiveListBase;

}

/**
 * \brief IntrusiveListNode class is the node that is needed for the object to be linked in IntrusiveList
 *
 * To some extent, this class can be considered to be a limited (raw) iterator.
 *
 * The object that wants to be linked in IntrusiveList must contain a variable of this type - one for each intrusive
 * list that will be used with object.
 */

class IntrusiveListNode
{
public:

	/// LinkAccessKey class is used to limit access to IntrusiveListNode::link() function - only
	/// internal::IntrusiveListBase can link nodes to the list
	class LinkAccessKey
	{
		friend internal::IntrusiveListBase;

		/**
		 * \brief LinkAccessKey's constructor
		 */

		constexpr LinkAccessKey()
		{

		}

		LinkAccessKey(const LinkAccessKey&) = delete;
		LinkAccessKey(LinkAccessKey&&) = delete;
		const LinkAccessKey& operator=(const LinkAccessKey&) = delete;
		LinkAccessKey& operator=(LinkAccessKey&&) = delete;
	};

	/**
	 * \brief IntrusiveListNode's constructor
	 */

	constexpr IntrusiveListNode() :
			nextNode_{this},
			previousNode_{this}
	{

	}

	/**
	 * \brief IntrusiveListNode's move constructor
	 *
	 * \param [in] other is a rvalue reference to IntrusiveListNode used as source of move construction
	 */

	IntrusiveListNode(IntrusiveListNode&& other)
	{
		if (other.isLinked() == false)
		{
			reset();
			return;
		}

		nextNode_ = other.nextNode_;
		previousNode_ = other.previousNode_;
		nextNode_->previousNode_ = previousNode_->nextNode_ = this;
		other.reset();
	}

	/**
	 * \brief IntrusiveListNode's destructor
	 *
	 * Unlinks the node from the list.
	 */

	~IntrusiveListNode()
	{
		unlink();
	}

	/**
	 * \return reference to next node on the list
	 */

	IntrusiveListNode& getNextNode() const
	{
		return *nextNode_;
	}

	/**
	 * \return reference to previous node on the list
	 */

	IntrusiveListNode& getPreviousNode() const
	{
		return *previousNode_;
	}

	/**
	 * \return true if the node is linked in some list, false otherwise
	 */

	bool isLinked() const
	{
		return nextNode_ != this;
	}

	/**
	 * \brief Links the node in the list before \a position.
	 *
	 * \note Access to this function is restricted only to functions from internal::IntrusiveListBase class
	 *
	 * \param [in] position is a reference to node before which this node will be linked
	 * \param [in] linkAccessKey is used to limit access to this function
	 */

	void link(IntrusiveListNode& position, LinkAccessKey)
	{
		unlink();

		nextNode_ = &position;
		previousNode_ = position.previousNode_;
		position.previousNode_->nextNode_ = this;
		position.previousNode_ = this;
	}

	/**
	 * \brief Swaps contents with another node.
	 *
	 * \param [in] other is a reference to IntrusiveListNode with which contents of this node will be swapped
	 */

	void swap(IntrusiveListNode& other)
	{
		const auto thisWasLinked = isLinked();
		const auto otherWasLinked = other.isLinked();

		if (thisWasLinked == true || otherWasLinked == true)
		{
			using std::swap;
			swap(nextNode_, other.nextNode_);
			swap(previousNode_, other.previousNode_);

			if (thisWasLinked == true)
				other.nextNode_->previousNode_ = other.previousNode_->nextNode_ = &other;
			else
				other.reset();

			if (otherWasLinked == true)
				nextNode_->previousNode_ = previousNode_->nextNode_ = this;
			else
				reset();
		}
	}

	/**
	 * \brief Unlinks the node from the list.
	 */

	void unlink()
	{
		previousNode_->nextNode_ = nextNode_;
		nextNode_->previousNode_ = previousNode_;

		reset();
	}

	IntrusiveListNode(const IntrusiveListNode&) = delete;
	const IntrusiveListNode& operator=(const IntrusiveListNode&) = delete;
	IntrusiveListNode& operator=(IntrusiveListNode&&) = delete;

private:

	/**
	 * \brief Resets the node to the same state as right after construction.
	 */

	void reset()
	{
		nextNode_ = this;
		previousNode_ = this;
	}

	/// reference to next node on the list
	IntrusiveListNode* nextNode_;

	/// reference to previous node on the list
	IntrusiveListNode* previousNode_;
};

namespace internal
{

/**
 * \brief IntrusiveListBase class provides base functionalities for IntrusiveList class, but without any knowledge about
 * types
 *
 * This class tries to provide an interface similar to std::list.
 */

class IntrusiveListBase
{
public:

	/**
	 * \brief IntrusiveListBase's constructor
	 */

	constexpr IntrusiveListBase() :
			rootNode_{}
	{

	}

	/**
	 * \brief IntrusiveListBase's destructor
	 *
	 * Unlinks all nodes from the list.
	 */

	~IntrusiveListBase()
	{
		clear();
	}

	/**
	 * \return reference to first node on the list
	 */

	IntrusiveListNode& begin()
	{
		return rootNode_.getNextNode();
	}

	/**
	 * \return const reference to first node on the list
	 */

	const IntrusiveListNode& begin() const
	{
		return rootNode_.getNextNode();
	}

	/**
	 * \return const reference to first node on the list
	 */

	const IntrusiveListNode& cbegin() const
	{
		return begin();
	}

	/**
	 * \return const reference to "one past the last" node on the list
	 */

	const IntrusiveListNode& cend() const
	{
		return end();
	}

	/**
	 * \brief Unlinks all nodes from the list.
	 */

	void clear()
	{
		while (empty() == false)
			pop_front();
	}

	/**
	 * \return true is the list is empty, false otherwise
	 */

	bool empty() const
	{
		return &begin() == &end();
	}

	/**
	 * \return reference to "one past the last" node on the list
	 */

	IntrusiveListNode& end()
	{
		return rootNode_;
	}

	/**
	 * \return const reference to "one past the last" node on the list
	 */

	const IntrusiveListNode& end() const
	{
		return rootNode_;
	}

	/**
	 * \brief Unlinks the last node from the list.
	 */

	void pop_back()
	{
		erase(end().getPreviousNode());
	}

	/**
	 * \brief Unlinks the first node from the list.
	 */

	void pop_front()
	{
		erase(begin());
	}

	/**
	 * \brief Links the node at the end of the list.
	 *
	 * \param [in] newNode is a reference to node that will be linked in the list
	 */

	void push_back(IntrusiveListNode& newNode)
	{
		insert(end(), newNode);
	}

	/**
	 * \brief Links the node at the beginning of the list.
	 *
	 * \param [in] newNode is a reference to node that will be linked in the list
	 */

	void push_front(IntrusiveListNode& newNode)
	{
		insert(begin(), newNode);
	}

	/**
	 * \brief Swaps contents with another list.
	 *
	 * \param [in] other is a reference to IntrusiveListBase with which contents of this list will be swapped
	 */

	void swap(IntrusiveListBase& other)
	{
		rootNode_.swap(other.rootNode_);
	}

	/**
	 * \brief Unlinks the node at \a position from the list.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is a reference to the node that will be unlinked from the list
	 *
	 * \return reference to the node that was following the node which was unlinked
	 */

	static IntrusiveListNode& erase(IntrusiveListNode& position)
	{
		auto& next = position.getNextNode();
		position.unlink();
		return next;
	}

	/**
	 * \brief Links the node in the list before \a position.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is a reference to node before which \a newNode will be linked
	 * \param [in] newNode is a reference to node that will be linked in the list
	 */

	static void insert(IntrusiveListNode& position, IntrusiveListNode& newNode)
	{
		newNode.link(position, {});
	}

	/**
	 * \brief Transfers the node from one list to another list before \a position.
	 *
	 * \note No instance of any list is needed for this operation.
	 *
	 * \param [in] position is a reference to node before which \a splicedNode will be linked
	 * \param [in] splicedNode is a reference to node that will be spliced from one list to another
	 */

	static void splice(IntrusiveListNode& position, IntrusiveListNode& splicedNode)
	{
		insert(position, splicedNode);
	}

	IntrusiveListBase(const IntrusiveListBase&) = delete;
	IntrusiveListBase(IntrusiveListBase&&) = default;
	const IntrusiveListBase& operator=(const IntrusiveListBase&) = delete;
	IntrusiveListBase& operator=(IntrusiveListBase&&) = delete;

private:

	/// root node of the intrusive list
	IntrusiveListNode rootNode_;
};

/**
 * \brief Swaps contents of two lists.
 *
 * \param [in] left is a reference to IntrusiveListBase with which contents of \a right will be swapped
 * \param [in] right is a reference to IntrusiveListBase with which contents of \a left will be swapped
 */

inline void swap(IntrusiveListBase& left, IntrusiveListBase& right)
{
	left.swap(right);
}

}	// namespace internal

/**
 * \brief IntrusiveListIterator class is an iterator of elements on IntrusiveList.
 *
 * This class provides an interface similar to std::list::iterator.
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 */

template<typename T, IntrusiveListNode T::* NodePointer, typename U = T>
class IntrusiveListIterator
{
public:

	/// difference type
	using difference_type = ptrdiff_t;

	/// category of the iterator
	using iterator_category = std::bidirectional_iterator_tag;

	/// pointer to object "pointed to" by the iterator
	using pointer = U*;

	/// reference to object "pointed to" by the iterator
	using reference = U&;

	/// value "pointed to" by the iterator
	using value_type = U;

	/**
	 * \brief IntrusiveListIterator's constructor
	 */

	constexpr IntrusiveListIterator() :
			node_{}
	{

	}

	/**
	 * \brief IntrusiveListIterator's constructor
	 *
	 * \param [in] node is a pointer to IntrusiveListNode of element that will be "pointed to" by the iterator
	 */

	constexpr explicit IntrusiveListIterator(IntrusiveListNode* const node) :
			node_{node}
	{

	}

	/**
	 * \brief IntrusiveListIterator's constructor
	 *
	 * \param [in] element is a reference to element that will be "pointed to" by the iterator
	 */

	constexpr explicit IntrusiveListIterator(reference element) :
			node_{&(element.*NodePointer)}
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");
	}

	/**
	 * \brief IntrusiveListIterator's binary infix pointer member access operator
	 *
	 * \return pointer to object "pointed to" by the iterator
	 */

	pointer operator->() const
	{
		return getPointer();
	}

	/**
	 * \brief IntrusiveListIterator's unary prefix dereference operator
	 *
	 * \return reference to object "pointed to" by the iterator
	 */

	reference operator*() const
	{
		return *getPointer();
	}

	/**
	 * \brief IntrusiveListIterator's unary prefix increment operator
	 *
	 * \return reference to "this" iterator
	 */

	IntrusiveListIterator& operator++()
	{
		node_ = &node_->getNextNode();
		return *this;
	}

	/**
	 * \brief IntrusiveListIterator's unary postfix increment operator
	 *
	 * \return copy of "this" iterator before increment
	 */

	IntrusiveListIterator operator++(int)
	{
		const auto temporary = *this;
		node_ = &node_->getNextNode();
		return temporary;
	}

	/**
	 * \brief IntrusiveListIterator's unary prefix decrement operator
	 *
	 * \return reference to "this" iterator
	 */

	IntrusiveListIterator& operator--()
	{
		node_ = &node_->getPreviousNode();
		return *this;
	}

	/**
	 * \brief IntrusiveListIterator's unary postfix decrement operator
	 *
	 * \return copy of "this" iterator before decrement
	 */

	IntrusiveListIterator operator--(int)
	{
		const auto temporary = *this;
		node_ = &node_->getPreviousNode();
		return temporary;
	}

	/**
	 * \brief IntrusiveListIterator's "equal to" comparison operator
	 *
	 * \param [in] other is a const reference to IntrusiveListIterator on right-hand side of comparison operator
	 *
	 * \return true if both iterators are equal, false otherwise
	 */

	bool operator==(const IntrusiveListIterator& other) const
	{
		return node_ == other.node_;
	}

private:

	/**
	 * \brief Converts contained pointer to IntrusiveListNode to pointer to object that contains this node.
	 *
	 * \return pointer to object "pointed to" by the iterator
	 */

	pointer getPointer() const
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");

		const auto offset = reinterpret_cast<size_t>(&(static_cast<pointer>(nullptr)->*NodePointer));
		return reinterpret_cast<pointer>(reinterpret_cast<size_t>(node_) - offset);
	}

	/// pointer to IntrusiveListNode of the object "pointed to" by the iterator
	IntrusiveListNode* node_;
};

/**
 * \brief IntrusiveListIterator's "not equal to" comparison operator
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveListIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveListIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, IntrusiveListNode T::* NodePointer, typename U = T>
inline bool operator!=(const IntrusiveListIterator<T, NodePointer, U>& left,
		const IntrusiveListIterator<T, NodePointer, U>& right)
{
	return (left == right) == false;
}

/**
 * \brief IntrusiveListConstIterator class is a const iterator of elements on IntrusiveList.
 *
 * This class provides an interface similar to std::list::const_iterator.
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a const pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 */

template<typename T, const IntrusiveListNode T::* NodePointer, typename U = T>
class IntrusiveListConstIterator
{
public:

	/// difference type
	using difference_type = ptrdiff_t;

	/// category of the iterator
	using iterator_category = std::bidirectional_iterator_tag;

	/// pointer to object "pointed to" by the iterator
	using pointer = const U*;

	/// reference to object "pointed to" by the iterator
	using reference = const U&;

	/// value "pointed to" by the iterator
	using value_type = U;

	/**
	 * \brief IntrusiveListConstIterator's constructor
	 */

	constexpr IntrusiveListConstIterator() :
			node_{}
	{

	}

	/**
	 * \brief IntrusiveListConstIterator's constructor
	 *
	 * \param [in] node is a pointer to const IntrusiveListNode of element that will be "pointed to" by the iterator
	 */

	constexpr explicit IntrusiveListConstIterator(const IntrusiveListNode* const node) :
			node_{node}
	{

	}

	/**
	 * \brief IntrusiveListConstIterator's constructor
	 *
	 * \param [in] element is a const reference to element that will be "pointed to" by the iterator
	 */

	constexpr explicit IntrusiveListConstIterator(reference element) :
			node_{&(element.*NodePointer)}
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");
	}

	/**
	 * \brief IntrusiveListConstIterator's constructor
	 *
	 * Converts non-const iterator (IntrusiveListIterator) to const iterator (IntrusiveListConstIterator).
	 *
	 * \tparam NonConstNodePointer is a non-const version of \a NodePointer
	 *
	 * \param [in] iterator is a const reference to non-const iterator (IntrusiveListIterator)
	 */

	template<IntrusiveListNode T::* NonConstNodePointer>
	constexpr IntrusiveListConstIterator(const IntrusiveListIterator<T, NonConstNodePointer, U>& iterator) :
			IntrusiveListConstIterator{*iterator}
	{

	}

	/**
	 * \brief IntrusiveListConstIterator's binary infix pointer member access operator
	 *
	 * \return pointer to object "pointed to" by the iterator
	 */

	pointer operator->() const
	{
		return getPointer();
	}

	/**
	 * \brief IntrusiveListConstIterator's unary prefix dereference operator
	 *
	 * \return reference to object "pointed to" by the iterator
	 */

	reference operator*() const
	{
		return *getPointer();
	}

	/**
	 * \brief IntrusiveListConstIterator's unary prefix increment operator
	 *
	 * \return reference to "this" iterator
	 */

	IntrusiveListConstIterator& operator++()
	{
		node_ = &node_->getNextNode();
		return *this;
	}

	/**
	 * \brief IntrusiveListConstIterator's unary postfix increment operator
	 *
	 * \return copy of "this" iterator before increment
	 */

	IntrusiveListConstIterator operator++(int)
	{
		const auto temporary = *this;
		node_ = &node_->getNextNode();
		return temporary;
	}

	/**
	 * \brief IntrusiveListConstIterator's unary prefix decrement operator
	 *
	 * \return reference to "this" iterator
	 */

	IntrusiveListConstIterator& operator--()
	{
		node_ = &node_->getPreviousNode();
		return *this;
	}

	/**
	 * \brief IntrusiveListConstIterator's unary postfix decrement operator
	 *
	 * \return copy of "this" iterator before decrement
	 */

	IntrusiveListConstIterator operator--(int)
	{
		const auto temporary = *this;
		node_ = &node_->getPreviousNode();
		return temporary;
	}

	/**
	 * \brief IntrusiveListConstIterator's "equal to" comparison operator
	 *
	 * \param [in] other is a const reference to IntrusiveListConstIterator on right-hand side of comparison operator
	 *
	 * \return true if both iterators are equal, false otherwise
	 */

	bool operator==(const IntrusiveListConstIterator& other) const
	{
		return node_ == other.node_;
	}

private:

	/**
	 * \brief Converts contained pointer to IntrusiveListNode to pointer to object that contains this node.
	 *
	 * \return pointer to object "pointed to" by the iterator
	 */

	pointer getPointer() const
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");

		const auto offset = reinterpret_cast<size_t>(&(static_cast<pointer>(nullptr)->*NodePointer));
		return reinterpret_cast<pointer>(reinterpret_cast<size_t>(node_) - offset);
	}

	/// pointer to const IntrusiveListNode of the object "pointed to" by the iterator
	const IntrusiveListNode* node_;
};

/**
 * \brief IntrusiveListConstIterator's "not equal to" comparison operator
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a const pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveListConstIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveListConstIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, const IntrusiveListNode T::* NodePointer, typename U = T>
inline bool operator!=(const IntrusiveListConstIterator<T, NodePointer, U>& left,
		const IntrusiveListConstIterator<T, NodePointer, U>& right)
{
	return (left == right) == false;
}

/**
 * \brief "Equal to" comparison operator for IntrusiveListIterator and IntrusiveListConstIterator
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam ConstNodePointer is a const pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveListIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveListConstIterator on right-hand side of comparison operator
 *
 * \return true if both iterators are equal, false otherwise
 */

template<typename T, IntrusiveListNode T::* NodePointer, const IntrusiveListNode T::* ConstNodePointer, typename U = T>
inline bool operator==(const IntrusiveListIterator<T, NodePointer, U>& left,
		const IntrusiveListConstIterator<T, ConstNodePointer, U>& right)
{
	return decltype(right){left} == right;
}

/**
 * \brief "Not equal to" comparison operator for IntrusiveListIterator and IntrusiveListConstIterator
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam ConstNodePointer is a const pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveListIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveListConstIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, IntrusiveListNode T::* NodePointer, const IntrusiveListNode T::* ConstNodePointer, typename U = T>
inline bool operator!=(const IntrusiveListIterator<T, NodePointer, U>& left,
		const IntrusiveListConstIterator<T, ConstNodePointer, U>& right)
{
	return (left == right) == false;
}

/**
 * \brief "Not equal to" comparison operator for IntrusiveListConstIterator and IntrusiveListIterator
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam ConstNodePointer is a const pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveListConstIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveListIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, IntrusiveListNode T::* NodePointer, const IntrusiveListNode T::* ConstNodePointer, typename U = T>
inline bool operator!=(const IntrusiveListConstIterator<T, ConstNodePointer, U>& left,
		const IntrusiveListIterator<T, NodePointer, U>& right)
{
	return right != left;
}

/**
 * \brief IntrusiveList class is an intrusive circular doubly linked list.
 *
 * This class tries to provide an interface similar to std::list.
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T; using different type than \a T can be
 * used to break circular dependencies, because \a T must be fully defined to instantiate this class, but it is enough
 * to forward declare \a U - it only needs to be fully defined to use member functions
 */

template<typename T, IntrusiveListNode T::* NodePointer, typename U = T>
class IntrusiveList
{
public:

	/// const iterator of elements on the list
	using const_iterator = IntrusiveListConstIterator<T, NodePointer, U>;

	/// const reverse iterator of elements on the list
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	/// const pointer to value linked in the list
	using const_pointer = const U*;

	/// const reference to value linked in the list
	using const_reference = const U&;

	/// iterator of elements on the list
	using iterator = IntrusiveListIterator<T, NodePointer, U>;

	/// reverse iterator of elements on the list
	using reverse_iterator = std::reverse_iterator<iterator>;

	/// pointer to value linked in the list
	using pointer = U*;

	/// reference to value linked in the list
	using reference = U&;

	/// value linked in the list
	using value_type = U;

	/**
	 * \brief IntrusiveList's constructor
	 */

	constexpr IntrusiveList() :
			intrusiveListBase_{}
	{

	}

	/**
	 * \return reference to last element on the list
	 */

	reference back()
	{
		return *--end();
	}

	/**
	 * \return const reference to last element on the list
	 */

	const_reference back() const
	{
		return *--end();
	}

	/**
	 * \return iterator of first element on the list
	 */

	iterator begin()
	{
		return iterator{&intrusiveListBase_.begin()};
	}

	/**
	 * \return const iterator of first element on the list
	 */

	const_iterator begin() const
	{
		return const_iterator{&intrusiveListBase_.begin()};
	}

	/**
	 * \return const iterator of first element on the list
	 */

	const_iterator cbegin() const
	{
		return begin();
	}

	/**
	 * \return const iterator of "one past the last" element on the list
	 */

	const_iterator cend() const
	{
		return end();
	}

	/**
	 * \brief Unlinks all elements from the list.
	 */

	void clear()
	{
		intrusiveListBase_.clear();
	}

	/**
	 * \return const reverse iterator to first element in the reversed list (last element of the non-reversed list)
	 */

	const_reverse_iterator crbegin() const
	{
		return rbegin();
	}

	/**
	 * \return const reverse iterator to "one past the last" element in the reversed list ("one before the first"
	 * element of the non-reversed list)
	 */

	const_reverse_iterator crend() const
	{
		return rend();
	}

	/**
	 * \return true is the list is empty, false otherwise
	 */

	bool empty() const
	{
		return intrusiveListBase_.empty();
	}

	/**
	 * \return iterator of "one past the last" element on the list
	 */

	iterator end()
	{
		return iterator{&intrusiveListBase_.end()};
	}

	/**
	 * \return const iterator of "one past the last" element on the list
	 */

	const_iterator end() const
	{
		return const_iterator{&intrusiveListBase_.end()};
	}

	/**
	 * \return reference to first element on the list
	 */

	reference front()
	{
		return *begin();
	}

	/**
	 * \return const reference to first element on the list
	 */

	const_reference front() const
	{
		return *begin();
	}

	/**
	 * \brief Unlinks the last element from the list.
	 */

	void pop_back()
	{
		erase(--end());
	}

	/**
	 * \brief Unlinks the first element from the list.
	 */

	void pop_front()
	{
		erase(begin());
	}

	/**
	 * \brief Links the element at the end of the list.
	 *
	 * \param [in] newElement is a reference to the element that will be linked in the list
	 */

	void push_back(reference newElement)
	{
		insert(end(), newElement);
	}

	/**
	 * \brief Links the element at the beginning of the list.
	 *
	 * \param [in] newElement is a reference to the element that will be linked in the list
	 */

	void push_front(reference newElement)
	{
		insert(begin(), newElement);
	}

	/**
	 * \return reverse iterator to first element in the reversed list (last element of the non-reversed list)
	 */

	reverse_iterator rbegin()
	{
		return reverse_iterator{end()};
	}

	/**
	 * \return const reverse iterator to first element in the reversed list (last element of the non-reversed list)
	 */

	const_reverse_iterator rbegin() const
	{
		return const_reverse_iterator{end()};
	}

	/**
	 * \return reverse iterator to "one past the last" element in the reversed list ("one before the first" element of
	 * the non-reversed list)
	 */

	reverse_iterator rend()
	{
		return reverse_iterator{begin()};
	}

	/**
	 * \return const reverse iterator to "one past the last" element in the reversed list ("one before the first"
	 * element of the non-reversed list)
	 */

	const_reverse_iterator rend() const
	{
		return const_reverse_iterator{begin()};
	}

	/**
	 * \brief Swaps contents with another list.
	 *
	 * \param [in] other is a reference to IntrusiveList with which contents of this list will be swapped
	 */

	void swap(IntrusiveList& other)
	{
		intrusiveListBase_.swap(other.intrusiveListBase_);
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
		auto& positionNode = (*position).*NodePointer;
		auto& nextNode = internal::IntrusiveListBase::erase(positionNode);
		return iterator{&nextNode};
	}

	/**
	 * \brief Links the element in the list before \a position.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is an iterator of the element before which \a newNode will be linked
	 * \param [in] newElement is a reference to the element that will be linked in the list
	 *
	 * \return iterator of \a newElement
	 */

	static iterator insert(const iterator position, reference newElement)
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");

		auto& positionNode = (*position).*NodePointer;
		auto& newElementNode = newElement.*NodePointer;
		internal::IntrusiveListBase::insert(positionNode, newElementNode);
		return iterator{&newElementNode};
	}

	/**
	 * \brief Transfers the element from one list to another list before \a position.
	 *
	 * \note No instance of any list is needed for this operation.
	 *
	 * \param [in] position is an iterator of the element before which \a splicedElement will be linked
	 * \param [in] splicedElement is an iterator of the element that will be spliced from one list to another
	 */

	static void splice(const iterator position, const iterator splicedElement)
	{
		auto& positionNode = (*position).*NodePointer;
		auto& splicedElementNode = (*splicedElement).*NodePointer;
		internal::IntrusiveListBase::splice(positionNode, splicedElementNode);
	}

	IntrusiveList(const IntrusiveList&) = delete;
	IntrusiveList(IntrusiveList&&) = default;
	const IntrusiveList& operator=(const IntrusiveList&) = delete;
	IntrusiveList& operator=(IntrusiveList&&) = delete;

private:

	/// internal IntrusiveListBase object
	internal::IntrusiveListBase intrusiveListBase_;
};

/**
 * \brief Swaps contents of two lists.
 *
 * \tparam T is the type that has the IntrusiveListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a reference to IntrusiveList with which contents of \a right will be swapped
 * \param [in] right is a reference to IntrusiveList with which contents of \a left will be swapped
 */

template<typename T, IntrusiveListNode T::* NodePointer, typename U = T>
inline void swap(IntrusiveList<T, NodePointer, U>& left, IntrusiveList<T, NodePointer, U>& right)
{
	left.swap(right);
}

}	// namespace estd

#endif	// ESTD_INTRUSIVELIST_HPP_
