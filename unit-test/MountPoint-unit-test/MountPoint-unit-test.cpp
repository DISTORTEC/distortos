/**
 * \file
 * \brief MountPoint test cases
 *
 * This test checks whether MountPoint perform all operations properly and in correct order.
 *
 * \author Copyright (C) 2021 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "distortos/internal/FileSystem/MountPoint.hpp"

#include "distortos/FileSystem/FileSystem.hpp"

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class FileSystem : public distortos::FileSystem
{
public:

	using OpenDirectoryResult = std::pair<int, std::unique_ptr<distortos::Directory>>;
	using OpenFileResult = std::pair<int, std::unique_ptr<distortos::File>>;

	MAKE_MOCK0(format, int());
	MAKE_MOCK2(getFileStatus, int(const char*, struct stat&));
	MAKE_MOCK1(getStatus, int(struct statvfs&));
	MAKE_MOCK0(lock, void());
	MAKE_MOCK2(makeDirectory, int(const char*, mode_t));
	MAKE_MOCK0(mount, int());
	MAKE_MOCK1(openDirectory, OpenDirectoryResult(const char*));
	MAKE_MOCK2(openFile, OpenFileResult(const char*, int));
	MAKE_MOCK1(remove, int(const char*));
	MAKE_MOCK2(rename, int(const char*, const char*));
	MAKE_MOCK0(unlock, void());
	MAKE_MOCK0(unmount, int());
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

const char* const mountPointName {"mountPointName"};

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing MountPoint", "[MountPoint]")
{
	FileSystem fileSystemMock {};
	distortos::mock::Mutex mutexMock {distortos::mock::Mutex::UnitTestTag{}};
	trompeloeil::sequence sequence {};

	auto mp = std::make_unique<distortos::internal::MountPoint>(fileSystemMock, mountPointName, strlen(mountPointName));

	{
		INFO("Mount point should return proper file system reference");
		REQUIRE(&mp->getFileSystem() == &fileSystemMock);
	}
	{
		INFO("Mount point should return proper name");
		REQUIRE(strcmp(mp->getName(), mountPointName) == 0);
	}
	{
		INFO("Mount point should allow incrementing its reference count 255 times");

		for (int i {}; i < 255; ++i)
		{
			REQUIRE(mp->getReferenceCount() == i);

			REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			mp->incrementReferenceCount();
		}
	}
	{
		INFO("Mount point should perform no action when decrementing its reference count down to 1");

		for (int i {255}; i > 1; --i)
		{
			REQUIRE(mp->getReferenceCount() == i);

			REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
			mp->decrementReferenceCount();
		}
	}
	{
		INFO("Mount point should unmount file system and delete itself when its reference count reaches 0");

		REQUIRE(mp->getReferenceCount() == 1);

		REQUIRE_CALL(mutexMock, lock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(mutexMock, unlock()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(fileSystemMock, unmount()).IN_SEQUENCE(sequence).RETURN(0);
		mp.release()->decrementReferenceCount();
	}
}
