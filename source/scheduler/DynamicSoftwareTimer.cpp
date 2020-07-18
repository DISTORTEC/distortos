/**
 * \file
 * \brief DynamicSoftwareTimer class implementation
 *
 * \author Copyright (C) 2017 Kamil Szczygiel https://distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "distortos/DynamicSoftwareTimer.hpp"

namespace distortos
{

/*---------------------------------------------------------------------------------------------------------------------+
| private functions
+---------------------------------------------------------------------------------------------------------------------*/

void DynamicSoftwareTimer::run()
{
	boundFunction_();
}

}	// namespace distortos
