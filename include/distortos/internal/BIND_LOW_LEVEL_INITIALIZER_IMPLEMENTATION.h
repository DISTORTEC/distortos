/**
 * \file
 * \brief BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION() macro
 *
 * \author Copyright (C) 2018-2019 Kamil Szczygiel http://www.distortec.com https://freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDE_DISTORTOS_INTERNAL_BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION_H_
#define INCLUDE_DISTORTOS_INTERNAL_BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION_H_

#include "distortos/CONCATENATE.h"

/*---------------------------------------------------------------------------------------------------------------------+
| global types
+---------------------------------------------------------------------------------------------------------------------*/

/// low-level initializer function
typedef void (LowLevelInitializer)(void);

/*---------------------------------------------------------------------------------------------------------------------+
| global defines
+---------------------------------------------------------------------------------------------------------------------*/

#define PRIORITY_STRING_0	"00"
#define PRIORITY_STRING_1	"01"
#define PRIORITY_STRING_2	"02"
#define PRIORITY_STRING_3	"03"
#define PRIORITY_STRING_4	"04"
#define PRIORITY_STRING_5	"05"
#define PRIORITY_STRING_6	"06"
#define PRIORITY_STRING_7	"07"
#define PRIORITY_STRING_8	"08"
#define PRIORITY_STRING_9	"09"

#define PRIORITY_STRING_10	"10"
#define PRIORITY_STRING_11	"11"
#define PRIORITY_STRING_12	"12"
#define PRIORITY_STRING_13	"13"
#define PRIORITY_STRING_14	"14"
#define PRIORITY_STRING_15	"15"
#define PRIORITY_STRING_16	"16"
#define PRIORITY_STRING_17	"17"
#define PRIORITY_STRING_18	"18"
#define PRIORITY_STRING_19	"19"

#define PRIORITY_STRING_20	"20"
#define PRIORITY_STRING_21	"21"
#define PRIORITY_STRING_22	"22"
#define PRIORITY_STRING_23	"23"
#define PRIORITY_STRING_24	"24"
#define PRIORITY_STRING_25	"25"
#define PRIORITY_STRING_26	"26"
#define PRIORITY_STRING_27	"27"
#define PRIORITY_STRING_28	"28"
#define PRIORITY_STRING_29	"29"

#define PRIORITY_STRING_30	"30"
#define PRIORITY_STRING_31	"31"
#define PRIORITY_STRING_32	"32"
#define PRIORITY_STRING_33	"33"
#define PRIORITY_STRING_34	"34"
#define PRIORITY_STRING_35	"35"
#define PRIORITY_STRING_36	"36"
#define PRIORITY_STRING_37	"37"
#define PRIORITY_STRING_38	"38"
#define PRIORITY_STRING_39	"39"

#define PRIORITY_STRING_40	"40"
#define PRIORITY_STRING_41	"41"
#define PRIORITY_STRING_42	"42"
#define PRIORITY_STRING_43	"43"
#define PRIORITY_STRING_44	"44"
#define PRIORITY_STRING_45	"45"
#define PRIORITY_STRING_46	"46"
#define PRIORITY_STRING_47	"47"
#define PRIORITY_STRING_48	"48"
#define PRIORITY_STRING_49	"49"

#define PRIORITY_STRING_50	"50"
#define PRIORITY_STRING_51	"51"
#define PRIORITY_STRING_52	"52"
#define PRIORITY_STRING_53	"53"
#define PRIORITY_STRING_54	"54"
#define PRIORITY_STRING_55	"55"
#define PRIORITY_STRING_56	"56"
#define PRIORITY_STRING_57	"57"
#define PRIORITY_STRING_58	"58"
#define PRIORITY_STRING_59	"59"

#define PRIORITY_STRING_60	"60"
#define PRIORITY_STRING_61	"61"
#define PRIORITY_STRING_62	"62"
#define PRIORITY_STRING_63	"63"
#define PRIORITY_STRING_64	"64"
#define PRIORITY_STRING_65	"65"
#define PRIORITY_STRING_66	"66"
#define PRIORITY_STRING_67	"67"
#define PRIORITY_STRING_68	"68"
#define PRIORITY_STRING_69	"69"

#define PRIORITY_STRING_70	"70"
#define PRIORITY_STRING_71	"71"
#define PRIORITY_STRING_72	"72"
#define PRIORITY_STRING_73	"73"
#define PRIORITY_STRING_74	"74"
#define PRIORITY_STRING_75	"75"
#define PRIORITY_STRING_76	"76"
#define PRIORITY_STRING_77	"77"
#define PRIORITY_STRING_78	"78"
#define PRIORITY_STRING_79	"79"

#define PRIORITY_STRING_80	"80"
#define PRIORITY_STRING_81	"81"
#define PRIORITY_STRING_82	"82"
#define PRIORITY_STRING_83	"83"
#define PRIORITY_STRING_84	"84"
#define PRIORITY_STRING_85	"85"
#define PRIORITY_STRING_86	"86"
#define PRIORITY_STRING_87	"87"
#define PRIORITY_STRING_88	"88"
#define PRIORITY_STRING_89	"89"

#define PRIORITY_STRING_90	"90"
#define PRIORITY_STRING_91	"91"
#define PRIORITY_STRING_92	"92"
#define PRIORITY_STRING_93	"93"
#define PRIORITY_STRING_94	"94"
#define PRIORITY_STRING_95	"95"
#define PRIORITY_STRING_96	"96"
#define PRIORITY_STRING_97	"97"
#define PRIORITY_STRING_98	"98"
#define PRIORITY_STRING_99	"99"

/**
 * \brief Transforms integer into a zero-padded string.
 *
 * \param [in] priority is an integer which will be transformed, [0; 99]
 */

#define PRIORITY_STRING(priority)	CONCATENATE2(PRIORITY_STRING_, priority)

/**
 * \brief Binds function as low-level (pre-)initializer with specified priority.
 *
 * \param [in] priority is an priority of the low-level (pre-)initializer, [0; 99]
 * \param [in] function is the low-level (pre-)initializer function
 * \param [in] sectionPrefix is the section prefix, {".low_level_preinitializers.", ".low_level_initializers."}
 */

#define BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION(priority, function, sectionPrefix) \
		__attribute__ ((section(sectionPrefix PRIORITY_STRING(priority)), used)) \
		static LowLevelInitializer* const CONCATENATE2(lowLevelInitializerPointer, __COUNTER__) = function

#endif	/* INCLUDE_DISTORTOS_INTERNAL_BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION_H_ */
