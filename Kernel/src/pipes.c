// #include <fileDescriptors.h>
// #include <process.h>
// #include <memoryManager.h>
// #include <strings.h>

// #include <pipes.h>

// // DEBO MANTENER LISTADO DE PIPES
// static NodePipe * first = 0;

// static void addPipeNode(NodeFd * fd, pipeState state);
// static NodePipe * searchName(char * name);
// static NodePipe * searchFd(int fd);

// // ToDo: ver si cambiar el error a -1 (si es que se puede)
// /* Create new named Pipe. Returns 0 if error */
// int newPipe(char * name) {
//     /* Check if name already exists */
//     if (searchName(name) != 0) return 0;

//     /* Add node to fileDescriptors list */
//     NodeFd * node = newFd(name);
//     if (node == 0) return 0;
//     int fd = node->fd.fd;

//     /* Add node to process fileDescriptors list */
//     addFd(fd);

//     /* Add node to pipes list */
//     addPipeNode(node, OPEN);

//     return fd;
// }

// /* Opens an existing Pipe. Returns 0 if error */
// int openPipe(char * name) {
//     /* Check if the pipe exists */
//     NodePipe * pipeNode = searchName(name);
//     if (pipeNode == 0) return 0;
//     if (pipeNode->fdNode == 0) return 0;

//     /* Get node fd number */
//     int fd = pipeNode->fdNode->fd.fd;
    
//     /* Add node to process fileDescriptors list */
//     addFd(fd);

//     /* Save new state in pipes list */
//     pipeNode->state = OPEN;

//     return fd;
// }

// /* Closes an existing Pipe */
// void closePipe(int fd) {
//     /* Check if the pipe exists */
//     NodePipe * pipeNode = searchFd(fd);
//     if (pipeNode == 0) return 0;
//     if (pipeNode->fdNode == 0) return 0;

//     /* Get node fd number */
//     int fd = pipeNode->fdNode->fd.fd;

//     /* Send EOF signal to pipe */
//     char aux = EOF;
//     write(fd, &aux, 1);

//     /* Remove node from fileDescriptors list */
//     // removeFdList(fd);

//     /* Remove node from process fileDescriptors list */
//     removeFd(fd);

//     /* Save new state in pipes list */
//     pipeNode->state = CLOSED;
// }

// // GUARDAR EL NOMBRE DE LOS PIPES APARTEEEE

// /* Prints all Pipes */
// void showAllPipes() {
//     /* If there are no pipes */
//     if (first == 0) {
//         print("\tThere are no Pipes created");
//         return;
//     }
    
//     NodePipe * iterator = first;
//     print("\nName\t\tState\t\tBlocked Processes\n");
//     while (iterator != 0) {
//         if (iterator->fdNode != 0) print(iterator->fdNode->fd.name); print("\t\t"); 
//         print((iterator->state == OPEN) ? "Open" : "Closed"); print("\t\t");
//         printNodeSemaphores(iterator->fdNode); print("\n");
//         iterator = iterator->next;
//     }
// }

// /* Add node to pipes list */
// static void addPipeNode(NodeFd * fd, pipeState state) {
//     if (state != OPEN && state != CLOSED) return;
//     NodePipe * node = (NodePipe *) malloc(sizeof(NodePipe));
//     if (node == 0) return;
//     node->fdNode = fd;
//     node->state = state;
//     if (first == 0) {
//         node->next = 0;
//         first = node;
//     } else {
//         node->next = first;
//         first = node;
//     }
// }

// /* Search if the fd with name exists */
// static NodePipe * searchName(char * name) {
//     NodePipe * aux = first;
//     while(aux != 0){
//         if (aux->fdNode != 0 && stringcmp(aux->fdNode->fd.name, name))
//             return aux;
//         aux = aux->next;  
//     } 
//     return 0;
// }

// /* Search for a node given its fd */
// static NodePipe * searchFd(int fd) {
//     NodePipe * aux = first;
//     while(aux != 0){
//         if (aux->fdNode != 0 && aux->fdNode->fd.fd == fd)
//             return aux;
//         aux = aux->next;  
//     } 
//     return 0;
// }
