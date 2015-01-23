#ifndef TIMERS_H
#define TIMERS_H

#define USE_PIT (0)

void Init_PIT(unsigned period0,unsigned period1);
void Start_PIT0(void);
void Stop_PIT0(void);
void Start_PIT1(void);
void Stop_PIT1(void);
#endif
// *******************************ARM University Program Copyright © ARM Ltd 2013*************************************   
