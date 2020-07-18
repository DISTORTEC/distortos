/**
 * \file
 * \brief SignalSet class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_SIGNALSET_HPP_
#define INCLUDE_DISTORTOS_SIGNALSET_HPP_

#include <bitset>

namespace distortos
{

/**
 * \brief SignalSet class is used as a set of signals.
 *
 * Similar to POSIX sigset_t - https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/signal.h.html
 *
 * \ingroup signals
 */

class SignalSet
{
public:

	/// type of internal bitset for 32 signals
	using Bitset = std::bitset<32>;

	/// tag struct to construct empty SignalSet
	struct Empty
	{

	};

	/// tag struct to construct full SignalSet
	struct Full
	{

	};

	/// tag object to construct empty SignalSet
	constexpr static Empty empty = {};

	/// tag object to construct full SignalSet
	constexpr static Full full = {};

	/**
	 * \brief SignalSet's constructor
	 *
	 * \param [in] bitmask is the bit mask used to initialize internal bitset
	 */

	constexpr explicit SignalSet(uint32_t bitmask) :
			bitset_{bitmask}
	{

	}

	/**
	 * \brief SignalSet's constructor
	 *
	 * \param [in] bitset is a reference to Bitset from which internal bitset is copy-constructed
	 */

	constexpr explicit SignalSet(const Bitset& bitset) :
			bitset_{bitset}
	{

	}

	/**
	 * \brief SignalSet's constructor
	 *
	 * Constructs empty SignalSet.
	 *
	 * Similar to sigemptyset() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigemptyset.html#
	 */

	constexpr explicit SignalSet(const Empty&) :
			SignalSet{uint32_t{}}
	{

	}

	/**
	 * \brief SignalSet's constructor
	 *
	 * Constructs full SignalSet.
	 *
	 * Similar to sigfillset() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigfillset.html#
	 */

	constexpr explicit SignalSet(const Full&) :
			SignalSet{~uint32_t{}}
	{

	}

	/**
	 * \brief Sets single bit.
	 *
	 * Similar to sigaddset() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigaddset.html#
	 *
	 * \param [in] signalNumber is the bit position that will be set, [0; 31]
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	int add(const uint8_t signalNumber)
	{
		return set(signalNumber, true);
	}

	/**
	 * \return copy of internal bitset
	 */

	Bitset getBitset() const
	{
		return bitset_;
	}

	/**
	 * \brief Clears single bit.
	 *
	 * Similar to sigdelset() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigdelset.html#
	 *
	 * \param [in] signalNumber is the bit position that will be cleared, [0; 31]
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	int remove(const uint8_t signalNumber)
	{
		return set(signalNumber, false);
	}

	/**
	 * \brief Tests whether the bit is set.
	 *
	 * Similar to sigismember() - https://pubs.opengroup.org/onlinepubs/9699919799/functions/sigismember.html#
	 *
	 * \param [in] signalNumber is the bit position that will be tested, [0; 31]
	 *
	 * \return pair with return code (0 on success, error code otherwise) and value of selected bit; error codes:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	std::pair<int, bool> test(uint8_t signalNumber) const;

private:

	/**
	 * \brief Sets value of single bit.
	 *
	 * \param [in] signalNumber is the bit position that will be modified, [0; 31]
	 * \param [in] value is the new value for selected bit
	 *
	 * \return 0 on success, error code otherwise:
	 * - EINVAL - \a signalNumber value is invalid;
	 */

	int set(uint8_t signalNumber, bool value);

	/// internal bitset for 32 signals
	Bitset bitset_;
};

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SIGNALSET_HPP_
