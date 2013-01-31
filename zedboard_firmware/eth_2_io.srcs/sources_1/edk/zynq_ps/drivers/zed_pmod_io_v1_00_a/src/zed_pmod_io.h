/*****************************************************************************
* Filename:          C:\Xilinx\projects\eth_2_io\eth_2_io.srcs\sources_1\edk\zynq_ps/drivers/zed_pmod_io_v1_00_a/src/zed_pmod_io.h
* Version:           1.00.a
* Description:       zed_pmod_io Driver Header File
* Date:              Thu Jan 24 22:08:21 2013 (by Create and Import Peripheral Wizard)
*****************************************************************************/

#ifndef ZED_PMOD_IO_H
#define ZED_PMOD_IO_H

/***************************** Include Files *******************************/

#include "xbasic_types.h"
#include "xstatus.h"
#include "xil_io.h"

/************************** Constant Definitions ***************************/


/**
 * User Logic Slave Space Offsets
 * -- SLV_REG0 : user logic slave module register 0
 */
#define ZED_PMOD_IO_USER_SLV_SPACE_OFFSET (0x00000000)
#define ZED_PMOD_IO_SLV_REG0_OFFSET (ZED_PMOD_IO_USER_SLV_SPACE_OFFSET + 0x00000000)

/**************************** Type Definitions *****************************/


/***************** Macros (Inline Functions) Definitions *******************/

/**
 *
 * Write a value to a ZED_PMOD_IO register. A 32 bit write is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is written.
 *
 * @param   BaseAddress is the base address of the ZED_PMOD_IO device.
 * @param   RegOffset is the register offset from the base to write to.
 * @param   Data is the data written to the register.
 *
 * @return  None.
 *
 * @note
 * C-style signature:
 * 	void ZED_PMOD_IO_mWriteReg(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Data)
 *
 */
#define ZED_PMOD_IO_mWriteReg(BaseAddress, RegOffset, Data) \
 	Xil_Out32((BaseAddress) + (RegOffset), (Xuint32)(Data))

/**
 *
 * Read a value from a ZED_PMOD_IO register. A 32 bit read is performed.
 * If the component is implemented in a smaller width, only the least
 * significant data is read from the register. The most significant data
 * will be read as 0.
 *
 * @param   BaseAddress is the base address of the ZED_PMOD_IO device.
 * @param   RegOffset is the register offset from the base to write to.
 *
 * @return  Data is the data from the register.
 *
 * @note
 * C-style signature:
 * 	Xuint32 ZED_PMOD_IO_mReadReg(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define ZED_PMOD_IO_mReadReg(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (RegOffset))


/**
 *
 * Write/Read 32 bit value to/from ZED_PMOD_IO user logic slave registers.
 *
 * @param   BaseAddress is the base address of the ZED_PMOD_IO device.
 * @param   RegOffset is the offset from the slave register to write to or read from.
 * @param   Value is the data written to the register.
 *
 * @return  Data is the data from the user logic slave register.
 *
 * @note
 * C-style signature:
 * 	void ZED_PMOD_IO_mWriteSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset, Xuint32 Value)
 * 	Xuint32 ZED_PMOD_IO_mReadSlaveRegn(Xuint32 BaseAddress, unsigned RegOffset)
 *
 */
#define ZED_PMOD_IO_mWriteSlaveReg0(BaseAddress, RegOffset, Value) \
 	Xil_Out32((BaseAddress) + (ZED_PMOD_IO_SLV_REG0_OFFSET) + (RegOffset), (Xuint32)(Value))

#define ZED_PMOD_IO_mReadSlaveReg0(BaseAddress, RegOffset) \
 	Xil_In32((BaseAddress) + (ZED_PMOD_IO_SLV_REG0_OFFSET) + (RegOffset))

/************************** Function Prototypes ****************************/


/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the ZED_PMOD_IO instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus ZED_PMOD_IO_SelfTest(void * baseaddr_p);
/**
*  Defines the number of registers available for read and write*/
#define TEST_AXI_LITE_USER_NUM_REG 1


#endif /** ZED_PMOD_IO_H */
