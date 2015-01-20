/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG
#define DEBUG_PRINT

/*
 * Include the generic CPU header file

 */
#include "derivative.h"

/*
 * Include the platform specific header file
 */
#if (defined(FREEDOM))
  #include "board.h"
#else
  #error "No valid platform defined"
#endif

/*
 * Include any toolchain specfic header files
 */
#if (defined(CW))
  #include "cw.h"
#elif (defined(IAR))
  /* Nothing */
#endif

#if defined(CPU_MKL25Z128LK4) || defined(CPU_MKL46Z48M)
  #include "mcg_kl25.h"
#elif defined(CPU_MKL05Z32LF4)
  #include "mcg_kl05.h"
#elif defined(CPU_MKL02Z32LF4)
  #include "mcg_kl02.h"
#else
  #error "No mcg init file selected"
#endif

/*
 * Include common utilities
 */
#include "startup.h"
#include "stdlib.h"

#if (defined(IAR))
  #include "intrinsics.h"
#endif

/********************************************************************/

#endif /* _COMMON_H_ */
