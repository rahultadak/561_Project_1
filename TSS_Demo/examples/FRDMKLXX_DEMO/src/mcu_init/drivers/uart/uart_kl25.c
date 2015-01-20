/*
 * File:        uart.c
 * Purpose:     Provide common UART routines for serial IO
 *
 * Notes:
 *
 */

#include "derivative.h"
#include "uart_kl25.h"

/********************************************************************/
/*
 * Initialize the UART for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the UARTs are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable UART pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  uartch      UART channel to initialize
 *  sysclk      UART module Clock in kHz(used to calculate baud)
 *  baud        UART baud rate
 */
void uart_init (UART_MemMapPtr uartch, int sysclk, int baud)
{
    register uint16 sbr;
#if defined(UART_C4_BRFA_SHIFT)
    register uint16 brfa;
#endif
    uint8 temp;

    /* Enable the clock to the selected UART */
    if(uartch == (UART_MemMapPtr)UART0_BASE_PTR)
		SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;
    else
    	if (uartch == UART1_BASE_PTR)
			SIM_SCGC4 |= SIM_SCGC4_UART1_MASK;
    	else
    		if (uartch == UART2_BASE_PTR)
    			SIM_SCGC4 |= SIM_SCGC4_UART2_MASK;

    /* Make sure that the transmitter and receiver are disabled while we
     * change settings.
     */
    UART_C2_REG(uartch) &= ~(UART_C2_TE_MASK
				| UART_C2_RE_MASK );

    /* Configure the UART for 8-bit mode, no parity */
    UART_C1_REG(uartch) = 0;	/* We need all default settings, so entire register is cleared */

    /* Calculate baud settings */
    sbr = (uint16)((sysclk)/(baud * 16));

    /* Save off the current value of the UARTx_BDH except for the SBR field */
    temp = UART_BDH_REG(uartch) & ~(UART_BDH_SBR(0x1F));

    UART_BDH_REG(uartch) = temp |  UART_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART_BDL_REG(uartch) = (uint8)(sbr & UART_BDL_SBR_MASK);

#if defined(UART_C4_BRFA_SHIFT)
    /* Determine if a fractional divider is needed to get closer to the baud rate */
    brfa = (((sysclk*32)/(baud * 16)) - (sbr * 32));

    /* Save off the current value of the UARTx_C4 register except for the BRFA field */
    temp = UART_C4_REG(uartch) & ~(UART_C4_BRFA(0x1F));

    UART_C4_REG(uartch) = temp |  UART_C4_BRFA(brfa);
#endif

    /* Enable receiver and transmitter */
    UART_C2_REG(uartch) |= (UART_C2_TE_MASK | UART_C2_RE_MASK );
}
/********************************************************************/
/*
 * Wait for a character to be received on the specified UART
 *
 * Parameters:
 *  channel      UART channel to read from
 *
 * Return Values:
 *  the received character
 */
char uart_getchar (UART_MemMapPtr channel)
{
    /* Wait until character has been received */
    while (!(UART_S1_REG(channel) & UART_S1_RDRF_MASK));

    /* Return the 8-bit data from the receiver */
    return UART_D_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the UART Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      UART channel to send to
 *  ch			 character to send
 */
void uart_putchar (UART_MemMapPtr channel, char ch)
{
    /* Wait until space is available in the FIFO */
    while(!(UART_S1_REG(channel) & UART_S1_TDRE_MASK));

    /* Send the character */
    UART_D_REG(channel) = (uint8)ch;
}
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      UART channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart_getchar_present (UART_MemMapPtr channel)
{
    return (UART_S1_REG(channel) & UART_S1_RDRF_MASK);
}

/********************************************************************/

/***************************************************************************
 * Begin UART0 functions
 **************************************************************************/
/********************************************************************/
/*
 * Initialize the uart for 8N1 operation, interrupts disabled, and
 * no hardware flow-control
 *
 * NOTE: Since the uarts are pinned out in multiple locations on most
 *       Kinetis devices, this driver does not enable uart pin functions.
 *       The desired pins should be enabled before calling this init function.
 *
 * Parameters:
 *  uartch      uart channel to initialize
 *  sysclk      uart module Clock in kHz(used to calculate baud)
 *  baud        uart baud rate
 */
void uart0_init (UART0_MemMapPtr uartch, int uart0clk, int baud)
{
    register uint16 sbr;
    uint8 temp;

    SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

    /* Make sure that the transmitter and receiver are disabled while we
     * change settings.
     */
    UART0_C2_REG(uartch) &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK );

    /* Configure the uart for 8-bit mode, no parity */
    UART0_C1_REG(uartch) = 0;	/* We need all default settings, so entire register is cleared */

      /* Calculate baud settings */
    temp = UART0_C4;
    temp = (temp & UART0_C4_OSR_MASK) + 1;
    sbr = (uint16)((uart0clk)/(baud * (temp)));

    /* Save off the current value of the uartx_BDH except for the SBR field */
    temp = UART0_BDH_REG(uartch) & ~(UART0_BDH_SBR(0x1F));

    UART0_BDH_REG(uartch) = temp |  UART0_BDH_SBR(((sbr & 0x1F00) >> 8));
    UART0_BDL_REG(uartch) = (uint8)(sbr & UART0_BDL_SBR_MASK);

    /* Enable receiver and transmitter */
    UART0_C2_REG(uartch) |= (UART0_C2_TE_MASK | UART0_C2_RE_MASK );

}
/********************************************************************/
/*
 * Wait for a character to be received on the specified uart
 *
 * Parameters:
 *  channel      uart channel to read from
 *
 * Return Values:
 *  the received character
 */
char uart0_getchar (UART0_MemMapPtr channel)
{
      /* Wait until character has been received */
      while (!(UART0_S1_REG(channel) & UART0_S1_RDRF_MASK));

      /* Return the 8-bit data from the receiver */
      return UART0_D_REG(channel);
}
/********************************************************************/
/*
 * Wait for space in the uart Tx FIFO and then send a character
 *
 * Parameters:
 *  channel      uart channel to send to
 *  ch			 character to send
 */
void uart0_putchar (UART0_MemMapPtr channel, char ch)
{
      /* Wait until space is available in the FIFO */
      while(!(UART0_S1_REG(channel) & UART0_S1_TDRE_MASK));

      /* Send the character */
      UART0_D_REG(channel) = (uint8)ch;

 }
/********************************************************************/
/*
 * Check to see if a character has been received
 *
 * Parameters:
 *  channel      uart channel to check for a character
 *
 * Return values:
 *  0       No character received
 *  1       Character has been received
 */
int uart0_getchar_present (UART0_MemMapPtr channel)
{
    return (UART0_S1_REG(channel) & UART0_S1_RDRF_MASK);
}
/********************************************************************/
