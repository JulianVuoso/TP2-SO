// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clib.h>
#include <shell.h>
#include <utils.h>

static char * command_strings[] = {"help", "date", 
                                        "time", "sleep", "clear", "beep", "door", "div_zero", 
                                        "inv_op", "exit", "mem", "ps", "kill", "block", "nice", 
                                        "loop", "cat", "wc", "filter", "sem", "pipe", "phylo"};
static int command_count = 22;
void (* command_functions[]) (int argc, char * argv[], int ground, int inFd, int outFd) = {help_cmd, date_cmd, 
                                            time_cmd, sleep_cmd, clear_cmd, beep_cmd, door_cmd, div_zero_cmd, 
                                            inv_op_cmd, exit_cmd, mem_cmd, ps_cmd, kill_cmd, block_cmd, nice_cmd,
                                            loop_cmd, cat_cmd, wc_cmd, filter_cmd, sem_cmd, pipe_cmd, phylo_cmd};

static int getCommandIndex(char * input, int * cursor);
int pipeNumber = 0;

// MAX_LENGTH esta en clib.h

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

    while (command != EXIT_CMD) {
        puts(PROMPT_STRING);
        gets(input, MAX_LENGTH);
        if ((command = getCommand(input)) == NO_CMD)
            puts("\nInvalid command");

        // executeCommand(command, param1, param2);
        if (command != CLEAR_CMD) newLine();
    }
	
	exit();
}

void initScreen() {
    clearScreen();
    puts("Bienvenido al programa. El comando help lo ayudara\n");
}

int getCommand(char * input){
    int cursor = 0;
    char vec[MAX_LENGTH] = {0};
    int j = 0;
    for (j = 0; j < MAX_LENGTH; j++){
        vec[j] = 0;
    }

    int command = NO_CMD;
    int nextStdin = STDIN;
    int nextCmd = 0;
    do {
        nextCmd = 0;
        command = getCommandIndex(input, &cursor);
        if (command == NO_CMD)
            return command;

        char arg1[MAX_LENGTH] = {0}, arg2[MAX_LENGTH] = {0};
        char * argv[] = {arg1, arg2, 0};
        int argc = 0;
        int haveArgs = 0;
        do {
            haveArgs = strcpyUntilSpace(vec, input + cursor);
            cursor += haveArgs + 1;
            if (haveArgs != 0) {
                switch (vec[0]) {
                    case '&':
                        // Creo el proceso en background y salgo
                        command_functions[command](argc, argv, BACKGROUND, nextStdin, STDOUT);
                        haveArgs = 0;
                        break;
                    case '|':
                        // Creo un pipe. Creo un proceso con stdout = pipe y actualizo nextStdin.
                        // Llamo nuevamente a esta funcion con el nextStdin
                        nextCmd = 1;
                        char pipeName[20] = "/pipeCmd_";
                        char auxBuf[5];
                        itoa(pipeNumber++, auxBuf, 10);
                        int aux = newPipe(strcat(pipeName, auxBuf));
                        command_functions[command](argc, argv, FOREGROUND, nextStdin, aux);
                        nextStdin = aux;
                        haveArgs = 0;
                        break;
                    default:
                        // Guardo el parametro en argv y sumo en argc.
                        if (argc < 2)
                            strcpy(argv[argc++], vec);
                        break;
                }
            } else { // Ejecuto comando/proceso con nextStdin
                command_functions[command](argc, argv, FOREGROUND, nextStdin, STDOUT);
            }
        } while (haveArgs != 0);
    } while (nextCmd != 0);

    return command;
        
        // exec_cmd(int argc, char * argv[], ground, stdin, stdout)

    // Agregar que los comandos reciban el fd de salida (stdin no hace falta)

}

// Create Process (entrypoint, name, ground, inalias, outalias, argc, argv)

// toDo: TODO LO QUE SE EJECUTA EN BACKGROUND NO PUEDE LEER DE STDIN

static int getCommandIndex(char * input, int * cursor) {
    int initCursor = *cursor;
    int i = 0;
    for (i = 0; i < command_count; i++) {
        if (strcmpUntilSpace(input, command_strings[i], cursor) == 0){
            (*cursor)++;
            return i;
        }
        *cursor = initCursor;
    }
    return NO_CMD;
}

// void executeCommand(int command, int param1, int param2) {
//     if (command != NO_CMD) {
//         command_functions[command](param1, param2);
//     }
//     else
//         puts("\nInvalid command");
// }

void help_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    int i;
    for (i = 0; i < argc; i++) {
        printf("\n\tARG: %s\n", argv[i]);
    }

    putsFd("\nLos comandos validos son los siguientes: ", outFd);
    putsFd("\nhelp ~ Muestra los comandos validos", outFd);
    putsFd("\ndate ~ Muestra la fecha actual", outFd);
    putsFd("\ntime ~ Muestra la hora actual", outFd);
    putsFd("\nsleep ~ Frena el funcionamiento un numero de segundos a ingresar", outFd);
    putsFd("\nclear ~ Limpia la consola", outFd);
    putsFd("\nbeep ~ Emite un sonido", outFd);
    putsFd("\ndoor ~ Hay alguien en la puerta", outFd);
    putsFd("\ndiv_zero ~ Ejecuta una division por cero", outFd);
    putsFd("\ninv_op ~ Ejecuta una operacion de codigo invalido", outFd);

    putsFd("\nmem ~ Imprime el estado de la memoria", outFd);
    putsFd("\nps ~ Imprime la lista de todos los procesos con sus propiedades", outFd);
    putsFd("\nkill ~ Mata al proceso de ID recibido", outFd);
    putsFd("\nnice ~ Cambia la prioridad del proceso de ID recibido a la prioridad recibida", outFd);
    putsFd("\nblock ~ Cambia el estado del proceso de ID recibido entre BLOCKED y READY", outFd);
    putsFd("\nloop ~ Ejecuta un proceso que imprime su ID con un saludo cada 3s en un loop", outFd);
    putsFd("\ncat ~ Ejecuta un proceso que imprime el STDIN tal como lo recibe", outFd);
    putsFd("\nwc ~ Ejecuta un proceso que cuenta la cantidad de lineas del input", outFd);
    putsFd("\nfilter ~ Ejecuta un proceso que filtra las vocales del input", outFd);
    putsFd("\nsem ~ Imprime la lista de todos los semaforos con sus propiedades", outFd);
    putsFd("\npipe ~ Imprime la lista de todos los pipes con sus propiedades", outFd);
    putsFd("\nphylo ~ Ejecuta un proceso que implementa el problema de los filosofos comensales", outFd);

    putsFd("\nexit ~ Termina la ejecucion", outFd);
}

void date_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    char date[11];
    printfFd(outFd, "\nHoy es %s", getDate(date));
}

void time_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    char time[9];
    printfFd(outFd, "\nSon las %s", getTime(time));
}

void sleep_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    int seconds;
    if (argc < 1 || (seconds = atoi(argv[0])) < 0) {
        puts("\nIngreso invalido. Debe ingresar el numero de segundos que desea esperar como primer argumento.");
    } else {        
        int millis = seconds * 1000;
        // sleep(millis);
        int pid = newProcess("SLEEP", argc, argv, ground, inFd, outFd);
        if (ground == BACKGROUND)
            printf("\nCreate %s. PID = %d", (pid == 0) ? "unsuccesfull":"successfull", pid);
    }
}

void clear_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    clearScreen();
}

void beep_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    beep(BEEP_FREQ, 300);
}

void door_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
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

void div_zero_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    int a = 10, b = 0;
    a = a / b;
    printf("%d", a);
}

void inv_op_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    uint64_t invalid = 0xFFFFFFFFFFFF;
	uint64_t * ptr = &invalid;
	((void(*)())ptr)();
}

void exit_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    putsFd("\nHasta Luego", outFd);
}

/* --------------------------------------- */

void mem_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    memStatus();
}

void ps_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    ps();
}

void kill_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    int pid;
    if (argc < 1 || (pid = atoi(argv[0])) < 0) {
        puts("\nIngreso invalido. Debe ingresar el ID del proceso que desea eliminar como primer argumento.");
    } else {
        if (pid <= 1) {
            puts("\nDelete unsuccesfull. Shell and Idle can't be killed.");
            return;
        }
        int ret = kill(pid); // Devuelve el PID del proceso o 0 si no se borro
        printf("\nDelete %s", (ret == 0) ? "unsuccesfull":"successfull");
    }
}

void block_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    int pid;
    if (argc < 1 || (pid = atoi(argv[0])) < 0) {
        puts("\nIngreso invalido. Debe ingresar el ID del proceso que desea cambiar de estado como primer argumento.");
    } else {
        int ret = changeState(pid); // Devuelve 0 si sale bien, 1 sino
        printf("\nChange %s", (ret != 0) ? "unsuccesfull":"successfull");
    }
}

void nice_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    int pid, priority;
    if (argc < 2 || (pid = atoi(argv[0])) < 0 || (priority = atoi(argv[1])) < 0) {
        puts("\nIngreso invalido. Debe ingresar el ID del proceso que desea cambiar de estado como primer argumento y la nueva prioridad del \nproceso como segundo argumento.");
    } else {
        if (pid <= 1) {
            puts("\nDelete unsuccesfull. Shell and Idle can't be modified.");
            return;
        }
        int ret = setPriority(pid, priority); // Devuelve 0 si sale bien, 1 si no encuentra pid, 2 si error en prioridad
        switch (ret)
        {
            case 1:
                puts("\nPID Invalido.");
                break;
            case 2:
                puts("\nPrioridad Invalida.");
                break;
            default: // case 0
                puts("\nChange successfull");
                break;
        }
    }
}

void loop_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {     
    int pid = newProcess("LOOP", argc, argv, ground, inFd, outFd);
    if (ground == BACKGROUND)
        printf("\nCreate %s. PID = %d", (pid == 0) ? "unsuccesfull":"successfull", pid);
}

void cat_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    if (ground == BACKGROUND) {
        puts("\nNo puedes correr este comando en background! Requiere ingreso por STDIN.");
    } else {
        newProcess("CAT", argc, argv, ground, inFd, outFd);
    }
}

void wc_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    if (ground == BACKGROUND) {
        puts("\nNo puedes correr este comando en background! Requiere ingreso por STDIN.");
    } else {
        newProcess("WC", argc, argv, ground, inFd, outFd);
    }
}

void filter_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    if (ground == BACKGROUND) {
        puts("\nNo puedes correr este comando en background! Requiere ingreso por STDIN.");
    } else {
        newProcess("FILTER", argc, argv, ground, inFd, outFd);
    }
}

void sem_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    sem_status();
}

void pipe_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    pipe_status();
}

void phylo_cmd(int argc, char * argv[], int ground, int inFd, int outFd) {
    if (ground == BACKGROUND) {
        puts("\nNo puedes correr este comando en background! Requiere ingreso por STDIN.");
    } else {
        newProcess("PHYLO", argc, argv, ground, inFd, outFd);
    }
}