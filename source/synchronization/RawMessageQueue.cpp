/**
 * \file
 * \brief RawMessageQueue class implementation
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * \date 2015-01-17
 */

#include "distortos/RawMessageQueue.hpp"

#include <cstring>
#include <cerrno>

namespace distortos
{

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

/// PushFunctor is a functor used for pushing of data to the RawMessageQueue
class PushFunctor : public synchronization::MessageQueueBase::Functor
{
public:

	/**
	 * \brief PushFunctor's constructor
	 *
	 * \param [in] data is a pointer to data that will be pushed to RawMessageQueue
	 * \param [in] size is the size of \a data, bytes
	 */

	constexpr PushFunctor(const void* const data, const size_t size) :
			data_{data},
			size_{size}
	{

	}

	/**
	 * \brief Copies the data to RawMessageQueue's storage (with memcpy()).
	 *
	 * \param [in,out] storage is a pointer to storage for element
	 */

	virtual void operator()(void* const storage) const override
	{
		memcpy(storage, data_, size_);
	}

private:

	/// pointer to data that will be pushed to RawMessageQueue
	const void* const data_;

	/// size of \a data_, bytes
	const size_t size_;
};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

int RawMessageQueue::pushInternal(const synchronization::SemaphoreFunctor& waitSemaphoreFunctor, const uint8_t priority,
		const void* const data, const size_t size)
{
	if (size != elementSize_)
		return EMSGSIZE;

	const PushFunctor pushFunctor {data, size};
	return messageQueueBase_.push(waitSemaphoreFunctor, priority, pushFunctor);
}

}	// namespace distortos
