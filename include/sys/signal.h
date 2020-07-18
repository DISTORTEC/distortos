/**
 * \file
 * \brief Override for newlib's sys/signal.h
 *
 * \author Copyright (C) 2015 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_SYS_SIGNAL_H_
#define INCLUDE_SYS_SIGNAL_H_

#ifdef __cplusplus
extern "C"
{
#endif	/* def __cplusplus */

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/** pointer to function taking an int argument (required by newlib's signal.h) */
typedef void (* _sig_func_ptr)(int);

/** standard sigval union - integer and void pointer */
union sigval
{
	/** integer signal value */
	int sival_int;

	/** pointer signal value */
	void* sival_ptr;
};

#ifdef __cplusplus
}	/* extern "C" */
#endif	/* def __cplusplus */

#endif	/* INCLUDE_SYS_SIGNAL_H_ */
