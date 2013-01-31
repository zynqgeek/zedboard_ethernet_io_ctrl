This directory stores a number of binary files that can be used with a Windows PC and a Avnet Zedboard.

	boot.bin - This file should be copied as it stands to the SD-Card of the Zedboard.  Note: may need to be renamed to BOOT.BIN.
	eth_2_io.elf - This file is the binary of the LWIP library and custom HW driver.  This can be used in SDK to be loaded on to the ARM core.
	eth_2_io.mcs - This is a MCS prom file.  You can load this onto the SPI prom on the board as an alternative to booting off the SD-Card.
	zedboard_ethernet_io_control.exe - This is a windows binary file written in C#, and allows you to contol the PMOD IO on the Zedboard
	zynq_ps_stub.bit - This is the Programmable Logic configuration bit file that can be loaded in via iMPACT.
	README.txt - This file.
	
To use this example, simply copy the boot.bin file to the SD-Card, formated as FAT32.  Reboot the Zedboard.  Launch the zedboard_ethernet_io_control.exe program
and connect to the board via it's default settings.

Note: if you want to see what the zedboard is doing, connect a USB cable to the UART Micro USB port on the board, and connect to it with
8-N-1 at 115200 Baud.