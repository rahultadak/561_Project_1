FRDMKLXX_DEMO
==================================================================

This folder contains demo presenting TSS with the ARM®Cortex™-M0+
Kinetis L plaforms KL02Z32, KL05Z32, KL25Z128 and KL26Z128 on 
the FRDM-KL02Z, FRDM-KL05Z, FRDM-KL25Z and FRDM-KL26Z evaluation boards. 
The demo presents touch sensing function by 2 electrodes arranged 
to single ANALOGUE SLIDER control. The sensing is performed by 
TSI module inside MCU, except FRDM-KL02Z where GPIO method is used.

Features & Information:
------------------------------------------------------------------
- FRDM-KL02Z evaluation board, KL02Z32
- FRDM-KL05Z evaluation board, KL05Z32
- FRDM-KL25Z evaluation board, KL25Z128
- FRDM-KL26Z evaluation board, KL26Z128
- CodeWarrior for MCU v 10.3 or higher supported
- IAR Embedded Workbench for ARM Version 6.50.2 or higher supported
- KEIL uVision 4.71.2 or higher supported
- TSI module performing sensing function
- FreeMASTER Visualization GUI supported via serial connection provided by 
  Open SDA virtual serial connection
- Open SDA debug interface available
- JTAG programing port available for J-Link Connection
- Wakeup from Low Power MCU mode function presented
- Triggering function

Demo Control
------------------------------------------------------------------
- The touch position on the analog slider control touch is indicated 
  by intensity of onboard tri-color LED. 
- The software automatically enters to Low Power mode after 8 seconds 
  of user's no activity. If user touch the control then MCU is woken up. 

Demo Settings
------------------------------------------------------------------

main.h
- #define APP_TSS_USE_DCTRACKER 1/0 - Enable DC tracker for whole demo
- #define APP_TSS_USE_TRIGGER_MODE ALWAYS/SW/AUTO - Enable required 
  TSS trigger mode for whole demo (ALWAYS mode = ~1ms period, 
  SW mode = set 20ms period, AUTO mode = set 10ms period 
  for presentation)
TSS_SystemSetup.h  
- User can select keydetector version by TSS_USE_KEYDETECTOR_VERSION macro 
  in TSS_SystemSetup.h
- User can select if sensitivity autocalibration is used by
  TSS_USE_AUTO_SENS_CALIBRATION macro for whole demo
  
FreeMASTER
------------------------------------------------------------------
Connection options:
- Open SDA virtual serial connection, speed 115200 bauds

Notes
------------------------------------------------------------------
FRDM-KL02Z example application uses GPIO measurement method, 
because KL02 MCU does not provide TSI module. Therefore AUTO
trigger mode is not presented in the example.

References
------------------------------------------------------------------
FRDM-KL02Z48M User's Manual (Document TWR-KL02Z-UM)
FRDM-KL05Z48M User's Manual (Document TWR-KL05Z-UM)
FRDM-KL25Z48M User's Manual (Document TWR-KL25Z-UM)
FRDM-KL46Z48M User's Manual (Document TWR-KL46Z-UM)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
