#ifndef PROJECT_1_H
#define PROJECT_1_H

#ifdef __cplusplus
  extern "C" {
#endif
  void FadeIn(int start,int end, int delay);
  void FadeOut(int start,int end, int delay);
	void SetLEDs(int R, int G, int B);
	
	//Variable to globally share the brightness that is set by the slider
	extern uint8_t SliderPos;
	extern uint16_t step_delay;
	extern int LED_on;
#ifdef __cplusplus
  }
#endif
#endif
