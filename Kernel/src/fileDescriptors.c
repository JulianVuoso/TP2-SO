#include <fileDescriptors.h>

int newFd(char * name);
void write(int fd, char * buffer, int count);
void read(int fd, char * buffer, int count);
void remove(int fd);

