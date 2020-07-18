/**
 * \file
 * \brief IntegerSequence template class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_INTEGERSEQUENCE_HPP_
#define ESTD_INTEGERSEQUENCE_HPP_

#include <type_traits>

namespace estd
{

/**
 * \brief Compile-time sequence of integers
 *
 * Similar to std::integer_sequence from C++14 - http://en.cppreference.com/w/cpp/utility/integer_sequence
 *
 * \tparam T is an integer type to use for the elements of the sequence
 * \tparam Integers is a non-type parameter pack representing the sequence
 */

template<typename T, T... Integers>
class IntegerSequence
{
public:

	/// integer type used for the elements of the sequence
	using value_type = T;

	/**
	 * \return number of elements in the sequence
	 */

	constexpr static std::size_t size() noexcept
	{
		return sizeof...(Integers);
	};
};

/**
 * \brief Compile-time sequence of std::size_t elements
 *
 * Similar to std::index_sequence from C++14 - http://en.cppreference.com/w/cpp/utility/integer_sequence
 *
 * \tparam Indexes is a non-type parameter pack representing the sequence
 */

template<std::size_t... Indexes>
using IndexSequence = IntegerSequence<std::size_t, Indexes...>;

namespace internal
{

/**
 * \brief IntegerSequence with two internal type aliases.
 *
 * \tparam T is an integer type to use for the elements of the sequence
 * \tparam Integers is a non-type parameter pack representing the sequence
 */

template<typename T, T... Integers>
struct TypedSequence : IntegerSequence<T, Integers...>
{
	/// type of base class
	using base = IntegerSequence<T, Integers...>;

	/// type of class
	using type = TypedSequence;
};

/**
 * \brief TypedSequence with doubled number of elements
 *
 * \tparam Sequence is the type of sequence that will be doubled
 */

template<typename Sequence>
struct DoubledIntegerSequence;

/**
 * \brief TypedSequence with doubled number of elements
 *
 * Specialization for TypedSequence.
 *
 * \tparam T is an integer type to use for the elements of the sequence
 * \tparam Integers is a non-type parameter pack representing the sequence
 */

template<typename T, T... Integers>
struct DoubledIntegerSequence<TypedSequence<T, Integers...>>
{
	/// TypedSequence with doubled number of elements - TypedSequence<T, 0, 1, ..., N - 1> is turned into
	/// TypedSequence<T, 0, 1, ..., N - 1, N, N + 1, ..., 2 * N - 1>
	using type = TypedSequence<T, Integers..., (sizeof...(Integers) + Integers)...>;
};

/**
 * \brief TypedSequence optionally extended by one element
 *
 * \tparam Extend selects whether the sequence will be extended by one element (true) or not (false)
 * \tparam Sequence is the type of sequence that will optionally be extended
 */

template<bool Extend, typename Sequence>
struct ExtendedIntegerSequence
{
	/// same as \a Sequence
	using type = Sequence;
};

/**
 * \brief TypedSequence optionally extended by one element
 *
 * Specialization for the case with extending.
 *
 * \tparam T is an integer type to use for the elements of the sequence
 * \tparam Integers is a non-type parameter pack representing the sequence
 */

template<typename T, T... Integers>
struct ExtendedIntegerSequence<true, TypedSequence<T, Integers...>>
{
	/// sequence extended by one element - TypedSequence<T, 0, 1, ..., N - 1> is turned into
	/// TypedSequence<T, 0, 1, ..., N - 1, N>
	using type = TypedSequence<T, Integers..., sizeof...(Integers)>;
};

/**
 * \brief Implementation of generator of IntegerSequence types
 *
 * Generates TypedSequence<T, 0, 1, ..., N - 1> type.
 *
 * \tparam T is an integer type to use for the elements of the sequence
 * \tparam N is the requested number of elements in the sequence
 */

template<typename T, std::size_t N>
struct MakeIntegerSequenceImplementation :
		ExtendedIntegerSequence<N % 2 != 0,
				typename DoubledIntegerSequence<typename MakeIntegerSequenceImplementation<T, N / 2>::type>::type>
{

};

/**
 * \brief Implementation of generator of IntegerSequence types
 *
 * Specialization for terminal case - 0 elements - generates TypedSequence<T> type.
 *
 * \tparam T is an integer type to use for the elements of the sequence
 */

template<typename T>
struct MakeIntegerSequenceImplementation<T, 0>
{
	/// empty TypedSequence<T> type
	using type = TypedSequence<T>;
};

/**
 * \brief Wrapper for MakeIntegerSequenceImplementation that ensures \a N is non-negative
 *
 * Generates TypedSequence<T, 0, 1, ..., N - 1> type.
 *
 * \tparam T is an integer type to use for the elements of the sequence
 * \tparam N is the requested number of elements in the sequence, must be non-negative
 */

template<typename T, T N>
struct MakeIntegerSequenceImplementationWrapper :
		std::enable_if<N >= 0, MakeIntegerSequenceImplementation<T, static_cast<std::size_t>(N)>>::type
{
	static_assert(N >= 0, "Number of elements in the sequence must be non-negative!");
};

}	// namespace internal

/**
 * \brief Generator of IntegerSequence types
 *
 * Similar to std::make_integer_sequence from C++14 - http://en.cppreference.com/w/cpp/utility/integer_sequence
 *
 * Whole implementation is based on code from http://stackoverflow.com/a/20101039/157344
 *
 * Generates IntegerSequence<T, 0, 1, ..., N - 1> type.
 *
 * \tparam T is an integer type to use for the elements of the sequence
 * \tparam N is the requested number of elements in the sequence
 */

template<typename T, T N>
using MakeIntegerSequence = typename internal::MakeIntegerSequenceImplementationWrapper<T, N>::type::base;

/**
 * \brief Generator of IndexSequence types
 *
 * Similar to std::make_index_sequence from C++14 - http://en.cppreference.com/w/cpp/utility/integer_sequence
 *
 * Generates IndexSequence<0, 1, ..., N - 1> type.
 *
 * \tparam N is the requested number of elements in the sequence
 */

template<std::size_t N>
using MakeIndexSequence = MakeIntegerSequence<std::size_t, N>;

/**
 * \brief Generator of IndexSequence types
 *
 * Similar to std::index_sequence_for from C++14 - http://en.cppreference.com/w/cpp/utility/integer_sequence
 *
 * Generates IndexSequence<0, 1, ..., sizeof...(T) - 1> type.
 *
 * \tparam T is the type parameter pack for which an index sequence of the same length will be generated
 */

template<typename... T>
using IndexSequenceFor = MakeIndexSequence<sizeof...(T)>;

}	// namespace estd

#endif	// ESTD_INTEGERSEQUENCE_HPP_
