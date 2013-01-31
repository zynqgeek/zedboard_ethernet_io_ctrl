/*
 * main.c - Main entry point for our application
 *
 *
 * Date			Author		Notes
 * --------------------------
 * 2013-01-24	ZynqGeek	Initial Version
 *
 *
 */


#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "netif/xadapter.h"
#include "platform_config.h"

// function declarations from platform.c
void print(char *str);

// defined by each RAW mode application
void print_app_header();
int start_application();
int transfer_data();

// helper functions in eth.c
void print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw);

// missing declaration in lwIP
void lwip_init();

// main entry point to our program
int main()
{
	// set the port for our application to listen on
	unsigned port = 7000;

    // init the Zynq platform
	init_platform();

    print("System Post.\n\r");

    print("Configuring Ethernet Interface.\r\n");

		struct netif *netif, server_netif;
		struct ip_addr ipaddr, netmask, gw;

		/* the mac address of the board. this should be unique per board */
		unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

		netif = &server_netif;

		/* initliaze IP addresses to be used */
		IP4_ADDR(&ipaddr,  192, 168,   1, 10);
		IP4_ADDR(&netmask, 255, 255, 255,  0);
		IP4_ADDR(&gw,      192, 168,   1,  1);

	print_ip_settings(&ipaddr, &netmask, &gw);

    print("Initializing LWIP.");

   		lwip_init();

    print("Adding created network interface to LWIP interface list.\r\n");

    	/* Add network interface to the netif_list, and set it as default */
    	if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, PLATFORM_EMAC_BASEADDR)) {
    		xil_printf("ERROR: Error adding N/W interface\n\r");
    		return -1;
    	}
    	netif_set_default(netif);

    //print(" INFO: Enabling Zynq Platform Interupts ...");

    	//platform_enable_interrupts();

    //print("Done.\r\n");

    print("Bringing up Ethernet Interface.\r\n");

    	/* specify that the network if is up */
    	netif_set_up(netif);

    print("Starting LWIP Listener.\r\n");

    	start_application(port);

    return 0;

}
