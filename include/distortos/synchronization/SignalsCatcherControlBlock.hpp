/**
 * \file
 * \brief SignalsCatcherControlBlock class header
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-04-16
 */

#ifndef INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_
#define INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_

#include "distortos/SignalAction.hpp"

namespace distortos
{

namespace synchronization
{

/// SignalsCatcherControlBlock class is a structure required by threads for "catching" and "handling" of signals
class SignalsCatcherControlBlock
{
public:

	/// association of signal number with SignalAction
	using Association = std::pair<uint8_t, SignalAction>;

	/// type of uninitialized storage for Association objects
	using Storage = std::aligned_storage<sizeof(Association), alignof(Association)>::type;

	/**
	 * \brief SignalsCatcherControlBlock's constructor
	 *
	 * \param [in] storageBegin is a pointer to first element of storage for Association objects
	 * \param [in] storageEnd is a pointer to "one past the last" element of storage for Association objects
	 */

	constexpr SignalsCatcherControlBlock(Storage* const storageBegin, Storage* const storageEnd) :
			associationsBegin_{reinterpret_cast<decltype(associationsBegin_)>(storageBegin)},
			storageBegin_{storageBegin},
			storageEnd_{storageEnd}
	{

	}

private:

	/// pointer to first element of range of Association objects
	Association* associationsBegin_;

	/// union binds \a associationsEnd_ and \a storageBegin_ - these point to the same address
	union
	{
		/// pointer to "one past the last" element of range of Association objects
		Association* associationsEnd_;

		/// pointer to first element of range of Storage objects
		Storage* storageBegin_;
	};

	/// pointer to "one past the last" element of range of Storage objects
	Storage* storageEnd_;
};

}	// namespace synchronization

}	// namespace distortos

#endif	// INCLUDE_DISTORTOS_SYNCHRONIZATION_SIGNALSCATCHERCONTROLBLOCK_HPP_
