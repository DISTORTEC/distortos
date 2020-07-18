/**
 * \file
 * \brief BufferingBlockDevice test cases
 *
 * This test checks whether BufferingBlockDevice perform all operations properly and in correct order.
 *
 * \author Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "unit-test-common.hpp"

#include "distortos/devices/memory/BufferingBlockDevice.hpp"

#include "estd/ContiguousRange.hpp"

using trompeloeil::_;

namespace
{

/*---------------------------------------------------------------------------------------------------------------------+
| local types
+---------------------------------------------------------------------------------------------------------------------*/

class BlockDevice : public distortos::devices::BlockDevice
{
public:

	MAKE_MOCK0(close, int());
	MAKE_MOCK2(erase, int(uint64_t, uint64_t));
	MAKE_CONST_MOCK0(getBlockSize, size_t());
	MAKE_CONST_MOCK0(getSize, uint64_t());
	MAKE_MOCK0(lock, void());
	MAKE_MOCK0(open, int());
	MAKE_MOCK3(read, int(uint64_t, void*, size_t));
	MAKE_MOCK0(synchronize, int());
	MAKE_MOCK0(unlock, void());
	MAKE_MOCK3(write, int(uint64_t, const void*, size_t));
};

/*---------------------------------------------------------------------------------------------------------------------+
| local objects
+---------------------------------------------------------------------------------------------------------------------*/

constexpr size_t alignment {DISTORTOS_BLOCKDEVICE_BUFFER_ALIGNMENT};
constexpr size_t blockSize {8};
constexpr uint64_t deviceSize {UINT64_MAX};
const uint8_t randomData[]
{
		0x50, 0xc7, 0x04, 0x8b, 0xbe, 0xd3, 0xb2, 0x70, 0xd8, 0xfd, 0x12, 0x26, 0x73, 0x99, 0x48, 0xc0,
		0xe7, 0x70, 0xdb, 0x9e, 0xa3, 0x6f, 0x29, 0x24, 0x86, 0xa1, 0x0e, 0xf5, 0xf1, 0x1c, 0xac, 0xb7,
		0x5f, 0x84, 0xbb, 0x2e, 0xce, 0xb6, 0xdd, 0x24, 0x51, 0x21, 0xcc, 0x11, 0xef, 0xe7, 0x1e, 0x05,
		0x73, 0x08, 0xe6, 0x72, 0x15, 0xf5, 0x5a, 0xb9, 0x5a, 0xad, 0xc7, 0xba, 0x9c, 0x1a, 0xd4, 0x92,
		0xac, 0xa2, 0x25, 0xf1, 0x05, 0x9f, 0x06, 0x67, 0x65, 0xbb, 0x83, 0xe6, 0x76, 0xa7, 0x53, 0xcd,
		0x89, 0x2a, 0xf1, 0x31, 0x05, 0x2b, 0x66, 0xce, 0x4f, 0x60, 0x17, 0xcb, 0x55, 0xab, 0x21, 0xee,
		0x75, 0xb7, 0x66, 0x16, 0x65, 0x17, 0x13, 0x8d, 0xcc, 0x30, 0xf5, 0xc8, 0x26, 0x5b, 0x3d, 0xe7,
		0x60, 0xbd, 0x16, 0x98, 0xbd, 0x0e, 0xae, 0x0c, 0xac, 0xda, 0xee, 0x55, 0xe2, 0x4c, 0x98, 0xa9,
		0x26, 0x3d, 0x2d, 0xdb, 0xd7, 0x5e, 0x79, 0xb0, 0x4a, 0x38, 0x7b, 0xfa, 0x6f, 0x73, 0xdb, 0x97,
		0x8e, 0x20, 0x59, 0xa1, 0x98, 0xd2, 0x88, 0x54, 0x80, 0x98, 0xce, 0xa0, 0x1b, 0x6c, 0x6e, 0x22,
		0x10, 0x64, 0xee, 0x77, 0x37, 0xc4, 0x50, 0x22, 0x50, 0x27, 0x32, 0xf3, 0x51, 0x2f, 0x9e, 0xd4,
		0xae, 0x83, 0x65, 0x16, 0x19, 0xe4, 0x7a, 0x15, 0xeb, 0x6a, 0xb6, 0x8f, 0xa4, 0xb4, 0x26, 0xb6,
		0x6b, 0x6b, 0x4b, 0x5c, 0x0e, 0x8c, 0xc2, 0x84, 0xb6, 0x18, 0x60, 0xd0, 0xd5, 0x12, 0x6c, 0x82,
		0x0b, 0x7a, 0xf2, 0x55, 0x00, 0x64, 0x05, 0x51, 0xb4, 0xc5, 0xa2, 0x9c, 0xe4, 0xfe, 0x04, 0x10,
		0x0f, 0x84, 0x22, 0x35, 0x42, 0x1e, 0xe6, 0x97, 0x13, 0xf3, 0xce, 0x2d, 0x80, 0xbe, 0x1b, 0xa4,
		0xd2, 0xb8, 0xa1, 0x85, 0x7e, 0xe9, 0xa1, 0x70, 0x86, 0x71, 0x95, 0x4c, 0x91, 0x55, 0xc6, 0x91,
		0xf4, 0x00, 0xdb, 0xc7, 0xa8, 0x95, 0xa9, 0xa6, 0xce, 0xad, 0x88, 0x6c, 0x01, 0xf6, 0xb0, 0x69,
		0xd5, 0xb7, 0x97, 0xf1, 0x4a, 0x43, 0x53, 0x34, 0x1a, 0x45, 0xad, 0x73, 0x6f, 0x0e, 0x2e, 0xcb,
		0xb3, 0x02, 0x74, 0x0f, 0xb1, 0x62, 0xb9, 0x3b, 0xed, 0x35, 0x24, 0x59, 0x29, 0x8a, 0x06, 0x8e,
		0x57, 0x63, 0x14, 0x68, 0xec, 0xc6, 0x79, 0x47, 0xf3, 0x3e, 0xc0, 0x12, 0xfa, 0xde, 0xac, 0xa1,
		0x51, 0x8a, 0xc4, 0x8d, 0xfd, 0x47, 0x03, 0xf3, 0x42, 0xde, 0x5b, 0xf5, 0xcc, 0x7e, 0x93, 0x2b,
		0x5c, 0xbe, 0x53, 0xc9, 0xcc, 0x2e, 0x5f, 0x21, 0x93, 0x3f, 0x05, 0xd1, 0x11, 0x36, 0x5c, 0x50,
		0x8f, 0x24, 0xdc, 0x59, 0x82, 0xb1, 0xad, 0x56, 0x15, 0xb6, 0xf4, 0xed, 0x5b, 0x9d, 0x28, 0x78,
		0x99, 0xb8, 0xfe, 0xea, 0x2e, 0xdf, 0x8f, 0x5b, 0x05, 0x75, 0xce, 0xec, 0x47, 0xbb, 0x08, 0x1f,
		0x68, 0x8e, 0x8a, 0xce, 0xa0, 0x52, 0x06, 0xb4, 0xce, 0x0a, 0xd8, 0xb0, 0xd6, 0x56, 0x60, 0x4f,
		0x49, 0x3c, 0x05, 0xee, 0x9e, 0x4e, 0x4f, 0x71, 0x9a, 0xa8, 0xfb, 0x23, 0x13, 0x39, 0x4d, 0x58,
		0xa6, 0xfa, 0x68, 0xa2, 0x45, 0x8f, 0xd2, 0xdb, 0x60, 0x78, 0xca, 0xca, 0x48, 0xa1, 0xe1, 0xa8,
		0xec, 0x44, 0xbb, 0x21, 0xc0, 0xfb, 0x18, 0xb7, 0x68, 0xc5, 0x91, 0x08, 0xf5, 0xe5, 0xb1, 0x81,
		0x28, 0x15, 0xe3, 0x47, 0x5a, 0xc6, 0x73, 0xe6, 0x66, 0x82, 0xde, 0xda, 0x7b, 0x67, 0xcf, 0x83,
		0xb1, 0x95, 0x2e, 0xa3, 0x71, 0x9a, 0xb5, 0x03, 0xb4, 0x25, 0xf1, 0xe8, 0x28, 0xb1, 0x9b, 0x79,
		0xc0, 0x52, 0x59, 0x59, 0x28, 0x93, 0xbf, 0x69, 0x32, 0x7d, 0x6a, 0xb9, 0x3d, 0xc1, 0xf6, 0xe8,
		0x5b, 0x17, 0x12, 0xed, 0xac, 0xbb, 0x4c, 0xbb, 0xad, 0x98, 0xc0, 0xaf, 0x7d, 0x67, 0xdf, 0x7b,
		0xa9, 0xed, 0x41, 0x84, 0xec, 0x66, 0x48, 0xbb, 0xc9, 0xf8, 0xe5, 0x01, 0x03, 0x7d, 0x9b, 0x35,
		0x27, 0xb2, 0xb2, 0x77, 0x74, 0x0a, 0xc1, 0xb7, 0x66, 0x5b, 0x76, 0x36, 0xc2, 0xfb, 0x78, 0x12,
		0x63, 0x25, 0xea, 0xc6, 0x06, 0x59, 0x7d, 0xaf, 0xb0, 0x95, 0xda, 0xec, 0x13, 0xe4, 0x19, 0x80,
		0x6c, 0x76, 0x44, 0x31, 0x9b, 0xa4, 0xe0, 0xb1, 0x5b, 0x29, 0x83, 0x64, 0x10, 0x8c, 0x4d, 0xc0,
		0xc9, 0x08, 0x0b, 0x88, 0x8c, 0x46, 0x31, 0x8e, 0x11, 0x37, 0x93, 0x6a, 0x93, 0x93, 0x8e, 0x4c,
		0x8f, 0xbb, 0x84, 0xb6, 0xf4, 0xc4, 0x7c, 0x75, 0xe4, 0x73, 0xb1, 0x4f, 0xa6, 0xab, 0xe6, 0xac,
		0x64, 0x0c, 0x8c, 0x6c, 0x71, 0xda, 0xe3, 0xbe, 0xef, 0xc5, 0x15, 0x89, 0xb8, 0x6b, 0x01, 0x5f,
		0xf9, 0x1d, 0x5c, 0x7b, 0x01, 0x01, 0x47, 0xa9, 0x72, 0x78, 0x23, 0xf7, 0x67, 0x4b, 0x94, 0xd6,
		0xe3, 0x52, 0x2a, 0x09, 0x63, 0x66, 0x14, 0xe9, 0x4f, 0xd7, 0x10, 0x37, 0xa4, 0x43, 0xba, 0x86,
		0x11, 0x3e, 0xe6, 0x48, 0x17, 0x9f, 0xd9, 0x39, 0x5a, 0x7f, 0x9a, 0x2d, 0xfc, 0x67, 0x8e, 0xa2,
		0x53, 0x9d, 0xf2, 0x31, 0x6d, 0xdf, 0x6d, 0x20, 0x5f, 0x91, 0x2b, 0xa9, 0x56, 0xcc, 0xc1, 0x74,
		0x2d, 0x35, 0xdc, 0x8d, 0x16, 0xf3, 0x63, 0xd7, 0x02, 0x81, 0x28, 0x95, 0xaa, 0x5f, 0x53, 0xc8,
		0xff, 0xd6, 0xa5, 0xf6, 0x6d, 0x90, 0x20, 0x79, 0xe0, 0x12, 0x8b, 0xe9, 0x57, 0xce, 0x3f, 0x98,
		0x7d, 0xca, 0xb3, 0x1e, 0x01, 0x4b, 0xb3, 0x26, 0x0a, 0xc9, 0x37, 0x80, 0x7c, 0x31, 0x08, 0xee,
		0x9e, 0x7b, 0xbe, 0xe9, 0x7a, 0xf8, 0xc9, 0x2c, 0xc7, 0x8d, 0x3e, 0xdc, 0x34, 0x17, 0x5b, 0xad,
		0x6c, 0x82, 0x9c, 0xdc, 0xc2, 0x56, 0x43, 0xe1, 0x59, 0xb8, 0x68, 0xb4, 0x1a, 0xad, 0x89, 0xd8,
		0x89, 0x29, 0x25, 0xe1, 0x60, 0x8d, 0xc4, 0x1e, 0x86, 0x70, 0xcc, 0x2d, 0x07, 0x3f, 0xda, 0x02,
		0xcf, 0x3f, 0xf8, 0x9b, 0xf4, 0xf4, 0x39, 0xd8, 0xec, 0x3c, 0x60, 0xb4, 0x2a, 0x18, 0xd8, 0x43,
		0x8f, 0xfd, 0x6c, 0x20, 0xe9, 0x63, 0xe8, 0x9b, 0x61, 0x34, 0x02, 0x73, 0x56, 0x2f, 0x2b, 0xcf,
		0xa9, 0xd3, 0x9f, 0x3f, 0xf0, 0xdf, 0x3e, 0x42, 0x3a, 0x37, 0x5e, 0xe5, 0x47, 0x6c, 0xf5, 0x86,
		0x15, 0x8f, 0x4b, 0x2d, 0x7d, 0x7f, 0x98, 0x81, 0x60, 0xfe, 0x46, 0x82, 0xa7, 0xcb, 0xad, 0xc7,
		0x28, 0x36, 0xc7, 0x53, 0x40, 0xf2, 0x05, 0xb5, 0x31, 0x85, 0x2f, 0xdf, 0xcf, 0x4e, 0xe5, 0x55,
		0x74, 0x54, 0xf1, 0x14, 0x3c, 0xc8, 0x54, 0xf6, 0x30, 0x3e, 0x32, 0x72, 0x11, 0x43, 0x85, 0x48,
		0xfd, 0x1f, 0x42, 0xdd, 0x20, 0x67, 0xb1, 0xa9, 0xca, 0x80, 0x51, 0xd2, 0x68, 0x9d, 0x78, 0x25,
		0x79, 0xf2, 0xd2, 0xe3, 0x50, 0x32, 0x83, 0x41, 0xcc, 0x88, 0x29, 0x10, 0x46, 0xb9, 0x43, 0x75,
		0x83, 0xd2, 0x30, 0xff, 0x62, 0x9c, 0x22, 0x3b, 0xcf, 0xcf, 0xb8, 0x53, 0xd7, 0x75, 0x3a, 0x52,
		0xe1, 0xb7, 0xbb, 0x1c, 0xb0, 0x4f, 0x0d, 0xe5, 0x90, 0x71, 0xf0, 0x22, 0xea, 0x81, 0x65, 0xd2,
		0xdb, 0x4e, 0xcf, 0x4a, 0xf4, 0x34, 0x20, 0x01, 0x36, 0xa2, 0x9b, 0xd5, 0xe8, 0x7f, 0xbd, 0x9f,
		0xf8, 0x5e, 0x23, 0x1f, 0xb4, 0x75, 0x12, 0x08, 0xec, 0x04, 0x33, 0xbc, 0xf5, 0xc4, 0x42, 0x34,
		0xaf, 0x10, 0xbb, 0xa6, 0x5d, 0xa3, 0x7b, 0x4e, 0x5a, 0xca, 0xfe, 0x6c, 0x05, 0xff, 0x42, 0x23,
		0xf1, 0x41, 0x8d, 0x56, 0x8d, 0xfc, 0x6c, 0x6c, 0xe9, 0x3f, 0x33, 0x5e, 0x55, 0x8a, 0xcc, 0x1a,
		0xaf, 0xad, 0x67, 0xf8, 0xef, 0x3a, 0x75, 0x48, 0xfe, 0x17, 0xda, 0x62, 0x96, 0x74, 0xad, 0x9f,
		0x7f, 0xc1, 0xca, 0x26, 0x47, 0x1a, 0xa0, 0xc6, 0x38, 0xcd, 0x3b, 0x25, 0x6e, 0x9d, 0xac, 0x0a,
		0x34, 0x02, 0xae, 0x54, 0xdb, 0x9a, 0x66, 0x84, 0xb7, 0x92, 0x8d, 0x45, 0xd4, 0xde, 0xcc, 0x35,
		0xb8, 0xdc, 0x22, 0x90, 0xdf, 0x78, 0x77, 0x6e, 0x73, 0x26, 0xe1, 0xba, 0x8f, 0x3e, 0xb1, 0x7e,
		0xe3, 0x6c, 0x35, 0x5b, 0x0f, 0xf8, 0xfb, 0x97, 0x67, 0x65, 0xcd, 0x20, 0x8c, 0xd4, 0xe8, 0x3e,
		0x4c, 0xb5, 0xf2, 0xef, 0x87, 0x4e, 0xab, 0xa0, 0xa8, 0x1f, 0xa6, 0x58, 0xd6, 0xea, 0x98, 0xa8,
		0x3f, 0x3c, 0xba, 0xa4, 0x8c, 0xea, 0xf2, 0x01, 0x89, 0x25, 0x34, 0x9b, 0x18, 0x32, 0xfd, 0x64,
		0x0b, 0xa4, 0x1e, 0x49, 0x7b, 0x3f, 0x4d, 0xff, 0x8d, 0x92, 0xa4, 0xf3, 0x24, 0x4b, 0x4c, 0xd8,
		0xee, 0xb8, 0x14, 0x7c, 0x32, 0x40, 0xb6, 0x1d, 0x94, 0x6e, 0xd2, 0x3c, 0xaa, 0x1d, 0xa4, 0x2d,
		0xd9, 0x5a, 0xe3, 0x78, 0x5e, 0x7f, 0x7a, 0xd2, 0x33, 0x1d, 0x90, 0x08, 0x7f, 0xfa, 0xea, 0x0b,
		0x4f, 0x62, 0x0f, 0xd5, 0xc0, 0x83, 0xff, 0x7c, 0xdd, 0x47, 0x91, 0x61, 0x20, 0x61, 0x67, 0x71,
		0xa1, 0x40, 0x41, 0x0b, 0xff, 0xae, 0x09, 0xc6, 0x74, 0xde, 0xca, 0xd4, 0x58, 0xd8, 0x51, 0xac,
		0xbd, 0xf3, 0x47, 0xba, 0x95, 0x58, 0x59, 0x86, 0x88, 0xae, 0x63, 0x71, 0x9e, 0x34, 0x07, 0x53,
		0x30, 0xe0, 0x5a, 0x5c, 0xd2, 0xd8, 0x1d, 0x2d, 0x03, 0x94, 0xbc, 0x45, 0xe1, 0xd7, 0xd0, 0x70,
		0x97, 0x5b, 0x72, 0xad, 0xc3, 0x23, 0x01, 0x16, 0xf1, 0xbb, 0xe3, 0x25, 0xd5, 0x14, 0x61, 0xf8,
		0x86, 0xdd, 0x1c, 0x3e, 0xdf, 0x2e, 0x39, 0x45, 0x58, 0xc7, 0xee, 0xd3, 0x31, 0x2f, 0x51, 0xfe,
		0x89, 0x05, 0xc2, 0x4b, 0xb5, 0x26, 0x9b, 0x51, 0x72, 0xa4, 0x41, 0x5a, 0xe4, 0x57, 0x4d, 0x0f,
		0xbb, 0xec, 0xaa, 0x39, 0x93, 0xf5, 0xcf, 0x9a, 0x78, 0xfd, 0x70, 0x0f, 0x6a, 0xc2, 0xca, 0x35,
		0x20, 0x02, 0xee, 0x77, 0x0e, 0xbc, 0x3a, 0x0f, 0xec, 0x1d, 0x82, 0x2f, 0xeb, 0x7e, 0x88, 0x2a,
		0xb7, 0x90, 0x7c, 0x81, 0x2e, 0x50, 0x2f, 0x87, 0x33, 0x45, 0x06, 0x46, 0x72, 0x11, 0x86, 0x3a,
		0x9a, 0xb5, 0x72, 0x49, 0xa2, 0x23, 0x47, 0x4a, 0xe1, 0xc9, 0x0f, 0x63, 0x7a, 0xa4, 0x40, 0x11,
		0xa2, 0x12, 0x9a, 0x8d, 0xb8, 0x13, 0xa4, 0x28, 0x4a, 0x38, 0x56, 0x96, 0xbc, 0xde, 0xd6, 0xba,
		0x6f, 0x30, 0x9e, 0x6a, 0x5d, 0x1f, 0x9a, 0x14, 0x3d, 0x8d, 0xdb, 0xf4, 0x31, 0xf0, 0xb2, 0x3a,
		0x66, 0x24, 0x5e, 0x22, 0x79, 0x2d, 0xa3, 0x0a, 0x8c, 0xda, 0x48, 0x0e, 0x78, 0xb9, 0x54, 0xd6,
		0x1f, 0x82, 0x14, 0x54, 0x78, 0xcf, 0x99, 0x34, 0xd1, 0x73, 0x0c, 0x45, 0x84, 0xa3, 0x05, 0x89,
		0x6d, 0x53, 0x1c, 0x93, 0xbc, 0x1a, 0x8f, 0x9a, 0xad, 0x43, 0x7c, 0x1d, 0x61, 0x4a, 0xe7, 0x11,
		0x27, 0x43, 0xeb, 0x74, 0x50, 0x57, 0x92, 0xcb, 0x40, 0x90, 0x21, 0x48, 0x21, 0x63, 0xa4, 0xe2,
		0xb6, 0xca, 0x13, 0xd4, 0xed, 0x47, 0x31, 0xdf, 0xf1, 0x1c, 0x36, 0xd0, 0x6f, 0x0d, 0x64, 0xe1,
};
static_assert(sizeof(randomData) % blockSize == 0);
static_assert(sizeof(randomData) / blockSize == 182);

}	// namespace

/*---------------------------------------------------------------------------------------------------------------------+
| global test cases
+---------------------------------------------------------------------------------------------------------------------*/

TEST_CASE("Testing getBlockSize()", "[getBlockSize]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, sizeof(readBuffer),
			writeBuffer, sizeof(writeBuffer)};

	constexpr size_t anotherBlockSize {0xa61d9f85};
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(anotherBlockSize);
	REQUIRE(bufferingBlockDevice.getBlockSize() == anotherBlockSize);
}

TEST_CASE("Testing getSize()", "[getSize]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, sizeof(readBuffer),
			writeBuffer, sizeof(writeBuffer)};

	constexpr size_t size {0x5729a11b};
	REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(size);
	REQUIRE(bufferingBlockDevice.getSize() == size);
}

TEST_CASE("Testing open() & close()", "[open/close]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, sizeof(readBuffer),
			writeBuffer, sizeof(writeBuffer)};

	SECTION("Block device open error should propagate error code to caller")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		constexpr int ret {0x1cd58756};
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.open() == ret);
	}
	SECTION("Opening closed device should succeed")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.open() == 0);

		SECTION("Opening device 255 times should succeed")
		{
			size_t openCount {1};
			while (openCount < UINT8_MAX)
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.open() == 0);
				++openCount;
			}
			while (openCount > 1)
			{
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.close() == 0);
				--openCount;
			}
		}

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.close() == 0);
	}
	SECTION("Last close of the device should flush any pending write")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.open() == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.open() == 0);

		constexpr uint64_t address {0x2a35acdb * blockSize};
		uint8_t buffer[3 * blockSize];

		memcpy(buffer, randomData, sizeof(buffer));

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address, buffer, sizeof(buffer)) == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.close() == 0);

		SECTION("Block device write error should propagate error code to caller and close the device anyway")
		{
			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
			constexpr int ret {0x2e482652};
			REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, sizeof(buffer)))
					.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(ret);
			REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0x6f17acb0);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
			REQUIRE(bufferingBlockDevice.close() == ret);
		}
		SECTION("Block device close error should propagate error code to caller and close the device anyway")
		{
			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, sizeof(buffer)))
					.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
			constexpr int ret {0x4ea52e69};
			REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(ret);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
			REQUIRE(bufferingBlockDevice.close() == ret);
		}
		SECTION("Testing successful close")
		{
			REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
			REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, sizeof(buffer)))
					.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
			REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
			REQUIRE(bufferingBlockDevice.close() == 0);
		}
	}
}

TEST_CASE("Testing read()", "[read]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr auto readBufferSize = sizeof(readBuffer);
	constexpr auto writeBufferSize = sizeof(writeBuffer);

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, readBufferSize,
			writeBuffer, writeBufferSize};

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.open() == 0);

	SECTION("Reading zero bytes should succeed")
	{
		constexpr uint64_t address {0x68f5a3b9 * blockSize};
		uint8_t buffer[1] {};

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.read(address, buffer, {}) == 0);
	}
	SECTION("Block device read error should propagate error code to caller")
	{
		constexpr uint64_t address {0x3c26717b * blockSize};
		uint8_t buffer[blockSize] {};

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		constexpr int ret {0x4ecca790};
		REQUIRE_CALL(blockDeviceMock, read(address, readBuffer, readBufferSize)).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.read(address, buffer, sizeof(buffer)) == ret);
	}
	SECTION("Testing unaligned reads")
	{
		struct ReadCall
		{
			size_t offset;
			size_t size;
		};
		struct Step
		{
			size_t offset;
			size_t size;
			std::array<const ReadCall, 7> readCalls;
		};
		struct Scenario
		{
			const char* description;
			std::array<const Step, 16> steps;
			bool finalCompare;
		};

		static const Scenario scenarios[]
		{
				{
						"forward adjacent",
						{{
								{0 * blockSize, 3 * blockSize, {0 * blockSize, readBufferSize}},
								{3 * blockSize, 3 * blockSize, {}},
								{6 * blockSize, 3 * blockSize, {8 * blockSize, readBufferSize}},
								{9 * blockSize, 3 * blockSize, {}},
								{12 * blockSize, 3 * blockSize, {}},
								{15 * blockSize, 3 * blockSize, {16 * blockSize, readBufferSize}},
								{18 * blockSize, 3 * blockSize, {}},
								{21 * blockSize, 3 * blockSize, {}},
								{24 * blockSize, 19 * blockSize, {{{24 * blockSize, readBufferSize},
										{32 * blockSize, readBufferSize}, {40 * blockSize, readBufferSize}}}},
								{43 * blockSize, 19 * blockSize, {{{48 * blockSize, readBufferSize},
										{56 * blockSize, readBufferSize}}}},
								{62 * blockSize, 19 * blockSize, {{{64 * blockSize, readBufferSize},
										{72 * blockSize, readBufferSize}, {80 * blockSize, readBufferSize}}}},
								{81 * blockSize, 19 * blockSize, {{{88 * blockSize, readBufferSize},
										{96 * blockSize, readBufferSize}}}},
								{100 * blockSize, 19 * blockSize, {{{104 * blockSize, readBufferSize},
										{112 * blockSize, readBufferSize}}}},
								{119 * blockSize, 19 * blockSize, {{{120 * blockSize, readBufferSize},
										{128 * blockSize, readBufferSize}, {136 * blockSize, readBufferSize}}}},
								{138 * blockSize, 19 * blockSize, {{{144 * blockSize, readBufferSize},
										{152 * blockSize, readBufferSize}}}},
								{157 * blockSize, 25 * blockSize, {{{160 * blockSize, readBufferSize},
										{168 * blockSize, readBufferSize}, {174 * blockSize, readBufferSize}}}},
						}},
						true
				},
				{
						"forward overlapping",
						{{
								{0 * blockSize, 5 * blockSize, {0 * blockSize, readBufferSize}},
								{3 * blockSize, 5 * blockSize, {}},
								{6 * blockSize, 5 * blockSize, {8 * blockSize, readBufferSize}},
								{9 * blockSize, 5 * blockSize, {}},
								{12 * blockSize, 5 * blockSize, {16 * blockSize, readBufferSize}},
								{15 * blockSize, 5 * blockSize, {8 * blockSize, readBufferSize}},
								{18 * blockSize, 5 * blockSize, {18 * blockSize, readBufferSize}},
								{21 * blockSize, 5 * blockSize, {}},
								{24 * blockSize, 22 * blockSize, {{{26 * blockSize, readBufferSize},
										{34 * blockSize, readBufferSize}, {42 * blockSize, readBufferSize}}}},
								{43 * blockSize, 22 * blockSize, {{{50 * blockSize, readBufferSize},
										{58 * blockSize, readBufferSize}}}},
								{62 * blockSize, 22 * blockSize, {{{66 * blockSize, readBufferSize},
										{74 * blockSize, readBufferSize}, {82 * blockSize, readBufferSize}}}},
								{81 * blockSize, 22 * blockSize, {{{74 * blockSize, readBufferSize},
										{90 * blockSize, readBufferSize}, {98 * blockSize, readBufferSize}}}},
								{100 * blockSize, 22 * blockSize, {{{106 * blockSize, readBufferSize},
										{114 * blockSize, readBufferSize}}}},
								{119 * blockSize, 22 * blockSize, {{{122 * blockSize, readBufferSize},
										{130 * blockSize, readBufferSize}, {138 * blockSize, readBufferSize}}}},
								{138 * blockSize, 22 * blockSize, {{{146 * blockSize, readBufferSize},
										{154 * blockSize, readBufferSize}}}},
								{157 * blockSize, 25 * blockSize, {{{162 * blockSize, readBufferSize},
										{170 * blockSize, readBufferSize}, {174 * blockSize, readBufferSize}}}},
						}},
						true
				},
				{
						"forward non-adjacent",
						{{
								{0 * blockSize, 3 * blockSize, {0 * blockSize, readBufferSize}},
								{5 * blockSize, 3 * blockSize, {}},
								{10 * blockSize, 3 * blockSize, {10 * blockSize, readBufferSize}},
								{15 * blockSize, 3 * blockSize, {}},
								{20 * blockSize, 3 * blockSize, {20 * blockSize, readBufferSize}},
								{25 * blockSize, 3 * blockSize, {}},
								{30 * blockSize, 3 * blockSize, {30 * blockSize, readBufferSize}},
								{35 * blockSize, 3 * blockSize, {}},
								{40 * blockSize, 15 * blockSize, {{{40 * blockSize, readBufferSize},
										{48 * blockSize, readBufferSize}}}},
								{57 * blockSize, 15 * blockSize, {{{57 * blockSize, readBufferSize},
										{65 * blockSize, readBufferSize}}}},
								{74 * blockSize, 15 * blockSize, {{{74 * blockSize, readBufferSize},
										{82 * blockSize, readBufferSize}}}},
								{91 * blockSize, 15 * blockSize, {{{91 * blockSize, readBufferSize},
										{99 * blockSize, readBufferSize}}}},
								{108 * blockSize, 15 * blockSize, {{{108 * blockSize, readBufferSize},
										{116 * blockSize, readBufferSize}}}},
								{125 * blockSize, 15 * blockSize, {{{125 * blockSize, readBufferSize},
										{133 * blockSize, readBufferSize}}}},
								{142 * blockSize, 15 * blockSize, {{{142 * blockSize, readBufferSize},
										{150 * blockSize, readBufferSize}}}},
								{159 * blockSize, 23 * blockSize, {{{159 * blockSize, readBufferSize},
										{167 * blockSize, readBufferSize}, {174 * blockSize, readBufferSize}}}},
						}},
						false
				},
				{
						"forward overlapping expanding",
						{{
								{59 * blockSize, 2 * blockSize, {59 * blockSize, readBufferSize}},
								{59 * blockSize, 3 * blockSize, {}},
								{59 * blockSize, 5 * blockSize, {}},
								{59 * blockSize, 7 * blockSize, {}},
								{59 * blockSize, 11 * blockSize, {67 * blockSize, readBufferSize}},
								{59 * blockSize, 13 * blockSize, {59 * blockSize, readBufferSize}},
								{59 * blockSize, 17 * blockSize, {{{67 * blockSize, readBufferSize},
										{75 * blockSize, readBufferSize}}}},
								{59 * blockSize, 19 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}}}},
								{59 * blockSize, 23 * blockSize, {{{59 * blockSize, readBufferSize},
										{75 * blockSize, readBufferSize}}}},
								{59 * blockSize, 29 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}, {83 * blockSize, readBufferSize}}}},
								{59 * blockSize, 31 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}, {75 * blockSize, readBufferSize}}}},
								{59 * blockSize, 37 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}, {83 * blockSize, readBufferSize},
										{91 * blockSize, readBufferSize}}}},
								{59 * blockSize, 41 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}, {75 * blockSize, readBufferSize},
										{83 * blockSize, readBufferSize}, {99 * blockSize, readBufferSize}}}},
								{59 * blockSize, 43 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}, {75 * blockSize, readBufferSize},
										{83 * blockSize, readBufferSize}, {91 * blockSize, readBufferSize}}}},
								{59 * blockSize, 47 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}, {75 * blockSize, readBufferSize},
										{83 * blockSize, readBufferSize}, {99 * blockSize, readBufferSize}}}},
								{59 * blockSize, 53 * blockSize, {{{59 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}, {75 * blockSize, readBufferSize},
										{83 * blockSize, readBufferSize}, {91 * blockSize, readBufferSize},
										{107 * blockSize, readBufferSize}}}},
						}},
						false
				},
				{
						"reverse adjacent",
						{{
								{157 * blockSize, 25 * blockSize, {{{157 * blockSize, readBufferSize},
										{165 * blockSize, readBufferSize}, {173 * blockSize, readBufferSize},
										{174 * blockSize, readBufferSize}}}},
								{138 * blockSize, 19 * blockSize, {{{138 * blockSize, readBufferSize},
										{146 * blockSize, readBufferSize}, {154 * blockSize, readBufferSize}}}},
								{119 * blockSize, 19 * blockSize, {{{119 * blockSize, readBufferSize},
										{127 * blockSize, readBufferSize}, {135 * blockSize, readBufferSize}}}},
								{100 * blockSize, 19 * blockSize, {{{100 * blockSize, readBufferSize},
										{108 * blockSize, readBufferSize}, {116 * blockSize, readBufferSize}}}},
								{81 * blockSize, 19 * blockSize, {{{81 * blockSize, readBufferSize},
										{89 * blockSize, readBufferSize}, {97 * blockSize, readBufferSize}}}},
								{62 * blockSize, 19 * blockSize, {{{62 * blockSize, readBufferSize},
										{70 * blockSize, readBufferSize}, {78 * blockSize, readBufferSize}}}},
								{43 * blockSize, 19 * blockSize, {{{43 * blockSize, readBufferSize},
										{51 * blockSize, readBufferSize}, {59 * blockSize, readBufferSize}}}},
								{24 * blockSize, 19 * blockSize, {{{24 * blockSize, readBufferSize},
										{32 * blockSize, readBufferSize}, {40 * blockSize, readBufferSize}}}},
								{21 * blockSize, 3 * blockSize, {21 * blockSize, readBufferSize}},
								{18 * blockSize, 3 * blockSize, {13 * blockSize, readBufferSize}},
								{15 * blockSize, 3 * blockSize, {}},
								{12 * blockSize, 3 * blockSize, {5 * blockSize, readBufferSize}},
								{9 * blockSize, 3 * blockSize, {}},
								{6 * blockSize, 3 * blockSize, {}},
								{3 * blockSize, 3 * blockSize, {0 * blockSize, readBufferSize}},
								{0 * blockSize, 3 * blockSize, {}},
						}},
						true
				},
				{
						"reverse overlapping",
						{{
								{157 * blockSize, 25 * blockSize, {{{157 * blockSize, readBufferSize},
										{165 * blockSize, readBufferSize}, {173 * blockSize, readBufferSize},
										{174 * blockSize, readBufferSize}}}},
								{138 * blockSize, 22 * blockSize, {{{138 * blockSize, readBufferSize},
										{146 * blockSize, readBufferSize}, {154 * blockSize, readBufferSize}}}},
								{119 * blockSize, 22 * blockSize, {{{119 * blockSize, readBufferSize},
										{127 * blockSize, readBufferSize}, {135 * blockSize, readBufferSize}}}},
								{100 * blockSize, 22 * blockSize, {{{100 * blockSize, readBufferSize},
										{108 * blockSize, readBufferSize}, {116 * blockSize, readBufferSize}}}},
								{81 * blockSize, 22 * blockSize, {{{81 * blockSize, readBufferSize},
										{89 * blockSize, readBufferSize}, {97 * blockSize, readBufferSize}}}},
								{62 * blockSize, 22 * blockSize, {{{62 * blockSize, readBufferSize},
										{70 * blockSize, readBufferSize}, {78 * blockSize, readBufferSize}}}},
								{43 * blockSize, 22 * blockSize, {{{43 * blockSize, readBufferSize},
										{51 * blockSize, readBufferSize}, {59 * blockSize, readBufferSize}}}},
								{24 * blockSize, 22 * blockSize, {{{24 * blockSize, readBufferSize},
										{32 * blockSize, readBufferSize}, {40 * blockSize, readBufferSize}}}},
								{21 * blockSize, 5 * blockSize, {21 * blockSize, readBufferSize}},
								{18 * blockSize, 5 * blockSize, {13 * blockSize, readBufferSize}},
								{15 * blockSize, 5 * blockSize, {}},
								{12 * blockSize, 5 * blockSize, {5 * blockSize, readBufferSize}},
								{9 * blockSize, 5 * blockSize, {13 * blockSize, readBufferSize}},
								{6 * blockSize, 5 * blockSize, {5 * blockSize, readBufferSize}},
								{3 * blockSize, 5 * blockSize, {0 * blockSize, readBufferSize}},
								{0 * blockSize, 5 * blockSize, {}},
						}},
						true
				},
				{
						"reverse non-adjacent",
						{{
								{159 * blockSize, 23 * blockSize, {{{159 * blockSize, readBufferSize},
										{167 * blockSize, readBufferSize}, {174 * blockSize, readBufferSize}}}},
								{142 * blockSize, 15 * blockSize, {{{142 * blockSize, readBufferSize},
										{150 * blockSize, readBufferSize}}}},
								{125 * blockSize, 15 * blockSize, {{{125 * blockSize, readBufferSize},
										{133 * blockSize, readBufferSize}}}},
								{108 * blockSize, 15 * blockSize, {{{108 * blockSize, readBufferSize},
										{116 * blockSize, readBufferSize}}}},
								{91 * blockSize, 15 * blockSize, {{{91 * blockSize, readBufferSize},
										{99 * blockSize, readBufferSize}}}},
								{74 * blockSize, 15 * blockSize, {{{74 * blockSize, readBufferSize},
										{82 * blockSize, readBufferSize}}}},
								{57 * blockSize, 15 * blockSize, {{{57 * blockSize, readBufferSize},
										{65 * blockSize, readBufferSize}}}},
								{40 * blockSize, 15 * blockSize, {{{40 * blockSize, readBufferSize},
										{48 * blockSize, readBufferSize}}}},
								{35 * blockSize, 3 * blockSize, {35 * blockSize, readBufferSize}},
								{30 * blockSize, 3 * blockSize, {27 * blockSize, readBufferSize}},
								{25 * blockSize, 3 * blockSize, {19 * blockSize, readBufferSize}},
								{20 * blockSize, 3 * blockSize, {}},
								{15 * blockSize, 3 * blockSize, {11 * blockSize, readBufferSize}},
								{10 * blockSize, 3 * blockSize, {3 * blockSize, readBufferSize}},
								{5 * blockSize, 3 * blockSize, {}},
								{0 * blockSize, 3 * blockSize, {0 * blockSize, readBufferSize}},
						}},
						false
				},
				{
						"reverse overlapping expanding",
						{{
								{57 * blockSize, 2 * blockSize, {57 * blockSize, readBufferSize}},
								{56 * blockSize, 3 * blockSize, {49 * blockSize, readBufferSize}},
								{54 * blockSize, 5 * blockSize, {57 * blockSize, readBufferSize}},
								{52 * blockSize, 7 * blockSize, {49 * blockSize, readBufferSize}},
								{48 * blockSize, 11 * blockSize, {{{41 * blockSize, readBufferSize},
										{57 * blockSize, readBufferSize}}}},
								{46 * blockSize, 13 * blockSize, {{{46 * blockSize, readBufferSize},
										{54 * blockSize, readBufferSize}}}},
								{42 * blockSize, 17 * blockSize, {{{42 * blockSize, readBufferSize},
										{50 * blockSize, readBufferSize}}}},
								{40 * blockSize, 19 * blockSize, {{{40 * blockSize, readBufferSize},
										{48 * blockSize, readBufferSize}, {58 * blockSize, readBufferSize}}}},
								{36 * blockSize, 23 * blockSize, {{{36 * blockSize, readBufferSize},
										{44 * blockSize, readBufferSize}, {52 * blockSize, readBufferSize}}}},
								{30 * blockSize, 29 * blockSize, {{{30 * blockSize, readBufferSize},
										{38 * blockSize, readBufferSize}, {46 * blockSize, readBufferSize}}}},
								{28 * blockSize, 31 * blockSize, {{{28 * blockSize, readBufferSize},
										{36 * blockSize, readBufferSize}, {44 * blockSize, readBufferSize},
										{54 * blockSize, readBufferSize}}}},
								{22 * blockSize, 37 * blockSize, {{{22 * blockSize, readBufferSize},
										{30 * blockSize, readBufferSize}, {38 * blockSize, readBufferSize},
										{46 * blockSize, readBufferSize}}}},
								{18 * blockSize, 41 * blockSize, {{{18 * blockSize, readBufferSize},
										{26 * blockSize, readBufferSize}, {34 * blockSize, readBufferSize},
										{42 * blockSize, readBufferSize}, {54 * blockSize, readBufferSize}}}},
								{16 * blockSize, 43 * blockSize, {{{16 * blockSize, readBufferSize},
										{24 * blockSize, readBufferSize}, {32 * blockSize, readBufferSize},
										{40 * blockSize, readBufferSize}, {48 * blockSize, readBufferSize}}}},
								{12 * blockSize, 47 * blockSize, {{{12 * blockSize, readBufferSize},
										{20 * blockSize, readBufferSize}, {28 * blockSize, readBufferSize},
										{36 * blockSize, readBufferSize}, {44 * blockSize, readBufferSize},
										{56 * blockSize, readBufferSize}}}},
								{6 * blockSize, 53 * blockSize, {{{6 * blockSize, readBufferSize},
										{14 * blockSize, readBufferSize}, {22 * blockSize, readBufferSize},
										{30 * blockSize, readBufferSize}, {38 * blockSize, readBufferSize},
										{46 * blockSize, readBufferSize}, {54 * blockSize, readBufferSize}}}},
						}},
						false
				},
				{
						"bidirectional overlapping expanding",
						{{
								{59 * blockSize, 2 * blockSize, {59 * blockSize, readBufferSize}},
								{59 * blockSize, 3 * blockSize, {}},
								{58 * blockSize, 5 * blockSize, {51 * blockSize, readBufferSize}},
								{57 * blockSize, 7 * blockSize, {59 * blockSize, readBufferSize}},
								{55 * blockSize, 11 * blockSize, {51 * blockSize, readBufferSize}},
								{54 * blockSize, 13 * blockSize, {59 * blockSize, readBufferSize}},
								{52 * blockSize, 17 * blockSize, {{{51 * blockSize, readBufferSize},
										{67 * blockSize, readBufferSize}}}},
								{51 * blockSize, 19 * blockSize, {{{51 * blockSize, readBufferSize},
										{59 * blockSize, readBufferSize}}}},
								{49 * blockSize, 23 * blockSize, {{{49 * blockSize, readBufferSize},
										{57 * blockSize, readBufferSize}, {67 * blockSize, readBufferSize}}}},
								{46 * blockSize, 29 * blockSize, {{{46 * blockSize, readBufferSize},
										{54 * blockSize, readBufferSize}, {62 * blockSize, readBufferSize}}}},
								{45 * blockSize, 31 * blockSize, {{{45 * blockSize, readBufferSize},
										{53 * blockSize, readBufferSize}, {61 * blockSize, readBufferSize},
										{70 * blockSize, readBufferSize}}}},
								{42 * blockSize, 37 * blockSize, {{{42 * blockSize, readBufferSize},
										{50 * blockSize, readBufferSize}, {58 * blockSize, readBufferSize},
										{66 * blockSize, readBufferSize}, {78 * blockSize, readBufferSize}}}},
								{40 * blockSize, 41 * blockSize, {{{40 * blockSize, readBufferSize},
										{48 * blockSize, readBufferSize}, {56 * blockSize, readBufferSize},
										{64 * blockSize, readBufferSize}, {72 * blockSize, readBufferSize}}}},
								{39 * blockSize, 43 * blockSize, {{{39 * blockSize, readBufferSize},
										{47 * blockSize, readBufferSize}, {55 * blockSize, readBufferSize},
										{63 * blockSize, readBufferSize}, {71 * blockSize, readBufferSize},
										{80 * blockSize, readBufferSize}}}},
								{37 * blockSize, 47 * blockSize, {{{37 * blockSize, readBufferSize},
										{45 * blockSize, readBufferSize}, {53 * blockSize, readBufferSize},
										{61 * blockSize, readBufferSize}, {69 * blockSize, readBufferSize},
										{77 * blockSize, readBufferSize}}}},
								{34 * blockSize, 53 * blockSize, {{{34 * blockSize, readBufferSize},
										{42 * blockSize, readBufferSize}, {50 * blockSize, readBufferSize},
										{58 * blockSize, readBufferSize}, {66 * blockSize, readBufferSize},
										{74 * blockSize, readBufferSize}, {85 * blockSize, readBufferSize}}}},
						}},
						false
				},
				{
						"overlapping shrinking",
						{{
								{83 * blockSize, 8 * blockSize, {83 * blockSize, readBufferSize}},
								{83 * blockSize, 7 * blockSize, {}},
								{85 * blockSize, 5 * blockSize, {}},
								{85 * blockSize, 2 * blockSize, {}},
								{86 * blockSize, 1 * blockSize, {}},
						}},
						false
				},
		};

		for (auto& scenario : scenarios)
		{
			DYNAMIC_SECTION("Testing unaligned " << scenario.description << " reads")
			{
				auto& steps = scenario.steps;

				constexpr uint64_t address {};
				uint8_t buffer[sizeof(randomData)] {};

				for (auto& step : steps)
				{
					if (step.size == 0)
						continue;

					REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
					REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
					REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(sizeof(randomData));
					for (auto& readCall : step.readCalls)
						if (readCall.size != 0)
							expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock,
									read(address + readCall.offset, readBuffer, readCall.size)).IN_SEQUENCE(sequence)
									.SIDE_EFFECT(memcpy(_2, randomData + _1 - address, _3)).RETURN(0));
					REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
					REQUIRE(bufferingBlockDevice.read(address + step.offset, buffer + step.offset, step.size) == 0);
					REQUIRE(memcmp(buffer + step.offset, randomData + step.offset, step.size) == 0);
				}

				if (scenario.finalCompare == true)
					REQUIRE(memcmp(buffer, randomData, sizeof(randomData)) == 0);
			}
		}
	}

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.close() == 0);
}

TEST_CASE("Testing write()", "[write]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr auto readBufferSize = sizeof(readBuffer);
	constexpr auto writeBufferSize = sizeof(writeBuffer);

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, readBufferSize,
			writeBuffer, writeBufferSize};

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.open() == 0);

	SECTION("Block device write error should propagate error code to caller")
	{
		constexpr uint64_t address {0x43b275ed * blockSize};
		uint8_t buffer[13 * blockSize];

		memcpy(buffer, randomData, sizeof(buffer));

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		constexpr int ret {0x27eb8f56};
		REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, writeBufferSize))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address, buffer, sizeof(buffer)) == ret);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, writeBufferSize))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.synchronize() == 0);
	}
	SECTION("Writing zero bytes should succeed")
	{
		constexpr uint64_t address {0xe8078871 * blockSize};
		uint8_t buffer[1] {};

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address, buffer, {}) == 0);
	}
	SECTION("Testing unaligned writes")
	{
		struct WriteCall
		{
			size_t offset;
			size_t size;
		};
		struct Step
		{
			size_t offset;
			size_t size;
			size_t randomDataOffset;
			size_t randomDataSize;
			std::array<const WriteCall, 6> writeCalls;
		};
		struct SynchronizeStep
		{
			size_t writeOffset;
			size_t writeSize;
		};
		struct Scenario
		{
			const char* description;
			std::array<const Step, 16> steps;
			SynchronizeStep synchronizeStep;
		};

		static const Scenario scenarios[]
		{
				{
						"forward adjacent",
						{{
								{0 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{3 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{6 * blockSize, 3 * blockSize, {}, 3 * blockSize, {0 * blockSize, writeBufferSize}},
								{9 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{12 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{15 * blockSize, 3 * blockSize, {}, 3 * blockSize, {8 * blockSize, writeBufferSize}},
								{18 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{21 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{24 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{16 * blockSize, writeBufferSize}, {24 * blockSize, writeBufferSize},
												{32 * blockSize, writeBufferSize}}}},
								{43 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{40 * blockSize, writeBufferSize}, {48 * blockSize, writeBufferSize}}}},
								{62 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{56 * blockSize, writeBufferSize}, {64 * blockSize, writeBufferSize},
												{72 * blockSize, writeBufferSize}}}},
								{81 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{80 * blockSize, writeBufferSize}, {88 * blockSize, writeBufferSize}}}},
								{100 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{96 * blockSize, writeBufferSize}, {104 * blockSize, writeBufferSize}}}},
								{119 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{112 * blockSize, writeBufferSize}, {120 * blockSize, writeBufferSize},
												{128 * blockSize, writeBufferSize}}}},
								{138 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{136 * blockSize, writeBufferSize}, {144 * blockSize, writeBufferSize}}}},
								{157 * blockSize, 25 * blockSize, {}, 25 * blockSize,
										{{{152 * blockSize, writeBufferSize}, {160 * blockSize, writeBufferSize},
												{168 * blockSize, writeBufferSize}}}},
						}},
						{176 * blockSize, 6 * blockSize}
				},
				{
						"forward overlapping",
						{{
								{0 * blockSize, 5 * blockSize, {}, 3 * blockSize, {}},
								{3 * blockSize, 5 * blockSize, {}, 3 * blockSize, {}},
								{6 * blockSize, 5 * blockSize, {}, 3 * blockSize, {0 * blockSize, writeBufferSize}},
								{9 * blockSize, 5 * blockSize, {}, 3 * blockSize, {}},
								{12 * blockSize, 5 * blockSize, {}, 4 * blockSize, {8 * blockSize, writeBufferSize}},
								{15 * blockSize, 5 * blockSize, {}, 3 * blockSize, {}},
								{18 * blockSize, 5 * blockSize, {}, 3 * blockSize, {}},
								{21 * blockSize, 5 * blockSize, {}, 3 * blockSize, {15 * blockSize, writeBufferSize}},
								{24 * blockSize, 22 * blockSize, {}, 19 * blockSize,
										{{{23 * blockSize, writeBufferSize}, {31 * blockSize, writeBufferSize}}}},
								{43 * blockSize, 22 * blockSize, {}, 20 * blockSize,
										{{{39 * blockSize, writeBufferSize}, {47 * blockSize, writeBufferSize},
												{55 * blockSize, writeBufferSize}}}},
								{62 * blockSize, 22 * blockSize, {}, 19 * blockSize,
										{{{62 * blockSize, writeBufferSize}, {70 * blockSize, writeBufferSize}}}},
								{81 * blockSize, 22 * blockSize, {}, 21 * blockSize,
										{{{78 * blockSize, writeBufferSize}, {86 * blockSize, writeBufferSize},
												{94 * blockSize, writeBufferSize}}}},
								{100 * blockSize, 22 * blockSize, {}, 19 * blockSize,
										{{{100 * blockSize, writeBufferSize}, {108 * blockSize, writeBufferSize}}}},
								{119 * blockSize, 22 * blockSize, {}, 21 * blockSize,
										{{{116 * blockSize, writeBufferSize}, {124 * blockSize, writeBufferSize},
												{132 * blockSize, writeBufferSize}}}},
								{138 * blockSize, 22 * blockSize, {}, 19 * blockSize,
										{{{138 * blockSize, writeBufferSize}, {146 * blockSize, writeBufferSize}}}},
								{157 * blockSize, 25 * blockSize, {}, 25 * blockSize,
										{{{154 * blockSize, writeBufferSize}, {162 * blockSize, writeBufferSize},
												{170 * blockSize, writeBufferSize}}}},
						}},
						{178 * blockSize, 4 * blockSize}
				},
				{
						"forward non-adjacent",
						{{
								{0 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{5 * blockSize, 3 * blockSize, {}, 3 * blockSize, {0 * blockSize, 3 * blockSize}},
								{10 * blockSize, 3 * blockSize, {}, 3 * blockSize, {5 * blockSize, 3 * blockSize}},
								{15 * blockSize, 3 * blockSize, {}, 3 * blockSize, {10 * blockSize, 3 * blockSize}},
								{20 * blockSize, 3 * blockSize, {}, 3 * blockSize, {15 * blockSize, 3 * blockSize}},
								{25 * blockSize, 3 * blockSize, {}, 3 * blockSize, {20 * blockSize, 3 * blockSize}},
								{30 * blockSize, 3 * blockSize, {}, 3 * blockSize, {25 * blockSize, 3 * blockSize}},
								{35 * blockSize, 3 * blockSize, {}, 3 * blockSize, {30 * blockSize, 3 * blockSize}},
								{40 * blockSize, 15 * blockSize, {}, 15 * blockSize, {{{35 * blockSize, 3 * blockSize},
										{40 * blockSize, writeBufferSize}}}},
								{57 * blockSize, 15 * blockSize, {}, 15 * blockSize, {{{48 * blockSize, 7 * blockSize},
										{57 * blockSize, writeBufferSize}}}},
								{74 * blockSize, 15 * blockSize, {}, 15 * blockSize, {{{65 * blockSize, 7 * blockSize},
										{74 * blockSize, writeBufferSize}}}},
								{91 * blockSize, 15 * blockSize, {}, 15 * blockSize, {{{82 * blockSize, 7 * blockSize},
										{91 * blockSize, writeBufferSize}}}},
								{108 * blockSize, 15 * blockSize, {}, 15 * blockSize, {{{99 * blockSize, 7 * blockSize},
										{108 * blockSize, writeBufferSize}}}},
								{125 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{116 * blockSize, 7 * blockSize}, {125 * blockSize, writeBufferSize}}}},
								{142 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{133 * blockSize, 7 * blockSize}, {142 * blockSize, writeBufferSize}}}},
								{159 * blockSize, 23 * blockSize, {}, 23 * blockSize,
										{{{150 * blockSize, 7 * blockSize}, {159 * blockSize, writeBufferSize},
												{167 * blockSize, writeBufferSize}}}},
						}},
						{175 * blockSize, 7 * blockSize}
				},
				{
						"forward overlapping expanding",
						{{
								{59 * blockSize, 2 * blockSize, {}, {}, {}},
								{59 * blockSize, 3 * blockSize, {}, {}, {}},
								{59 * blockSize, 5 * blockSize, {}, {}, {}},
								{59 * blockSize, 7 * blockSize, {}, {}, {}},
								{59 * blockSize, 11 * blockSize, {}, 8 * blockSize, {59 * blockSize, writeBufferSize}},
								{59 * blockSize, 13 * blockSize, {}, 8 * blockSize, {59 * blockSize, writeBufferSize}},
								{59 * blockSize, 17 * blockSize, {}, 16 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 19 * blockSize, {}, 16 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 23 * blockSize, {}, 16 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 29 * blockSize, {}, 24 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize},
												{75 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 31 * blockSize, {}, 24 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize},
												{75 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 37 * blockSize, {}, 32 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize},
												{75 * blockSize, writeBufferSize}, {83 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 41 * blockSize, {}, 40 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize},
												{75 * blockSize, writeBufferSize}, {83 * blockSize, writeBufferSize},
												{91 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 43 * blockSize, {}, 40 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize},
												{75 * blockSize, writeBufferSize}, {83 * blockSize, writeBufferSize},
												{91 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 47 * blockSize, {}, 40 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize},
												{75 * blockSize, writeBufferSize}, {83 * blockSize, writeBufferSize},
												{91 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 53 * blockSize, {}, 53 * blockSize,
										{{{59 * blockSize, writeBufferSize}, {67 * blockSize, writeBufferSize},
												{75 * blockSize, writeBufferSize}, {83 * blockSize, writeBufferSize},
												{91 * blockSize, writeBufferSize}, {99 * blockSize, writeBufferSize}}}},
						}},
						{107 * blockSize, 5 * blockSize}
				},
				{
						"reverse adjacent",
						{{
								{179 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{176 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{173 * blockSize, 3 * blockSize, {}, 3 * blockSize, {174 * blockSize, writeBufferSize}},
								{170 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{167 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{164 * blockSize, 3 * blockSize, {}, 3 * blockSize, {166 * blockSize, writeBufferSize}},
								{161 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{158 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{139 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{158 * blockSize, writeBufferSize}, {139 * blockSize, writeBufferSize},
												{147 * blockSize, writeBufferSize}}}},
								{120 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{155 * blockSize, 3 * blockSize}, {120 * blockSize, writeBufferSize},
												{128 * blockSize, writeBufferSize}}}},
								{101 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{136 * blockSize, 3 * blockSize}, {101 * blockSize, writeBufferSize},
												{109 * blockSize, writeBufferSize}}}},
								{82 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{117 * blockSize, 3 * blockSize}, {82 * blockSize, writeBufferSize},
												{90 * blockSize, writeBufferSize}}}},
								{63 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{98 * blockSize, 3 * blockSize}, {63 * blockSize, writeBufferSize},
												{71 * blockSize, writeBufferSize}}}},
								{44 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{79 * blockSize, 3 * blockSize}, {44 * blockSize, writeBufferSize},
												{52 * blockSize, writeBufferSize}}}},
								{25 * blockSize, 19 * blockSize, {}, 19 * blockSize,
										{{{60 * blockSize, 3 * blockSize}, {25 * blockSize, writeBufferSize},
												{33 * blockSize, writeBufferSize}}}},
								{0 * blockSize, 25 * blockSize, {}, 25 * blockSize,
										{{{41 * blockSize, 3 * blockSize}, {0 * blockSize, writeBufferSize},
												{8 * blockSize, writeBufferSize}, {16 * blockSize, writeBufferSize}}}},
						}},
						{24 * blockSize, 1 * blockSize}
				},
				{
						"reverse overlapping",
						{{
								{179 * blockSize, 3 * blockSize, 2 * blockSize, 1 * blockSize, {}},
								{176 * blockSize, 5 * blockSize, 2 * blockSize, 3 * blockSize, {}},
								{173 * blockSize, 5 * blockSize, 1 * blockSize, 4 * blockSize,
										{174 * blockSize, writeBufferSize}},
								{170 * blockSize, 5 * blockSize, 2 * blockSize, 3 * blockSize, {}},
								{167 * blockSize, 5 * blockSize, 2 * blockSize, 3 * blockSize, {}},
								{164 * blockSize, 5 * blockSize, 2 * blockSize, 3 * blockSize,
										{167 * blockSize, writeBufferSize}},
								{161 * blockSize, 5 * blockSize, 2 * blockSize, 3 * blockSize, {}},
								{158 * blockSize, 5 * blockSize, 1 * blockSize, 4 * blockSize,
										{{159 * blockSize, writeBufferSize}}},
								{139 * blockSize, 22 * blockSize, {}, 22 * blockSize,
										{{{139 * blockSize, writeBufferSize}, {147 * blockSize, writeBufferSize}}}},
								{120 * blockSize, 22 * blockSize, {}, 22 * blockSize,
										{{{155 * blockSize, 6 * blockSize}, {120 * blockSize, writeBufferSize},
												{128 * blockSize, writeBufferSize}}}},
								{101 * blockSize, 22 * blockSize, {}, 22 * blockSize,
										{{{136 * blockSize, 6 * blockSize}, {101 * blockSize, writeBufferSize},
												{109 * blockSize, writeBufferSize}}}},
								{82 * blockSize, 22 * blockSize, {}, 22 * blockSize, {{{117 * blockSize, 6 * blockSize},
										{82 * blockSize, writeBufferSize}, {90 * blockSize, writeBufferSize}}}},
								{63 * blockSize, 22 * blockSize, {}, 22 * blockSize, {{{98 * blockSize, 6 * blockSize},
										{63 * blockSize, writeBufferSize}, {71 * blockSize, writeBufferSize}}}},
								{44 * blockSize, 22 * blockSize, {}, 22 * blockSize, {{{79 * blockSize, 6 * blockSize},
										{44 * blockSize, writeBufferSize}, {52 * blockSize, writeBufferSize}}}},
								{25 * blockSize, 22 * blockSize, {}, 22 * blockSize, {{{60 * blockSize, 6 * blockSize},
										{25 * blockSize, writeBufferSize}, {33 * blockSize, writeBufferSize}}}},
								{0 * blockSize, 27 * blockSize, {}, 27 * blockSize, {{{41 * blockSize, 6 * blockSize},
										{0 * blockSize, writeBufferSize}, {8 * blockSize, writeBufferSize},
										{16 * blockSize, writeBufferSize}}}},
						}},
						{24 * blockSize, 3 * blockSize}
				},
				{
						"reverse non-adjacent",
						{{
								{179 * blockSize, 3 * blockSize, {}, 3 * blockSize, {}},
								{174 * blockSize, 3 * blockSize, {}, 3 * blockSize, {179 * blockSize, 3 * blockSize}},
								{169 * blockSize, 3 * blockSize, {}, 3 * blockSize, {174 * blockSize, 3 * blockSize}},
								{164 * blockSize, 3 * blockSize, {}, 3 * blockSize, {169 * blockSize, 3 * blockSize}},
								{159 * blockSize, 3 * blockSize, {}, 3 * blockSize, {164 * blockSize, 3 * blockSize}},
								{154 * blockSize, 3 * blockSize, {}, 3 * blockSize, {159 * blockSize, 3 * blockSize}},
								{149 * blockSize, 3 * blockSize, {}, 3 * blockSize, {154 * blockSize, 3 * blockSize}},
								{144 * blockSize, 3 * blockSize, {}, 3 * blockSize, {149 * blockSize, 3 * blockSize}},
								{127 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{144 * blockSize, 3 * blockSize}, {127 * blockSize, writeBufferSize}}}},
								{110 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{135 * blockSize, 7 * blockSize}, {110 * blockSize, writeBufferSize}}}},
								{93 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{118 * blockSize, 7 * blockSize}, {93 * blockSize, writeBufferSize}}}},
								{76 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{101 * blockSize, 7 * blockSize}, {76 * blockSize, writeBufferSize}}}},
								{59 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{84 * blockSize, 7 * blockSize}, {59 * blockSize, writeBufferSize}}}},
								{42 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{67 * blockSize, 7 * blockSize}, {42 * blockSize, writeBufferSize}}}},
								{25 * blockSize, 15 * blockSize, {}, 15 * blockSize,
										{{{50 * blockSize, 7 * blockSize}, {25 * blockSize, writeBufferSize}}}},
								{0 * blockSize, 23 * blockSize, {}, 23 * blockSize,
										{{{33 * blockSize, 7 * blockSize}, {0 * blockSize, writeBufferSize},
												{8 * blockSize, writeBufferSize}}}},
						}},
						{16 * blockSize, 7 * blockSize}
				},
				{
						"reverse overlapping expanding",
						{{
								{57 * blockSize, 2 * blockSize, {}, {}, {}},
								{56 * blockSize, 3 * blockSize, {}, {}, {}},
								{54 * blockSize, 5 * blockSize, {}, {}, {}},
								{52 * blockSize, 7 * blockSize, {}, {}, {}},
								{48 * blockSize, 11 * blockSize, {}, 8 * blockSize, {48 * blockSize, writeBufferSize}},
								{46 * blockSize, 13 * blockSize, {}, 8 * blockSize, {46 * blockSize, writeBufferSize}},
								{42 * blockSize, 17 * blockSize, {}, 16 * blockSize,
										{{{42 * blockSize, writeBufferSize}, {50 * blockSize, writeBufferSize}}}},
								{40 * blockSize, 19 * blockSize, {}, 16 * blockSize,
										{{{40 * blockSize, writeBufferSize}, {48 * blockSize, writeBufferSize}}}},
								{36 * blockSize, 23 * blockSize, {}, 16 * blockSize,
										{{{36 * blockSize, writeBufferSize}, {44 * blockSize, writeBufferSize}}}},
								{30 * blockSize, 29 * blockSize, {}, 24 * blockSize,
										{{{30 * blockSize, writeBufferSize}, {38 * blockSize, writeBufferSize},
												{46 * blockSize, writeBufferSize}}}},
								{28 * blockSize, 31 * blockSize, {}, 24 * blockSize,
										{{{28 * blockSize, writeBufferSize}, {36 * blockSize, writeBufferSize},
												{44 * blockSize, writeBufferSize}}}},
								{22 * blockSize, 37 * blockSize, {}, 32 * blockSize,
										{{{22 * blockSize, writeBufferSize}, {30 * blockSize, writeBufferSize},
												{38 * blockSize, writeBufferSize}, {46 * blockSize, writeBufferSize}}}},
								{18 * blockSize, 41 * blockSize, {}, 40 * blockSize,
										{{{18 * blockSize, writeBufferSize}, {26 * blockSize, writeBufferSize},
												{34 * blockSize, writeBufferSize}, {42 * blockSize, writeBufferSize},
												{50 * blockSize, writeBufferSize}}}},
								{16 * blockSize, 43 * blockSize, {}, 40 * blockSize,
										{{{16 * blockSize, writeBufferSize}, {24 * blockSize, writeBufferSize},
												{32 * blockSize, writeBufferSize}, {40 * blockSize, writeBufferSize},
												{48 * blockSize, writeBufferSize}}}},
								{12 * blockSize, 47 * blockSize, {}, 40 * blockSize,
										{{{12 * blockSize, writeBufferSize}, {20 * blockSize, writeBufferSize},
												{28 * blockSize, writeBufferSize}, {36 * blockSize, writeBufferSize},
												{44 * blockSize, writeBufferSize}}}},
								{6 * blockSize, 53 * blockSize, {}, 53 * blockSize,
										{{{6 * blockSize, writeBufferSize}, {14 * blockSize, writeBufferSize},
												{22 * blockSize, writeBufferSize}, {30 * blockSize, writeBufferSize},
												{38 * blockSize, writeBufferSize}, {46 * blockSize, writeBufferSize}}}},
						}},
						{54 * blockSize, 5 * blockSize}
				},
				{
						"bidirectional overlapping expanding",
						{{
								{59 * blockSize, 2 * blockSize, {}, {}, {}},
								{59 * blockSize, 3 * blockSize, {}, {}, {}},
								{58 * blockSize, 5 * blockSize, {}, {}, {}},
								{57 * blockSize, 7 * blockSize, {}, {}, {}},
								{55 * blockSize, 11 * blockSize, {}, 8 * blockSize, {55 * blockSize, writeBufferSize}},
								{54 * blockSize, 13 * blockSize, {}, 8 * blockSize, {54 * blockSize, writeBufferSize}},
								{52 * blockSize, 17 * blockSize, {}, 16 * blockSize,
										{{{52 * blockSize, writeBufferSize}, {60 * blockSize, writeBufferSize}}}},
								{51 * blockSize, 19 * blockSize, {}, 16 * blockSize,
										{{{51 * blockSize, writeBufferSize}, {59 * blockSize, writeBufferSize}}}},
								{49 * blockSize, 23 * blockSize, {}, 16 * blockSize,
										{{{49 * blockSize, writeBufferSize}, {57 * blockSize, writeBufferSize}}}},
								{46 * blockSize, 29 * blockSize, {}, 24 * blockSize,
										{{{46 * blockSize, writeBufferSize}, {54 * blockSize, writeBufferSize},
												{62 * blockSize, writeBufferSize}}}},
								{45 * blockSize, 31 * blockSize, {}, 24 * blockSize,
										{{{45 * blockSize, writeBufferSize}, {53 * blockSize, writeBufferSize},
												{61 * blockSize, writeBufferSize}}}},
								{42 * blockSize, 37 * blockSize, {}, 32 * blockSize,
										{{{42 * blockSize, writeBufferSize}, {50 * blockSize, writeBufferSize},
												{58 * blockSize, writeBufferSize}, {66 * blockSize, writeBufferSize}}}},
								{40 * blockSize, 41 * blockSize, {}, 40 * blockSize,
										{{{40 * blockSize, writeBufferSize}, {48 * blockSize, writeBufferSize},
												{56 * blockSize, writeBufferSize}, {64 * blockSize, writeBufferSize},
												{72 * blockSize, writeBufferSize}}}},
								{39 * blockSize, 43 * blockSize, {}, 40 * blockSize,
										{{{39 * blockSize, writeBufferSize}, {47 * blockSize, writeBufferSize},
												{55 * blockSize, writeBufferSize}, {63 * blockSize, writeBufferSize},
												{71 * blockSize, writeBufferSize}}}},
								{37 * blockSize, 47 * blockSize, {}, 40 * blockSize,
										{{{37 * blockSize, writeBufferSize}, {45 * blockSize, writeBufferSize},
												{53 * blockSize, writeBufferSize}, {61 * blockSize, writeBufferSize},
												{69 * blockSize, writeBufferSize}}}},
								{34 * blockSize, 53 * blockSize, {}, 53 * blockSize,
										{{{34 * blockSize, writeBufferSize}, {42 * blockSize, writeBufferSize},
												{50 * blockSize, writeBufferSize}, {58 * blockSize, writeBufferSize},
												{66 * blockSize, writeBufferSize}, {74 * blockSize, writeBufferSize}}}},
						}},
						{82 * blockSize, 5 * blockSize}
				},
				{
						"overlapping shrinking",
						{{
								{83 * blockSize, 8 * blockSize, 7 * blockSize, 1 * blockSize, {}},
								{83 * blockSize, 7 * blockSize, 0 * blockSize, 2 * blockSize, {}},
								{85 * blockSize, 5 * blockSize, 2 * blockSize, 3 * blockSize, {}},
								{85 * blockSize, 2 * blockSize, 0 * blockSize, 1 * blockSize, {}},
								{86 * blockSize, 1 * blockSize, 0 * blockSize, 1 * blockSize, {}},
						}},
						{83 * blockSize, 8 * blockSize}
				},
		};

		for (auto& scenario : scenarios)
		{
			DYNAMIC_SECTION("Testing unaligned " << scenario.description << " writes")
			{
				auto& steps = scenario.steps;
				auto& synchronizeStep = scenario.synchronizeStep;

				constexpr uint64_t address {0xed9e2751 * blockSize};
				uint8_t buffer[sizeof(randomData)] {};

				for (auto& step : steps)
				{
					if (step.size == 0)
						continue;

					memset(buffer, 0xf2, sizeof(buffer));
					memcpy(buffer + step.randomDataOffset, randomData + step.offset + step.randomDataOffset,
							step.randomDataSize);

					REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
					REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
					REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
					for (auto& writeCall : step.writeCalls)
						if (writeCall.size != 0)
							expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock,
									write(address + writeCall.offset, writeBuffer, writeCall.size))
									.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence)
									.RETURN(0));
					REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
					REQUIRE(bufferingBlockDevice.write(address + step.offset, buffer, step.size) == 0);
				}

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock,
						write(address + synchronizeStep.writeOffset, writeBuffer, synchronizeStep.writeSize))
						.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.synchronize() == 0);
			}
		}
	}

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.close() == 0);
}

TEST_CASE("Testing read() & write() interactions", "[read][write]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr auto readBufferSize = sizeof(readBuffer);
	constexpr auto writeBufferSize = sizeof(writeBuffer);

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, readBufferSize,
			writeBuffer, writeBufferSize};

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.open() == 0);

	SECTION("Read should take part of data from write buffer")
	{
		struct ReadCall
		{
			size_t offset;
			size_t size;
			size_t dataOffset0;
			size_t dataSize0;
			size_t dataOffset1;
			size_t dataSize1;
		};
		struct Scenario
		{
			size_t read0Offset;
			size_t read0Size;
			ReadCall read0Call;
			size_t writeOffset;
			size_t writeSize;
			size_t read1Offset;
			size_t read1Size;
			std::array<const ReadCall, 3> readCalls;
		};
		static const Scenario scenarios[]
		{
				// read buffer before write buffer, non-adjacent
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                        XXXXX
						// read:                           XXXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						39 * blockSize, 5 * blockSize, 18 * blockSize, 25 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                        XXX
						// read:                           XXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						39 * blockSize, 3 * blockSize, 18 * blockSize, 24 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                  XXXXX
						// read:                             XXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						33 * blockSize, 5 * blockSize, 20 * blockSize, 19 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, 2 * blockSize, 7 * blockSize, 1 * blockSize}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						//                                                         XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                   XXXXXXXX
						// read:                            XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						34 * blockSize, 8 * blockSize, 19 * blockSize, 30 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, 3 * blockSize, {}, {}},
								{42 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						//                                                       XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                  XXXXXXX
						// read:                                XXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						33 * blockSize, 7 * blockSize, 23 * blockSize, 24 * blockSize,
						{{{31 * blockSize, readBufferSize, {}, 2 * blockSize, {}, {}},
								{40 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                   XX
						// read:                                       XXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						34 * blockSize, 2 * blockSize, 30 * blockSize, 9 * blockSize,
						{31 * blockSize, readBufferSize, {}, 3 * blockSize, 5 * blockSize, 3 * blockSize}
				},

				// read buffer before write buffer, adjacent
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                XXXXXX
						// read:                               XXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						31 * blockSize, 6 * blockSize, 22 * blockSize, 12 * blockSize,
						{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                XXXX
						// read:                              XXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						31 * blockSize, 4 * blockSize, 21 * blockSize, 14 * blockSize,
						{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                                 XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                XXX
						// read:                             XXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						31 * blockSize, 3 * blockSize, 20 * blockSize, 17 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{34 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                          XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                XXXXXXXX
						// read:                                XXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						31 * blockSize, 8 * blockSize, 23 * blockSize, 17 * blockSize,
						{39 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                         XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                                XXXXXXX
						// read:                                       XXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						31 * blockSize, 7 * blockSize, 30 * blockSize, 16 * blockSize,
						{38 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},

				// read buffer before write buffer, partially overlapping
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                            XXXXXX
						// read:                           XXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, 4 * blockSize, {}, {}},
						27 * blockSize, 6 * blockSize, 18 * blockSize, 14 * blockSize,
						{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                               XXXX
						// read:                         XXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, 7 * blockSize, {}, {}},
						30 * blockSize, 4 * blockSize, 16 * blockSize, 18 * blockSize,
						{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                                 XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                              XXXXX
						// read:                            XXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, 6 * blockSize, {}, {}},
						29 * blockSize, 5 * blockSize, 19 * blockSize, 21 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{34 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                    XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                          XXXXXXXX
						// read:                                XXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, 2 * blockSize, {}, {}},
						25 * blockSize, 8 * blockSize, 23 * blockSize, 17 * blockSize,
						{33 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                      XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                             XXXXXXX
						// read:                                  XXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, 5 * blockSize, {}, {}},
						28 * blockSize, 7 * blockSize, 25 * blockSize, 17 * blockSize,
						{35 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},

				// read buffer overlapping write buffer
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                         XXXXXX
						// read:                               XXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, 1 * blockSize, {}, {}},
						24 * blockSize, 6 * blockSize, 22 * blockSize, 7 * blockSize,
						{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                         XXXX
						// read:                          XXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
						{23 * blockSize, readBufferSize, {}, 1 * blockSize, 5 * blockSize, 3 * blockSize},
						24 * blockSize, 4 * blockSize, 17 * blockSize, 14 * blockSize,
						{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                          XXXXX
						// read:                        XXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
						{23 * blockSize, readBufferSize, {}, 2 * blockSize, 7 * blockSize, 1 * blockSize},
						25 * blockSize, 5 * blockSize, 15 * blockSize, 23 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                        XXXXXXXX
						// read:                             XXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, {}, {}, {}},
						23 * blockSize, 8 * blockSize, 20 * blockSize, 16 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                            XX
						// read:                                XXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
						{23 * blockSize, readBufferSize, {}, 4 * blockSize, 6 * blockSize, 2 * blockSize},
						27 * blockSize, 2 * blockSize, 23 * blockSize, 14 * blockSize,
						{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                           XXX
						// read:                                  XXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
						{23 * blockSize, readBufferSize, {}, 3 * blockSize, 6 * blockSize, 2 * blockSize},
						26 * blockSize, 3 * blockSize, 25 * blockSize, 14 * blockSize,
						{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},

				// write buffer before read buffer, partially overlapping
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                 XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                      XXXXXX
						// read:                       XXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
								{23 * blockSize, readBufferSize, 4 * blockSize, 4 * blockSize, {}, {}},
						21 * blockSize, 6 * blockSize, 14 * blockSize, 15 * blockSize,
						{14 * blockSize, readBufferSize, {}, 7 * blockSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                       XXX
						// read:                              XXXXXXXXXX
						23 * blockSize, 1 * blockSize,
								{23 * blockSize, readBufferSize, 2 * blockSize, 6 * blockSize, {}, {}},
						22 * blockSize, 3 * blockSize, 21 * blockSize, 10 * blockSize,
						{15 * blockSize, readBufferSize, {}, 7 * blockSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                 XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                    XXXXX
						// read:                       XXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
								{23 * blockSize, readBufferSize, 1 * blockSize, 7 * blockSize, {}, {}},
						19 * blockSize, 5 * blockSize, 14 * blockSize, 22 * blockSize,
						{{{14 * blockSize, readBufferSize, {}, 5 * blockSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                      XXXXXXXX
						// read:                              XXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
								{23 * blockSize, readBufferSize, 6 * blockSize, 2 * blockSize, {}, {}},
						21 * blockSize, 8 * blockSize, 21 * blockSize, 18 * blockSize,
						{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                      XXXXXXX
						// read:                               XXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize,
								{23 * blockSize, readBufferSize, 5 * blockSize, 3 * blockSize, {}, {}},
						21 * blockSize, 7 * blockSize, 22 * blockSize, 14 * blockSize,
						{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},

				// write buffer before read buffer, adjacent
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                 XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                  XXXXXX
						// read:                       XXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						17 * blockSize, 6 * blockSize, 14 * blockSize, 11 * blockSize,
						{14 * blockSize, readBufferSize, {}, 3 * blockSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                    XXXX
						// read:                           XXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						19 * blockSize, 4 * blockSize, 18 * blockSize, 13 * blockSize,
						{15 * blockSize, readBufferSize, {}, 4 * blockSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:               XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                     XXX
						// read:                     XXXXXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						20 * blockSize, 3 * blockSize, 12 * blockSize, 27 * blockSize,
						{{{12 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                XXXXXXXX
						// read:                        XXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						15 * blockSize, 8 * blockSize, 15 * blockSize, 20 * blockSize,
						{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                                  XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                 XXXXXXX
						// read:                            XXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						16 * blockSize, 7 * blockSize, 19 * blockSize, 18 * blockSize,
						{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}
				},

				// write buffer before read buffer, non-adjacent
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:        XXXXXXXX
						//                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:           XXXXX
						// read:              XXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						10 * blockSize, 5 * blockSize, 5 * blockSize, 23 * blockSize,
						{{{5 * blockSize, readBufferSize, {}, 5 * blockSize, {}, {}},
								{15 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:          XXXXXXXX
						//                                XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:               XXX
						// read:                XXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						14 * blockSize, 3 * blockSize, 7 * blockSize, 24 * blockSize,
						{{{7 * blockSize, readBufferSize, {}, 7 * blockSize, {}, {}},
								{17 * blockSize, readBufferSize, {}, 6 * blockSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:                 XXXXX
						// read:                        XXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						16 * blockSize, 5 * blockSize, 15 * blockSize, 19 * blockSize,
						{{{15 * blockSize, readBufferSize, {}, 1 * blockSize, 6 * blockSize, 2 * blockSize},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:          XXXXXXXX
						//                                 XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:           XXXXXXXX
						// read:                XXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						10 * blockSize, 8 * blockSize, 7 * blockSize, 30 * blockSize,
						{{{7 * blockSize, readBufferSize, {}, 3 * blockSize, {}, {}},
								{18 * blockSize, readBufferSize, {}, 5 * blockSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:               XXXXXXX
						// read:                       XXXXXXXXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						14 * blockSize, 7 * blockSize, 14 * blockSize, 24 * blockSize,
						{{{15 * blockSize, readBufferSize, 6 * blockSize, 2 * blockSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
				{
						//               0         1         2         3         4
						//               01234567890123456789012345678901234567890123456789
						// read calls:                  XXXXXXXX
						//                                              XXXXXXXX
						// read buffer:                         XXXXXXXX
						// write buffer:               XXXXXX
						// read:                           XXXXXXXXXXXXXXXXXX
						23 * blockSize, 1 * blockSize, {23 * blockSize, readBufferSize, {}, readBufferSize, {}, {}},
						14 * blockSize, 6 * blockSize, 18 * blockSize, 18 * blockSize,
						{{{15 * blockSize, readBufferSize, 5 * blockSize, 3 * blockSize, {}, {}},
								{31 * blockSize, readBufferSize, {}, readBufferSize, {}, {}}}}
				},
		};

		for (auto& scenario : scenarios)
		{
			DYNAMIC_SECTION("Testing interactions of read and write buffers: scenario " << &scenario - scenarios + 1)
			{
				constexpr uint64_t address {0xd572b1ba * blockSize};
				uint8_t buffer[sizeof(randomData)] {};

				const auto readSideEffect =
						[](const uint64_t _1, void* const _2, const ReadCall& readCall)
						{
							const auto offset0 = readCall.dataOffset0;
							const auto size0 = readCall.dataSize0;
							memcpy(static_cast<uint8_t*>(_2) + offset0, randomData + _1 - address + offset0, size0);
							const auto offset1 = readCall.dataOffset1;
							const auto size1 = readCall.dataSize1;
							memcpy(static_cast<uint8_t*>(_2) + offset1, randomData + _1 - address + offset1, size1);
						};

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				REQUIRE_CALL(blockDeviceMock, read(address + scenario.read0Offset, readBuffer, readBufferSize))
						.IN_SEQUENCE(sequence).SIDE_EFFECT(readSideEffect(_1, _2, scenario.read0Call)).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.read(address + scenario.read0Offset, buffer, scenario.read0Size) == 0);

				memcpy(buffer, randomData + scenario.writeOffset, scenario.writeSize);

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.write(address + scenario.writeOffset, buffer, scenario.writeSize) == 0);

				memset(buffer, 0xd5, sizeof(buffer));

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				for (auto& readCall : scenario.readCalls)
					if (readCall.size != 0)
						expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock,
								read(address + readCall.offset, readBuffer, readCall.size)).IN_SEQUENCE(sequence)
								.SIDE_EFFECT(readSideEffect(_1, _2, readCall)).RETURN(0));
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.read(address + scenario.read1Offset, buffer, scenario.read1Size) == 0);
				REQUIRE(memcmp(buffer, randomData + scenario.read1Offset, scenario.read1Size) == 0);

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, write(address + scenario.writeOffset, writeBuffer, scenario.writeSize))
						.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.synchronize() == 0);
			}
		}
	}
	SECTION("Flushing of write buffer should update read buffer contents")
	{
		constexpr uint64_t address {0xf0151d7d * blockSize};
		constexpr uint64_t offset {71 * blockSize};
		uint8_t buffer[sizeof(randomData)] {};

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, read(address + offset, readBuffer, readBufferSize)).IN_SEQUENCE(sequence)
				.SIDE_EFFECT(memset(_2, 0x4d, _3)).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.read(address + offset, buffer, 1 * blockSize) == 0);

		constexpr uint64_t offset0 {offset + 6 * blockSize};
		constexpr size_t size0 {5 * blockSize};
		memset(buffer, 0x39, sizeof(buffer));
		memcpy(buffer, randomData + offset0, size0);
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address + offset0, buffer, size0) == 0);

		constexpr uint64_t offset1 {offset - 5 * blockSize};
		constexpr size_t size1 {8 * blockSize};
		memset(buffer, 0xa2, sizeof(buffer));
		memcpy(buffer, randomData + offset1, size1);
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, write(address + offset0, writeBuffer, size0))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address + offset1, buffer, size1) == 0);

		constexpr uint64_t offset2 {offset + 3 * blockSize};
		constexpr size_t size2 {3 * blockSize};
		memset(buffer, 0xb7, sizeof(buffer));
		memcpy(buffer, randomData + offset2, size2);
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, write(address + offset1, writeBuffer, size1))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address + offset2, buffer, size2) == 0);

		constexpr uint64_t offset3 {offset + 2 * writeBufferSize};
		constexpr size_t size3 {writeBufferSize};
		memset(buffer, 0x0a, sizeof(buffer));
		memcpy(buffer, randomData + offset3, size3);
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, write(address + offset2, writeBuffer, size2))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address + offset3, buffer, size3) == 0);

		memset(buffer, 0x92, sizeof(buffer));
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.read(address + offset, buffer, readBufferSize) == 0);
		REQUIRE(memcmp(buffer, randomData + offset, readBufferSize) == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, write(address + offset3, writeBuffer, size3))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.synchronize() == 0);
	}

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.close() == 0);
}

TEST_CASE("Testing erase()", "[erase]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};
	std::vector<std::unique_ptr<trompeloeil::expectation>> expectations {};

	constexpr auto readBufferSize = sizeof(readBuffer);
	constexpr auto writeBufferSize = sizeof(writeBuffer);

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, readBufferSize,
			writeBuffer, writeBufferSize};

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.open() == 0);

	SECTION("Block device erase error should propagate error code to caller")
	{
		constexpr uint64_t address {0xc0e48a3d * blockSize};
		constexpr uint64_t size {0xeeee9d45 * blockSize};

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		constexpr int ret {0x67dd33cf};
		REQUIRE_CALL(blockDeviceMock, erase(address, size)).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.erase(address, size) == ret);
	}
	SECTION("Block device write error should propagate error code to caller")
	{
		constexpr uint64_t address {0xd10d12d9 * blockSize};
		uint8_t buffer[7 * blockSize];

		memcpy(buffer, randomData, sizeof(buffer));

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address, buffer, sizeof(buffer)) == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		constexpr int ret {0x29071e02};
		REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, sizeof(buffer) - 5 * blockSize))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.erase(address + 2 * blockSize, 3 * blockSize) == ret);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, sizeof(buffer)))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.synchronize() == 0);
	}
	SECTION("Erasing zero bytes should succeed")
	{
		constexpr uint64_t address {0x746e7a04 * blockSize};

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.erase(address, 0) == 0);
	}
	SECTION("Testing erase() & read() interactions")
	{
		struct Scenario
		{
			size_t readOffset;
			size_t readSize;
			size_t eraseOffset;
			size_t eraseSize;
			bool secondReadCall;
		};
		static const Scenario scenarios[]
		{
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:        XXXXXXXXXXXXXXX
				{19 * blockSize, 5 * blockSize, 3 * blockSize, 15 * blockSize, false},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:            XXXXXXXXXXXX
				{19 * blockSize, 5 * blockSize, 7 * blockSize, 12 * blockSize, false},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:         XXXXXXXXXXXXXXXXXX
				{19 * blockSize, 5 * blockSize, 4 * blockSize, 18 * blockSize, true},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:                     XXXXXXXXXXXXX
				{19 * blockSize, 5 * blockSize, 16 * blockSize, 13 * blockSize, true},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:                        XXXXXXXX
				{19 * blockSize, 5 * blockSize, 19 * blockSize, 8 * blockSize, true},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:                             XX
				{19 * blockSize, 5 * blockSize, 24 * blockSize, 2 * blockSize, true},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:                              XXXXXXXXXXXXXX
				{19 * blockSize, 5 * blockSize, 25 * blockSize, 14 * blockSize, true},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:                                XXXXXXX
				{19 * blockSize, 5 * blockSize, 27 * blockSize, 7 * blockSize, false},
				//            0         1         2         3         4
				//            01234567890123456789012345678901234567890123456789
				// read call:                    XXXXXXXX
				// read:                         XXXXX
				// erase:                                 XXXXXXXXXX
				{19 * blockSize, 5 * blockSize, 28 * blockSize, 10 * blockSize, false},
		};
		for (auto& scenario : scenarios)
		{
			DYNAMIC_SECTION("Testing interactions of erase and read buffers: scenario " << &scenario - scenarios + 1)
			{
				constexpr uint64_t address {0x807cb8d1 * blockSize};
				uint8_t buffer[sizeof(randomData)] {};

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				REQUIRE_CALL(blockDeviceMock, read(address + scenario.readOffset, readBuffer, readBufferSize))
						.IN_SEQUENCE(sequence).SIDE_EFFECT(memcpy(_2, randomData + _1 - address, _3)).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.read(address + scenario.readOffset, buffer, scenario.readSize) == 0);
				REQUIRE(memcmp(buffer, randomData + scenario.readOffset, scenario.readSize) == 0);

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				REQUIRE_CALL(blockDeviceMock, erase(address + scenario.eraseOffset, scenario.eraseSize))
						.IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.erase(address + scenario.eraseOffset, scenario.eraseSize) == 0);

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				if (scenario.secondReadCall == true)
					expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock,
							read(address + scenario.readOffset, readBuffer, readBufferSize)).IN_SEQUENCE(sequence)
							.SIDE_EFFECT(memcpy(_2, randomData + _1 - address, _3)).RETURN(0));
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.read(address + scenario.readOffset, buffer, scenario.readSize) == 0);
				REQUIRE(memcmp(buffer, randomData + scenario.readOffset, scenario.readSize) == 0);
			}
		}
	}
	SECTION("Testing erase() & write() interactions")
	{
		struct WriteCall
		{
			size_t offset;
			size_t size;
		};
		struct Scenario
		{
			size_t writeOffset;
			size_t writeSize;
			size_t eraseOffset;
			size_t eraseSize;
			WriteCall eraseWriteCall;
			WriteCall synchronizeWriteCall;
		};
		static const Scenario scenarios[]
		{
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                       XXXXXXXXXXX
				// erase write call:
				// synchronize write call:                  XXXXXXX
				{17 * blockSize, 7 * blockSize, 5 * blockSize, 11 * blockSize, {}, {17 * blockSize, 7 * blockSize}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                    XXXXXXXXXXXXXXX
				// erase write call:
				// synchronize write call:                  XXXXXXX
				{17 * blockSize, 7 * blockSize, 2 * blockSize, 15 * blockSize, {}, {17 * blockSize, 7 * blockSize}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                          XXXXXXXXXXX
				// erase write call:
				// synchronize write call:                    XXXXX
				{17 * blockSize, 7 * blockSize, 8 * blockSize, 11 * blockSize, {}, {19 * blockSize, 5 * blockSize}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                                 XXXXXXXXXXXXXXXXXX
				// erase write call:
				// synchronize write call:
				{17 * blockSize, 7 * blockSize, 15 * blockSize, 18 * blockSize, {}, {}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                                   XXXXXXX
				// erase write call:
				// synchronize write call:
				{17 * blockSize, 7 * blockSize, 17 * blockSize, 7 * blockSize, {}, {}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                                     XX
				// erase write call:                        XX
				// synchronize write call:                      XXX
				{17 * blockSize, 7 * blockSize, 19 * blockSize, 2 * blockSize, {17 * blockSize, 2 * blockSize},
						{21 * blockSize, 3 * blockSize}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                                     XXXXXXXXXXXXXXXXX
				// erase write call:
				// synchronize write call:                  XX
				{17 * blockSize, 7 * blockSize, 19 * blockSize, 17 * blockSize, {}, {17 * blockSize, 2 * blockSize}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                                          XXXXXXXXXXXXXXXXXXXXXXX
				// erase write call:
				// synchronize write call:                  XXXXXXX
				{17 * blockSize, 7 * blockSize, 24 * blockSize, 23 * blockSize, {}, {17 * blockSize, 7 * blockSize}},
				//                         0         1         2         3         4
				//                         01234567890123456789012345678901234567890123456789
				// write:                                   XXXXXXX
				// erase:                                           XXXXXXXXXXXXXXXXXXXX
				// erase write call:
				// synchronize write call:                  XXXXXXX
				{17 * blockSize, 7 * blockSize, 25 * blockSize, 20 * blockSize, {}, {17 * blockSize, 7 * blockSize}},
		};
		for (auto& scenario : scenarios)
		{
			DYNAMIC_SECTION("Testing interactions of erase and write buffers: scenario " << &scenario - scenarios + 1)
			{
				constexpr uint64_t address {0xe138fa00 * blockSize};
				uint8_t buffer[sizeof(randomData)] {};

				memset(buffer, 0x5a, sizeof(buffer));
				memcpy(buffer, randomData + scenario.writeOffset, scenario.writeSize);

				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.write(address + scenario.writeOffset, buffer, scenario.writeSize) == 0);

				auto& eraseWriteCall = scenario.eraseWriteCall;
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
				REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
				if (eraseWriteCall.size != 0)
					expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock,
							write(address + eraseWriteCall.offset, writeBuffer, eraseWriteCall.size))
							.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0));
				REQUIRE_CALL(blockDeviceMock, erase(address + scenario.eraseOffset, scenario.eraseSize))
						.IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.erase(address + scenario.eraseOffset, scenario.eraseSize) == 0);

				auto& synchronizeWriteCall = scenario.synchronizeWriteCall;
				REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
				if (synchronizeWriteCall.size != 0)
					expectations.emplace_back(NAMED_REQUIRE_CALL(blockDeviceMock,
							write(address + synchronizeWriteCall.offset, writeBuffer, synchronizeWriteCall.size))
							.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0));
				REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
				REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
				REQUIRE(bufferingBlockDevice.synchronize() == 0);
			}
		}
	}

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.close() == 0);
}

TEST_CASE("Testing synchronize()", "[synchronize]")
{
	BlockDevice blockDeviceMock;
	uint8_t readBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	uint8_t writeBuffer[blockSize * 8] __attribute__ ((aligned(alignment))) {};
	trompeloeil::sequence sequence {};

	constexpr auto readBufferSize = sizeof(readBuffer);
	constexpr auto writeBufferSize = sizeof(writeBuffer);

	distortos::devices::BufferingBlockDevice bufferingBlockDevice {blockDeviceMock, readBuffer, readBufferSize,
			writeBuffer, writeBufferSize};

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, open()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.open() == 0);

	SECTION("Block device synchronize error should propagate error code to caller")
	{
		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		constexpr int ret {0x6adc3b62};
		REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.synchronize() == ret);
	}
	SECTION("Block device write error should propagate error code to caller")
	{
		constexpr uint64_t address {0x66b7617d * blockSize};
		uint8_t buffer[3 * blockSize];

		memcpy(buffer, randomData, sizeof(buffer));

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, getBlockSize()).IN_SEQUENCE(sequence).RETURN(blockSize);
		REQUIRE_CALL(blockDeviceMock, getSize()).IN_SEQUENCE(sequence).RETURN(deviceSize);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.write(address, buffer, sizeof(buffer)) == 0);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		constexpr int ret {0x5bd9c2ed};
		REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, sizeof(buffer)))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(ret);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.synchronize() == ret);

		REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
		REQUIRE_CALL(blockDeviceMock, write(address, writeBuffer, sizeof(buffer)))
				.WITH(memcmp(_2, randomData + _1 - address, _3) == 0).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, synchronize()).IN_SEQUENCE(sequence).RETURN(0);
		REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
		REQUIRE(bufferingBlockDevice.synchronize() == 0);
	}

	REQUIRE_CALL(blockDeviceMock, lock()).IN_SEQUENCE(sequence);
	REQUIRE_CALL(blockDeviceMock, close()).IN_SEQUENCE(sequence).RETURN(0);
	REQUIRE_CALL(blockDeviceMock, unlock()).IN_SEQUENCE(sequence);
	REQUIRE(bufferingBlockDevice.close() == 0);
}
