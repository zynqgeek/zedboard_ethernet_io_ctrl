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
* @file image_mover.c
*
* Move partitions to either DDR to execute or to program FPGA.
* It performs partition walk.
*
* <pre>
* MODIFICATION HISTORY:
*
* Ver	Who	Date		Changes
* ----- ---- -------- -------------------------------------------------------
* 1.00a jz	05/24/11	Initial release
* 2.00a jz	06/30/11	Updated partition header defs for 64-byte
*			 	alignment change in data2mem tool
* 2.00a mb	25/05/12	Updated for standalone based bsp FSBL
* 			 	Nand/SD encryption and review comments
* 3.00a np	30/08/12	Added FSBL user hook calls
* 				(before and after bitstream download.)
*
* </pre>
*
* @note
*	A partition is either an executable or a bitstream to program FPGA
*
******************************************************************************/

/***************************** Include Files *********************************/
#include "fsbl.h"
#include "image_mover.h"
#include "xil_printf.h"
#include "xreg_cortexa9.h"
#include "pcap.h"

#ifdef	XPAR_XWDTPS_0_BASEADDR
#include "xwdtps.h"
#endif

/************************** Constant Definitions *****************************/

/* We are 32-bit machine */
#define MAXIMUM_IMAGE_WORD_LEN 0x40000000

/**************************** Type Definitions *******************************/
struct PartHeader {
	u32 ImageWordLen;	/* 0x0 */
	u32 DataWordLen;	/* 0x4 */
	u32 PartitionWordLen;	/* 0x8 */
	u32 LoadAddr;		/* 0xC */
	u32 ExecAddr;		/* 0x10 */
	u32 PartitionStart;	/* 0x14 */
	u32 PartitionAttr;	/* 0x18 */
	u32 SectionCount;	/* 0x1C */
	u32 Pads[7];
	u32 CheckSum;		/* 0x3C */
};

struct HeaderArray {
	u32 Fields[16];
};
/***************** Macros (Inline Functions) Definitions *********************/

/************************** Function Prototypes ******************************/

extern u32 FsblHookBeforeBitstreamDload(void);
extern u32 FsblHookAfterBitstreamDload(void);

static u32 PartitionHeaderChecksum(struct HeaderArray *H);
static u32 GoToPartition(u32 ImageAddress, int PartNum);
static void GetHeader(u32 Address, struct PartHeader *H);
static int IsEmptyHeader(struct HeaderArray *H);
static int IsLastPartition(struct HeaderArray *H);
static void DumpHeader(struct PartHeader *Header);


/************************** Variable Definitions *****************************/
extern u32 FlashReadBaseAddress;

ImageMoverType MoveImage;

#ifdef XPAR_XWDTPS_0_BASEADDR
extern XWdtPs Watchdog;	/* Instance of WatchDog Timer	*/
#endif
/******************************************************************************/
/**
*
* This function dumps the partition header.
*
* @param	Partition header pointer
*
* @return	None
*
* @note		None
*
*******************************************************************************/
void DumpHeader(struct PartHeader *Header)
{
	fsbl_printf(DEBUG_INFO,"Image Word Len:%08x\n\r", Header->ImageWordLen);
	fsbl_printf(DEBUG_INFO,"Data Word Len:	%08x\n\r", Header->DataWordLen);
	fsbl_printf(DEBUG_INFO,"Partition Word Len:%08x\n\r", Header->DataWordLen);
	fsbl_printf(DEBUG_INFO,"Load Addr:%08x\n\r", Header->LoadAddr);
	fsbl_printf(DEBUG_INFO,"Exec Addr:%08x\n\r", Header->ExecAddr);
	fsbl_printf(DEBUG_INFO,"Partition Start:%08x\n\r", Header->PartitionStart);
	fsbl_printf(DEBUG_INFO,"Partition Attr:%08x\n\r", Header->PartitionAttr);
	fsbl_printf(DEBUG_INFO,"Section Count:%08x\n\r", Header->SectionCount);
	fsbl_printf(DEBUG_INFO,"Checksum:%08x\n\r", Header->CheckSum);
}

/******************************************************************************/
/**
*
* This function validates the partition header.
*
* @param	Partition header pointer
*
* @return
*		- MOVE_IMAGE_FAIL if bad header.
* 		- XST_SUCCESS if successful.
*
* @note		None
*
*******************************************************************************/
u32 ValidateHeader(struct PartHeader *Header)
{
	struct HeaderArray *Hap;
	u32 ImageLength;
	u32 PartitionLength;

	Hap = (struct HeaderArray *)Header;

	/* If there are no partitions to load, fail */
	if (IsLastPartition(Hap) ||
		IsEmptyHeader(Hap)) {
		fsbl_printf(DEBUG_GENERAL,"IMAGE_HAS_NO_PARTITIONS\r\n");
		OutputStatus(IMAGE_HAS_NO_PARTITIONS);
		return MOVE_IMAGE_FAIL;
	}

	/* Validate Checksum of partition header */
	if (PartitionHeaderChecksum(Hap) == XST_FAILURE) {
		fsbl_printf(DEBUG_GENERAL,"PARTITION_HEADER_CORRUPTION\r\n");
		OutputStatus(PARTITION_HEADER_CORRUPTION);
		return MOVE_IMAGE_FAIL;
	}

	ImageLength = Header->ImageWordLen;
	PartitionLength = Header->PartitionWordLen;

	if (ImageLength > MAXIMUM_IMAGE_WORD_LEN) {
		fsbl_printf(DEBUG_GENERAL,"INVALID_PARTITION_LENGTH\r\n");
		OutputStatus(INVALID_PARTITION_LENGTH);
		return MOVE_IMAGE_FAIL;
	}

	/* For current tool implementation, partition header is not
	 * encrypted. If the tool will encrypt the partition header,
	 * the following should be overwritten after header decryption
	 */
	if (ImageLength == 0) {
		/* if it is an empty partition, it is OK */
		if (PartitionLength != 0) {
			fsbl_printf(DEBUG_INFO,"PartitionImageMove:"
			"0 Image Length, "
			"non-zero partition Length\r\n");
			fsbl_printf(DEBUG_GENERAL,"INVALID_PARTITION_HEADER\r\n");
			OutputStatus(INVALID_PARTITION_HEADER);
			return MOVE_IMAGE_FAIL;
		}
	}

	return XST_SUCCESS;
} /* end of ValidateHeader */

/******************************************************************************/
/**
*
* This function reads the data at the provided FLASH address and expects
* the partition header structure to be present. If the header is not
* present (blank FLASH) or does not match expected values, MOVE_IMAGE_FAIL
* is returned.
*
* There are four cases in partition handling:
*
* 1. If attribute indicates this is a bitstream partition, FPGA is programmed,
*	and if this is the last partition, FSBL hangs with WDT kicking
* 2. If Image word Length != data word length, the partition is encrypted
*	partition. The partition is decrypted into DDR
* 3. Other cases, partition is moved from flash to DDR through copying or DMA
* 4. If partition data word length is 0, the partition is to be skipped. Note
*	that partition header cannot be encrypted for this to work
*
* Except case 1, the reboot status register is updated to reflect the next
* partition.
*
* @param	ImageAddress is the start of the image
* @param	PartitionNum is the index of the partition to process
*
* @return
*		- MOVE_IMAGE_FAIL (0xFFFFFFFF) if image load failed
*		- Address to begin executing at if successful
*
* @note
*
****************************************************************************/
u32 PartitionMove(u32 ImageAddress, int PartitionNum)
{
	u32 SourceAddr;
	u32 LoadAddr;
	u32 ExecAddr;
	u32 ImageLength;
	u32 DataLength;
	u32 SectionCount;
	u32 PartitionStart;
	u32 PartitionLength;
	u32 NextPartitionAddr;
	int NextPartition;
	u32 Status;
	u32 Attribute;

	 /* Flag to determine if image is encrypted or not */
	u32 IsEncrypted = 0;
	struct HeaderArray *Hap;
	struct PartHeader Header;
	/* Execution Address */
	ExecAddr = MOVE_IMAGE_FAIL;

	/* Default is not to skip partition execution */
	SkipPartition = 0;

	fsbl_printf(DEBUG_INFO,"image move with partition number %d\r\n",
			PartitionNum);

	PartitionStart = GoToPartition(ImageAddress, PartitionNum);

	GetHeader(PartitionStart, &Header);

	fsbl_printf(DEBUG_INFO,"Header dump:\n\r");
	DumpHeader(&Header);

	Hap = (struct HeaderArray *)&Header;
	Status = ValidateHeader(&Header);

	if(Status != XST_SUCCESS) {
		return Status;
	}

	ImageLength = Header.ImageWordLen;
	DataLength = Header.DataWordLen;
	PartitionLength = Header.PartitionWordLen;
	LoadAddr = Header.LoadAddr;
	ExecAddr = Header.ExecAddr;
	SourceAddr = Header.PartitionStart;
	Attribute = Header.PartitionAttr;
	SectionCount = Header.SectionCount;

	/* The offset in header is word offset */
	SourceAddr = SourceAddr << WORD_LENGTH_SHIFT;

	/* Consider image offset */
	SourceAddr += ImageAddress;

	fsbl_printf(DEBUG_INFO,"Partition Start %08x, "
						"Partition Length %08x\r\n",
						PartitionStart, PartitionLength);

	fsbl_printf(DEBUG_INFO,"Source addr %08x, Load addr %08x, "
						"Exec addr %08x\r\n",
						SourceAddr, LoadAddr, ExecAddr);

	/*
	 * If encrypted partition header, will get wrong next partition
	 * address. Next run will fail and invoke FSBL fallback.
	 * WARNING: Cannot handle skipping over encrypted partition header
	 */
	if (DataLength == 0) {
		fsbl_printf(DEBUG_INFO,"PartitionImageMove: skip partition. "
			"If encrypted partition, will fail\r\n");

		SkipPartition = 1;
		goto update_status_reg;
	}

	if (Attribute == ATTRIBUTE_PL_IMAGE_MASK) {

		/* FSBL user hook call before bitstream download. */
		Status = FsblHookBeforeBitstreamDload();

		if (Status != XST_SUCCESS) {
			/* Error Handling here */
			fsbl_printf(DEBUG_GENERAL,"FSBL_BEFORE_BSTREAM_HOOK_FAILED\r\n");
			OutputStatus(FSBL_BEFORE_BSTREAM_HOOK_FAIL);
			FsblFallback();
		}

		/* Check if encrypted or non encrypted */
		/* If the ImageLength not equal to DataLength, encrypted */
		if (ImageLength != DataLength) {
			IsEncrypted = 1;
			fsbl_printf(DEBUG_INFO,"Encrypted partition \r \n");
		}

		fsbl_printf(DEBUG_INFO,"Source addr %x, load addr %x, "
				"exec addr %x\r\n", SourceAddr, LoadAddr, ExecAddr);

		fsbl_printf(DEBUG_GENERAL,"Bitstream Download Start \r\n");

		if (IsEncrypted) {
			/* Bitstream NAND/SD encrypted */
			if (((FlashReadBaseAddress & 0xFF000000) == XPS_NAND_BASEADDR) ||
				(FlashReadBaseAddress == XPS_SDIO0_BASEADDR)) {
				fsbl_printf(DEBUG_INFO,"Bitstream NAND/SD encrypted \r\n");
				Status = WritePcapXferData(
						(u32 *)(SourceAddr),
						(u32 *)XDCFG_DMA_INVALID_ADDRESS,
						(ImageLength), 0, XDCFG_SECURE_PCAP_WRITE);
			} else {
			/* Bitstream QSPI/NOR Encrypted */
				fsbl_printf(DEBUG_INFO,"Bit stream QSPI/NOR encrypted \r\n");
				Status = WritePcapXferData(
		 					(u32 *)(FlashReadBaseAddress + SourceAddr),
							(u32 *)XDCFG_DMA_INVALID_ADDRESS,
							(ImageLength), 0, XDCFG_SECURE_PCAP_WRITE);
			}

		} else {
				/* Bitstream NAND/SD Unencrypted */
				if (((FlashReadBaseAddress & 0xFF000000) == XPS_NAND_BASEADDR) ||
						(FlashReadBaseAddress == XPS_SDIO0_BASEADDR)) {
					fsbl_printf(DEBUG_INFO,"Bit stream NAND/SD Unencrypted \r\n");
					Status = WritePcapXferData(
							(u32 *)(SourceAddr),
							(u32 *)XDCFG_DMA_INVALID_ADDRESS,
							(ImageLength), 0, XDCFG_NON_SECURE_PCAP_WRITE);
				} else {
					/* Bitstream QSPI/NOR - unencrypted */
					fsbl_printf(DEBUG_INFO,"Bitstream QSPI/NOR Unencrypted \r\n");
					Status = WritePcapXferData(
						(u32 *)(FlashReadBaseAddress + SourceAddr),
						(u32 *)XDCFG_DMA_INVALID_ADDRESS,
						(ImageLength), 0, XDCFG_NON_SECURE_PCAP_WRITE);
				}
		}

		if (Status != XST_SUCCESS) {
			fsbl_printf(DEBUG_GENERAL,"BITSTREAM_DOWNLOAD_FAIL");
			OutputStatus(BITSTREAM_DOWNLOAD_FAIL);
			return MOVE_IMAGE_FAIL;
		} else {
			fsbl_printf(DEBUG_GENERAL,"Bitstream download successful!\r\n");
		}

		/* FSBL user hook call after bitstream download. */
		Status = FsblHookAfterBitstreamDload();

		if (Status != XST_SUCCESS) {
			fsbl_printf(DEBUG_GENERAL,"FSBL_AFTER_BSTREAM_HOOK_FAIL");
			/* Error Handling here */
			OutputStatus(FSBL_AFTER_BSTREAM_HOOK_FAIL);
			FsblFallback();
		}
		/* We will skip soft reset and start loading the next partition */
		SkipPartition = 1;

		/* Go to the next partition */
		goto update_status_reg;

	} /* End of bitstream */
	/* Process all the partitions of application */

	while(SectionCount-- > 0) {

	/* If image length not equal to data length, encrypted */
	if (ImageLength != DataLength) {
		IsEncrypted = 1;
		fsbl_printf(DEBUG_INFO,"Application encrypted\n\r");
		/* NAND and SD do not support encrypted partitions */
		 if (((FlashReadBaseAddress & 0xFF000000) == XPS_NAND_BASEADDR) ||
			 (FlashReadBaseAddress == XPS_SDIO0_BASEADDR)) {

			 fsbl_printf(DEBUG_INFO,"Images on NAND or SD	encrypted\r\n");
			 Status = WritePcapXferData(
						(u32 *)SourceAddr,
						(u32 *)LoadAddr, ImageLength,
						 DataLength, XDCFG_SECURE_PCAP_WRITE);

		} else {
			 Status = WritePcapXferData(
					 	 (u32 *)(FlashReadBaseAddress + SourceAddr),
					 	 (u32 *)LoadAddr, ImageLength,
					 	 DataLength, XDCFG_SECURE_PCAP_WRITE);

		}
		 
		if (Status != XST_SUCCESS) {
			fsbl_printf(DEBUG_GENERAL,"DECRYPTION_FAIL \r\n");
			OutputStatus(DECRYPTION_FAIL);
			return MOVE_IMAGE_FAIL;
		 }
		 
		fsbl_printf(DEBUG_INFO,"Transfer is completed \r\n");
		if (SectionCount != 0) {
			fsbl_printf(DEBUG_INFO,"load_next_partition\n\r");
			goto load_next_partition;
		}

		goto update_status_reg;
	}	else {/*End of if ImageLength != DataLength */

		fsbl_printf(DEBUG_INFO,"Start transfer data into DDR\n\r");

		/* Copy the flash contents to DDR */

		MoveImage(SourceAddr, LoadAddr, (DataLength << WORD_LENGTH_SHIFT));

	}

load_next_partition:
	if (SectionCount == 0) {
		goto update_status_reg;
	}

	PartitionNum += 1;

	/* Read the next partition */
	fsbl_printf(DEBUG_INFO,"Get next partition header\n\r");

	NextPartitionAddr = GoToPartition(ImageAddress, PartitionNum);
	GetHeader(NextPartitionAddr, &Header);

	fsbl_printf(DEBUG_INFO,"Next partition header dump\r\n");
	DumpHeader(&Header);

	Status = ValidateHeader(&Header);
	if(Status != XST_SUCCESS) {
		fsbl_printf(DEBUG_INFO,"Validation of Header failed\r\n");
		return Status;
	}

	/* Don't re-initialize ExecAddr & SectionCount */
	ImageLength = Header.ImageWordLen;
	DataLength = Header.DataWordLen;
	PartitionLength = Header.PartitionWordLen;
	LoadAddr = Header.LoadAddr;
	SourceAddr = Header.PartitionStart;

	/* The offset in header is word offset */
	SourceAddr = SourceAddr << WORD_LENGTH_SHIFT;

	/* Consider image offset */
	SourceAddr += ImageAddress;

	fsbl_printf(DEBUG_INFO,"Partition Start %08x, Partition Length %08x\r\n",
						PartitionStart, PartitionLength);

	fsbl_printf(DEBUG_INFO,"Source addr %08x, Load addr %08x, Exec addr %08x\r\n",
						SourceAddr, LoadAddr, ExecAddr);

	} /* End of while Section count > 0 */


update_status_reg:

	NextPartition = PartitionNum + 1;

	fsbl_printf(DEBUG_INFO,"Get next partition header\n\r");

	NextPartitionAddr = GoToPartition(ImageAddress, PartitionNum + 1);
	GetHeader(NextPartitionAddr, &Header);
	Hap = (struct HeaderArray *)&Header;

	fsbl_printf(DEBUG_INFO,"Next Header dump:\n\r");
	DumpHeader(&Header);

	/* If it is the last partition, then we will keep on running this one */
	if (IsLastPartition(Hap)) {
		fsbl_printf(DEBUG_INFO,"There are no more partitions to load\r\n");
		NextPartition = PartitionNum;
		/*
		 * Fix for CR #663277
		 * Review : Since we donot support OCM remap,
		 * any elf whose execution address is 0x0
		 * We will do a JTAG exit 
		 */
 
		if (ExecAddr == 0) {
			fsbl_printf(DEBUG_INFO,"No Execution Address JTAG handoff \r\n");
			/* Stop the Watchdog before JTAG handoff */
#ifdef	XPAR_XWDTPS_0_BASEADDR
			XWdtPs_Stop(&Watchdog);
#endif
			ClearFSBLIn();
			FsblHandoffJtagExit();
		}

	} else if(IsEmptyHeader(Hap)) {
		fsbl_printf(DEBUG_GENERAL,"Empty partition header \r\n");
		OutputStatus(EMPTY_PARTITION_HEADER);
		return MOVE_IMAGE_FAIL;
	}


	Status = FsblSetNextPartition(NextPartition);
	if (Status != XST_SUCCESS) {
		fsbl_printf(DEBUG_INFO,"Next partition number %d is not valid\n",
							NextPartition);
		OutputStatus(TOO_MANY_PARTITIONS);
		return MOVE_IMAGE_FAIL;
	 }

	fsbl_printf(DEBUG_INFO,"end of partition move, reboot status reg %x, "
		 "Next partition %d\r\n", MoverIn32(REBOOT_STATUS_REG), NextPartition);

	/* Returning Execution Address */
	return ExecAddr;
} /* End of Partition Move */

/******************************************************************************/
/**
*
* This function checks the header checksum If the header checksum is not valid
* XST_FAILURE is returned.
*
* @param	H is a pointer to struct HeaderArray
*
* @return
*		- XST_SUCCESS is header checksum is ok
*		- XST_FAILURE if the header checksum is not correct
*
* @note		None.
*
****************************************************************************/
static u32 PartitionHeaderChecksum(struct HeaderArray *H)
{

	u32 Checksum;
	u32 Count;

	Checksum = 0;

	for (Count = 0; Count < PARTITION_HDR_CHECKSUM_WORD_COUNT; Count++) {
		/* Read the word from the header */
		Checksum += H->Fields[Count];
	}

	/* Invert checksum, last bit of error checking */
	Checksum ^= 0xFFFFFFFF;

	/* Validate the checksum */
	if (H->Fields[PARTITION_HDR_CHECKSUM_WORD_COUNT] != Checksum) {
		fsbl_printf(DEBUG_GENERAL,"Checksum %8.8x != %8.8x\r\n",
			Checksum, H->Fields[PARTITION_HDR_CHECKSUM_WORD_COUNT]);
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

/******************************************************************************/
/**
* This function check whether the current partition is the end of partitions
*
* The partition is the end of the partitions if it looks like this:
*	0x00000000
*	0x00000000
*	....
*	0x00000000
*	0x00000000
*	0xFFFFFFFF
*
* This function only checks the first, second, third and eighth word.
*
* @param	H is a pointer to struct HeaderArray
*
* @return
*		- TRUE if it is the last partition
*		- FALSE if it is not last partition
*
****************************************************************************/
static int IsLastPartition(struct HeaderArray *H)
{
	int Index;

	if (H->Fields[PARTITION_HDR_CHECKSUM_WORD_COUNT] != 0xFFFFFFFF) {
		return FALSE;
	}

	for (Index = 0; Index < PARTITION_HDR_WORD_COUNT - 1; Index++) {

		if (H->Fields[Index] != 0x0) {
			return FALSE;
		}
	}

	return TRUE;
}

/******************************************************************************/
/**
* This function check whether the current partition header is empty.
* A partition header is considered empty if image word length is 0 and the
* last word is 0.
*
* @param	H is a pointer to struct HeaderArray
*
* @return
*		- TRUE ,  If the partition header is empty
*		- FALSE , If the partition header is NOT empty
*
* @note		Caller is responsible to make sure the address is valid.
*
*
****************************************************************************/
static int IsEmptyHeader(struct HeaderArray *H)
{
	int Index;

	for (Index = 0; Index < PARTITION_HDR_WORD_COUNT; Index++) {

		if (H->Fields[Index] != 0x0) {
			return FALSE;
		}
	}

	return TRUE;
}

/******************************************************************************/
/**
*
* This function goes to the partition header of the specified partition
*
* @param	ImageAddress is the start address of the image
* @param	PartNum is the partition number to go to
*
* @return	Partition header address of the partition
*
* @note		None
*
****************************************************************************/
static u32 GoToPartition(u32 ImageAddress, int PartNum)
{
	u32 HdrAddr;

	fsbl_printf(DEBUG_INFO,"ImageAddress = 0x%x \r \n",ImageAddress);
	MoveImage(ImageAddress + IMAGE_PHDR_OFFSET, (u32)&HdrAddr, 4);

	fsbl_printf(DEBUG_INFO,"Partition hdr for %d: %x\n\r", PartNum,
		ImageAddress + HdrAddr + PARTITION_HDR_TOTAL_LEN * (PartNum + 1));

	return (ImageAddress + HdrAddr + PARTITION_HDR_TOTAL_LEN * (PartNum + 1));
}

/******************************************************************************/
/**
*
* This function gets the partition header
*
* @param	Address is the start address of the image
* @param	H is the pointer to the partition Header structure
*
* @return	None
*
* @note		None
*
****************************************************************************/
static void GetHeader(u32 Address, struct PartHeader *H)
{

	MoveImage(Address, (u32)H, PARTITION_HDR_TOTAL_LEN);

}


