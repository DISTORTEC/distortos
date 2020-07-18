/**
 * \file
 * \brief _sbrk_r() system call implementation
 *
 * \author Copyright (C) 2014 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include <cerrno>
#include <cstdint>

extern "C"
{

/*---------------------------------------------------------------------------------------------------------------------+
| global functions
+---------------------------------------------------------------------------------------------------------------------*/

/**
 * \brief Increase program data space.
 *
 * This version of _sbrk_r() requires the heap area to be defined explicitly in linker script with symbols __heap_start
 * and __heap_end.
 *
 * \param [in] size is the requested data space size
 *
 * \return pointer to new data space
 */

void* _sbrk_r(_reent*, const intptr_t size)
{
	extern char __heap_start[];						// imported from linker script
	extern char __heap_end[];						// imported from linker script
	static auto currentHeapEnd_ = __heap_start;

	if (currentHeapEnd_ + size > __heap_end)		// is there enough space on the heap left?
	{
		errno = ENOMEM;
		return reinterpret_cast<void*>(-1);
	}

	const auto previousHeapEnd = currentHeapEnd_;
	currentHeapEnd_ += size;

	return previousHeapEnd;
}

}	// extern "C"
