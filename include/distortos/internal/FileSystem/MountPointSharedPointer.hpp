/**
 * \file
 * \brief MountPointSharedPointer class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_MOUNTPOINTSHAREDPOINTER_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_MOUNTPOINTSHAREDPOINTER_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/internal/FileSystem/MountPoint.hpp"

#include <memory>

namespace distortos
{

class FileSystem;

namespace internal
{

/// MountPointSharedPointer class is a shared pointer to a mount point
class MountPointSharedPointer
{
public:

	/**
	 * \brief MountPointSharedPointer's default constructor
	 */

	constexpr MountPointSharedPointer() :
			mountPoint_{}
	{

	}

	/**
	 * \brief MountPointSharedPointer's constructor
	 *
	 * \param [in] mountPoint is an unique pointer to mount point
	 */

	explicit MountPointSharedPointer(std::unique_ptr<MountPoint>&& mountPoint) :
			mountPoint_{mountPoint.release()}
	{
		if (mountPoint_ != nullptr)
			mountPoint_->incrementReferenceCount();
	}

	/**
	 * \brief MountPointSharedPointer's copy constructor
	 *
	 * \param [in] other is a reference to MountPointSharedPointer object used as source of copy
	 */

	MountPointSharedPointer(const MountPointSharedPointer& other) :
			mountPoint_{other.mountPoint_}
	{
		if (mountPoint_ != nullptr)
			mountPoint_->incrementReferenceCount();
	}

	/**
	 * \brief MountPointSharedPointer's move constructor
	 *
	 * \param [in] other is a reference to MountPointSharedPointer object used as source of move
	 */

	MountPointSharedPointer(MountPointSharedPointer&& other) :
			mountPoint_{other.mountPoint_}
	{
		other.mountPoint_ = {};
	}

	/**
	 * \brief MountPointSharedPointer's destructor
	 */

	~MountPointSharedPointer()
	{
		if (mountPoint_ != nullptr)
			mountPoint_->decrementReferenceCount();
	}

	/**
	 * \brief MountPointSharedPointer's copy assignment
	 *
	 * \param [in] other is a reference to MountPointSharedPointer object used as source of copy assignment
	 */

	MountPointSharedPointer& operator=(const MountPointSharedPointer& other)
	{
		if (mountPoint_ != other.mountPoint_)
		{
			if (other.mountPoint_ != nullptr)
				other.mountPoint_->incrementReferenceCount();

			if (mountPoint_ != nullptr)
				mountPoint_->decrementReferenceCount();

			mountPoint_ = other.mountPoint_;
		}

		return *this;
	}

	/**
	 * \brief MountPointSharedPointer's move assignment
	 *
	 * \param [in] other is a reference to MountPointSharedPointer object used as source of move assignment
	 */

	MountPointSharedPointer& operator=(MountPointSharedPointer&& other)
	{
		if (this != &other)
		{
			if (mountPoint_ != nullptr)
				mountPoint_->decrementReferenceCount();

			mountPoint_ = other.mountPoint_;
			other.mountPoint_ = nullptr;
		}

		return *this;
	}

	/**
	 * \return pointer to shared mount point
	 */

	MountPoint* operator->() const
	{
		return mountPoint_;
	}

	/**
	 * \return reference to shared mount point
	 */

	MountPoint& operator*() const
	{
		return *mountPoint_;
	}

	/**
	 * \return pointer to shared mount point
	 */

	operator MountPoint*() const
	{
		return mountPoint_;
	}

	/**
	 * \return true if valid pointer to shared mount point is held by this object, false otherwise
	 */

	operator bool() const
	{
		return mountPoint_ != nullptr;
	}

	/**
	 * \return pointer to shared mount point
	 */

	MountPoint* get() const
	{
		return mountPoint_;
	}

private:

	/// pointer to shared mount point
	MountPoint* mountPoint_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_MOUNTPOINTSHAREDPOINTER_HPP_
