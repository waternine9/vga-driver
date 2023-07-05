#ifndef H_VGA_DRIVER
#define H_VGA_DRIVER

#include <stdint.h>

// Initialize 60hz 640x480 16-color video mode
void VGA_Init();

// Turn off screen
void VGA_TurnScreenOff();

// Turn on screen
void VGA_TurnScreenOn();

// Set current draw color
void VGA_SetColor(uint8_t Red, uint8_t Green, uint8_t Blue, uint8_t Alpha);

// Write a pixel to the display
void VGA_WritePixel(uint32_t X, uint32_t Y, uint8_t Value);

// Write a rectangle to the display
void VGA_WriteRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h);

// Clear the display
void VGA_Clear();

#endif // H_VGA_DRIVER
