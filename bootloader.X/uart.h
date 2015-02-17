#ifndef UART_H
#define	UART_H

#ifdef	__cplusplus
extern "C" {
#endif

    void uart_setup();
    void uart_send_uchar(unsigned char value);
    unsigned char uart_recv_uchar();

#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */
