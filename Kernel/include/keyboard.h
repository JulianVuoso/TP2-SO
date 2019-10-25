/***************************************************
                Keyboard.h
****************************************************/

#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define BUFFER_SIZE 50

#define UP_KEY  24
#define LEFT_KEY  27
#define RIGHT_KEY  26
#define DOWN_KEY  25

/* Keyboard handler function */
void keyboard_handler();

/* Returns the car in Upper */
char toUpper(char car);

#endif