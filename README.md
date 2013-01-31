zedboard_ethernet_io_ctrl
=========================

Control the IO on the PMOD's on the Zedboard via the Ethernet port using baremetal and LWIP

Connect to the Zedboard via USB UART with 8-N-1 @ 115200.  Copy the BOOT.BIN file from the binaries folder to a FAT32 formatted SD-Card.  Boot the Zedboard.  Launch zedboard_ethernet_io_control.exe, connect, send commands.

The default IP Address of the Zedboard is 192.168.1.10.  If you want to change that, you will need to recompile the code.

The protocol is very simple.  Just sent the 4 bytes to the board in the order of PMODA, PMODB, PMODC, PMODD.