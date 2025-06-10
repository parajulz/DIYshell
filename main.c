#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARGS 64

void parse_command(char *input, char **args, int *is_background) {
    int i = 0;
    *is_background = 0;

    args[i] = strtok(input, " ");
    while (args[i] != NULL) {
        if (strcmp(args[i], "&") == 0) {
            *is_background = 1;
            args[i] = NULL;
            break;
        }
        i++;
        args[i] = strtok(NULL, " ");
    }
}

int main() {
    char input[1024];

    while (1) {
        char *line = readline("ANUSKAsShell> ");
        if (!line) break;
        if (*line) add_history(line);
        strncpy(input, line, sizeof(input));
        free(line);

        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "exit") == 0) break;

        if (strncmp(input, "cd ", 3) == 0) {
            char *dir = input + 3;
            if (chdir(dir) != 0) perror("cd failed");
            continue;
        }

        char *pipe_parts[2];
        pipe_parts[0] = strtok(input, "|");
        pipe_parts[1] = strtok(NULL, "|");

        if (pipe_parts[1]) {
            int pipefd[2];
            pipe(pipefd);
            pid_t p1 = fork();
            if (p1 == 0) {
                dup2(pipefd[1], STDOUT_FILENO);
                close(pipefd[0]); close(pipefd[1]);

                char *args1[MAX_ARGS];
                parse_command(pipe_parts[0], args1, &(int){0});
                execvp(args1[0], args1);
                perror("exec1 failed"); exit(1);
            }
            pid_t p2 = fork();
            if (p2 == 0) {
                dup2(pipefd[0], STDIN_FILENO);
                close(pipefd[1]); close(pipefd[0]);

                char *args2[MAX_ARGS];
                parse_command(pipe_parts[1], args2, &(int){0});
                execvp(args2[0], args2);
                perror("exec2 failed"); exit(1);
            }
            close(pipefd[0]); close(pipefd[1]);
            waitpid(p1, NULL, 0);
            waitpid(p2, NULL, 0);
            continue;
        }

        char *out = strstr(input, ">");
        char *in = strstr(input, "<");
        int out_fd = -1, in_fd = -1;
        if (out) {
            *out = 0;
            char *filename = strtok(out + 1, " ");
            out_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (in) {
            *in = 0;
            char *filename = strtok(in + 1, " ");
            in_fd = open(filename, O_RDONLY);
        }

        pid_t pid = fork();
        if (pid == 0) {
            if (out_fd != -1) {
                dup2(out_fd, STDOUT_FILENO);
                close(out_fd);
            }
            if (in_fd != -1) {
                dup2(in_fd, STDIN_FILENO);
                close(in_fd);
            }
            char *args[MAX_ARGS];
            int bg;
            parse_command(input, args, &bg);
            execvp(args[0], args);
            perror("exec failed");
            exit(1);
        } else {
            int bg;
            char *args[MAX_ARGS];
            parse_command(input, args, &bg);
            if (!bg) {
                waitpid(pid, NULL, 0);
            }
        }
    }

    return 0;
}
