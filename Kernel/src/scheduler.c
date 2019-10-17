#include <scheduler.h>
#include <time.h>

Process first;
Process second;
int aux = 1;
int go = 0;

uint64_t scheduler(uint64_t sp) {
    timer_handler(); // DONDE HAGO PARA PODER HACER LOS SLEEPS?? HAY OTRO TIMER?

    if (go != 0) {
        if (aux == 1) {
            first.sp = sp;
            return second.sp;
        } else {
            second.sp = sp;
            return first.sp;
        }
    }

    return sp;
}

void setFirst (Process p) {
    first = p;
}

void setSecond (Process p) {
    second = p;
}

void setGo () {
    go = 1;
}