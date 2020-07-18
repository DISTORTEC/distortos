/**
 * \file
 * \brief OperationCountingType class header
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef TEST_OPERATIONCOUNTINGTYPE_HPP_
#define TEST_OPERATIONCOUNTINGTYPE_HPP_

#include <utility>

#include <cstddef>

namespace distortos
{

namespace test
{

/// OperationCountingType class counts important operations (construction, destruction, assignment, swap) in static
/// variables; counting is not thread-safe!
class OperationCountingType
{
public:

	/// type used for OperationCountingType's "value"
	using Value = unsigned int;

	/**
	 * \brief OperationCountingType's constructor
	 *
	 * \param [in] value is the value held by object, default - 0
	 */

	explicit OperationCountingType(const Value value = {}) :
			value_{value}
	{
		++constructed_;
	}

	/**
	 * \brief OperationCountingType's copy constructor
	 *
	 * \param [in] other is a reference to OperationCountingType object used as source of copy
	 */

	OperationCountingType(const OperationCountingType& other) :
			value_{other.value_}
	{
		++copyConstructed_;
	}

	/**
	 * \brief OperationCountingType's move constructor
	 *
	 * \param [in] other is a rvalue reference to OperationCountingType object used as source of move
	 */

	OperationCountingType(OperationCountingType&& other) :
			value_{other.value_}
	{
		other.value_ = {};
		++moveConstructed_;
	}

	/**
	 * \brief OperationCountingType's destructor
	 */

	~OperationCountingType()
	{
		value_ = {};
		++destructed_;
	}

	/**
	 * \brief OperationCountingType's copy assignment
	 *
	 * \param [in] other is a reference to OperationCountingType object used as source of copy assignment
	 *
	 * \return reference to this
	 */

	OperationCountingType& operator=(const OperationCountingType& other)
	{
		value_ = other.value_;
		++copyAssigned_;
		return *this;
	}

	/**
	 * \brief OperationCountingType's move assignment
	 *
	 * \param [in] other is a rvalue reference to OperationCountingType object used as source of move assignment
	 *
	 * \return reference to this
	 */

	OperationCountingType& operator=(OperationCountingType&& other)
	{
		value_ = other.value_;
		other.value_ = {};
		++moveAssigned_;
		return *this;
	}

	/**
	 * \brief OperationCountingType's swap overload
	 *
	 * \param [in] left is a reference to first OperationCountingType object
	 * \param [in] right is a reference to second OperationCountingType object
	 */

	friend void swap(OperationCountingType& left, OperationCountingType& right)
	{
		using std::swap;
		swap(left.value_, right.value_);
		++swapped_;
	}

	/**
	 * \brief Equality comparison operator for OperationCountingType
	 *
	 * \param [in] left is a reference to left operand of equality comparison
	 * \param [in] right is a reference to right operand of equality comparison
	 *
	 * \return true if operands are equal, false otherwise
	 */

	friend bool operator==(const OperationCountingType& left, const OperationCountingType& right)
	{
		return left.value_ == right.value_;
	}

	/**
	 * \return value held by object
	 */

	Value getValue() const
	{
		return value_;
	}

	/**
	 * \brief Checks value of counters.
	 *
	 * \param [in] constructed is expected number of constructor calls since last resetCounters() call
	 * \param [in] copyConstructed is expected number of copy constructor calls since last resetCounters() call
	 * \param [in] moveConstructed is expected number of move constructor calls since last resetCounters() call
	 * \param [in] destructed is expected number of destructor calls since last resetCounters() call
	 * \param [in] copyAssigned is expected number of copy assignment calls since last resetCounters() call
	 * \param [in] moveAssigned is expected number of move assignment calls since last resetCounters() call
	 * \param [in] swapped is expected number of swap calls since last resetCounters() call
	 *
	 * \return true if counters match expected values, false otherwise
	 */

	static bool checkCounters(size_t constructed, size_t copyConstructed, size_t moveConstructed, size_t destructed,
			size_t copyAssigned, size_t moveAssigned, size_t swapped);

	/**
	 * \brief Resets value of all counters.
	 */

	static void resetCounters();

private:

	/// value held by object
	Value value_;

	/// counter of constructor calls since last resetCounters() call
	static size_t constructed_;

	/// counter of copy constructor calls since last resetCounters() call
	static size_t copyConstructed_;

	/// counter of move constructor calls since last resetCounters() call
	static size_t moveConstructed_;

	/// counter of destructor calls since last resetCounters() call
	static size_t destructed_;

	/// counter of copy assignment calls since last resetCounters() call
	static size_t copyAssigned_;

	/// counter of move assignment calls since last resetCounters() call
	static size_t moveAssigned_;

	/// counter of swap calls since last resetCounters() call
	static size_t swapped_;
};

/**
 * \brief Inequality comparison operator for OperationCountingType
 *
 * \param [in] left is a reference to left operand of inequality comparison
 * \param [in] right is a reference to right operand of inequality comparison
 *
 * \return true if operands are not equal, false otherwise
 */

inline bool operator!=(const OperationCountingType& left, const OperationCountingType& right)
{
	return (left == right) == false;
}

}	// namespace test

}	// namespace distortos

#endif	// TEST_OPERATIONCOUNTINGTYPE_HPP_
