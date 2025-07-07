#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define NOME_DO_SERVER_PIPE "pipe_entre_cliente_servidor"

typedef struct {
    int time; // tempo estimado de execução em ms (que é fornecido pelo usuário)
    char args[300]; // argumentos do programa
    int id; // Identificador único da tarefa
} TaskData;

int main(int argc, char *argv[]){
    int server_fd;
    TaskData task;

    // Verifica se o número de argumentos é válido
    if (argc < 5 || strcmp(argv[1], "execute") != 0) {
        if (argc == 2 && strcmp(argv[1], "status") == 0) {
            // Abre o pipe nomeado do cliente
            server_fd = open(NOME_DO_SERVER_PIPE, O_WRONLY);
            if (server_fd == -1) {
                perror("Erro ao abrir o pipe do servidor");
                exit(1);
            }

            // Enviar comando de status para o servidor
            strcpy(task.args, "status");
            write(server_fd, &task, sizeof(task));

            printf("Solicitação de status enviada ao servidor.\n");

            close(server_fd);
            return 0;
        } else {
            printf("Uso correto: %s execute <tempo> -u \"<comando>\"\n", argv[0]);
            exit(1);
        }
    }

    // Abre o pipe nomeado do cliente
    server_fd = open(NOME_DO_SERVER_PIPE, O_WRONLY);
    if (server_fd == -1) {
        perror("Erro ao abrir o pipe do servidor");
        exit(1);
    }
    
    if(strcmp(argv[3], "-u") == 0){
        task.id = getpid(); // Usando o PID do processo cliente como ID da tarefa
        if (task.id == -1) {
            task.id = 0; // Se não for possível obter o PID, atribui 0 como ID
        }
        task.time = atoi(argv[2]);
        strcpy(task.args, argv[4]);
        
        write(server_fd, &task, sizeof(task));

        printf("Tarefa enviada com sucesso. ID da tarefa: %d\n", task.id);

        close(server_fd);
    } else {
        printf("Formato incorreto! Utilize: %s execute <tempo> -u \"<comando>\"\n", argv[0]);
        exit(1);
    }

    return 0;
}
