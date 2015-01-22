#ifndef TIMERS_H
#define TIMERS_H

#define USE_PIT (0)

void Init_PIT(unsigned period0,unsigned period1);
void Start_PIT0(void);
void Stop_PIT0(void);
void Start_PIT1(void);
void Stop_PIT1(void);

void Init_TPM(unsigned period_ms);
void Start_TPM(void);

void Init_LPTMR(void);
void Start_LPTMR(void);
void Stop_LPTMR(void);

#endif
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
