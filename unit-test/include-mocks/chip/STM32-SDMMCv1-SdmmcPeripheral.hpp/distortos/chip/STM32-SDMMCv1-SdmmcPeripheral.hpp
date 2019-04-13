/**
 * \file
 * \brief Mock of SdmmcPeripheral class for SDMMCv1 in STM32
 *
 * \author Copyright (C) 2019 Kamil Szczygiel http://www.distortec.com http://www.freddiechopin.info
 *
 * \par License
 * This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
 * distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_DISTORTOS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_
#define UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_DISTORTOS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_

#include "unit-test-common.hpp"

namespace distortos
{

namespace chip
{

class SdmmcPeripheral
{
public:

	MAKE_CONST_MOCK0(getAdapterFrequency, uint32_t());
	MAKE_CONST_MOCK0(getFifoAddress, uintptr_t());
	MAKE_CONST_MOCK0(getPeripheralFrequency, uint32_t());
	MAKE_CONST_MOCK0(readPower, uint32_t());
	MAKE_CONST_MOCK0(readClkcr, uint32_t());
	MAKE_CONST_MOCK0(readArg, uint32_t());
	MAKE_CONST_MOCK0(readCmd, uint32_t());
	MAKE_CONST_MOCK0(readRespcmd, uint32_t());
	MAKE_CONST_MOCK0(readResp1, uint32_t());
	MAKE_CONST_MOCK0(readResp2, uint32_t());
	MAKE_CONST_MOCK0(readResp3, uint32_t());
	MAKE_CONST_MOCK0(readResp4, uint32_t());
	MAKE_CONST_MOCK0(readDtimer, uint32_t());
	MAKE_CONST_MOCK0(readDlen, uint32_t());
	MAKE_CONST_MOCK0(readDctrl, uint32_t());
	MAKE_CONST_MOCK0(readDcount, uint32_t());
	MAKE_CONST_MOCK0(readSta, uint32_t());
	MAKE_CONST_MOCK0(readIcr, uint32_t());
	MAKE_CONST_MOCK0(readMask, uint32_t());
	MAKE_CONST_MOCK0(readFifocnt, uint32_t());
	MAKE_CONST_MOCK0(readFifo, uint32_t());
	MAKE_CONST_MOCK1(writePower, void(uint32_t));
	MAKE_CONST_MOCK1(writeClkcr, void(uint32_t));
	MAKE_CONST_MOCK1(writeArg, void(uint32_t));
	MAKE_CONST_MOCK1(writeCmd, void(uint32_t));
	MAKE_CONST_MOCK1(writeDtimer, void(uint32_t));
	MAKE_CONST_MOCK1(writeDlen, void(uint32_t));
	MAKE_CONST_MOCK1(writeDctrl, void(uint32_t));
	MAKE_CONST_MOCK1(writeIcr, void(uint32_t));
	MAKE_CONST_MOCK1(writeMask, void(uint32_t));
	MAKE_CONST_MOCK1(writeFifo, void(uint32_t));
};

}	// namespace chip

}	// namespace distortos

// following definitions were copied from CMSIS-STM32F7/stm32f722xx.h

/******************  Bit definition for SDMMC_POWER register  ******************/
#define SDMMC_POWER_PWRCTRL_Pos         (0U)
#define SDMMC_POWER_PWRCTRL_Msk         (0x3U << SDMMC_POWER_PWRCTRL_Pos)      /*!< 0x00000003 */
#define SDMMC_POWER_PWRCTRL             SDMMC_POWER_PWRCTRL_Msk                /*!<PWRCTRL[1:0] bits (Power supply control bits) */
#define SDMMC_POWER_PWRCTRL_0           (0x1U << SDMMC_POWER_PWRCTRL_Pos)      /*!< 0x01 */
#define SDMMC_POWER_PWRCTRL_1           (0x2U << SDMMC_POWER_PWRCTRL_Pos)      /*!< 0x02 */

/******************  Bit definition for SDMMC_CLKCR register  ******************/
#define SDMMC_CLKCR_CLKDIV_Pos          (0U)
#define SDMMC_CLKCR_CLKDIV_Msk          (0xFFU << SDMMC_CLKCR_CLKDIV_Pos)      /*!< 0x000000FF */
#define SDMMC_CLKCR_CLKDIV              SDMMC_CLKCR_CLKDIV_Msk                 /*!<Clock divide factor             */
#define SDMMC_CLKCR_CLKEN_Pos           (8U)
#define SDMMC_CLKCR_CLKEN_Msk           (0x1U << SDMMC_CLKCR_CLKEN_Pos)        /*!< 0x00000100 */
#define SDMMC_CLKCR_CLKEN               SDMMC_CLKCR_CLKEN_Msk                  /*!<Clock enable bit                */
#define SDMMC_CLKCR_PWRSAV_Pos          (9U)
#define SDMMC_CLKCR_PWRSAV_Msk          (0x1U << SDMMC_CLKCR_PWRSAV_Pos)       /*!< 0x00000200 */
#define SDMMC_CLKCR_PWRSAV              SDMMC_CLKCR_PWRSAV_Msk                 /*!<Power saving configuration bit  */
#define SDMMC_CLKCR_BYPASS_Pos          (10U)
#define SDMMC_CLKCR_BYPASS_Msk          (0x1U << SDMMC_CLKCR_BYPASS_Pos)       /*!< 0x00000400 */
#define SDMMC_CLKCR_BYPASS              SDMMC_CLKCR_BYPASS_Msk                 /*!<Clock divider bypass enable bit */

#define SDMMC_CLKCR_WIDBUS_Pos          (11U)
#define SDMMC_CLKCR_WIDBUS_Msk          (0x3U << SDMMC_CLKCR_WIDBUS_Pos)       /*!< 0x00001800 */
#define SDMMC_CLKCR_WIDBUS              SDMMC_CLKCR_WIDBUS_Msk                 /*!<WIDBUS[1:0] bits (Wide bus mode enable bit) */
#define SDMMC_CLKCR_WIDBUS_0            (0x1U << SDMMC_CLKCR_WIDBUS_Pos)       /*!< 0x0800 */
#define SDMMC_CLKCR_WIDBUS_1            (0x2U << SDMMC_CLKCR_WIDBUS_Pos)       /*!< 0x1000 */

#define SDMMC_CLKCR_NEGEDGE_Pos         (13U)
#define SDMMC_CLKCR_NEGEDGE_Msk         (0x1U << SDMMC_CLKCR_NEGEDGE_Pos)      /*!< 0x00002000 */
#define SDMMC_CLKCR_NEGEDGE             SDMMC_CLKCR_NEGEDGE_Msk                /*!<SDMMC_CK dephasing selection bit */
#define SDMMC_CLKCR_HWFC_EN_Pos         (14U)
#define SDMMC_CLKCR_HWFC_EN_Msk         (0x1U << SDMMC_CLKCR_HWFC_EN_Pos)      /*!< 0x00004000 */
#define SDMMC_CLKCR_HWFC_EN             SDMMC_CLKCR_HWFC_EN_Msk                /*!<HW Flow Control enable          */

/*******************  Bit definition for SDMMC_ARG register  *******************/
#define SDMMC_ARG_CMDARG_Pos            (0U)
#define SDMMC_ARG_CMDARG_Msk            (0xFFFFFFFFU << SDMMC_ARG_CMDARG_Pos)  /*!< 0xFFFFFFFF */
#define SDMMC_ARG_CMDARG                SDMMC_ARG_CMDARG_Msk                   /*!<Command argument */

/*******************  Bit definition for SDMMC_CMD register  *******************/
#define SDMMC_CMD_CMDINDEX_Pos          (0U)
#define SDMMC_CMD_CMDINDEX_Msk          (0x3FU << SDMMC_CMD_CMDINDEX_Pos)      /*!< 0x0000003F */
#define SDMMC_CMD_CMDINDEX              SDMMC_CMD_CMDINDEX_Msk                 /*!<Command Index                               */

#define SDMMC_CMD_WAITRESP_Pos          (6U)
#define SDMMC_CMD_WAITRESP_Msk          (0x3U << SDMMC_CMD_WAITRESP_Pos)       /*!< 0x000000C0 */
#define SDMMC_CMD_WAITRESP              SDMMC_CMD_WAITRESP_Msk                 /*!<WAITRESP[1:0] bits (Wait for response bits) */
#define SDMMC_CMD_WAITRESP_0            (0x1U << SDMMC_CMD_WAITRESP_Pos)       /*!< 0x0040 */
#define SDMMC_CMD_WAITRESP_1            (0x2U << SDMMC_CMD_WAITRESP_Pos)       /*!< 0x0080 */

#define SDMMC_CMD_WAITINT_Pos           (8U)
#define SDMMC_CMD_WAITINT_Msk           (0x1U << SDMMC_CMD_WAITINT_Pos)        /*!< 0x00000100 */
#define SDMMC_CMD_WAITINT               SDMMC_CMD_WAITINT_Msk                  /*!<CPSM Waits for Interrupt Request                               */
#define SDMMC_CMD_WAITPEND_Pos          (9U)
#define SDMMC_CMD_WAITPEND_Msk          (0x1U << SDMMC_CMD_WAITPEND_Pos)       /*!< 0x00000200 */
#define SDMMC_CMD_WAITPEND              SDMMC_CMD_WAITPEND_Msk                 /*!<CPSM Waits for ends of data transfer (CmdPend internal signal) */
#define SDMMC_CMD_CPSMEN_Pos            (10U)
#define SDMMC_CMD_CPSMEN_Msk            (0x1U << SDMMC_CMD_CPSMEN_Pos)         /*!< 0x00000400 */
#define SDMMC_CMD_CPSMEN                SDMMC_CMD_CPSMEN_Msk                   /*!<Command path state machine (CPSM) Enable bit                   */
#define SDMMC_CMD_SDIOSUSPEND_Pos       (11U)
#define SDMMC_CMD_SDIOSUSPEND_Msk       (0x1U << SDMMC_CMD_SDIOSUSPEND_Pos)    /*!< 0x00000800 */
#define SDMMC_CMD_SDIOSUSPEND           SDMMC_CMD_SDIOSUSPEND_Msk              /*!<SD I/O suspend command                                         */

/*****************  Bit definition for SDMMC_RESPCMD register  *****************/
#define SDMMC_RESPCMD_RESPCMD_Pos       (0U)
#define SDMMC_RESPCMD_RESPCMD_Msk       (0x3FU << SDMMC_RESPCMD_RESPCMD_Pos)   /*!< 0x0000003F */
#define SDMMC_RESPCMD_RESPCMD           SDMMC_RESPCMD_RESPCMD_Msk              /*!<Response command index */

/******************  Bit definition for SDMMC_RESP0 register  ******************/
#define SDMMC_RESP0_CARDSTATUS0_Pos     (0U)
#define SDMMC_RESP0_CARDSTATUS0_Msk     (0xFFFFFFFFU << SDMMC_RESP0_CARDSTATUS0_Pos) /*!< 0xFFFFFFFF */
#define SDMMC_RESP0_CARDSTATUS0         SDMMC_RESP0_CARDSTATUS0_Msk            /*!<Card Status */

/******************  Bit definition for SDMMC_RESP1 register  ******************/
#define SDMMC_RESP1_CARDSTATUS1_Pos     (0U)
#define SDMMC_RESP1_CARDSTATUS1_Msk     (0xFFFFFFFFU << SDMMC_RESP1_CARDSTATUS1_Pos) /*!< 0xFFFFFFFF */
#define SDMMC_RESP1_CARDSTATUS1         SDMMC_RESP1_CARDSTATUS1_Msk            /*!<Card Status */

/******************  Bit definition for SDMMC_RESP2 register  ******************/
#define SDMMC_RESP2_CARDSTATUS2_Pos     (0U)
#define SDMMC_RESP2_CARDSTATUS2_Msk     (0xFFFFFFFFU << SDMMC_RESP2_CARDSTATUS2_Pos) /*!< 0xFFFFFFFF */
#define SDMMC_RESP2_CARDSTATUS2         SDMMC_RESP2_CARDSTATUS2_Msk            /*!<Card Status */

/******************  Bit definition for SDMMC_RESP3 register  ******************/
#define SDMMC_RESP3_CARDSTATUS3_Pos     (0U)
#define SDMMC_RESP3_CARDSTATUS3_Msk     (0xFFFFFFFFU << SDMMC_RESP3_CARDSTATUS3_Pos) /*!< 0xFFFFFFFF */
#define SDMMC_RESP3_CARDSTATUS3         SDMMC_RESP3_CARDSTATUS3_Msk            /*!<Card Status */

/******************  Bit definition for SDMMC_RESP4 register  ******************/
#define SDMMC_RESP4_CARDSTATUS4_Pos     (0U)
#define SDMMC_RESP4_CARDSTATUS4_Msk     (0xFFFFFFFFU << SDMMC_RESP4_CARDSTATUS4_Pos) /*!< 0xFFFFFFFF */
#define SDMMC_RESP4_CARDSTATUS4         SDMMC_RESP4_CARDSTATUS4_Msk            /*!<Card Status */

/******************  Bit definition for SDMMC_DTIMER register  *****************/
#define SDMMC_DTIMER_DATATIME_Pos       (0U)
#define SDMMC_DTIMER_DATATIME_Msk       (0xFFFFFFFFU << SDMMC_DTIMER_DATATIME_Pos) /*!< 0xFFFFFFFF */
#define SDMMC_DTIMER_DATATIME           SDMMC_DTIMER_DATATIME_Msk              /*!<Data timeout period. */

/******************  Bit definition for SDMMC_DLEN register  *******************/
#define SDMMC_DLEN_DATALENGTH_Pos       (0U)
#define SDMMC_DLEN_DATALENGTH_Msk       (0x1FFFFFFU << SDMMC_DLEN_DATALENGTH_Pos) /*!< 0x01FFFFFF */
#define SDMMC_DLEN_DATALENGTH           SDMMC_DLEN_DATALENGTH_Msk              /*!<Data length value    */

/******************  Bit definition for SDMMC_DCTRL register  ******************/
#define SDMMC_DCTRL_DTEN_Pos            (0U)
#define SDMMC_DCTRL_DTEN_Msk            (0x1U << SDMMC_DCTRL_DTEN_Pos)         /*!< 0x00000001 */
#define SDMMC_DCTRL_DTEN                SDMMC_DCTRL_DTEN_Msk                   /*!<Data transfer enabled bit         */
#define SDMMC_DCTRL_DTDIR_Pos           (1U)
#define SDMMC_DCTRL_DTDIR_Msk           (0x1U << SDMMC_DCTRL_DTDIR_Pos)        /*!< 0x00000002 */
#define SDMMC_DCTRL_DTDIR               SDMMC_DCTRL_DTDIR_Msk                  /*!<Data transfer direction selection */
#define SDMMC_DCTRL_DTMODE_Pos          (2U)
#define SDMMC_DCTRL_DTMODE_Msk          (0x1U << SDMMC_DCTRL_DTMODE_Pos)       /*!< 0x00000004 */
#define SDMMC_DCTRL_DTMODE              SDMMC_DCTRL_DTMODE_Msk                 /*!<Data transfer mode selection      */
#define SDMMC_DCTRL_DMAEN_Pos           (3U)
#define SDMMC_DCTRL_DMAEN_Msk           (0x1U << SDMMC_DCTRL_DMAEN_Pos)        /*!< 0x00000008 */
#define SDMMC_DCTRL_DMAEN               SDMMC_DCTRL_DMAEN_Msk                  /*!<DMA enabled bit                   */

#define SDMMC_DCTRL_DBLOCKSIZE_Pos      (4U)
#define SDMMC_DCTRL_DBLOCKSIZE_Msk      (0xFU << SDMMC_DCTRL_DBLOCKSIZE_Pos)   /*!< 0x000000F0 */
#define SDMMC_DCTRL_DBLOCKSIZE          SDMMC_DCTRL_DBLOCKSIZE_Msk             /*!<DBLOCKSIZE[3:0] bits (Data block size) */
#define SDMMC_DCTRL_DBLOCKSIZE_0        (0x1U << SDMMC_DCTRL_DBLOCKSIZE_Pos)   /*!< 0x0010 */
#define SDMMC_DCTRL_DBLOCKSIZE_1        (0x2U << SDMMC_DCTRL_DBLOCKSIZE_Pos)   /*!< 0x0020 */
#define SDMMC_DCTRL_DBLOCKSIZE_2        (0x4U << SDMMC_DCTRL_DBLOCKSIZE_Pos)   /*!< 0x0040 */
#define SDMMC_DCTRL_DBLOCKSIZE_3        (0x8U << SDMMC_DCTRL_DBLOCKSIZE_Pos)   /*!< 0x0080 */

#define SDMMC_DCTRL_RWSTART_Pos         (8U)
#define SDMMC_DCTRL_RWSTART_Msk         (0x1U << SDMMC_DCTRL_RWSTART_Pos)      /*!< 0x00000100 */
#define SDMMC_DCTRL_RWSTART             SDMMC_DCTRL_RWSTART_Msk                /*!<Read wait start         */
#define SDMMC_DCTRL_RWSTOP_Pos          (9U)
#define SDMMC_DCTRL_RWSTOP_Msk          (0x1U << SDMMC_DCTRL_RWSTOP_Pos)       /*!< 0x00000200 */
#define SDMMC_DCTRL_RWSTOP              SDMMC_DCTRL_RWSTOP_Msk                 /*!<Read wait stop          */
#define SDMMC_DCTRL_RWMOD_Pos           (10U)
#define SDMMC_DCTRL_RWMOD_Msk           (0x1U << SDMMC_DCTRL_RWMOD_Pos)        /*!< 0x00000400 */
#define SDMMC_DCTRL_RWMOD               SDMMC_DCTRL_RWMOD_Msk                  /*!<Read wait mode          */
#define SDMMC_DCTRL_SDIOEN_Pos          (11U)
#define SDMMC_DCTRL_SDIOEN_Msk          (0x1U << SDMMC_DCTRL_SDIOEN_Pos)       /*!< 0x00000800 */
#define SDMMC_DCTRL_SDIOEN              SDMMC_DCTRL_SDIOEN_Msk                 /*!<SD I/O enable functions */

/******************  Bit definition for SDMMC_DCOUNT register  *****************/
#define SDMMC_DCOUNT_DATACOUNT_Pos      (0U)
#define SDMMC_DCOUNT_DATACOUNT_Msk      (0x1FFFFFFU << SDMMC_DCOUNT_DATACOUNT_Pos) /*!< 0x01FFFFFF */
#define SDMMC_DCOUNT_DATACOUNT          SDMMC_DCOUNT_DATACOUNT_Msk             /*!<Data count value */

/******************  Bit definition for SDMMC_STA register  ********************/
#define SDMMC_STA_CCRCFAIL_Pos          (0U)
#define SDMMC_STA_CCRCFAIL_Msk          (0x1U << SDMMC_STA_CCRCFAIL_Pos)       /*!< 0x00000001 */
#define SDMMC_STA_CCRCFAIL              SDMMC_STA_CCRCFAIL_Msk                 /*!<Command response received (CRC check failed)  */
#define SDMMC_STA_DCRCFAIL_Pos          (1U)
#define SDMMC_STA_DCRCFAIL_Msk          (0x1U << SDMMC_STA_DCRCFAIL_Pos)       /*!< 0x00000002 */
#define SDMMC_STA_DCRCFAIL              SDMMC_STA_DCRCFAIL_Msk                 /*!<Data block sent/received (CRC check failed)   */
#define SDMMC_STA_CTIMEOUT_Pos          (2U)
#define SDMMC_STA_CTIMEOUT_Msk          (0x1U << SDMMC_STA_CTIMEOUT_Pos)       /*!< 0x00000004 */
#define SDMMC_STA_CTIMEOUT              SDMMC_STA_CTIMEOUT_Msk                 /*!<Command response timeout                      */
#define SDMMC_STA_DTIMEOUT_Pos          (3U)
#define SDMMC_STA_DTIMEOUT_Msk          (0x1U << SDMMC_STA_DTIMEOUT_Pos)       /*!< 0x00000008 */
#define SDMMC_STA_DTIMEOUT              SDMMC_STA_DTIMEOUT_Msk                 /*!<Data timeout                                  */
#define SDMMC_STA_TXUNDERR_Pos          (4U)
#define SDMMC_STA_TXUNDERR_Msk          (0x1U << SDMMC_STA_TXUNDERR_Pos)       /*!< 0x00000010 */
#define SDMMC_STA_TXUNDERR              SDMMC_STA_TXUNDERR_Msk                 /*!<Transmit FIFO underrun error                  */
#define SDMMC_STA_RXOVERR_Pos           (5U)
#define SDMMC_STA_RXOVERR_Msk           (0x1U << SDMMC_STA_RXOVERR_Pos)        /*!< 0x00000020 */
#define SDMMC_STA_RXOVERR               SDMMC_STA_RXOVERR_Msk                  /*!<Received FIFO overrun error                   */
#define SDMMC_STA_CMDREND_Pos           (6U)
#define SDMMC_STA_CMDREND_Msk           (0x1U << SDMMC_STA_CMDREND_Pos)        /*!< 0x00000040 */
#define SDMMC_STA_CMDREND               SDMMC_STA_CMDREND_Msk                  /*!<Command response received (CRC check passed)  */
#define SDMMC_STA_CMDSENT_Pos           (7U)
#define SDMMC_STA_CMDSENT_Msk           (0x1U << SDMMC_STA_CMDSENT_Pos)        /*!< 0x00000080 */
#define SDMMC_STA_CMDSENT               SDMMC_STA_CMDSENT_Msk                  /*!<Command sent (no response required)           */
#define SDMMC_STA_DATAEND_Pos           (8U)
#define SDMMC_STA_DATAEND_Msk           (0x1U << SDMMC_STA_DATAEND_Pos)        /*!< 0x00000100 */
#define SDMMC_STA_DATAEND               SDMMC_STA_DATAEND_Msk                  /*!<Data end (data counter, SDIDCOUNT, is zero)   */
#define SDMMC_STA_DBCKEND_Pos           (10U)
#define SDMMC_STA_DBCKEND_Msk           (0x1U << SDMMC_STA_DBCKEND_Pos)        /*!< 0x00000400 */
#define SDMMC_STA_DBCKEND               SDMMC_STA_DBCKEND_Msk                  /*!<Data block sent/received (CRC check passed)   */
#define SDMMC_STA_CMDACT_Pos            (11U)
#define SDMMC_STA_CMDACT_Msk            (0x1U << SDMMC_STA_CMDACT_Pos)         /*!< 0x00000800 */
#define SDMMC_STA_CMDACT                SDMMC_STA_CMDACT_Msk                   /*!<Command transfer in progress                  */
#define SDMMC_STA_TXACT_Pos             (12U)
#define SDMMC_STA_TXACT_Msk             (0x1U << SDMMC_STA_TXACT_Pos)          /*!< 0x00001000 */
#define SDMMC_STA_TXACT                 SDMMC_STA_TXACT_Msk                    /*!<Data transmit in progress                     */
#define SDMMC_STA_RXACT_Pos             (13U)
#define SDMMC_STA_RXACT_Msk             (0x1U << SDMMC_STA_RXACT_Pos)          /*!< 0x00002000 */
#define SDMMC_STA_RXACT                 SDMMC_STA_RXACT_Msk                    /*!<Data receive in progress                      */
#define SDMMC_STA_TXFIFOHE_Pos          (14U)
#define SDMMC_STA_TXFIFOHE_Msk          (0x1U << SDMMC_STA_TXFIFOHE_Pos)       /*!< 0x00004000 */
#define SDMMC_STA_TXFIFOHE              SDMMC_STA_TXFIFOHE_Msk                 /*!<Transmit FIFO Half Empty: at least 8 words can be written into the FIFO */
#define SDMMC_STA_RXFIFOHF_Pos          (15U)
#define SDMMC_STA_RXFIFOHF_Msk          (0x1U << SDMMC_STA_RXFIFOHF_Pos)       /*!< 0x00008000 */
#define SDMMC_STA_RXFIFOHF              SDMMC_STA_RXFIFOHF_Msk                 /*!<Receive FIFO Half Full: there are at least 8 words in the FIFO */
#define SDMMC_STA_TXFIFOF_Pos           (16U)
#define SDMMC_STA_TXFIFOF_Msk           (0x1U << SDMMC_STA_TXFIFOF_Pos)        /*!< 0x00010000 */
#define SDMMC_STA_TXFIFOF               SDMMC_STA_TXFIFOF_Msk                  /*!<Transmit FIFO full                            */
#define SDMMC_STA_RXFIFOF_Pos           (17U)
#define SDMMC_STA_RXFIFOF_Msk           (0x1U << SDMMC_STA_RXFIFOF_Pos)        /*!< 0x00020000 */
#define SDMMC_STA_RXFIFOF               SDMMC_STA_RXFIFOF_Msk                  /*!<Receive FIFO full                             */
#define SDMMC_STA_TXFIFOE_Pos           (18U)
#define SDMMC_STA_TXFIFOE_Msk           (0x1U << SDMMC_STA_TXFIFOE_Pos)        /*!< 0x00040000 */
#define SDMMC_STA_TXFIFOE               SDMMC_STA_TXFIFOE_Msk                  /*!<Transmit FIFO empty                           */
#define SDMMC_STA_RXFIFOE_Pos           (19U)
#define SDMMC_STA_RXFIFOE_Msk           (0x1U << SDMMC_STA_RXFIFOE_Pos)        /*!< 0x00080000 */
#define SDMMC_STA_RXFIFOE               SDMMC_STA_RXFIFOE_Msk                  /*!<Receive FIFO empty                            */
#define SDMMC_STA_TXDAVL_Pos            (20U)
#define SDMMC_STA_TXDAVL_Msk            (0x1U << SDMMC_STA_TXDAVL_Pos)         /*!< 0x00100000 */
#define SDMMC_STA_TXDAVL                SDMMC_STA_TXDAVL_Msk                   /*!<Data available in transmit FIFO               */
#define SDMMC_STA_RXDAVL_Pos            (21U)
#define SDMMC_STA_RXDAVL_Msk            (0x1U << SDMMC_STA_RXDAVL_Pos)         /*!< 0x00200000 */
#define SDMMC_STA_RXDAVL                SDMMC_STA_RXDAVL_Msk                   /*!<Data available in receive FIFO                */
#define SDMMC_STA_SDIOIT_Pos            (22U)
#define SDMMC_STA_SDIOIT_Msk            (0x1U << SDMMC_STA_SDIOIT_Pos)         /*!< 0x00400000 */
#define SDMMC_STA_SDIOIT                SDMMC_STA_SDIOIT_Msk                   /*!<SDMMC interrupt received                       */

/*******************  Bit definition for SDMMC_ICR register  *******************/
#define SDMMC_ICR_CCRCFAILC_Pos         (0U)
#define SDMMC_ICR_CCRCFAILC_Msk         (0x1U << SDMMC_ICR_CCRCFAILC_Pos)      /*!< 0x00000001 */
#define SDMMC_ICR_CCRCFAILC             SDMMC_ICR_CCRCFAILC_Msk                /*!<CCRCFAIL flag clear bit */
#define SDMMC_ICR_DCRCFAILC_Pos         (1U)
#define SDMMC_ICR_DCRCFAILC_Msk         (0x1U << SDMMC_ICR_DCRCFAILC_Pos)      /*!< 0x00000002 */
#define SDMMC_ICR_DCRCFAILC             SDMMC_ICR_DCRCFAILC_Msk                /*!<DCRCFAIL flag clear bit */
#define SDMMC_ICR_CTIMEOUTC_Pos         (2U)
#define SDMMC_ICR_CTIMEOUTC_Msk         (0x1U << SDMMC_ICR_CTIMEOUTC_Pos)      /*!< 0x00000004 */
#define SDMMC_ICR_CTIMEOUTC             SDMMC_ICR_CTIMEOUTC_Msk                /*!<CTIMEOUT flag clear bit */
#define SDMMC_ICR_DTIMEOUTC_Pos         (3U)
#define SDMMC_ICR_DTIMEOUTC_Msk         (0x1U << SDMMC_ICR_DTIMEOUTC_Pos)      /*!< 0x00000008 */
#define SDMMC_ICR_DTIMEOUTC             SDMMC_ICR_DTIMEOUTC_Msk                /*!<DTIMEOUT flag clear bit */
#define SDMMC_ICR_TXUNDERRC_Pos         (4U)
#define SDMMC_ICR_TXUNDERRC_Msk         (0x1U << SDMMC_ICR_TXUNDERRC_Pos)      /*!< 0x00000010 */
#define SDMMC_ICR_TXUNDERRC             SDMMC_ICR_TXUNDERRC_Msk                /*!<TXUNDERR flag clear bit */
#define SDMMC_ICR_RXOVERRC_Pos          (5U)
#define SDMMC_ICR_RXOVERRC_Msk          (0x1U << SDMMC_ICR_RXOVERRC_Pos)       /*!< 0x00000020 */
#define SDMMC_ICR_RXOVERRC              SDMMC_ICR_RXOVERRC_Msk                 /*!<RXOVERR flag clear bit  */
#define SDMMC_ICR_CMDRENDC_Pos          (6U)
#define SDMMC_ICR_CMDRENDC_Msk          (0x1U << SDMMC_ICR_CMDRENDC_Pos)       /*!< 0x00000040 */
#define SDMMC_ICR_CMDRENDC              SDMMC_ICR_CMDRENDC_Msk                 /*!<CMDREND flag clear bit  */
#define SDMMC_ICR_CMDSENTC_Pos          (7U)
#define SDMMC_ICR_CMDSENTC_Msk          (0x1U << SDMMC_ICR_CMDSENTC_Pos)       /*!< 0x00000080 */
#define SDMMC_ICR_CMDSENTC              SDMMC_ICR_CMDSENTC_Msk                 /*!<CMDSENT flag clear bit  */
#define SDMMC_ICR_DATAENDC_Pos          (8U)
#define SDMMC_ICR_DATAENDC_Msk          (0x1U << SDMMC_ICR_DATAENDC_Pos)       /*!< 0x00000100 */
#define SDMMC_ICR_DATAENDC              SDMMC_ICR_DATAENDC_Msk                 /*!<DATAEND flag clear bit  */
#define SDMMC_ICR_DBCKENDC_Pos          (10U)
#define SDMMC_ICR_DBCKENDC_Msk          (0x1U << SDMMC_ICR_DBCKENDC_Pos)       /*!< 0x00000400 */
#define SDMMC_ICR_DBCKENDC              SDMMC_ICR_DBCKENDC_Msk                 /*!<DBCKEND flag clear bit  */
#define SDMMC_ICR_SDIOITC_Pos           (22U)
#define SDMMC_ICR_SDIOITC_Msk           (0x1U << SDMMC_ICR_SDIOITC_Pos)        /*!< 0x00400000 */
#define SDMMC_ICR_SDIOITC               SDMMC_ICR_SDIOITC_Msk                  /*!<SDMMCIT flag clear bit   */

/******************  Bit definition for SDMMC_MASK register  *******************/
#define SDMMC_MASK_CCRCFAILIE_Pos       (0U)
#define SDMMC_MASK_CCRCFAILIE_Msk       (0x1U << SDMMC_MASK_CCRCFAILIE_Pos)    /*!< 0x00000001 */
#define SDMMC_MASK_CCRCFAILIE           SDMMC_MASK_CCRCFAILIE_Msk              /*!<Command CRC Fail Interrupt Enable          */
#define SDMMC_MASK_DCRCFAILIE_Pos       (1U)
#define SDMMC_MASK_DCRCFAILIE_Msk       (0x1U << SDMMC_MASK_DCRCFAILIE_Pos)    /*!< 0x00000002 */
#define SDMMC_MASK_DCRCFAILIE           SDMMC_MASK_DCRCFAILIE_Msk              /*!<Data CRC Fail Interrupt Enable             */
#define SDMMC_MASK_CTIMEOUTIE_Pos       (2U)
#define SDMMC_MASK_CTIMEOUTIE_Msk       (0x1U << SDMMC_MASK_CTIMEOUTIE_Pos)    /*!< 0x00000004 */
#define SDMMC_MASK_CTIMEOUTIE           SDMMC_MASK_CTIMEOUTIE_Msk              /*!<Command TimeOut Interrupt Enable           */
#define SDMMC_MASK_DTIMEOUTIE_Pos       (3U)
#define SDMMC_MASK_DTIMEOUTIE_Msk       (0x1U << SDMMC_MASK_DTIMEOUTIE_Pos)    /*!< 0x00000008 */
#define SDMMC_MASK_DTIMEOUTIE           SDMMC_MASK_DTIMEOUTIE_Msk              /*!<Data TimeOut Interrupt Enable              */
#define SDMMC_MASK_TXUNDERRIE_Pos       (4U)
#define SDMMC_MASK_TXUNDERRIE_Msk       (0x1U << SDMMC_MASK_TXUNDERRIE_Pos)    /*!< 0x00000010 */
#define SDMMC_MASK_TXUNDERRIE           SDMMC_MASK_TXUNDERRIE_Msk              /*!<Tx FIFO UnderRun Error Interrupt Enable    */
#define SDMMC_MASK_RXOVERRIE_Pos        (5U)
#define SDMMC_MASK_RXOVERRIE_Msk        (0x1U << SDMMC_MASK_RXOVERRIE_Pos)     /*!< 0x00000020 */
#define SDMMC_MASK_RXOVERRIE            SDMMC_MASK_RXOVERRIE_Msk               /*!<Rx FIFO OverRun Error Interrupt Enable     */
#define SDMMC_MASK_CMDRENDIE_Pos        (6U)
#define SDMMC_MASK_CMDRENDIE_Msk        (0x1U << SDMMC_MASK_CMDRENDIE_Pos)     /*!< 0x00000040 */
#define SDMMC_MASK_CMDRENDIE            SDMMC_MASK_CMDRENDIE_Msk               /*!<Command Response Received Interrupt Enable */
#define SDMMC_MASK_CMDSENTIE_Pos        (7U)
#define SDMMC_MASK_CMDSENTIE_Msk        (0x1U << SDMMC_MASK_CMDSENTIE_Pos)     /*!< 0x00000080 */
#define SDMMC_MASK_CMDSENTIE            SDMMC_MASK_CMDSENTIE_Msk               /*!<Command Sent Interrupt Enable              */
#define SDMMC_MASK_DATAENDIE_Pos        (8U)
#define SDMMC_MASK_DATAENDIE_Msk        (0x1U << SDMMC_MASK_DATAENDIE_Pos)     /*!< 0x00000100 */
#define SDMMC_MASK_DATAENDIE            SDMMC_MASK_DATAENDIE_Msk               /*!<Data End Interrupt Enable                  */
#define SDMMC_MASK_DBCKENDIE_Pos        (10U)
#define SDMMC_MASK_DBCKENDIE_Msk        (0x1U << SDMMC_MASK_DBCKENDIE_Pos)     /*!< 0x00000400 */
#define SDMMC_MASK_DBCKENDIE            SDMMC_MASK_DBCKENDIE_Msk               /*!<Data Block End Interrupt Enable            */
#define SDMMC_MASK_CMDACTIE_Pos         (11U)
#define SDMMC_MASK_CMDACTIE_Msk         (0x1U << SDMMC_MASK_CMDACTIE_Pos)      /*!< 0x00000800 */
#define SDMMC_MASK_CMDACTIE             SDMMC_MASK_CMDACTIE_Msk                /*!<CCommand Acting Interrupt Enable           */
#define SDMMC_MASK_TXACTIE_Pos          (12U)
#define SDMMC_MASK_TXACTIE_Msk          (0x1U << SDMMC_MASK_TXACTIE_Pos)       /*!< 0x00001000 */
#define SDMMC_MASK_TXACTIE              SDMMC_MASK_TXACTIE_Msk                 /*!<Data Transmit Acting Interrupt Enable      */
#define SDMMC_MASK_RXACTIE_Pos          (13U)
#define SDMMC_MASK_RXACTIE_Msk          (0x1U << SDMMC_MASK_RXACTIE_Pos)       /*!< 0x00002000 */
#define SDMMC_MASK_RXACTIE              SDMMC_MASK_RXACTIE_Msk                 /*!<Data receive acting interrupt enabled      */
#define SDMMC_MASK_TXFIFOHEIE_Pos       (14U)
#define SDMMC_MASK_TXFIFOHEIE_Msk       (0x1U << SDMMC_MASK_TXFIFOHEIE_Pos)    /*!< 0x00004000 */
#define SDMMC_MASK_TXFIFOHEIE           SDMMC_MASK_TXFIFOHEIE_Msk              /*!<Tx FIFO Half Empty interrupt Enable        */
#define SDMMC_MASK_RXFIFOHFIE_Pos       (15U)
#define SDMMC_MASK_RXFIFOHFIE_Msk       (0x1U << SDMMC_MASK_RXFIFOHFIE_Pos)    /*!< 0x00008000 */
#define SDMMC_MASK_RXFIFOHFIE           SDMMC_MASK_RXFIFOHFIE_Msk              /*!<Rx FIFO Half Full interrupt Enable         */
#define SDMMC_MASK_TXFIFOFIE_Pos        (16U)
#define SDMMC_MASK_TXFIFOFIE_Msk        (0x1U << SDMMC_MASK_TXFIFOFIE_Pos)     /*!< 0x00010000 */
#define SDMMC_MASK_TXFIFOFIE            SDMMC_MASK_TXFIFOFIE_Msk               /*!<Tx FIFO Full interrupt Enable              */
#define SDMMC_MASK_RXFIFOFIE_Pos        (17U)
#define SDMMC_MASK_RXFIFOFIE_Msk        (0x1U << SDMMC_MASK_RXFIFOFIE_Pos)     /*!< 0x00020000 */
#define SDMMC_MASK_RXFIFOFIE            SDMMC_MASK_RXFIFOFIE_Msk               /*!<Rx FIFO Full interrupt Enable              */
#define SDMMC_MASK_TXFIFOEIE_Pos        (18U)
#define SDMMC_MASK_TXFIFOEIE_Msk        (0x1U << SDMMC_MASK_TXFIFOEIE_Pos)     /*!< 0x00040000 */
#define SDMMC_MASK_TXFIFOEIE            SDMMC_MASK_TXFIFOEIE_Msk               /*!<Tx FIFO Empty interrupt Enable             */
#define SDMMC_MASK_RXFIFOEIE_Pos        (19U)
#define SDMMC_MASK_RXFIFOEIE_Msk        (0x1U << SDMMC_MASK_RXFIFOEIE_Pos)     /*!< 0x00080000 */
#define SDMMC_MASK_RXFIFOEIE            SDMMC_MASK_RXFIFOEIE_Msk               /*!<Rx FIFO Empty interrupt Enable             */
#define SDMMC_MASK_TXDAVLIE_Pos         (20U)
#define SDMMC_MASK_TXDAVLIE_Msk         (0x1U << SDMMC_MASK_TXDAVLIE_Pos)      /*!< 0x00100000 */
#define SDMMC_MASK_TXDAVLIE             SDMMC_MASK_TXDAVLIE_Msk                /*!<Data available in Tx FIFO interrupt Enable */
#define SDMMC_MASK_RXDAVLIE_Pos         (21U)
#define SDMMC_MASK_RXDAVLIE_Msk         (0x1U << SDMMC_MASK_RXDAVLIE_Pos)      /*!< 0x00200000 */
#define SDMMC_MASK_RXDAVLIE             SDMMC_MASK_RXDAVLIE_Msk                /*!<Data available in Rx FIFO interrupt Enable */
#define SDMMC_MASK_SDIOITIE_Pos         (22U)
#define SDMMC_MASK_SDIOITIE_Msk         (0x1U << SDMMC_MASK_SDIOITIE_Pos)      /*!< 0x00400000 */
#define SDMMC_MASK_SDIOITIE             SDMMC_MASK_SDIOITIE_Msk                /*!<SDMMC Mode Interrupt Received interrupt Enable */

/*****************  Bit definition for SDMMC_FIFOCNT register  *****************/
#define SDMMC_FIFOCNT_FIFOCOUNT_Pos     (0U)
#define SDMMC_FIFOCNT_FIFOCOUNT_Msk     (0xFFFFFFU << SDMMC_FIFOCNT_FIFOCOUNT_Pos) /*!< 0x00FFFFFF */
#define SDMMC_FIFOCNT_FIFOCOUNT         SDMMC_FIFOCNT_FIFOCOUNT_Msk            /*!<Remaining number of words to be written to or read from the FIFO */

/******************  Bit definition for SDMMC_FIFO register  *******************/
#define SDMMC_FIFO_FIFODATA_Pos         (0U)
#define SDMMC_FIFO_FIFODATA_Msk         (0xFFFFFFFFU << SDMMC_FIFO_FIFODATA_Pos) /*!< 0xFFFFFFFF */
#define SDMMC_FIFO_FIFODATA             SDMMC_FIFO_FIFODATA_Msk                /*!<Receive and transmit FIFO data */

#endif	// UNIT_TEST_INCLUDE_MOCKS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_DISTORTOS_CHIP_STM32_SDMMCV1_SDMMCPERIPHERAL_HPP_
