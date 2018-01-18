/**
 * \file
 * \brief BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION() macro
 *
 * \author Copyright (C) 2018 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
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

#define ORDER_STRING_0	"00"
#define ORDER_STRING_1	"01"
#define ORDER_STRING_2	"02"
#define ORDER_STRING_3	"03"
#define ORDER_STRING_4	"04"
#define ORDER_STRING_5	"05"
#define ORDER_STRING_6	"06"
#define ORDER_STRING_7	"07"
#define ORDER_STRING_8	"08"
#define ORDER_STRING_9	"09"

#define ORDER_STRING_10	"10"
#define ORDER_STRING_11	"11"
#define ORDER_STRING_12	"12"
#define ORDER_STRING_13	"13"
#define ORDER_STRING_14	"14"
#define ORDER_STRING_15	"15"
#define ORDER_STRING_16	"16"
#define ORDER_STRING_17	"17"
#define ORDER_STRING_18	"18"
#define ORDER_STRING_19	"19"

#define ORDER_STRING_20	"20"
#define ORDER_STRING_21	"21"
#define ORDER_STRING_22	"22"
#define ORDER_STRING_23	"23"
#define ORDER_STRING_24	"24"
#define ORDER_STRING_25	"25"
#define ORDER_STRING_26	"26"
#define ORDER_STRING_27	"27"
#define ORDER_STRING_28	"28"
#define ORDER_STRING_29	"29"

#define ORDER_STRING_30	"30"
#define ORDER_STRING_31	"31"
#define ORDER_STRING_32	"32"
#define ORDER_STRING_33	"33"
#define ORDER_STRING_34	"34"
#define ORDER_STRING_35	"35"
#define ORDER_STRING_36	"36"
#define ORDER_STRING_37	"37"
#define ORDER_STRING_38	"38"
#define ORDER_STRING_39	"39"

#define ORDER_STRING_40	"40"
#define ORDER_STRING_41	"41"
#define ORDER_STRING_42	"42"
#define ORDER_STRING_43	"43"
#define ORDER_STRING_44	"44"
#define ORDER_STRING_45	"45"
#define ORDER_STRING_46	"46"
#define ORDER_STRING_47	"47"
#define ORDER_STRING_48	"48"
#define ORDER_STRING_49	"49"

#define ORDER_STRING_50	"50"
#define ORDER_STRING_51	"51"
#define ORDER_STRING_52	"52"
#define ORDER_STRING_53	"53"
#define ORDER_STRING_54	"54"
#define ORDER_STRING_55	"55"
#define ORDER_STRING_56	"56"
#define ORDER_STRING_57	"57"
#define ORDER_STRING_58	"58"
#define ORDER_STRING_59	"59"

#define ORDER_STRING_60	"60"
#define ORDER_STRING_61	"61"
#define ORDER_STRING_62	"62"
#define ORDER_STRING_63	"63"
#define ORDER_STRING_64	"64"
#define ORDER_STRING_65	"65"
#define ORDER_STRING_66	"66"
#define ORDER_STRING_67	"67"
#define ORDER_STRING_68	"68"
#define ORDER_STRING_69	"69"

#define ORDER_STRING_70	"70"
#define ORDER_STRING_71	"71"
#define ORDER_STRING_72	"72"
#define ORDER_STRING_73	"73"
#define ORDER_STRING_74	"74"
#define ORDER_STRING_75	"75"
#define ORDER_STRING_76	"76"
#define ORDER_STRING_77	"77"
#define ORDER_STRING_78	"78"
#define ORDER_STRING_79	"79"

#define ORDER_STRING_80	"80"
#define ORDER_STRING_81	"81"
#define ORDER_STRING_82	"82"
#define ORDER_STRING_83	"83"
#define ORDER_STRING_84	"84"
#define ORDER_STRING_85	"85"
#define ORDER_STRING_86	"86"
#define ORDER_STRING_87	"87"
#define ORDER_STRING_88	"88"
#define ORDER_STRING_89	"89"

#define ORDER_STRING_90	"90"
#define ORDER_STRING_91	"91"
#define ORDER_STRING_92	"92"
#define ORDER_STRING_93	"93"
#define ORDER_STRING_94	"94"
#define ORDER_STRING_95	"95"
#define ORDER_STRING_96	"96"
#define ORDER_STRING_97	"97"
#define ORDER_STRING_98	"98"
#define ORDER_STRING_99	"99"

/**
 * \brief Transforms integer into a zero-padded string.
 *
 * \param [in] order is an integer which will be transformed, [0; 99]
 */

#define ORDER_STRING(order)	CONCATENATE2(ORDER_STRING_, order)

/**
 * \brief Binds function as low-level (pre-)initializer with specified order.
 *
 * \param [in] order is an order of the low-level (pre-)initializer, [0; 99]
 * \param [in] function is the low-level (pre-)initializer function
 * \param [in] sectionPrefix is the section prefix, {".low_level_preinitializers.", ".low_level_initializers."}
 */

#define BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION(order, function, sectionPrefix) \
		__attribute__ ((section(sectionPrefix ORDER_STRING(order)), used)) \
		static LowLevelInitializer* const CONCATENATE2(lowLevelInitializerPointer, __COUNTER__) = function

#endif	/* INCLUDE_DISTORTOS_INTERNAL_BIND_LOW_LEVEL_INITIALIZER_IMPLEMENTATION_H_ */
