#ifndef SHELL_H_
#define SHELL_H_

#define NO_CMD          -1
#define HELP_CMD        0
#define DATE_CMD        1
#define TIME_CMD        2
#define SLEEP_CMD       3
#define CLEAR_CMD       4
#define BEEP_CMD        5
#define DOOR_CMD        6
#define DIV_ZERO_CMD    7
#define INV_OP_CMD      8
#define EXIT_CMD        9

#define BACKGROUND      0
#define FOREGROUND      1

#define STDIN           0
#define STDOUT          1
#define STDERROR        2

#define PROMPT_STRING   "mjj@itba.edu.ar $ "
#define BEEP_FREQ       400
#define DOOR_FREQ       600

void initShell();
void initScreen();
int getCommand(char * input, int * param1, int * param2);
void executeCommand(int command, int param1, int param2);

// Si una funcion que no deberia recibir parametros los recibe, los ignora
// Si una funcion que debia recibir parametros no los recibe, no se ejecuta

void help_cmd(int param1, int param2);
void date_cmd(int param1, int param2);
void time_cmd(int param1, int param2);
void sleep_cmd(int param1, int param2);
void clear_cmd(int param1, int param2);
void beep_cmd(int param1, int param2);
void door_cmd(int param1, int param2);
void div_zero_cmd(int param1, int param2);
void inv_op_cmd(int param1, int param2);
void exit_cmd(int param1, int param2);

void mem_cmd(int param1, int param2);
void ps_cmd(int param1, int param2);
void kill_cmd(int param1, int param2);
void block_cmd(int param1, int param2);
void nice_cmd(int param1, int param2);

#endif /* SHELL_H_ */