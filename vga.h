#ifndef H_TOS_VGA
#define H_TOS_VGA

#include <stdint.h>

// Initialize 60hz 640x480 16-color video mode
void VGA_Init();

// Turn off screen
void VGA_TurnScreenOff();

// Turn on screen
void VGA_TurnScreenOn();

// Write a pixel to the display
void VGA_WritePixel(uint32_t X, uint32_t Y, uint8_t RedMask, uint8_t GreenMask, uint8_t BlueMask, uint8_t Value);

#endif // H_TOS_VGA