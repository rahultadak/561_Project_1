#ifndef LEDS_H
#define LEDS_H

// Freedom KL25Z LEDs
#define RED_LED_POS (18)		// on port B
#define GREEN_LED_POS (19)	// on port B
#define BLUE_LED_POS (1)		// on port D
#define MASK(x) (1UL << (x)) //Make a mask with single bit at position x

// function prototypes
void Init_RGB_LEDs(void);
void Control_RGB_LEDs(unsigned int red_on, unsigned int green_on, unsigned int blue_on);
void Toggle_RGB_LEDs(unsigned int red, unsigned int green, unsigned int blue);

#endif