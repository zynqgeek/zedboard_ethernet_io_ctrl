/******************************************************************************
*
* (c) Copyright 2011-2012 Xilinx, Inc. All rights reserved.
*
* This file contains confidential and proprietary information of Xilinx, Inc.
* and is protected under U.S. and international copyright and other
* intellectual property laws.
*
* DISCLAIMER
* This disclaimer is not a license and does not grant any rights to the
* materials distributed herewith. Except as otherwise provided in a valid
* license issued to you by Xilinx, and to the maximum extent permitted by
* applicable law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND WITH ALL
* FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES AND CONDITIONS, EXPRESS,
* IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
* MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE;
* and (2) Xilinx shall not be liable (whether in contract or tort, including
* negligence, or under any other theory of liability) for any loss or damage
* of any kind or nature related to, arising under or in connection with these
* materials, including for any direct, or any indirect, special, incidental,
* or consequential loss or damage (including loss of data, profits, goodwill,
* or any type of loss or damage suffered as a result of any action brought by
* a third party) even if such damage or loss was reasonably foreseeable or
* Xilinx had been advised of the possibility of the same.
*
* CRITICAL APPLICATIONS
* Xilinx products are not designed or intended to be fail-safe, or for use in
* any application requiring fail-safe performance, such as life-support or
* safety devices or systems, Class III medical devices, nuclear facilities,
* applications related to the deployment of airbags, or any other applications
* that could lead to death, personal injury, or severe property or
* environmental damage (individually and collectively, "Critical
* Applications"). Customer assumes the sole risk and liability of any use of
* Xilinx products in Critical Applications, subject only to applicable laws
* and regulations governing limitations on product liability.
*
* THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE
* AT ALL TIMES.
*
*******************************************************************************/
/*****************************************************************************/
/**
*
* @file main.c
*
* The main file for the First Stage Boot Loader (FSBL).
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver	Who	Date		Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a jz	06/04/11	Initial release
* 2.00a mb	25/05/12	standalone based FSBL
* 3.00a np/mb	08/03/12	Added call to FSBL user hook - before handoff.
*				DDR ECC initialization added
* 				fsbl print with verbose added
* 				Performance measurement added
* 				Flushed the UART Tx buffer
* 				Added the performance time for ECC DDR init
* 				Added clearing of ECC Error Code
* 				Added the watchdog timer value
*
*
* </pre>
*
* @note
* FSBL runs from OCM, Based on the bootmode selected, FSBL will copy
* the partitions from the flashdevice. If the partition is bitstream then
* the bitstream is programmed in the Fabric and for an partition that is
* an application , FSBL will copy the application into DDR and does a
* handoff.The application should not be starting at the OCM address,
* FSBL does not remap the OCM. Application should use DDR starting from 1MB
*
* FSBL can be stitched along with bitstream and application using bootgen
*
* Refer to fsbl.h file for details on the compilation flags supported in FSBL
*
******************************************************************************/

/***************************** Include Files *********************************/

#include "fsbl.h"
#include "qspi.h"
#include "nand.h"
#include "nor.h"
#include "sd.h"
#include "pcap.h"
#include "image_mover.h"
#include "xparameters.h"
#include "xil_cache.h"
#include "xil_exception.h"
#include "xstatus.h"

#ifdef XPAR_XWDTPS_0_BASEADDR
#include "xwdtps.h"
#endif
#ifdef STDOUT_BASEADDRESS
#include "xuartps_hw.h"
#endif
/************************** Constant Definitions *****************************/


/* Reboot status register defines:
 * 0xF0000000 for FSBL fallback mask to notify Boot Rom
 * 0x60000000 for FSBL to mark that FSBL has not handoff yet
 * 0x0F000000 for FSBL to record partition number to work on
 * 0x00FFFFFF for user application to use across soft reset
 */
#define FSBL_FAIL_MASK		0xF0000000
#define FSBL_IN_MASK		0x60000000
#define PARTITION_NUMBER_MASK	0x0F000000

#define PARTITION_NUMBER_SHIFT	24
#define MAX_PARTITION_NUMBER	0xE

/* The address that holds the base address for the image Boot ROM found */
#define BASEADDR_HOLDER		0xFFFFFFF8

#ifdef XPAR_XWDTPS_0_BASEADDR
#define WDT_DEVICE_ID		XPAR_XWDTPS_0_DEVICE_ID
#define WDT_EXPIRE_TIME		10
#define WDT_CRV_SHIFT		12
#endif

/**************************** Type Definitions *******************************/

/***************** Macros (Inline Functions) Definitions *********************/

#ifdef XPAR_XWDTPS_0_BASEADDR
XWdtPs Watchdog;		/* Instance of WatchDog Timer	*/
#endif
/************************** Function Prototypes ******************************/
extern void FsblHandoffExit(u32 FsblStartAddr);
extern void FsblHandoffJtagExit(void);
extern void init_ddr(void);
extern int ps7_init();

u32 FsblHookBeforeHandoff(void);
void MarkFSBLIn(void);
void FsblHandoff(u32 FsblStartAddr);
u32 GetResetReason(void);

static void Update_MultiBootRegister(void);
/* Exception handlers */
static void RegisterHandlers(void);
static void Undef_Handler (void);
static void SVC_Handler (void);
static void PreFetch_Abort_Handler (void);
static void Data_Abort_Handler (void);
static void IRQ_Handler (void);
static void FIQ_Handler (void);

void CheckSRST(void);

#ifdef ECC_ENABLE
int DDREcc_Init(void);
void DDRClearEccError(void);
#endif

#ifdef XPAR_XWDTPS_0_BASEADDR
int InitWatchDog(void);
u32 ConvertTime_WdtCounter(u32 seconds);
void  CheckWDTReset(void);
#endif

/************************** Variable Definitions *****************************/
int SkipPartition;
/* Base Address for the Read Functionality for Image Processing */
u32 FlashReadBaseAddress = 0;
/* Silicon Version */
u32 Silicon_Version;

extern ImageMoverType MoveImage;
extern XDcfg *DcfgInstPtr;

/*****************************************************************************/
/**
*
* This is the main function for the FSBL ROM code.
*
*
* @param	None.
*
* @return
*		- XST_SUCCESS to indicate success
*		- XST_FAILURE.to indicate failure
*
* @note
*
****************************************************************************/
int main(void)
{
#ifdef XPAR_PS7_DDR_0_S_AXI_BASEADDR
	u32 BootModeRegister = 0;
	u32 HandoffAddress;
	volatile u32 RebootStatusRegister = 0;
	volatile u32 MultiBootReg = 0;
	u32 ImageStartAddress = 0;
	u32 PartitionNumber = 0;
	u32 Status = XST_SUCCESS;
#endif

#ifdef PEEP_CODE
	/* initialize the DDR for application code */
	init_ddr();
#else
	/* PCW initialization for MIO,PLL,CLK and DDR */
	ps7_init();
#endif
	/* Unlock the SLCR */
	SlcrUnlock();

	/* If Performance measurement is required 
	 * then read the Global Timer value , Please note that the
	 * time taken for mio, clock and ddr initialisation
	 * done in the ps7_init function is not accounted in the FSBL
	 *
	 */
#ifdef FSBL_PERF
	XTime tCur = 0;
	FsblGetGlobalTime(tCur);
#endif

	/* Flush the Caches */
	Xil_DCacheFlush();

	/* Disable Data Cache */
	Xil_DCacheDisable();

	/* Register the Exception handlers */
	RegisterHandlers();
	
	/* Print the FSBL Banner */
	fsbl_printf(DEBUG_GENERAL,"\n\rXilinx First Stage Boot Loader \n\r");
	fsbl_printf(DEBUG_GENERAL,"Release 14.3 %s-%s\r\n",__DATE__,__TIME__);

#ifdef XPAR_PS7_DDR_0_S_AXI_BASEADDR
	/* check if soft reset occured */
	CheckSRST();
	/* Pcap initialization here */
	Status = InitPcap();
	if (Status == XST_FAILURE) {
		fsbl_printf(DEBUG_GENERAL,"PCAP_INIT_FAIL \n\r");
		/* Error Handling here */
		OutputStatus(PCAP_INIT_FAIL);
		FsblFallback();
	}
	fsbl_printf(DEBUG_INFO,"Devcfg driver initialized \r\n");
	/* Get the Silicon Version */
	GetSiliconVersion();

#ifdef XPAR_XWDTPS_0_BASEADDR
	/* Check if WDT Reset has occured or not */
	CheckWDTReset();

	 /* Initialize the Watchdog Timer so that it is ready to use */
	Status = InitWatchDog();
	if (Status == XST_FAILURE) {
		fsbl_printf(DEBUG_GENERAL,"WATCHDOG_INIT_FAIL \n\r");
		/* Error Handling here */
		OutputStatus(WDT_INIT_FAIL);
		FsblFallback();
	}
	fsbl_printf(DEBUG_INFO,"Watchdog driver initialized \r\n");
#endif
	MarkFSBLIn();

#ifdef ECC_ENABLE
	Status = DDREcc_Init();
	if (Status == XST_FAILURE) {
		fsbl_printf(DEBUG_GENERAL,"DDR_ECC_INIT_FAIL \r\n");
		/* Error Handling here */
		OutputStatus(DDR_ECC_INIT_FAIL);
		FsblFallback();
	}
	fsbl_printf(DEBUG_GENERAL,"DDR Init done for ECC\r\n");
#endif

#ifndef FSBL_PERF
	Status = Check_ddr_init();
	if (Status == XST_FAILURE) {
		fsbl_printf(DEBUG_GENERAL,"DDR_INIT_FAIL \r\n");
		/* Error Handling here */
		OutputStatus(DDR_INIT_FAIL);
		FsblFallback();
	}
#endif
	/* Read bootmode register */
	BootModeRegister = FsblIn32(BOOT_MODE_REG);
	BootModeRegister &= BOOT_MODES_MASK;

	/* QSPI BOOT MODE */
#ifdef XPAR_PS7_QSPI_LINEAR_0_S_AXI_BASEADDR
	if (BootModeRegister == QSPI_MODE) {
		fsbl_printf(DEBUG_GENERAL,"Boot mode is QSPI\n\r");

		InitQspi();

		MoveImage = QspiAccess;
		FlashReadBaseAddress = XPS_QSPI_LINEAR_BASEADDR;
		fsbl_printf(DEBUG_INFO,"QSPI Init Done %x\r\n", FlashReadBaseAddress);
	} else
#endif
	/* NAND BOOT MODE */
#ifdef XPAR_PS7_NAND_0_BASEADDR
	if (BootModeRegister == NAND_FLASH_MODE) {
		/*
	 	* Boot ROM always initialize the nand at lower speed
	 	* This is the chance to put it to an optimum speed for your nand
	 	* device
	 	*/
		fsbl_printf(DEBUG_GENERAL,"Boot mode is NAND\n");

		Status = InitNand();
		if (Status != XST_SUCCESS) {
			fsbl_printf(DEBUG_GENERAL,"NAND_INIT_FAIL \r\n");
			/* Error Handling here */
			OutputStatus(NAND_INIT_FAIL);
			FsblFallback();
		}
		MoveImage = NandAccess;
		fsbl_printf(DEBUG_INFO,"NAND Init Done \r\n");
	} else
#endif
	/* NOR BOOT MODE */
	if (BootModeRegister == NOR_FLASH_MODE) {
		fsbl_printf(DEBUG_GENERAL,"Boot mode is NOR\n\r");
		/*
		 * Boot ROM always initialize the nor at lower speed
		 * This is the chance to put it to an optimum speed for your nor
		 * device
		 */
		InitNor();
		fsbl_printf(DEBUG_INFO,"NOR Init Done \r\n");
		MoveImage = NorAccess;

	} else
	/* SD BOOT MODE */
#ifdef XPAR_PS7_SD_0_S_AXI_BASEADDR
	if (BootModeRegister == SD_MODE) {
		fsbl_printf(DEBUG_GENERAL,"Boot mode is SD\r\n");
		Status = InitSD("BOOT.BIN");
		/* returns file open error or success */
		if (Status != XST_SUCCESS){
			fsbl_printf(DEBUG_GENERAL,"SD_INIT_FAIL\r\n");
			OutputStatus(SD_INIT_FAIL);
			FsblFallback();
		}
		MoveImage = SDAccess;
		fsbl_printf(DEBUG_INFO,"SD Init Done \r\n");
	} else
#endif
	/* JTAG  BOOT MODE */
	if (BootModeRegister == JTAG_MODE) {
		fsbl_printf(DEBUG_GENERAL,"Boot mode is JTAG\r\n");
		/* Stop the Watchdog before JTAG handoff */
#ifdef	XPAR_XWDTPS_0_BASEADDR
		XWdtPs_Stop(&Watchdog);
#endif
		/* Clear our mark in reboot status register */
		ClearFSBLIn();
		FsblHandoffJtagExit();
	} else {
		fsbl_printf(DEBUG_GENERAL,"ILLEGAL_BOOT_MODE \r\n");
		OutputStatus(ILLEGAL_BOOT_MODE);
		/* fallback fsbl starts, no return */
		FsblFallback();
	}
	fsbl_printf(DEBUG_INFO,"Flash BaseAddress %x\r\n", FlashReadBaseAddress);

	if ((FlashReadBaseAddress != XPS_QSPI_LINEAR_BASEADDR) &&
			(FlashReadBaseAddress != XPS_NAND_BASEADDR) &&
			(FlashReadBaseAddress != XPS_NOR_BASEADDR) &&
			(FlashReadBaseAddress != XPS_SDIO0_BASEADDR)) {
		fsbl_printf(DEBUG_GENERAL,"INVALID_FLASH_ADDRESS \r\n");
		OutputStatus(INVALID_FLASH_ADDRESS);
		FsblFallback();
	}

	/* Partition walk
	 * If partition is bitstream, then no reset is necessary.
	 * SkipPartition is to the next partition without soft reset
	 */

	SkipPartition = 1;

	while (SkipPartition) {
		RebootStatusRegister = FsblIn32(REBOOT_STATUS_REG);
		fsbl_printf(DEBUG_INFO,
				"Reboot status register 0x%x \r\n",RebootStatusRegister);
#ifdef	XPAR_XWDTPS_0_BASEADDR
		/* Prevent WDT reset */
		XWdtPs_RestartWdt(&Watchdog);
#endif
		/* Clear out fallback mask from previous run
		 * We start from the first partition again
		 */

		if ((RebootStatusRegister & FSBL_FAIL_MASK) ==
 				FSBL_FAIL_MASK) {
			fsbl_printf(DEBUG_INFO,
					"Reboot status shows previous run falls back\r\n");
			RebootStatusRegister &= ~(FSBL_FAIL_MASK | PARTITION_NUMBER_MASK);
			FsblOut32(REBOOT_STATUS_REG,
					RebootStatusRegister &
					~(FSBL_FAIL_MASK | PARTITION_NUMBER_MASK));
		}
		if (Silicon_Version == SILICON_VERSION_0) {
			/* Read the image start address */
			ImageStartAddress = *(u32 *)BASEADDR_HOLDER;
		} else {
			/* read the mulitboot register */
			MultiBootReg =  XDcfg_ReadReg(DcfgInstPtr->Config.BaseAddr,
					XDCFG_MULTIBOOT_ADDR_OFFSET);
			fsbl_printf(DEBUG_INFO,"Multiboot Register = %08x\r\n",MultiBootReg);
			/* Compute the image start address */
			ImageStartAddress = (MultiBootReg & PCAP_MBOOT_REG_REBOOT_OFFSET_MASK)
									* GOLDEN_IMAGE_OFFSET;
		}
		/* Read Partition number */
		PartitionNumber = (RebootStatusRegister & PARTITION_NUMBER_MASK)
						>> PARTITION_NUMBER_SHIFT;

		fsbl_printf(DEBUG_INFO,"ImageStartAddress = %08x\r\n",ImageStartAddress);
		fsbl_printf(DEBUG_INFO,"PartitionNumber = %08x\r\n",PartitionNumber);
		fsbl_printf(DEBUG_INFO,"flash read base addr %x, image base %x\r\n",
				FlashReadBaseAddress, ImageStartAddress);

		SkipPartition = 0;
		HandoffAddress = PartitionMove(ImageStartAddress, PartitionNumber);

		if (SkipPartition) {
			fsbl_printf(DEBUG_INFO,"FSBL main: Skip partition\r\n");
			continue;
		}
		/* Image move encounters errors, fallback
		 * Error code has been printed in image mover */
		if (HandoffAddress == MOVE_IMAGE_FAIL) {
			fsbl_printf(DEBUG_GENERAL,"FSBL FALLBACK\r\n");
		/* fallback starts, no return */
			FsblFallback();
		}
	} /* end of while SkipPartition */

	/* FSBL user hook call before handoff to the application */
	Status = FsblHookBeforeHandoff();

	if (Status != XST_SUCCESS) {
		fsbl_printf(DEBUG_GENERAL,"FSBL_HANDOFF_HOOK_FAIL\r\n");
 		/* Error Handling here */
		OutputStatus(FSBL_HANDOFF_HOOK_FAIL);
		FsblFallback();
	}

#ifdef XPAR_PS7_SD_0_S_AXI_BASEADDR
	/* If using SD, close the file */
	if (BootModeRegister == SD_MODE) {
		ReleaseSD();
	}
#endif
#ifdef XPAR_XWDTPS_0_BASEADDR
	XWdtPs_Stop(&Watchdog);
#endif

	/* For Performance measurement */
#ifdef FSBL_PERF
	XTime tEnd = 0;
	fsbl_printf(DEBUG_GENERAL,"Total Execution time is ");
	FsblMeasurePerfTime(tCur,tEnd);
#endif

	FsblHandoff(HandoffAddress);
	/* Should not come over here */
	OutputStatus(ILLEGAL_RETURN);
	FsblFallback();
#else
	OutputStatus(NO_DDR);
	FsblFallback();
#endif
	return Status;
} /* end of main */

/******************************************************************************/
/**
*
* This function reset the CPU and goes for Boot ROM fallback handling
*
* @param	None
*
* @return	None
*
* @note		None
*
****************************************************************************/
void FsblFallback(void)
{
	volatile u32 RebootStatusReg = FsblIn32(REBOOT_STATUS_REG);

	/* update the Multiboot Register for Golden search hunt */
	Update_MultiBootRegister();
	/* Notify Boot ROM something is wrong */
	RebootStatusReg =  FsblIn32(REBOOT_STATUS_REG);
	/* Set the FSBL Fail mask */
	FsblOut32(REBOOT_STATUS_REG, RebootStatusReg | FSBL_FAIL_MASK);
	/* Barrier */
		asm(
			"dsb\n\t"
			"isb"
		);
	/* Reset PS, so Boot ROM will restart */
	FsblOut32(PS_RST_CTRL_REG, PS_RST_MASK);
}


/******************************************************************************/
/**
* This function set the address for the next partition
*
* @param 	Num is the partition number for the next partition,
		it should be less than 15
*
* @return
*	. 	- XST_SUCCESS if everything works
*	. 	- XST_INVALID_PARAM if partition number is more than 15
*
****************************************************************************/
int FsblSetNextPartition(int Num) 
{
	u32 RebootStatusReg;
	u32 ResetReg;

	/* Address must be word-aligned */
	if (Num > MAX_PARTITION_NUMBER) {
		fsbl_printf(DEBUG_GENERAL,"TOO MANY PARTITIONS %d \n",
			MAX_PARTITION_NUMBER);
	/*
	 * Let the caller decide what to do next
	 * If they continue with its partition walk, the last partition
	 * will be repeated
	 */
		return XST_INVALID_PARAM;
	}

	RebootStatusReg = FsblIn32(REBOOT_STATUS_REG);
	ResetReg = RebootStatusReg & ~PARTITION_NUMBER_MASK;

	FsblOut32(REBOOT_STATUS_REG, ResetReg |
		 (Num << PARTITION_NUMBER_SHIFT));

	return XST_SUCCESS;
} /* End of FsblSetNextPartition */

/******************************************************************************/
/**
*
* This function hands the A9/PS to the loaded user code.
*
* @param	none
*
* @return	none
*
* @note		This function does not return.
*
****************************************************************************/

void FsblHandoff(u32 FsblStartAddr) {
	fsbl_printf(DEBUG_GENERAL,"SUCCESSFUL_HANDOFF\r\n");
	OutputStatus(SUCCESSFUL_HANDOFF);
	/* Clear our mark in reboot status register */
	ClearFSBLIn();

	SlcrLock();
	FsblHandoffExit(FsblStartAddr);

	OutputStatus(ILLEGAL_RETURN);
	FsblFallback();
} /* End of FsblHandoff */

/******************************************************************************/
/**
*
* This function outputs the status for the provided State in the boot process.
*
* @param	State is where in the boot process the output is desired.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void OutputStatus(u32 State)
{
#ifdef STDOUT_BASEADDRESS
	u32 UartReg = 0;

	fsbl_printf(DEBUG_GENERAL,"  FSBLStatus = 0x%.4x\r\n", State);
	/* The TX buffer needs to be flushed out
	 * If this is not done some of the prints will not appear on the
	 * serial output
	 * */
	UartReg = FsblIn32(XPS_UART1_BASEADDR + XUARTPS_SR_OFFSET);
	while ((UartReg & XUARTPS_SR_TXEMPTY) != XUARTPS_SR_TXEMPTY) {
		UartReg = FsblIn32(XPS_UART1_BASEADDR + XUARTPS_SR_OFFSET);
	}
#endif
} /* end of OutputStatus */

/******************************************************************************/
/**
*
* This function handles the error and lockdown processing and outputs the status
* for the provided State in the boot process.
*
* This function is called upon exceptions.
*
* @param	State - where in the boot process the error occured.
*
* @return	None.
*
* @note		This function does not return, the PS block is reset
*
****************************************************************************/
void ErrorLockdown(u32 State) 
{
	OutputStatus(State);

	FsblFallback();

} /* End of ErrorLockdown */

/******************************************************************************/
/**
*
* This function copies a memory region to another memory region
*
* @param 	s1 is starting address for destination
* @param 	s2 is starting address for the source
* @param 	n is the number of bytes to copy
*
* @return	Starting address for destination
*
****************************************************************************/
void *(memcpy_rom)(void * s1, const void * s2, u32 n)
{
	char *dst = (char *)s1;
	const char *src = (char *)s2;

	/* Loop and copy.	*/
	while (n-- != 0)
		*dst++ = *src++;
	return s1;
}
/******************************************************************************/
/**
*
* This function copies a string to another, the source string must be null-
* terminated.
*
* @param 	Dest is starting address for the destination string
* @param 	Src is starting address for the source string
*
* @return	Starting address for the destination string
*
****************************************************************************/
char *strcpy_rom(char *Dest, const char *Src)
{
	unsigned i;
	for (i=0; Src[i] != '\0'; ++i)
		Dest[i] = Src[i];
	Dest[i] = '\0';
	return Dest;
}



/******************************************************************************/
/**
*
* This function sets FSBL is running mask in reboot status register
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void MarkFSBLIn(void) {

	FsblOut32(REBOOT_STATUS_REG,
		FsblIn32(REBOOT_STATUS_REG) | FSBL_IN_MASK);
} /* End of MarkFSBLIn */

/******************************************************************************/
/**
*
* This function clears FSBL is running mask in reboot status register
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
void ClearFSBLIn(void) 
{
	FsblOut32(REBOOT_STATUS_REG,
		(FsblIn32(REBOOT_STATUS_REG)) &
		~(FSBL_FAIL_MASK | PARTITION_NUMBER_MASK));
} /* End of ClearFSBLIn */

/******************************************************************************/
/**
*
* This function Registers the Exception Handlers
*
* @param	None.
*
* @return	None.
*
* @note		None.
*
****************************************************************************/
static void RegisterHandlers(void) 
{

	Xil_ExceptionInit();

	 /*
	 * Initialize the vector table. Register the stub Handler for each
	 * exception.
	 */

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_UNDEFINED_INT,
					(Xil_ExceptionHandler)Undef_Handler,
					(void *) 0);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_SWI_INT,
					(Xil_ExceptionHandler)SVC_Handler,
					(void *) 0);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_PREFETCH_ABORT_INT,
				(Xil_ExceptionHandler)PreFetch_Abort_Handler,
				(void *) 0);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_DATA_ABORT_INT,
				(Xil_ExceptionHandler)Data_Abort_Handler,
				(void *) 0);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)IRQ_Handler,(void *) 0);
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_FIQ_INT,
			(Xil_ExceptionHandler)FIQ_Handler,(void *) 0);

	Xil_ExceptionEnable();

}/* end of Register Handlers*/

static void Undef_Handler (void)
{
	fsbl_printf(DEBUG_GENERAL,"UNDEFINED_HANDLER\r\n");
	ErrorLockdown (EXCEPTION_ID_UNDEFINED_INT);
}

static void SVC_Handler (void)
{
	fsbl_printf(DEBUG_GENERAL,"SVC_HANDLER \r\n");
	ErrorLockdown (EXCEPTION_ID_SWI_INT);
}

static void PreFetch_Abort_Handler (void)
{
	fsbl_printf(DEBUG_GENERAL,"PREFETCH_ABORT_HANDLER \r\n");
	ErrorLockdown (EXCEPTION_ID_PREFETCH_ABORT_INT);
}

static void Data_Abort_Handler (void)
{
	fsbl_printf(DEBUG_GENERAL,"DATA_ABORT_HANDLER \r\n");
	ErrorLockdown (EXCEPTION_ID_DATA_ABORT_INT);
}

static void IRQ_Handler (void)
{
	fsbl_printf(DEBUG_GENERAL,"IRQ_HANDLER \r\n");
	ErrorLockdown (EXCEPTION_ID_IRQ_INT);
}

static void FIQ_Handler (void)
{
	fsbl_printf(DEBUG_GENERAL,"FIQ_HANDLER \r\n");
	ErrorLockdown (EXCEPTION_ID_FIQ_INT);
}
/******************************************************************************/
/**
*
* This function Checks for the ddr initialization completion
*
* @param	None.
*
* @return
*		- XST_SUCCESS if the initialization is successful
*		- XST_FAILURE if the  initialization is NOT successful
*
* @note		None.
*
****************************************************************************/
int Check_ddr_init(void)
{

	unsigned int reg;

	/* Write and Read from the DDR location for sanity checks */
	Xil_Out32(DDR_MEMORY_1, PATTERN);
	reg = FsblIn32(DDR_MEMORY_1);
	if (reg != PATTERN)
		return XST_FAILURE;

	/* Write and Read from the DDR location for sanity checks */
	Xil_Out32(DDR_MEMORY_2, PATTERN);
	reg = FsblIn32(DDR_MEMORY_2);
	if (reg != PATTERN)
		return XST_FAILURE;

	return XST_SUCCESS;
}

/******************************************************************************/
/**
*
* This function Updates the Multi boot Register to enable golden image
* search for boot rom
*
* @param None
*
* @return
* return  none
*
****************************************************************************/

static void Update_MultiBootRegister(void)
{
	u32 ImageStartAddress = 0;
	volatile u32 MultiBootReg = 0;

	/* Get the Image base Address */
	if (Silicon_Version == SILICON_VERSION_0) {
		/* Read the image start address */
		ImageStartAddress = *(u32 *)BASEADDR_HOLDER;
	} else {
		/* read the mulitboot register */
		MultiBootReg =	XDcfg_ReadReg(DcfgInstPtr->Config.BaseAddr,
					XDCFG_MULTIBOOT_ADDR_OFFSET);
		fsbl_printf(DEBUG_INFO,"Multiboot Register = %08x\r\n",
				MultiBootReg);
		/* Compute the image start address */
		ImageStartAddress = (MultiBootReg &
				PCAP_MBOOT_REG_REBOOT_OFFSET_MASK) * GOLDEN_IMAGE_OFFSET;
	}


	/* Should not return here, if it ever does, it is an error */
	if (Silicon_Version != SILICON_VERSION_0) {
		/* Increment the offset address looking for the ID */
		ImageStartAddress += GOLDEN_IMAGE_OFFSET;
		MultiBootReg &= ~PCAP_MBOOT_REG_REBOOT_OFFSET_MASK;
		MultiBootReg |=  (ImageStartAddress/GOLDEN_IMAGE_OFFSET)
			& PCAP_MBOOT_REG_REBOOT_OFFSET_MASK;

		XDcfg_WriteReg(DcfgInstPtr->Config.BaseAddr,
				XDCFG_MULTIBOOT_ADDR_OFFSET,
				MultiBootReg);
		fsbl_printf(DEBUG_INFO,"Updated MultiBootReg = 0x%08x\r\n",
				MultiBootReg);
	} /* End of Silicon_Version */

} /* End of Update_MultiBootRegister */

/******************************************************************************
*
* This function reset the CPU and goes for Boot ROM fallback handling
*
* @param	None
*
* @return	None
*
* @note		None
*
*******************************************************************************/

u32 GetResetReason(void)
{
	volatile u32 Regval;

	/* We are using REBOOT_STATUS_REG, we have to use bits 23:16 */
	/* for storing the RESET_REASON register value*/
	Regval = ((FsblIn32(REBOOT_STATUS_REG) >> 16) & 0xFF);

	return Regval;

}
/******************************************************************************
*
* This function initializes the entire DDR for ECC
*
* @param	None
*
* @return
*		- XST_SUCCESS if the DDR is initialized for ECC
*		- XST_FAILURE if DDR initialization fails
*
* @note		None
*
*******************************************************************************/
#ifdef ECC_ENABLE
int DDREcc_Init(void)
{

	int Status = XST_SUCCESS;
	u32 PcapReg;

#ifdef FSBL_PERF
	XTime tEccCurrent = 0;
	FsblGetGlobalTime(tEccCurrent);
#endif

#ifdef	XPAR_XWDTPS_0_BASEADDR
	/* Prevent WDT reset */
	XWdtPs_RestartWdt(&Watchdog);
#endif
	/* Set the loopback bit in the devcfg */
	fsbl_printf(DEBUG_INFO,"Set the loopback bit \r \n");
	XDcfg_SetMiscControlRegister(DcfgInstPtr,
			XDCFG_MCTRL_PCAP_LPBK_MASK);

	fsbl_printf(DEBUG_INFO,"PCAP MCTRL %x: %08x\r\n",
					XPS_DEV_CFG_APB_BASEADDR + XDCFG_MCTRL_OFFSET,
					FsblIn32(XPS_DEV_CFG_APB_BASEADDR + XDCFG_MCTRL_OFFSET));

	/* Start the Transfer */
	Status = XDcfg_Transfer(DcfgInstPtr,
						(u8 *)(SRCADDR_OCM | PCAP_LAST_TRANSFER),
						DDR_LENGTH/4,
						(u8 *)(XPAR_PS7_DDR_0_S_AXI_BASEADDR),
						DDR_LENGTH/4, XDCFG_NON_SECURE_PCAP_WRITE);
	/* Check the status of the transfer */
	if (Status != XST_SUCCESS) {
		fsbl_printf(DEBUG_GENERAL,"PCAP_DMA_TRANSFER_FAIL \r\n");
		return XST_FAILURE;
	}

	/* poll for the DMA done */
	XDcfg_PollDone(XDCFG_IXR_DMA_DONE_MASK, MAX_COUNT);
	ClearPcap_Status();
	
	/* Restore the loopback bit in the devcfg */
	PcapReg = FsblIn32(XPS_DEV_CFG_APB_BASEADDR + XDCFG_MCTRL_OFFSET);
	PcapReg &= ~XDCFG_MCTRL_PCAP_LPBK_MASK;
	FsblOut32(XPS_DEV_CFG_APB_BASEADDR + XDCFG_MCTRL_OFFSET,PcapReg);

	fsbl_printf(DEBUG_INFO,"PCAP MCTRL %x: %08x\r\n",
					XPS_DEV_CFG_APB_BASEADDR + XDCFG_MCTRL_OFFSET,
	FsblIn32(XPS_DEV_CFG_APB_BASEADDR + XDCFG_MCTRL_OFFSET));

	/* Fix for CR 656095 */
	DDRClearEccError();
#ifdef FSBL_PERF
	XTime tEccEnd = 0;
	fsbl_printf(DEBUG_GENERAL,"Time taken to initialize DDR for ECC is ");
	FsblMeasurePerfTime(tEccCurrent,tEccEnd);
#endif

	return XST_SUCCESS;
} /* End of DDR initialization for ECC */

/******************************************************************************
*
* This function Clears the ECC Error Codes in DDR
*
* @param	None
*
* @return
*			None
*
* @note		None
*
*******************************************************************************/
void DDRClearEccError(void)
{
	/* Fix for CR 656905
	 * This is to Clear the ECC Error Code in DDR
	 */
	FsblOut32(DDR_CHE_ECC_CONTROL_REG_OFFSET, CLEAR_ERROR_ECC);
}
#endif

/******************************************************************************
*
* This function Gets the ticks from the Global Timer
*
* @param	Current time
*
* @return
*			None
*
* @note		None
*
*******************************************************************************/
#ifdef FSBL_PERF
void FsblGetGlobalTime (XTime tCur)
{
	XTime_GetTime(&tCur);
}

/******************************************************************************
*
* This function Measures the execution time
*
* @param	Current time , End time
*
* @return
*			None
*
* @note		None
*
*******************************************************************************/
void FsblMeasurePerfTime (XTime tCur, XTime tEnd)
{
	double tDiff = 0.0;
	double tPerfSeconds;
	XTime_GetTime(&tEnd);
	tDiff  = (double)tEnd - (double)tCur;
	/* Convert tPerf into Seconds */
	tPerfSeconds = tDiff/COUNTS_PER_SECOND;

	/* Convert tPerf into Seconds */
	tPerfSeconds = tDiff/COUNTS_PER_SECOND;

#if defined(FSBL_DEBUG) || defined(FSBL_DEBUG_INFO)
	printf("%f seconds \r\n",tPerfSeconds);
#endif


	/* Time can als be printed in terms of millisecond or nano seconds */
}
#endif

/******************************************************************************
*
* This function initializes the Watchdog driver and starts the timer
*
* @param	None
*
* @return
*		- XST_SUCCESS if the Watchdog driver is initialized
*		- XST_FAILURE if Watchdog driver initialization fails
*
* @note		None
*
*******************************************************************************/
#ifdef XPAR_XWDTPS_0_BASEADDR
int InitWatchDog(void)
{
	u32 Status = XST_SUCCESS;
	XWdtPs_Config *ConfigPtr; 	/* Config structure of the WatchDog Timer */
	u32 CounterValue = 1;

	ConfigPtr = XWdtPs_LookupConfig(WDT_DEVICE_ID);
	Status = XWdtPs_CfgInitialize(&Watchdog,
				ConfigPtr,
				ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		fsbl_printf(DEBUG_INFO,"Watchdog Driver init Failed \n\r");
		return XST_FAILURE;
	}
	/* Setting the divider value */
	XWdtPs_SetControlValue(&Watchdog,
			XWDTPS_CLK_PRESCALE,
			XWDTPS_CCR_PSCALE_4096);
	/* Convert time to  Watchdog counter reset value */
	CounterValue = ConvertTime_WdtCounter(WDT_EXPIRE_TIME);
	/* Set the Watchdog counter reset value */
	XWdtPs_SetControlValue(&Watchdog,
			XWDTPS_COUNTER_RESET,
			CounterValue);
	/* enable reset output, as we are only using this as a basic counter */
	XWdtPs_EnableOutput(&Watchdog, XWDTPS_RESET_SIGNAL);
	/* Start the Watchdog timer */
	XWdtPs_Start(&Watchdog);
	XWdtPs_RestartWdt(&Watchdog);

	return XST_SUCCESS;

} /* End of InitWatchDog */

/******************************************************************************/
/**
*
* This function checks whether WDT reset has happened during FSBL run
*
* If WDT reset happened during FSBL run, then need to fallback
*
* @param	None.
*
* @return
*		None
*
* @note		None
*
****************************************************************************/
void CheckWDTReset(void)
{
	volatile u32 ResetReason;
	volatile u32 RebootStatusRegister;

	RebootStatusRegister = FsblIn32(REBOOT_STATUS_REG);

	/* For 1.0 Silicon the reason for Reset is in the ResetReason Register
	 * Hence this register can be read to know the cause for previous reset
	 * that happened.
	 * Check if that reset is a Software WatchDog reset that happened
	 */
	if (Silicon_Version == SILICON_VERSION_0) {
		ResetReason = FsblIn32(RESET_REASON_REG);
	} else {
		ResetReason = GetResetReason();
	}
	/* If the FSBL_IN_MASK Has not been cleared, WDT happened
	 * before FSBL exits
	 *
	 */
	if ((ResetReason & RESET_REASON_SWDT) == RESET_REASON_SWDT ) {
		if ((RebootStatusRegister & FSBL_FAIL_MASK) == FSBL_IN_MASK) {
			/* Clear the SWDT Reset bit */
			ResetReason &= ~RESET_REASON_SWDT;
			if (Silicon_Version == SILICON_VERSION_0) {
				/* for 1.0 Silicon we need to write
				 * 1 to the RESET REASON Clear register 
				 *
				 */
				FsblOut32(RESET_REASON_CLR, 1);
			} else {
				FsblOut32(REBOOT_STATUS_REG, ResetReason);
			}

			fsbl_printf(DEBUG_GENERAL,"WDT_RESET_OCCURED \n\r");
		}
	}
} /* End of CheckWDTReset */
/******************************************************************************
*
* This function converts time into Watchdog counter value
*
* @param	watchdog expire time in seconds
*
* @return
*			Counter value for Watchdog
*
* @note		None
*
*******************************************************************************/

u32 ConvertTime_WdtCounter(u32 seconds)
{
	double time = 0.0;
	double CounterValue;
	u32 Crv = 0;
	u32 Prescaler,PrescalerValue;

	Prescaler = XWdtPs_GetControlValue(&Watchdog, XWDTPS_CLK_PRESCALE);

	if (Prescaler == XWDTPS_CCR_PSCALE_0008)
		PrescalerValue = 8;
	if (Prescaler == XWDTPS_CCR_PSCALE_0064)
		PrescalerValue = 64;
	if (Prescaler == XWDTPS_CCR_PSCALE_4096)
		PrescalerValue = 4096;

	time = (double)(PrescalerValue) / (double)XPAR_PS7_WDT_0_WDT_CLK_FREQ_HZ;

	CounterValue = seconds / time;

	Crv = (u32)CounterValue;
	Crv >>= WDT_CRV_SHIFT;
	return Crv;

} /* End of ConvertTime_WdtCounter */

#endif

/******************************************************************************
*
* This function checks for the soft reset
*
* @param	None
*
* @return
*		None
*
* @note		None
*
*******************************************************************************/
void CheckSRST(void)
{
	volatile u32 ResetReason;
	/* Check if SRST occured, if so clear the Partition number */
	ResetReason = GetResetReason();
	if (ResetReason == RESET_REASON_SRST ) {
		FsblOut32(REBOOT_STATUS_REG,
				(FsblIn32(REBOOT_STATUS_REG)) &
				~(FSBL_FAIL_MASK | PARTITION_NUMBER_MASK));
	}
} /* End of CheckSRT */


/******************************************************************************
*
* This function Gets the Silicon Version
*
* @param	None
*
* @return
*		Updates the Silicon_Version
*
* @note		None
*
*******************************************************************************/
void GetSiliconVersion(void)
{
	/* Get the silicon version */
	Silicon_Version = XDcfg_GetPsVersion(DcfgInstPtr);

	if (Silicon_Version == SILICON_VERSION_0) {
		fsbl_printf( DEBUG_GENERAL,"Silicon Version 1.0\r\n");
	} else if(Silicon_Version == SILICON_VERSION_1) {
		fsbl_printf(DEBUG_GENERAL,"Silicon Version 2.0\r\n");
	} else {
		fsbl_printf(DEBUG_GENERAL,"Unsupported Silicon Version %d  \r\n",
				Silicon_Version);
	}
} /* End of GetSiliconVersion */


