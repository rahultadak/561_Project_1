/*
 * File:        sysinit.c
 * Purpose:     Kinetis L Family Configuration
 *              Initializes processor to a default state
 *
 * Notes:
 *
 *
 */

#include "common.h"
#include "sysinit.h"
#include "uart_kl25.h"

/********************************************************************/

/* Actual system clock frequency */
int mcg_clk_hz;

/********************************************************************/

void clk_out_init(void)
{

          /* Enable the CLKOUT function on PTC3 (alt5 function) */
	  PORTC_PCR3 = ( PORT_PCR_MUX(0x5));

          /* Select the CLKOUT in the SMI_SOPT2 mux to be bus clk*/
          SIM_SOPT2 |= SIM_SOPT2_CLKOUTSEL(2);

}

/********************************************************************/

void sysinit (void)
{
        /* Setup Allowed Power modes */
        SMC_PMPROT = (SMC_PMPROT_AVLP_MASK
                      | SMC_PMPROT_ALLS_MASK
                      | SMC_PMPROT_AVLLS_MASK);

        /* Read RCM for identification of Chip Reset reason */
        if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK)
        {
          /* Releases hold with ACKISO:  Only has an effect if recovering from VLLS1, VLLS2, or VLLS3
             if ACKISO is set you must clear ackiso before calling pll_init
             or pll init hangs waiting for OSC to initialize
             if osc enabled in low power modes - enable it first before ack
             if I/O needs to be maintained without glitches enable outputs and modules first before ack. */
          if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK)
          {
            PMC_REGSC |= PMC_REGSC_ACKISO_MASK;	/* Clear ACKISO bit */
            LLWU_ME &= ~LLWU_ME_WUME4_MASK; /* TSI LLWU Input Disable = LLWU_M4IF */
          }
        } else if (RCM_SRS0 & RCM_SRS0_LOC_MASK)
        {
          /* Loss of clock case */
        }

        /* Enable all of the port clocks. These have to be enabled to configure
         * pin muxing options, so most code will need all of these on anyway.
         */
        SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                      | SIM_SCGC5_PORTB_MASK
                      | SIM_SCGC5_PORTC_MASK
                      | SIM_SCGC5_PORTD_MASK
                      | SIM_SCGC5_PORTE_MASK );

        clk_out_init();

#if defined(NO_PLL_INIT)

        mcg_clk_hz = 21000000; //FEI mode

        SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // clear PLLFLLSEL to select the FLL for this clock source

#else
       /* Ramp up the system clock */
       /* Set the system dividers */
       /* NOTE: The PLL init will not configure the system clock dividers,
        * so they must be configured appropriately before calling the PLL
        * init function to ensure that clocks remain in valid ranges.
        */
        SIM_CLKDIV1 = ( 0
                        | SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV4(1) );

       /* Initialize PLL */
       /* PLL will be the source for MCG CLKOUT so the core, system, and flash clocks are derived from it */
       mcg_clk_hz = pll_init(CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                             LOW_POWER,     /* Set the oscillator for low power mode */
                             CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                             PLL0_PRDIV,    /* PLL predivider value */
                             PLL0_VDIV,     /* PLL multiplier */
                             MCGOUT);       /* Use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
       if (mcg_clk_hz < 0x100)
         while(1);

       SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; // set PLLFLLSEL to select the PLL for this clock source

#endif
}

/********************************************************************/
