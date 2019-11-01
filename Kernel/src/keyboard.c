// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <fileDescriptors.h>

#include <keyboard.h>

#include <process.h>

char buffer[BUFFER_SIZE];
unsigned int save_index = 0;
unsigned int read_index = 0;

static unsigned char map[90]={'`','\e','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
        'C','a','s','d','f','g','h','j','k','l',';','\'', 0, 'S','\\','z','x','c','v','b','n','m',',','.','/','S',0,0/*ALT*/,' ',
        'M',0,0,0,0,0,0,0,0,0,0,0,0,0,UP_KEY,0,0,LEFT_KEY,0,RIGHT_KEY,0,0,DOWN_KEY };

static unsigned char shift_map[90]={'~','\e','!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','q','w','e','r','t','y','u','i','o','p','{','}','\n',
        'C','a','s','d','f','g','h','j','k','l',':','\"', 0, 'S','|','z','x','c','v','b','n','m','<','>','?','S',0,0/*ALT*/,' ',
        'M',0,0,0,0,0,0,0,0,0,0,0,0,0,UP_KEY,0,0,LEFT_KEY,0,RIGHT_KEY,0,0,DOWN_KEY };

// ESC = \e
// BACK = \b
// SHIFT = S
// CAPS = M
// CTRL = C
// ALT = A
// UP = U
// LEFT = L
// RIGHT = R
// DOWN = D

// Solo chequeo si presione SHIFT o MAY, los demas pasan como vienen
void keyboard_handler() {
    static int caps = 1, shift = 1, release_caps = 1, ctrl = -1;
    unsigned char key = read_port(0x60);

    if ((key >= 90 && key < 0x80) || key >= 0x80 + 90)
        return;
        
    char car;
    if (key < 0x80){ // Aprieto tecla
        if (shift == 1)
            car = map[key];
        else
            car = shift_map[key];
        switch (car){
            case 'S':   
                shift = -1;
                break;
            case 'M':   
                if (release_caps){
                    caps *= -1;
                    release_caps = 0;
                }
                break;
            case 'C':
                ctrl = 1;
                break;
            default:
                if (ctrl == 1) {
                    if (car == 'c'){
                        // buffer[save_index % BUFFER_SIZE] = SIGINT;
                        car = SIGINT;
                        // write(0, buffer + save_index % BUFFER_SIZE, 1);
                        // write(0, &car, 1);
                        sigInt();
                        // save_index++;
                    } else if (car == 'd') {
                        // buffer[save_index % BUFFER_SIZE] = EOF;
                        car = EOF;
                        // write(0, buffer + save_index % BUFFER_SIZE, 1);
                        write(0, &car, 1);
                        // save_index++;
                    }
                } else {
                    if (shift * caps == -1){ // Uno de los dos activos, paso a mayus
                        // buffer[save_index % BUFFER_SIZE] = toUpper(car);
                        car = toUpper(car);
                    } else{
                        // buffer[save_index % BUFFER_SIZE] = car;
                    }
                    // write(0, buffer + save_index % BUFFER_SIZE, 1);
                    if (car != 0)
                        write(0, &car, 1);
                    // save_index++;
                }
                break;
        }
    } else { // Suelto tecla
        car = map[key - 0x80];
        switch (car){
            case 'S':
                shift = 1;
                break;
            case 'M':
                release_caps = 1;
                break;
            case 'C':
                ctrl = -1;
                break;
        }
    }
}

/* Returns the char in Upper */
char toUpper(char car){
    if (car >= 'a' && car <= 'z')
        car -= ('a' - 'A');
    return car;
}