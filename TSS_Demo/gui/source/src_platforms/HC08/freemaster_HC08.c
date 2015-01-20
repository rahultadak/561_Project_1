/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2004-2012 Freescale Semiconductor
* ALL RIGHTS RESERVED.
*
****************************************************************************//*!
*
* @file   freemaster_HC08.c
*
* @brief  FreeMASTER Driver HC08-hardware dependent stuff
*
* @version 1.0.3.0
* 
* @date Oct-3-2012
* 
*******************************************************************************/

#include "freemaster.h"
#include "freemaster_private.h"
#include "freemaster_HC08.h"

#if !(FMSTR_DISABLE)
/*******************************************************************************
*
*  @brief    API: User callback called from FMSTR_Isr() handler
*
*******************************************************************************/

#if defined(FMSTR_ISR_CALLBACK)
  extern void FMSTR_ISR_CALLBACK(void);
#endif 

/*******************************************************************************
*
* @brief    API: Main SCI / CAN Interrupt handler call
*
* This Interrupt Service Routine handles the SCI or CAN interrupts for the FreeMASTER 
* driver. In case you want to handle the interrupt in the application yourselves,
* call the FMSTR_ProcessSCI, FMSTR_ProcessCanRx or FMSTR_ProcessCanTx functions which 
* does the same job but is not compiled as an Interrupt Service Routine.
*
* In poll-driven mode (FMSTR_POLL_DRIVEN) this function does nothing.
*
*******************************************************************************/

/* HC08 interrupt routine declaration */
#if (FMSTR_USE_SCI) && defined(FMSTR_SCI_RX_INTERRUPT) && defined(FMSTR_SCI_TX_INTERRUPT)
/* user may define SCI RX interrupt number in the configuration */
interrupt FMSTR_SCI_RX_INTERRUPT

#elif (FMSTR_USE_SCI) && defined(FMSTR_SCI_INTERRUPT)
/* user may define SCI interrupt number in the configuration */
interrupt FMSTR_SCI_INTERRUPT

#elif (FMSTR_USE_MSCAN) && defined(FMSTR_CAN_RX_INTERRUPT) && defined(FMSTR_CAN_TX_INTERRUPT)
/* user may define CAN RX interrupt number in the configuration */
interrupt FMSTR_CAN_RX_INTERRUPT

#else
/* or may want to use the PRM file to set the vector to FMSTR_Isr */
#pragma TRAP_PROC
#endif

void FMSTR_Isr(void)
{
#if (FMSTR_LONG_INTR) || (FMSTR_SHORT_INTR)

    /* process serial interface */
#if FMSTR_USE_SCI
    FMSTR_ProcessSCI();

    /* process CAN interface */
#elif FMSTR_USE_CAN
    /* separate interrupts specified */
  #if defined(FMSTR_CAN_RX_INTERRUPT) && defined(FMSTR_CAN_TX_INTERRUPT) && !defined(FMSTR_CAN_INTERRUPT)
    /* CAN RX only! */
    FMSTR_ProcessCanRx();
  #else
    /* both CAN interrupts */
    FMSTR_ProcessCanRx();
    FMSTR_ProcessCanTx();
  #endif 
#endif /* FMSTR_USE_SCI */

    /* process application callback */
#if defined(FMSTR_ISR_CALLBACK)
    if((FMSTR_ISR_CALLBACK) != NULL)
        FMSTR_ISR_CALLBACK();
#endif

#endif /* (FMSTR_LONG_INTR) || (FMSTR_SHORT_INTR) */
}

/*******************************************************************************
*
* @brief    The 2nd FMSTR interrupt handler 
*
* In case the user wants to use automatic interrupt code genration, he may use
* FMSTR_SCI_RX_INTERRUPT and FMSTR_SCI_TX_INTERRUPT (FMSTR_CAN_RX_INTERRUPT and FMSTR_CAN_TX_INTERRUPT)
* constants in the configuration file (freemaster_cfg.h). The service code is the 
* same for both interrupts, and it would be definitelly better for the user to use 
* two VECTOR directives in the PRM file. 
*
* Use the FMSTR_SCI_RX_INTERRUPT and FMSTR_SCI_TX_INTERRUPT 
* (FMSTR_CAN_RX_INTERRUPT and FMSTR_CAN_TX_INTERRUPT) constants in case 
* you do not want or can not modify the PRM file.
*
*******************************************************************************/

/* HC08 interrupt routine declaration */
#if ((FMSTR_USE_SCI) && defined(FMSTR_SCI_RX_INTERRUPT) && defined(FMSTR_SCI_TX_INTERRUPT)) || \
    ((FMSTR_USE_MSCAN) && defined(FMSTR_CAN_RX_INTERRUPT) && defined(FMSTR_CAN_TX_INTERRUPT))
#if FMSTR_USE_SCI
/* user may define SCI TX interrupt number in the configuration */
interrupt FMSTR_SCI_TX_INTERRUPT
#elif FMSTR_USE_MSCAN
/* user may define CAN TX interrupt number in the configuration */
interrupt FMSTR_CAN_TX_INTERRUPT
#endif

void FMSTR_Isr2(void)
{
#if (FMSTR_LONG_INTR) || (FMSTR_SHORT_INTR)

    /* process serial interface */
#if FMSTR_USE_SCI
    FMSTR_ProcessSCI(); 

    /* process CAN interface */
#elif FMSTR_USE_CAN
    FMSTR_ProcessCanTx();
#endif

    /* process application callback */
#if defined(FMSTR_ISR_CALLBACK)
    if((FMSTR_ISR_CALLBACK) != NULL)
        FMSTR_ISR_CALLBACK();
#endif

#endif
}
#endif

/**************************************************************************//*!
*
* @brief    The "memcpy" used internally in FreeMASTER driver
*
* @param    nDestAddr - destination memory address
* @param    nSrcAddr  - source memory address
* @param    nSize     - memory size (always in bytes)
*
******************************************************************************/

void FMSTR_CopyMemory(FMSTR_ADDR nDestAddr, FMSTR_ADDR nSrcAddr, FMSTR_SIZE8 nSize)
{
    FMSTR_U8* ps = (FMSTR_U8*) nSrcAddr;
    FMSTR_U8* pd = (FMSTR_U8*) nDestAddr;

    while(nSize--)
        *pd++ = *ps++;
}

/**************************************************************************//*!
*
* @brief  Write-into the communication buffer memory
*
* @param  pDestBuff - pointer to destination memory in communication buffer
* @param  nSrcAddr  - source memory address
* @param  nSize     - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

FMSTR_BPTR FMSTR_CopyToBuffer(FMSTR_BPTR pDestBuff, FMSTR_ADDR nSrcAddr, FMSTR_SIZE8 nSize)
{
    FMSTR_U8* ps = (FMSTR_U8*) nSrcAddr;
    FMSTR_U8* pd = (FMSTR_U8*) pDestBuff;

    while(nSize--)
        *pd++ = *ps++;

    return (FMSTR_BPTR) pd;
}

/**************************************************************************//*!
*
* @brief  Read-out memory from communication buffer
*
* @param  nDestAddr - destination memory address
* @param  pSrcBuff  - pointer to source memory in communication buffer
* @param  nSize     - buffer size (always in bytes)
*
* @return This function returns a pointer to next byte in comm. buffer
*
******************************************************************************/

FMSTR_BPTR FMSTR_CopyFromBuffer(FMSTR_ADDR nDestAddr, FMSTR_BPTR pSrcBuff, FMSTR_SIZE8 nSize)
{
    FMSTR_U8* ps = (FMSTR_U8*) pSrcBuff;
    FMSTR_U8* pd = (FMSTR_U8*) nDestAddr;

    while(nSize--)
        *pd++ = *ps++;

    return (FMSTR_BPTR) ps;
}


/**************************************************************************//*!
*
* @brief  Read-out memory from communication buffer, perform AND-masking
*
* @param  nDestAddr - destination memory address
* @param  pSrcBuff  - source memory in communication buffer, mask follows data
* @param  nSize     - buffer size (always in bytes)
*
******************************************************************************/

void FMSTR_CopyFromBufferWithMask(FMSTR_ADDR nDestAddr, FMSTR_BPTR pSrcBuff, FMSTR_SIZE8 nSize)
{
    FMSTR_U8* ps = (FMSTR_U8*) pSrcBuff;
    FMSTR_U8* pd = (FMSTR_U8*) nDestAddr;
    FMSTR_U8* pm = ps + nSize;
    FMSTR_U8 mask, stmp, dtmp;

    while(nSize--)
    {
        mask = *pm++;
        stmp = *ps++;
        dtmp = *pd;

        /* perform AND-masking */
        stmp = (FMSTR_U8) ((stmp & mask) | (dtmp & ~mask));

        /* put the result back */
        *pd++ = stmp;
    }
}
#endif /* !(FMSTR_DISABLE) */
