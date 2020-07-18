/**
 * \file
 * \brief ReferenceHolder template class header.
 *
 * \author Copyright (C) 2014-2017 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_REFERENCEHOLDER_HPP_
#define ESTD_REFERENCEHOLDER_HPP_

namespace estd
{

/**
 * \brief ReferenceHolder template class is a ROMable holder of a reference.
 *
 * \tparam T is the type of reference held in the object
 */

template<typename T>
class ReferenceHolder
{
public:

	/**
	 * \brief ReferenceHolder's constructor.
	 *
	 * \param [in] reference is a reference that will be held by the object
	 */

	constexpr explicit ReferenceHolder(T& reference) noexcept :
			reference_{reference}
	{

	}

	/// \return reference held by the object
	constexpr operator T&() const noexcept
	{
		return reference_;
	}

	/// \return reference held by the object
	constexpr T& get() const noexcept
	{
		return reference_;
	}

private:

	/// reference held by the object
	T& reference_;
};

}	// namespace estd

#endif	// ESTD_REFERENCEHOLDER_HPP_
