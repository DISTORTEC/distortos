/**
 * \file
 * \brief IntrusiveForwardList template class header
 *
 * \author Copyright (C) 2015-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_INTRUSIVEFORWARDLIST_HPP_
#define ESTD_INTRUSIVEFORWARDLIST_HPP_

#include <iterator>

#include <cstddef>

namespace estd
{

namespace internal
{

class IntrusiveForwardListBase;

}

/**
 * \brief IntrusiveForwardListNode class is the node that is needed for the object to be linked in IntrusiveForwardList
 *
 * To some extent, this class can be considered to be a limited (raw) iterator.
 *
 * The object that wants to be linked in IntrusiveForwardList must contain a variable of this type - one for each
 * intrusive forward list that will be used with object.
 */

class IntrusiveForwardListNode
{
public:

	/// AccessKey class is used to limit access to IntrusiveForwardListNode's linkAfter() and unlinkNext() functions -
	/// only internal::IntrusiveForwardListBase can link/unlink nodes
	class AccessKey
	{
		friend internal::IntrusiveForwardListBase;

		/**
		 * \brief AccessKey's constructor
		 */

		constexpr AccessKey()
		{

		}

		AccessKey(const AccessKey&) = delete;
		AccessKey(AccessKey&&) = delete;
		const AccessKey& operator=(const AccessKey&) = delete;
		AccessKey& operator=(AccessKey&&) = delete;
	};

	/**
	 * \brief IntrusiveForwardListNode's constructor
	 */

	constexpr IntrusiveForwardListNode() :
			nextNode_{}
	{

	}

	/**
	 * \brief IntrusiveForwardListNode's move constructor
	 *
	 * \param [in] other is a rvalue reference to IntrusiveForwardListNode used as source of move construction
	 */

	IntrusiveForwardListNode(IntrusiveForwardListNode&& other) :
			nextNode_{other.nextNode_}
	{
		other.reset();
	}

	/**
	 * \return pointer to next node on the list
	 */

	IntrusiveForwardListNode* getNextNode() const
	{
		return nextNode_;
	}

	/**
	 * \return true if the node is linked in some list, false otherwise
	 */

	bool isLinked() const
	{
		return nextNode_ != nullptr;
	}

	/**
	 * \brief Links the node in the list after \a position.
	 *
	 * \note Access to this function is restricted only to functions from internal::IntrusiveForwardListBase class
	 *
	 * \param [in] position is a pointer to node after which this node will be linked
	 * \param [in] accessKey is used to limit access to this function
	 */

	void linkAfter(IntrusiveForwardListNode* const position, AccessKey)
	{
		nextNode_ = position->getNextNode();
		position->nextNode_ = this;
	}

	/**
	 * \brief Swaps contents with another node.
	 *
	 * \param [in] other is a reference to IntrusiveForwardListNode with which contents of this node will be swapped
	 */

	void swap(IntrusiveForwardListNode& other)
	{
		using std::swap;
		swap(nextNode_, other.nextNode_);
	}

	/**
	 * \brief Unlinks the node following this one from the list.
	 *
	 * \note Access to this function is restricted only to functions from internal::IntrusiveForwardListBase class
	 *
	 * \param [in] accessKey is used to limit access to this function
	 */

	void unlinkNext(AccessKey)
	{
		auto& nextNode = *nextNode_;
		nextNode_ = nextNode.nextNode_;

		nextNode.reset();
	}

	IntrusiveForwardListNode(const IntrusiveForwardListNode&) = delete;
	const IntrusiveForwardListNode& operator=(const IntrusiveForwardListNode&) = delete;
	IntrusiveForwardListNode& operator=(IntrusiveForwardListNode&&) = delete;

private:

	/**
	 * \brief Resets the node to the same state as right after construction.
	 */

	void reset()
	{
		nextNode_ = {};
	}

	/// pointer to next node on the list
	IntrusiveForwardListNode* nextNode_;
};

/**
 * \brief Swaps contents of two nodes.
 *
 * \param [in] left is a reference to IntrusiveForwardListNode with which contents of \a right will be swapped
 * \param [in] right is a reference to IntrusiveForwardListNode with which contents of \a left will be swapped
 */

inline void swap(IntrusiveForwardListNode& left, IntrusiveForwardListNode& right)
{
	left.swap(right);
}

namespace internal
{

/**
 * \brief IntrusiveForwardListBase class provides base functionalities for IntrusiveForwardList class, but without any
 * knowledge about types
 *
 * This class tries to provide an interface similar to std::forward_list.
 */

class IntrusiveForwardListBase
{
public:

	/**
	 * \brief IntrusiveForwardListBase's constructor
	 */

	constexpr IntrusiveForwardListBase() :
			rootNode_{}
	{

	}

	/**
	 * \brief IntrusiveForwardListBase's destructor
	 *
	 * Unlinks all nodes from the list.
	 */

	~IntrusiveForwardListBase()
	{
		clear();
	}

	/**
	 * \return pointer to "one before the first" node on the list
	 */

	IntrusiveForwardListNode* before_begin()
	{
		return &rootNode_;
	}

	/**
	 * \return const pointer to "one before the first" node on the list
	 */

	const IntrusiveForwardListNode* before_begin() const
	{
		return &rootNode_;
	}

	/**
	 * \return pointer to first node on the list
	 */

	IntrusiveForwardListNode* begin()
	{
		return rootNode_.getNextNode();
	}

	/**
	 * \return const pointer to first node on the list
	 */

	const IntrusiveForwardListNode* begin() const
	{
		return rootNode_.getNextNode();
	}

	/**
	 * \return const pointer to "one before the first" node on the list
	 */

	const IntrusiveForwardListNode* cbefore_begin() const
	{
		return before_begin();
	}

	/**
	 * \return const pointer to first node on the list
	 */

	const IntrusiveForwardListNode* cbegin() const
	{
		return begin();
	}

	/**
	 * \return const pointer to "one past the last" node on the list
	 */

	const IntrusiveForwardListNode* cend() const
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
		return begin() == end();
	}

	/**
	 * \return pointer to "one past the last" node on the list
	 */

	IntrusiveForwardListNode* end()
	{
		return nullptr;
	}

	/**
	 * \return const pointer to "one past the last" node on the list
	 */

	const IntrusiveForwardListNode* end() const
	{
		return nullptr;
	}

	/**
	 * \brief Unlinks the first node from the list.
	 */

	void pop_front()
	{
		erase_after(before_begin());
	}

	/**
	 * \brief Links the node at the beginning of the list.
	 *
	 * \param [in] newNode is a reference to node that will be linked in the list
	 */

	void push_front(IntrusiveForwardListNode& newNode)
	{
		insert_after(before_begin(), newNode);
	}

	/**
	 * \brief Swaps contents with another list.
	 *
	 * \param [in] other is a reference to IntrusiveForwardListBase with which contents of this list will be swapped
	 */

	void swap(IntrusiveForwardListBase& other)
	{
		rootNode_.swap(other.rootNode_);
	}

	/**
	 * \brief Unlinks the node following \a position from the list.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is a pointer to the node preceding the one which will be unlinked from the list
	 *
	 * \return pointer to the node that was following the node which was unlinked
	 */

	static IntrusiveForwardListNode* erase_after(IntrusiveForwardListNode* const position)
	{
		position->unlinkNext({});
		return position->getNextNode();
	}

	/**
	 * \brief Links the node in the list after \a position.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is a pointer to node after which \a newNode will be linked
	 * \param [in] newNode is a reference to node that will be linked in the list
	 */

	static void insert_after(IntrusiveForwardListNode* const position, IntrusiveForwardListNode& newNode)
	{
		newNode.linkAfter(position, {});
	}

	/**
	 * \brief Transfers the node from one list to another list after \a position.
	 *
	 * \note No instance of any list is needed for this operation.
	 *
	 * \param [in] position is a pointer to node after which spliced node will be linked
	 * \param [in] beforeSplicedNode is a pointer to node preceding the one which will be spliced from one list to
	 * another
	 */

	static void splice_after(IntrusiveForwardListNode* const position,
			IntrusiveForwardListNode* const beforeSplicedNode)
	{
		const auto splicedNode = beforeSplicedNode->getNextNode();
		erase_after(beforeSplicedNode);
		insert_after(position, *splicedNode);
	}

	IntrusiveForwardListBase(const IntrusiveForwardListBase&) = delete;
	IntrusiveForwardListBase(IntrusiveForwardListBase&&) = default;
	const IntrusiveForwardListBase& operator=(const IntrusiveForwardListBase&) = delete;
	IntrusiveForwardListBase& operator=(IntrusiveForwardListBase&&) = delete;

private:

	/// root node of the intrusive forward list
	IntrusiveForwardListNode rootNode_;
};

/**
 * \brief Swaps contents of two lists.
 *
 * \param [in] left is a reference to IntrusiveForwardListBase with which contents of \a right will be swapped
 * \param [in] right is a reference to IntrusiveForwardListBase with which contents of \a left will be swapped
 */

inline void swap(IntrusiveForwardListBase& left, IntrusiveForwardListBase& right)
{
	left.swap(right);
}

}	// namespace internal

/**
 * \brief IntrusiveForwardListIterator class is an iterator of elements on IntrusiveForwardList.
 *
 * This class provides an interface similar to std::forward_list::iterator.
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 */

template<typename T, IntrusiveForwardListNode T::* NodePointer, typename U = T>
class IntrusiveForwardListIterator
{
public:

	/// difference type
	using difference_type = ptrdiff_t;

	/// category of the iterator
	using iterator_category = std::forward_iterator_tag;

	/// pointer to object "pointed to" by the iterator
	using pointer = U*;

	/// reference to object "pointed to" by the iterator
	using reference = U&;

	/// value "pointed to" by the iterator
	using value_type = U;

	/**
	 * \brief IntrusiveForwardListIterator's constructor
	 */

	constexpr IntrusiveForwardListIterator() :
			node_{}
	{

	}

	/**
	 * \brief IntrusiveForwardListIterator's constructor
	 *
	 * \param [in] node is a pointer to IntrusiveForwardListNode of element that will be "pointed to" by the iterator
	 */

	constexpr explicit IntrusiveForwardListIterator(IntrusiveForwardListNode* const node) :
			node_{node}
	{

	}

	/**
	 * \brief IntrusiveForwardListIterator's constructor
	 *
	 * \param [in] element is a reference to element that will be "pointed to" by the iterator
	 */

	constexpr explicit IntrusiveForwardListIterator(reference element) :
			node_{&(element.*NodePointer)}
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");
	}

	/**
	 * \brief IntrusiveForwardListIterator's binary infix pointer member access operator
	 *
	 * \return pointer to object "pointed to" by the iterator
	 */

	pointer operator->() const
	{
		return getPointer();
	}

	/**
	 * \brief IntrusiveForwardListIterator's unary prefix dereference operator
	 *
	 * \return reference to object "pointed to" by the iterator
	 */

	reference operator*() const
	{
		return *getPointer();
	}

	/**
	 * \brief IntrusiveForwardListIterator's unary prefix increment operator
	 *
	 * \return reference to "this" iterator
	 */

	IntrusiveForwardListIterator& operator++()
	{
		node_ = node_->getNextNode();
		return *this;
	}

	/**
	 * \brief IntrusiveForwardListIterator's unary postfix increment operator
	 *
	 * \return copy of "this" iterator before increment
	 */

	IntrusiveForwardListIterator operator++(int)
	{
		const auto temporary = *this;
		node_ = node_->getNextNode();
		return temporary;
	}

	/**
	 * \brief IntrusiveForwardListIterator's "equal to" comparison operator
	 *
	 * \param [in] other is a const reference to IntrusiveForwardListIterator on right-hand side of equality operator
	 *
	 * \return true if both iterators are equal, false otherwise
	 */

	bool operator==(const IntrusiveForwardListIterator& other) const
	{
		return node_ == other.node_;
	}

private:

	/**
	 * \brief Converts contained pointer to IntrusiveForwardListNode to pointer to object that contains this node.
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

	/// pointer to IntrusiveForwardListNode of the object "pointed to" by the iterator
	IntrusiveForwardListNode* node_;
};

/**
 * \brief IntrusiveForwardListIterator's "not equal to" comparison operator
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveForwardListIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveForwardListIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, IntrusiveForwardListNode T::* NodePointer, typename U = T>
inline bool operator!=(const IntrusiveForwardListIterator<T, NodePointer, U>& left,
		const IntrusiveForwardListIterator<T, NodePointer, U>& right)
{
	return (left == right) == false;
}

/**
 * \brief IntrusiveForwardListConstIterator class is a const iterator of elements on IntrusiveForwardList.
 *
 * This class provides an interface similar to std::forward_list::const_iterator.
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a const pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 */

template<typename T, const IntrusiveForwardListNode T::* NodePointer, typename U = T>
class IntrusiveForwardListConstIterator
{
public:

	/// difference type
	using difference_type = ptrdiff_t;

	/// category of the iterator
	using iterator_category = std::forward_iterator_tag;

	/// pointer to object "pointed to" by the iterator
	using pointer = const U*;

	/// reference to object "pointed to" by the iterator
	using reference = const U&;

	/// value "pointed to" by the iterator
	using value_type = U;

	/**
	 * \brief IntrusiveForwardListConstIterator's constructor
	 */

	constexpr IntrusiveForwardListConstIterator() :
			node_{}
	{

	}

	/**
	 * \brief IntrusiveForwardListConstIterator's constructor
	 *
	 * \param [in] node is a pointer to const IntrusiveForwardListNode of element that will be "pointed to" by the
	 * iterator
	 */

	constexpr explicit IntrusiveForwardListConstIterator(const IntrusiveForwardListNode* const node) :
			node_{node}
	{

	}

	/**
	 * \brief IntrusiveForwardListConstIterator's constructor
	 *
	 * \param [in] element is a const reference to element that will be "pointed to" by the iterator
	 */

	constexpr explicit IntrusiveForwardListConstIterator(reference element) :
			node_{&(element.*NodePointer)}
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");
	}

	/**
	 * \brief IntrusiveForwardListConstIterator's constructor
	 *
	 * Converts non-const iterator (IntrusiveForwardListIterator) to const iterator (IntrusiveForwardListConstIterator).
	 *
	 * \tparam NonConstNodePointer is a non-const version of \a NodePointer
	 *
	 * \param [in] iterator is a const reference to non-const iterator (IntrusiveForwardListIterator)
	 */

	template<IntrusiveForwardListNode T::* NonConstNodePointer>
	constexpr
	IntrusiveForwardListConstIterator(const IntrusiveForwardListIterator<T, NonConstNodePointer, U>& iterator) :
			IntrusiveForwardListConstIterator{*iterator}
	{

	}

	/**
	 * \brief IntrusiveForwardListConstIterator's binary infix pointer member access operator
	 *
	 * \return pointer to object "pointed to" by the iterator
	 */

	pointer operator->() const
	{
		return getPointer();
	}

	/**
	 * \brief IntrusiveForwardListConstIterator's unary prefix dereference operator
	 *
	 * \return reference to object "pointed to" by the iterator
	 */

	reference operator*() const
	{
		return *getPointer();
	}

	/**
	 * \brief IntrusiveForwardListConstIterator's unary prefix increment operator
	 *
	 * \return reference to "this" iterator
	 */

	IntrusiveForwardListConstIterator& operator++()
	{
		node_ = node_->getNextNode();
		return *this;
	}

	/**
	 * \brief IntrusiveForwardListConstIterator's unary postfix increment operator
	 *
	 * \return copy of "this" iterator before increment
	 */

	IntrusiveForwardListConstIterator operator++(int)
	{
		const auto temporary = *this;
		node_ = node_->getNextNode();
		return temporary;
	}

	/**
	 * \brief IntrusiveForwardListConstIterator's "equal to" comparison operator
	 *
	 * \param [in] other is a const reference to IntrusiveForwardListConstIterator on right-hand side of equality
	 * operator
	 *
	 * \return true if both iterators are equal, false otherwise
	 */

	bool operator==(const IntrusiveForwardListConstIterator& other) const
	{
		return node_ == other.node_;
	}

private:

	/**
	 * \brief Converts contained pointer to IntrusiveForwardListNode to pointer to object that contains this node.
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

	/// pointer to const IntrusiveForwardListNode of the object "pointed to" by the iterator
	const IntrusiveForwardListNode* node_;
};

/**
 * \brief IntrusiveForwardListConstIterator's "not equal to" comparison operator
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a const pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveForwardListConstIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveForwardListConstIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, const IntrusiveForwardListNode T::* NodePointer, typename U = T>
inline bool operator!=(const IntrusiveForwardListConstIterator<T, NodePointer, U>& left,
		const IntrusiveForwardListConstIterator<T, NodePointer, U>& right)
{
	return (left == right) == false;
}

/**
 * \brief "Equal to" comparison operator for IntrusiveForwardListIterator and IntrusiveForwardListConstIterator
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam ConstNodePointer is a const pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveForwardListIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveForwardListConstIterator on right-hand side of comparison operator
 *
 * \return true if both iterators are equal, false otherwise
 */

template<typename T, IntrusiveForwardListNode T::* NodePointer, const IntrusiveForwardListNode T::* ConstNodePointer,
		typename U = T>
inline bool operator==(const IntrusiveForwardListIterator<T, NodePointer, U>& left,
		const IntrusiveForwardListConstIterator<T, ConstNodePointer, U>& right)
{
	return decltype(right){left} == right;
}

/**
 * \brief "Not equal to" comparison operator for IntrusiveForwardListIterator and IntrusiveForwardListConstIterator
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam ConstNodePointer is a const pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveForwardListIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveForwardListConstIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, IntrusiveForwardListNode T::* NodePointer, const IntrusiveForwardListNode T::* ConstNodePointer,
		typename U = T>
inline bool operator!=(const IntrusiveForwardListIterator<T, NodePointer, U>& left,
		const IntrusiveForwardListConstIterator<T, ConstNodePointer, U>& right)
{
	return (left == right) == false;
}

/**
 * \brief "Not equal to" comparison operator for IntrusiveForwardListConstIterator and IntrusiveForwardListIterator
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam ConstNodePointer is a const pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a const reference to IntrusiveForwardListConstIterator on left-hand side of comparison operator
 * \param [in] right is a const reference to IntrusiveForwardListIterator on right-hand side of comparison operator
 *
 * \return true if iterators are not equal, false otherwise
 */

template<typename T, IntrusiveForwardListNode T::* NodePointer, const IntrusiveForwardListNode T::* ConstNodePointer,
		typename U = T>
inline bool operator!=(const IntrusiveForwardListConstIterator<T, ConstNodePointer, U>& left,
		const IntrusiveForwardListIterator<T, NodePointer, U>& right)
{
	return right != left;
}

/**
 * \brief IntrusiveForwardList class is an intrusive linear singly linked list.
 *
 * This class tries to provide an interface similar to std::forward_list.
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T; using different type than \a T can be
 * used to break circular dependencies, because \a T must be fully defined to instantiate this class, but it is enough
 * to forward declare \a U - it only needs to be fully defined to use member functions
 */

template<typename T, IntrusiveForwardListNode T::* NodePointer, typename U = T>
class IntrusiveForwardList
{
public:

	/// const iterator of elements on the list
	using const_iterator = IntrusiveForwardListConstIterator<T, NodePointer, U>;

	/// const pointer to value linked in the list
	using const_pointer = const U*;

	/// const reference to value linked in the list
	using const_reference = const U&;

	/// iterator of elements on the list
	using iterator = IntrusiveForwardListIterator<T, NodePointer, U>;

	/// pointer to value linked in the list
	using pointer = U*;

	/// reference to value linked in the list
	using reference = U&;

	/// value linked in the list
	using value_type = U;

	/**
	 * \brief IntrusiveForwardList's constructor
	 */

	constexpr IntrusiveForwardList() :
			intrusiveForwardListBase_{}
	{

	}

	/**
	 * \return iterator of "one before the first" element on the list
	 */

	iterator before_begin()
	{
		return iterator{intrusiveForwardListBase_.before_begin()};
	}

	/**
	 * \return const iterator of "one before the first" element on the list
	 */

	const_iterator before_begin() const
	{
		return const_iterator{intrusiveForwardListBase_.before_begin()};
	}

	/**
	 * \return iterator of first element on the list
	 */

	iterator begin()
	{
		return iterator{intrusiveForwardListBase_.begin()};
	}

	/**
	 * \return const iterator of first element on the list
	 */

	const_iterator begin() const
	{
		return const_iterator{intrusiveForwardListBase_.begin()};
	}

	/**
	 * \return const iterator of "one before the first" element on the list
	 */

	const_iterator cbefore_begin() const
	{
		return before_begin();
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
		intrusiveForwardListBase_.clear();
	}

	/**
	 * \return true is the list is empty, false otherwise
	 */

	bool empty() const
	{
		return intrusiveForwardListBase_.empty();
	}

	/**
	 * \return iterator of "one past the last" element on the list
	 */

	iterator end()
	{
		return iterator{intrusiveForwardListBase_.end()};
	}

	/**
	 * \return const iterator of "one past the last" element on the list
	 */

	const_iterator end() const
	{
		return const_iterator{intrusiveForwardListBase_.end()};
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
	 * \brief Unlinks the first element from the list.
	 */

	void pop_front()
	{
		erase_after(before_begin());
	}

	/**
	 * \brief Links the element at the beginning of the list.
	 *
	 * \param [in] newElement is a reference to the element that will be linked in the list
	 */

	void push_front(reference newElement)
	{
		insert_after(before_begin(), newElement);
	}

	/**
	 * \brief Swaps contents with another list.
	 *
	 * \param [in] other is a reference to IntrusiveForwardList with which contents of this list will be swapped
	 */

	void swap(IntrusiveForwardList& other)
	{
		intrusiveForwardListBase_.swap(other.intrusiveForwardListBase_);
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
		auto& positionNode = (*position).*NodePointer;
		const auto afterNextNode = internal::IntrusiveForwardListBase::erase_after(&positionNode);
		return iterator{afterNextNode};
	}

	/**
	 * \brief Links the element in the list after \a position.
	 *
	 * \note No instance of the list is needed for this operation.
	 *
	 * \param [in] position is an iterator of the element after which \a newNode will be linked
	 * \param [in] newElement is a reference to the element that will be linked in the list
	 *
	 * \return iterator of \a newElement
	 */

	static iterator insert_after(const iterator position, reference newElement)
	{
		static_assert(std::is_same<U, T>::value == true || std::is_convertible<U*, T*>::value == true,
				"U* must be implicitly convertible to T*!");

		auto& positionNode = (*position).*NodePointer;
		auto& newElementNode = newElement.*NodePointer;
		internal::IntrusiveForwardListBase::insert_after(&positionNode, newElementNode);
		return iterator{&newElementNode};
	}

	/**
	 * \brief Transfers the element from one list to another list after \a position.
	 *
	 * \note No instance of any list is needed for this operation.
	 *
	 * \param [in] position is an iterator of the element after which spliced element will be linked
	 * \param [in] beforeSplicedElement is an iterator of the element preceding the one which will be spliced from one
	 * list to another
	 */

	static void splice_after(const iterator position, const iterator beforeSplicedElement)
	{
		auto& positionNode = (*position).*NodePointer;
		auto& beforeSplicedElementNode = (*beforeSplicedElement).*NodePointer;
		internal::IntrusiveForwardListBase::splice_after(&positionNode, &beforeSplicedElementNode);
	}

	IntrusiveForwardList(const IntrusiveForwardList&) = delete;
	IntrusiveForwardList(IntrusiveForwardList&&) = default;
	const IntrusiveForwardList& operator=(const IntrusiveForwardList&) = delete;
	IntrusiveForwardList& operator=(IntrusiveForwardList&&) = delete;

private:

	/// internal IntrusiveForwardListBase object
	internal::IntrusiveForwardListBase intrusiveForwardListBase_;
};

/**
 * \brief Swaps contents of two lists.
 *
 * \tparam T is the type that has the IntrusiveForwardListNode variable
 * \tparam NodePointer is a pointer-to-member to IntrusiveForwardListNode variable in \a T
 * \tparam U is the type that will be stored on the list; it can be different from \a T, but \a U* must be implicitly
 * convertible to \a T* (so usually a type derived from \a T); default - \a T;
 *
 * \param [in] left is a reference to IntrusiveForwardList with which contents of \a right will be swapped
 * \param [in] right is a reference to IntrusiveForwardList with which contents of \a left will be swapped
 */

template<typename T, IntrusiveForwardListNode T::* NodePointer, typename U = T>
inline void swap(IntrusiveForwardList<T, NodePointer, U>& left, IntrusiveForwardList<T, NodePointer, U>& right)
{
	left.swap(right);
}

}	// namespace estd

#endif	// ESTD_INTRUSIVEFORWARDLIST_HPP_
