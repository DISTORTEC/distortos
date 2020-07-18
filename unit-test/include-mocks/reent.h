/**
 * \file
 * \brief Mock of newlib's _reent struct and related functionalities
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_REENT_H_
#define UNIT_TEST_INCLUDE_MOCKS_REENT_H_

struct _reent
{

};

extern struct _reent* _impure_ptr;

#endif	/* UNIT_TEST_INCLUDE_MOCKS_REENT_H_ */
