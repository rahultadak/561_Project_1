#ifndef PROJECT_1_H
#define PROJECT_1_H

#ifdef __cplusplus
  extern "C" {
#endif
  void FadeIn(int start,int end, int delay);
  void FadeOut(int start,int end, int delay);
	void SetLEDs(int R, int G, int B);
	void LowBattWarn_ON(void);
	uint8_t LowBattCheck(void);
		
	//Variable to globally share the brightness that is set by the slider
	extern uint8_t SliderPos,LED_on,LowBattery,LowBattWarned;
	extern uint16_t step_delay;
	extern uint8_t BoardTilted;
#ifdef __cplusplus
  }
#endif
#endif
