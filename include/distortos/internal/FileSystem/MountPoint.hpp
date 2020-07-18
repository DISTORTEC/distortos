/**
 * \file
 * \brief MountPoint class header
 *
 * \author Copyright (C) 2020 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_MOUNTPOINT_HPP_
#define INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_MOUNTPOINT_HPP_

#include "distortos/distortosConfiguration.h"

#if DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#include "distortos/Mutex.hpp"

namespace distortos
{

class FileSystem;

namespace internal
{

/// MountPoint class is a mount point for file system
class MountPoint
{
public:

	/// max length of mount point name
	constexpr static size_t maxNameLength {31};

	/**
	 * \brief MountPoint's constructor
	 *
	 * \pre %File system is mounted.
	 * \pre \a length is valid.
	 *
	 * \param [in] fileSystem is a reference to file system managed by this mount point
	 * \param [in] name is the name of the mountpoint
	 * \param [in] length is the length of \a name, must be less than or equal to \a maxNameLength
	 */

	MountPoint(FileSystem& fileSystem, const char* name, size_t length);

	/**
	 * \brief MountPoint's destructor
	 *
	 * \pre The reference count of this object is zero.
	 */

	~MountPoint();

	/**
	 * \brief Decrements reference count of this object.
	 *
	 * When reference count reaches zero, file system is unmounted and the object deletes itself.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The reference count of this object is not zero.
	 */

	void decrementReferenceCount();

	/**
	 * \return reference to file system managed by this control block
	 */

	FileSystem& getFileSystem() const
	{
		return fileSystem_;
	}

	/**
	 * \return name of the mount point
	 */

	const char* getName() const
	{
		return name_;
	}

	/**
	 * \return number of references to this object
	 */

	uint8_t getReferenceCount() const
	{
		return referenceCount_;
	}

	/**
	 * \brief Increments reference count of this object.
	 *
	 * \warning This function must not be called from interrupt context!
	 *
	 * \pre The reference count of this object is less than 255.
	 */

	void incrementReferenceCount();

	MountPoint(const MountPoint&) = delete;
	MountPoint(MountPoint&&) = delete;
	MountPoint& operator=(const MountPoint&) = delete;
	MountPoint& operator=(MountPoint&&) = delete;

private:

	/// mutex for serializing access to this object
	Mutex mutex_;

	/// reference to file system managed by this mount point
	FileSystem& fileSystem_;

	/// name of the mountpoint
	char name_[maxNameLength + 1];

	/// number of references to this object
	uint8_t referenceCount_;
};

}	// namespace internal

}	// namespace distortos

#endif	// DISTORTOS_FILESYSTEMS_STANDARD_LIBRARY_INTEGRATION_ENABLE == 1

#endif	// INCLUDE_DISTORTOS_INTERNAL_FILESYSTEM_MOUNTPOINT_HPP_
