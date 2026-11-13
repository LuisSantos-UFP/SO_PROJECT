#include "header.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

//void execute_command_list(void){
//    command_list;
// }

int is_builtin(CMD *cmd) {
    if (cmd == NULL || cmd->name == NULL) {  return 0; }
    if (strcmp(cmd->name, "cd") == 0 || strcmp(cmd->name, "pwd") == 0 || strcmp(cmd->name, "echo") == 0 || strcmp(cmd->name, "exit") == 0 || strcmp(cmd->name, "help") == 0 || strcmp(cmd->name, "history") == 0) {
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
    if (cmd == NULL) { return;}
    char *destino = cmd->argv[1];
    if (destino == NULL) {
        destino = getenv("HOME");
        if (destino == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return;
        }
    }
    if (chdir(destino) != 0) { perror("cd failed"); }
    
}

void builtin_echo(CMD *cmd){
    if (cmd == NULL) { return; }
    for (int i = 1; cmd->argv[i] != NULL; i++) {
        if (i > 1) { printf(" "); }
        printf("%s", cmd->argv[i]);
    }
    printf("\n");
}

void builtin_history(void){
    HIST_ENTRY **h_list = history_list();

    if (h_list == NULL) { return; }
    for (int i = 0; h_list[i] != NULL; i++) {
        printf("%d: %s\n", i + history_base, h_list[i]->line);
    }

}

void builtin_exit(void) {
    exit(0);
}

void builtin_help(void) {
    printf("Available built-in commands:\n");
    printf("cd [directory] - Change the current directory to 'directory'.\n");
    printf("pwd - Print the current working directory.\n");
    printf("echo [text] - Print 'text' to the standard output.\n");
    printf("exit - Exit the shell.\n");
    printf("help - Display this help message.\n");
}


int run_builtin(CMD *cmd) {
    if (cmd == NULL || cmd->name == NULL) { return -1; }
    if (strcmp(cmd->name, "cd") == 0) {
        builtin_cd(cmd);
        return 0;
    } else if (strcmp(cmd->name, "pwd") == 0) {
        builtin_pwd();
        return 0;
    } else if (strcmp(cmd->name, "echo") == 0) {
        builtin_echo(cmd);
        return 0;
    } else if (strcmp(cmd->name, "exit") == 0) {
        builtin_exit();
        return 0;
    } else if (strcmp(cmd->name, "help") == 0) {
        builtin_help();
        return 0;
    } else if (strcmp(cmd->name, "history") == 0) {
        builtin_history();
        return 0;
    }
    return -1; 
}

static int redirect_fd(const char *path, int flags, mode_t mode, int target) {
    int fd = open(path, flags, mode);
    if (fd == -1) {
        perror(path);
        return -1;
    }
    if (dup2(fd, target) == -1) {
        perror("dup2 failed");
        close(fd);
        return -1;
    }
    close(fd);
    return 0;
}

int redirections(CMD *cmd) {
    if (cmd == NULL) { return -1; }
    if (cmd->infile != NULL) {
        if (redirect_fd(cmd->infile, O_RDONLY, 0, STDIN_FILENO) == -1) {
            return -1;
        }
    }
    if (cmd->outfile != NULL) {
        if (redirect_fd(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644, STDOUT_FILENO) == -1) {
            return -1;
        }
    }
    if (cmd->errfile != NULL) {
        if (redirect_fd(cmd->errfile, O_WRONLY | O_CREAT | O_TRUNC, 0644, STDERR_FILENO) == -1) {
            return -1;
        }
    }
    return 0;
}

void execute_simple(CMD *cmd){ 
    if (cmd == NULL || cmd->name == NULL) {
        return;
    }

    if (is_builtin(cmd)) {
        run_builtin(cmd);
        return;
    }

    pid_t pid = fork();
    
    if (pid < 0) { perror("Fork failed"); return; }
 
    if (pid == 0) { // Processo filho
        redirections(cmd);
        execvp(cmd->name, cmd->argv);
        perror("Exec failed");
        exit(EXIT_FAILURE);
    } else { // Processo pai
        int status;
        if (cmd->background) {
            printf("O processo %d esta a correr em background\n", pid);
            return; // Não espera pelo processo filho
        }
        else if (waitpid(pid, &status, 0) == -1) {
            perror("Waitpid failed");
            return;
        }
    }
}