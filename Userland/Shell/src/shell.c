// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clib.h>
#include <shell.h>
#include <utils.h>

static char * command_strings[] = {"help", "date", "time", "sleep", "clear", "beep", "door", "div_zero", 
                                        "inv_op", "exit", "mem", "ps", "kill", "block", "nice"};
static int command_count = 15;
void (* command_functions[]) (int p1, int p2) = {help_cmd, date_cmd, time_cmd, sleep_cmd, clear_cmd, beep_cmd, door_cmd, div_zero_cmd, 
                                            inv_op_cmd, exit_cmd, mem_cmd, ps_cmd, kill_cmd, block_cmd, nice_cmd};

#define MAX_LENGTH  100

static void newLine(){
    putchar('\n');
}

int main () {
    initShell();
    return 0;
}

void initShell() {
    initScreen();
    int command = NO_CMD;
    char input[MAX_LENGTH] = {0};
    int param1 = -1, param2 = -1;

    while (command != EXIT_CMD) {
        puts(PROMPT_STRING);
        gets(input, MAX_LENGTH);
        command = getCommand(input, &param1, &param2);
        executeCommand(command, param1, param2);
        if (command != CLEAR_CMD) newLine();
    }
	
	exit();
}

void initScreen() {
    clearScreen();
    puts("Bienvenido al programa. El comando help lo ayudara\n");
}

int getCommand(char * input, int * param1, int * param2){
    char vec1[MAX_LENGTH] = {0}, vec2[MAX_LENGTH] = {0};
    int j = 0;
    for (j = 0; j < MAX_LENGTH; j++){
        vec1[j] = 0;
        vec2[j] = 0;
    }
    int cursor = 0;
    for (int i = 0; i < command_count; i++) {
        if (strcmpUntilSpace(input, command_strings[i], &cursor) == 0){
            if (cursor >= 0) {
                cursor += strcpyUntilSpace(vec1, input + cursor + 1); // +1 por el space
                *param1 = atoi(vec1);
                cursor += strcpyUntilSpace(vec2, input + cursor + 2); // +2 por los dos spaces
                *param2 = atoi(vec2);
            } else {
                *param1 = -1;
                *param2 = -1;
            }
            return i;
        }
    }
    return NO_CMD;
}

void executeCommand(int command, int param1, int param2) {
    if (command != NO_CMD) {
        command_functions[command](param1, param2);
    }
    else
        puts("\nInvalid command");
}

void help_cmd(int param1, int param2) {
    puts("\nLos comandos validos son los siguientes: ");
    puts("\nhelp ~ Muestra los comandos validos");
    puts("\ndate ~ Muestra la fecha actual");
    puts("\ntime ~ Muestra la hora actual");
    puts("\nsleep ~ Frena el funcionamiento un numero de segundos a ingresar");
    puts("\nclear ~ Limpia la consola");
    puts("\nbeep ~ Emite un sonido");
    puts("\ndoor ~ Hay alguien en la puerta");
    puts("\ndiv_zero ~ Ejecuta una division por cero");
    puts("\ninv_op ~ Ejecuta una operacion de codigo invalido");

    puts("\nmem ~ Imprime el estado de la memoria");
    puts("\nps ~ Imprime la lista de todos los procesos con sus propiedades");
    puts("\nkill ~ Mata al proceso de ID recibido");
    puts("\nnice ~ Cambia la prioridad del proceso de ID recibido a la prioridad recibida");
    puts("\nblock ~ Cambia el estado del proceso de ID recibido entre BLOCKED y READY");

    puts("\nexit ~ Termina la ejecucion");
}

void date_cmd(int param1, int param2) {
    char date[11];
    printf("\nHoy es %s", getDate(date));
}

void time_cmd(int param1, int param2) {
    char time[9];
    puts("\nSon las  ");
	puts(getTime(time));
}

void sleep_cmd(int param1, int param2) {
    if (param1 < 0) {
        puts("\nIngreso invalido. Debe ingresar el numero de segundos que desea esperar como primer argumento.");
    } else {
        int millis = param1 * 1000;
        sleep(millis);
    }
}

void clear_cmd(int param1, int param2) {
    clearScreen();
}

void beep_cmd(int param1, int param2) {
    beep(BEEP_FREQ, 300);
}

void door_cmd(int param1, int param2) {
    beep(DOOR_FREQ, 300);
    sleep(300);
    beep(DOOR_FREQ, 150);
    sleep(150);
    beep(DOOR_FREQ, 150);
    sleep(100);
    beep(DOOR_FREQ, 150);
    sleep(250);
    beep(DOOR_FREQ, 150);
    sleep(800);
    beep(DOOR_FREQ, 150);
    sleep(150);
    beep(DOOR_FREQ, 150);
}

void div_zero_cmd(int param1, int param2) {
    int a = 10, b = 0;
    a = a / b;
    printf("%d", a);
}

void inv_op_cmd(int param1, int param2) {
    uint64_t invalid = 0xFFFFFFFFFFFF;
	uint64_t * ptr = &invalid;
	((void(*)())ptr)();
}

void exit_cmd(int param1, int param2) {
    puts("\nHasta Luego");
}

/* --------------------------------------- */

void mem_cmd(int param1, int param2) {
    memStatus();
}

void ps_cmd(int param1, int param2) {
    ps();
}

void kill_cmd(int param1, int param2) {
    if (param1 < 0) {
        puts("\nIngreso invalido. Debe ingresar el ID del proceso que desea eliminar como primer argumento.");
    } else {
        int ret = kill(param1); // Devuelve el PID del proceso o 0 si no se borro
        printf("\nDelete %s", (ret == 0) ? "unsuccesfull":"successfull");
    }
}

void block_cmd(int param1, int param2) {
    if (param1 < 0) {
        puts("\nIngreso invalido. Debe ingresar el ID del proceso que desea cambiar de estado como primer argumento.");
    } else {
        int ret = changeState(param1); // Devuelve 0 si sale bien, 1 sino
        printf("\nChange %s", (ret != 0) ? "unsuccesfull":"successfull");
    }
}

void nice_cmd(int param1, int param2) {
    if (param1 < 0 || param2 < 0) {
        puts("\nIngreso invalido. Debe ingresar el ID del proceso que desea cambiar de estado como primer argumento y la nueva prioridad del \nproceso como segundo argumento.");
    } else {
        int ret = setPriority(param1, param2); // Devuelve 0 si sale bien, 1 si no encuentra pid, 2 si error en prioridad
        switch (ret)
        {
            case 1:
                puts("\nPID Invalido.");
                break;
            case 2:
                puts("\nPrioridad Invalida. La prioridad debe estar entre 0 y 3.");
                break;
            default: // case 0
                puts("\nChange successfull");
                break;
        }
    }
}