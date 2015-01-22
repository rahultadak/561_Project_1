#ifndef ADC_H
#define ADC_H

#define VBG_VALUE (1.00)
/* Function prototypes */
#ifdef __cplusplus
  extern "C" {
#endif
	void Init_ADC(void);
	float Measure_VRail(void);
	
	//tmp
	extern int res;
#ifdef __cplusplus
  }
#endif
#endif
