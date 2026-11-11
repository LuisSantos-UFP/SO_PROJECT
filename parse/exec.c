#include "header.h"
#include <sys/wait.h>
#include <unistd.h>

//void execute_command_list(void){
//    command_list;
// }

void execute_simple(CMD *cmd){ 
    if (cmd == NULL || cmd->name == NULL) {
        fprintf(stderr, "Invalid command\n");
        return;
    }

    pid_t pid = fork();
    
    if (pid < 0) { perror("Fork failed"); exit(EXIT_FAILURE); }

    if (pid == 0) { // Processo filho
        execvp(cmd->name, cmd->argv);
        perror("Exec failed");
    } else { // Processo pai
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("Waitpid failed");
            exit(EXIT_FAILURE);
        }
    }
}