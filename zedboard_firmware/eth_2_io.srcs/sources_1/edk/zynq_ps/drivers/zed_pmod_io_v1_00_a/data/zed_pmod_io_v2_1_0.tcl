##############################################################################
## Filename:          C:\Xilinx\projects\eth_2_io\eth_2_io.srcs\sources_1\edk\zynq_ps/drivers/zed_pmod_io_v1_00_a/data/zed_pmod_io_v2_1_0.tcl
## Description:       Microprocess Driver Command (tcl)
## Date:              Thu Jan 24 22:08:21 2013 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "zed_pmod_io" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" 
}
