
/***************************************************
                    Sound.h
****************************************************/

#ifndef IOHANDLER_H_
#define IOHANDLER_H_

#include <lib.h>
#include <stdint.h>

/* Plays a sound given a frequency */
void play_sound(uint32_t frequency);

/* Stop sound */
void no_sound();

#endif /* IOHANDLER_H_ */