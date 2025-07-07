#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>

#define NOME_DO_SERVER_PIPE "pipe_entre_cliente_servidor"
#define MAX_RUNNING_TASKS 100
#define REGISTRO_DE_TAREFAS "log_de_tarefas.txt"
#define OUTPUT_FILE "output.txt"

typedef struct {
    int time; // tempo estimado de execução em ms (fornecido pelo usuário)
    char args[300]; // argumentos do programa
    int id; // Identificador único da tarefa
} TaskData;

// Incluir um novo tipo de enumeração para o estado das tarefas
typedef enum {
    EM_ESPERA,
    EM_EXECUCAO,
    TERMINADA
} StatusTarefa;

typedef struct {
    TaskData task;
    pid_t pid; // PID do processo em execução
    int task_id; // ID da tarefa
    struct timeval start_time; // tempo de início de execução
    struct timeval end_time;
} RunningTask;

int num_running_tasks = 0;
RunningTask running_tasks[MAX_RUNNING_TASKS];

typedef struct Item {
    TaskData data;
    struct Item* proximo;
} Item;

typedef struct {
    Item *primeiro, *ultimo;
} Fila;

Item* novoItem(TaskData data) {
    Item* temp = (Item*)malloc(sizeof(Item));
    temp->data = data;
    temp->proximo = NULL;
    return temp;
}

Fila* criaFila() {
    Fila* fila = (Fila*)malloc(sizeof(Fila));
    fila->primeiro = fila->ultimo = NULL;
    return fila;
}

void coloca_na_fila(Fila* fila, TaskData data) {
    Item* temp = novoItem(data);

    if (fila->ultimo == NULL) {
        fila->primeiro = fila->ultimo = temp;
        return;
    }

    fila->ultimo->proximo = temp;
    fila->ultimo = temp;
}

void retira_da_fila(Fila* fila, int id) {
    if (fila->primeiro == NULL)
        return;

    Item *atual = fila->primeiro;
    Item *anterior = NULL;

    while (atual != NULL) {
        if (atual->data.id == id) {
            if (anterior == NULL) {
                fila->primeiro = atual->proximo;
                free(atual);
                if (fila->primeiro == NULL)
                    fila->ultimo = NULL;
            } else {
                anterior->proximo = atual->proximo;
                free(atual);
                if (anterior->proximo == NULL)
                    fila->ultimo = anterior;
            }
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
}

TaskData primeiro(Fila* fila) {
    if (fila->primeiro == NULL) {
        TaskData emptyTask = {0, "", -1};
        return emptyTask;
    }
    return fila->primeiro->data;
}

int vazia(Fila* fila) {
    return fila->primeiro == NULL;
}

void executeTask(TaskData task, int output_filename) {

    if (output_filename == -1) {
        perror("Erro ao abrir arquivo de saída");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Erro ao criar processo filho");
        exit(1);
    } else if (pid == 0) {
        // Processo filho
        dup2(output_filename, 1); // Redireciona stdout para o arquivo
        close(output_filename);

        char *command = strtok(task.args, " "); // Separa os comandos nos argumentos
        char *args[10];
        int i = 0;

        while (command != NULL) {
            args[i++] = command;
            command = strtok(NULL, " ");
        }
        args[i] = NULL;

        struct timeval start_time, end_time;
        gettimeofday(&start_time, NULL);

        execvp(args[0], args);

        // Se execvp retornar, houve um erro
        perror("Erro ao executar o programa");
        exit(1);
    } else {
        // Processo pai
        // Adiciona a tarefa à lista de tarefas em execução
        running_tasks[num_running_tasks].task = task;
        running_tasks[num_running_tasks].pid = pid;
        running_tasks[num_running_tasks].task_id = task.id;
        gettimeofday(&running_tasks[num_running_tasks].start_time, NULL);
        num_running_tasks++;
    }
}

void registraTarefa(int id, struct timeval start_time, struct timeval end_time) {
    FILE *file = fopen(REGISTRO_DE_TAREFAS, "a");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de registro");
        exit(1);
    }

    long int elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 +
                            (end_time.tv_usec - start_time.tv_usec) / 1000;

    fprintf(file, "Tarefa %d: Tempo de execução: %ldms\n", id, elapsed_time);

    fclose(file);
}

void escreve_registro(TaskData task, struct timeval start_time, struct timeval end_time) {
    FILE *file = fopen(REGISTRO_DE_TAREFAS, "a");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de registro");
        exit(1);
    }

    long int elapsed_time = (end_time.tv_sec - start_time.tv_sec) * 1000 +
                            (end_time.tv_usec - start_time.tv_usec) / 1000;

    fprintf(file, "Tarefa %d: Tempo de execução: %ldms\n", task.id, elapsed_time);

    fclose(file);
}

void cria_arquivo_registro() {
    FILE *file = fopen(REGISTRO_DE_TAREFAS, "w");
    if (file == NULL) {
        perror("Erro ao criar arquivo de registro");
        exit(1);
    }
    fclose(file);
}

void le_registro() {
    FILE *file = fopen(REGISTRO_DE_TAREFAS, "r");
    if (file == NULL) {
        perror("Erro ao abrir arquivo de registro");
        exit(1);
    }

    char buffer[100];
    while (fgets(buffer, 100, file) != NULL) {
        printf("%s", buffer);
    }

    fclose(file);
}

int main(int argc, char *argv[]) {
    int server_fd;
    TaskData taskdata;
    int bytes_read;
    int status_requested = 0;

    // Cria o pipe nomeado do servidor
    mkfifo(NOME_DO_SERVER_PIPE, 0666);

    printf("Servidor de Orquestração iniciado.\n");

    // Abre o pipe nomeado do servidor
    server_fd = open(NOME_DO_SERVER_PIPE, O_RDONLY);
    if (server_fd == -1) {
        perror("Erro ao abrir o pipe do servidor");
        exit(1);
    }

    // Cria o arquivo de registro se não existir
    cria_arquivo_registro();

    // Abre o arquivo de saída para redirecionar as saídas das tarefas
    int output_fd = open(OUTPUT_FILE, O_CREAT | O_TRUNC | O_WRONLY, 0666);
    if (output_fd == -1) {
        perror("Erro ao abrir o arquivo de saída");
        exit(1);
    }

    Fila* fila = criaFila(); // Cria a fila fora do loop

    while(1){
        while((bytes_read = read(server_fd, &taskdata,sizeof(taskdata)))>0) {
            if (strcmp(taskdata.args, "status") == 0) {
                if (!status_requested) {
                    printf("Solicitação de status recebida.\n");

                    printf("Tarefas em execução:\n");
                    for (int i = 0; i < num_running_tasks; i++) {
                        struct timeval now;
                        gettimeofday(&now, NULL);
                        printf("ID: %d - Tempo de execução: %ldms\n", running_tasks[i].task_id, 
                            ((now.tv_sec - running_tasks[i].start_time.tv_sec) * 1000 
                            + (now.tv_usec - running_tasks[i].start_time.tv_usec) / 1000));
                    }

                    printf("Tarefas em espera para executar:\n");
                    Item *temp = fila->primeiro;
                    while (temp != NULL) {
                        printf("Tempo estimado: %d ms\n", temp->data.time);
                        printf("Argumentos: %s\n", temp->data.args);
                        printf("Identificador: %d\n", temp->data.id);
                        temp = temp->proximo;
                    }

                    printf("Tarefas terminadas:\n");
                    le_registro(); // Mostra as tarefas terminadas

                    status_requested = 1; // Redefine a variável de controle de status
                }
            } else {
                printf("Tempo estimado: %d ms\n", taskdata.time);
                printf("Argumentos: %s\n", taskdata.args);
                printf("Identificador: %d\n", taskdata.id);

                struct timeval start_time, end_time;
                gettimeofday(&start_time, NULL);

                executeTask(taskdata, output_fd);
                wait(NULL);

                gettimeofday(&end_time, NULL);

                // Atualiza o tempo de término da tarefa
                for (int i = 0; i < num_running_tasks; i++) {
                    if (running_tasks[i].task_id == taskdata.id) {
                        running_tasks[i].end_time = end_time;
                        break;
                    }
                }

                // Registra a tarefa no arquivo de registro
                escreve_registro(taskdata, start_time, end_time);

                retira_da_fila(fila, taskdata.id); // Remove a tarefa da fila
            }
        }

        // Fecha o descritor de arquivo somente após a execução de todas as tarefas
        if (vazia(fila) && num_running_tasks == 0)
            break;
    }

    close(output_fd); // Feche o descritor de arquivo aqui

    close(server_fd);
    unlink(NOME_DO_SERVER_PIPE);
    return 0;
}
