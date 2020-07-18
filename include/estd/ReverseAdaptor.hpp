/**
 * \file
 * \brief ReverseAdaptor template class header.
 *
 * \author Copyright (C) 2014-2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef ESTD_REVERSEADAPTOR_HPP_
#define ESTD_REVERSEADAPTOR_HPP_

namespace estd
{

/**
 * \brief ReverseAdaptor template class is an adaptor that "reverses" access to the container
 *
 * \tparam T is the type of container
 */

template<typename T>
class ReverseAdaptor
{
public:

	/**
	 * \brief ReverseAdaptor's constructor.
	 *
	 * \param [in] container is a reference to container
	 */

	constexpr explicit ReverseAdaptor(T& container) noexcept :
			container_{container}
	{

	}

	/**
	 * \return reverse_iterator to the beginning of "reversed" container (last element of original container)
	 */

	typename T::reverse_iterator begin() const noexcept
	{
		return container_.rbegin();
	}

	/**
	 * \return reverse_iterator to the end of "reversed" container (before-the-first element of original container)
	 */

	typename T::reverse_iterator end() const noexcept
	{
		return container_.rend();
	}

private:

	/// reference to container
	T& container_;
};

/**
 * \brief Helper factory function to make ReverseAdaptor object with deduced template arguments
 *
 * \tparam T is the type of container
 *
 * \param [in] container is a reference to container
 *
 * \return ReverseAdaptor object
 */

template<typename T>
ReverseAdaptor<T> makeReverseAdaptor(T& container)
{
	return ReverseAdaptor<T>{container};
}

}	// namespace estd

#endif	// ESTD_REVERSEADAPTOR_HPP_
