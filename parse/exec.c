#include "header.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>

//void execute_command_list(void){
//    command_list;
// }

int is_builtin(CMD *cmd) {
    if (cmd == NULL || cmd->name == NULL) {  return 0; }
    if (strcmp(cmd->name, "cd") == 0 || strcmp(cmd->name, "pwd") == 0) {
        return 1;
    }
    return 0;
}

void builtin_pwd(void) {
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd() error");
    } else {
        printf("%s\n", cwd);
    
    }
}

void builtin_cd(CMD *cmd) {
    if (cmd == NULL || cmd->argv[1] == NULL) { fprintf(stderr, "cd: missing argument\n");
        return;
    }
    if (chdir(cmd->argv[1]) != 0) { perror("cd failed"); }
    
}

int run_builtin(CMD *cmd) {
    if (cmd == NULL || cmd->name == NULL) { return -1; }
    if (strcmp(cmd->name, "cd") == 0) {
        builtin_cd(cmd);
        return 0;
    } else if (strcmp(cmd->name, "pwd") == 0) {
        builtin_pwd();
        return 0;
    }
    return -1; 
}

void execute_simple(CMD *cmd){ 
    if (cmd == NULL || cmd->name == NULL) {
        fprintf(stderr, "Invalid command\n");
        return;
    }

    if (is_builtin(cmd)) {
        run_builtin(cmd);
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