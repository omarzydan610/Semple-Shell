#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_WORDS 50
#define MAX_INPUT_SIZE 200

char *vars[MAX_WORDS];
char *values[MAX_WORDS];
int currentVar = 0;

void setup_environment()
{
    chdir("/");
}

void evaluate_expression(char *input)
{
    char result[MAX_INPUT_SIZE] = "";
    int len = strlen(input);

    for (int i = 0; i < len; i++)
    {
        if (input[i] == '$')
        {
            int j = i + 1;
            while (input[j] != ' ' && input[j] != '\0')
                j++;

            int varLength = j - (i + 1);
            char var[varLength + 1];
            strncpy(var, input + i + 1, varLength);
            var[varLength] = '\0';

            int found = 0;
            for (int k = 0; k < currentVar; k++)
            {
                if (strcmp(vars[k], var) == 0)
                {
                    found = 1;
                    strncat(result, input, i);
                    strcat(result, values[k]);
                    strcat(result, input + j);
                    break;
                }
            }

            if (!found)
            {
                strncat(result, input, i + varLength + 1);
            }
            strcpy(input, result);
            return;
        }
    }
}

void parseInput(char *input, char *words[])
{
    for (int i = 0; i < MAX_WORDS; i++)
    {
        words[i] = NULL;
    }

    input[strcspn(input, "\n")] = '\0';

    if (strcmp(input, "exit") == 0)
    {
        words[0] = NULL;
        return;
    }

    char *token = strtok(input, " ");
    int i = 0;
    while (token != NULL && i < MAX_WORDS - 1)
    {
        words[i++] = token;
        token = strtok(NULL, " ");
    }
    words[i] = NULL;
}

void execute_shell_bultin(int x, char *args)
{
    switch (x)
    {
    case 0:
        chdir(args);
        break;
    case 1:
        printf("%s\n", args);
        break;
    case 2:
        args = args;
        char argsCopy[100];
        strncpy(argsCopy, args, sizeof(argsCopy) - 1);
        argsCopy[sizeof(argsCopy) - 1] = '\0';

        char *token = strtok(argsCopy, "=");
        if (token != NULL)
        {
            vars[currentVar] = strdup(token);
            token = strtok(NULL, "=");
            values[currentVar] = (token != NULL) ? strdup(token) : strdup("");
            currentVar++;
        }
        break;
    default:
        break;
    }
}
void execute_command(char *command[])
{
    int isBackground = 0;
    int i = 0;
    for (int i = 0; command[i] != NULL; i++)
    {
        if (strcmp(command[i], "&") == 0)
        {
            isBackground = 1;
            command[i] = NULL;
            break;
        }
    }
    pid_t pid = fork();
    if (pid == 0)
    {
        execvp(command[0], command);
    }
    else if (pid > 0 && !isBackground)
    {
        waitpid(pid, NULL, 0);
    }
}

void shell()
{
    char input[MAX_INPUT_SIZE];
    char *words[MAX_WORDS];

    while (1)
    {
        printf("Enter input: ");
        scanf(" %[^\n]", input);
        evaluate_expression(input);
        parseInput(input, words);
        if (words[0] == NULL)
        {
            printf("Exiting...\n");
            return;
        }
        if (strcmp(words[0], "cd") == 0)
        {
            execute_shell_bultin(0, words[1]);
        }
        else if (strcmp(words[0], "echo") == 0)
        {
            execute_shell_bultin(1, words[1]);
        }
        else if (strcmp(words[0], "export") == 0)
        {
            execute_shell_bultin(2, words[1]);
        }
        else
        {
            execute_command(words);
        }
    }
}

void on_child_exit(int sig)
{
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0)
    {
        printf("\nChild process %d terminated\n", pid);
    }
}

int main()
{
    setup_environment();
    signal(SIGCHLD, on_child_exit);
    shell();
    return 0;
}