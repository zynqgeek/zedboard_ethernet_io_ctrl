/*
 * eth.c - All of the LWIP functions we need
 *
 *	Note: functions taken from version 14.4 of the LWIP echo server example
 *
 *
 * Date			Author		Notes
 * --------------------------
 * 2013-01-24	ZynqGeek	Initial Version
 *
 *
 */

#include "lwip/err.h"
#include "lwip/tcp.h"

void print(char *str);

void print_ip(char *msg, struct ip_addr *ip)
{
	print(msg);
	xil_printf("%d.%d.%d.%d\n\r", ip4_addr1(ip), ip4_addr2(ip),
			ip4_addr3(ip), ip4_addr4(ip));
}

void print_ip_settings(struct ip_addr *ip, struct ip_addr *mask, struct ip_addr *gw)
{

	print_ip("\tBoard IP: ", ip);
	print_ip("\tNetmask : ", mask);
	print_ip("\tGateway : ", gw);
}

err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {

		print("Connection lost, reseting buffers ...");

			tcp_close(tpcb);
			tcp_recv(tpcb, NULL);

		print("Done.\r\n");

		return ERR_OK;
	}

	print("Processing command packet.\r\n");

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);

	/* echo back the payload */
	/* in this case, we assume that the payload is < TCP_SND_BUF */
	if (tcp_sndbuf(tpcb) > p->len)
	{
		//err = tcp_write(tpcb, p->payload, p->len, 1);

		xil_printf("\tSetting IO state to: %08x.");

		// TODO: set io
	}
	else
	{
		print("ERROR: no space in tcp_sndbuf\n\r");
	}

	/* free the received pbuf */
	pbuf_free(p);

	return ERR_OK;
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	xil_printf("Accepting new connection ...");

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}


int start_application(unsigned port)
{
	struct tcp_pcb *pcb;
	err_t err;
	//unsigned port = 7000;

	/* create new TCP PCB structure */
	pcb = tcp_new();
	if (!pcb) {
		xil_printf("ERROR: Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ADDR_ANY, port);
	if (err != ERR_OK) {
		xil_printf("ERROR: Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("ERROR: Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}
