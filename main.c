#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 50

typedef struct registro {
    int mat;
    char nome[50];
    char curso[50];
    int info;
    int disponibilidade; // 0 – em uso; 1 – disponível
} registro;

int hash(int key) {
    return key % N;
}

void inicializar(char *nomeArq) {
    FILE *arq = fopen(nomeArq, "wb");
    if (arq == NULL) {
        perror("Erro ao criar arquivo");
        exit(1);
    }

    registro a;
    a.disponibilidade = 1;

    for (int i = 0; i < N; i++) {
        fwrite(&a, sizeof(registro), 1, arq);
    }

    fclose(arq);
}

int AcharPosicao(char *nomeArq, int x) {
    int pos = hash(x);
    registro a;

    FILE *arq = fopen(nomeArq, "rb");
    if (arq == NULL) {
        perror("Erro ao abrir arquivo");
        exit(1);
    }

    fseek(arq, pos * sizeof(registro), SEEK_SET);
    fread(&a, sizeof(registro), 1, arq);

    while (a.disponibilidade == 0 && a.mat != x) {
        pos = (pos + 1) % N;
        fseek(arq, pos * sizeof(registro), SEEK_SET);
        fread(&a, sizeof(registro), 1, arq);
    }

    fclose(arq);
    return pos;
}

void inserir(char *nomeArq, int mat, char *nome, char *curso) {
    int pos = AcharPosicao(nomeArq, mat);

    FILE *arq = fopen(nomeArq, "r+b");
    if (arq == NULL) {
        perror("Erro ao abrir arquivo para escrita");
        exit(1);
    }

    registro a;
    a.mat = mat;
    strncpy(a.nome, nome, 49);
    strncpy(a.curso, curso, 49);
    a.info = mat;
    a.disponibilidade = 0;

    fseek(arq, pos * sizeof(registro), SEEK_SET);
    fwrite(&a, sizeof(registro), 1, arq);

    fclose(arq);
}

void listar(char *nomeArq) {
    FILE *arq = fopen(nomeArq, "rb");
    if (arq == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    registro a;
    for (int i = 0; i < N; i++) {
        fread(&a, sizeof(registro), 1, arq);
        if (a.disponibilidade == 0) {
            printf("[%02d] Mat: %d | Nome: %-15s | Curso: %-15s\n",
                   i, a.mat, a.nome, a.curso);
        }
    }

    fclose(arq);
}

void buscar(char *nomeArq, int mat) {
    int pos = hash(mat);
    registro a;

    FILE *arq = fopen(nomeArq, "rb");
    if (arq == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    fseek(arq, pos * sizeof(registro), SEEK_SET);
    fread(&a, sizeof(registro), 1, arq);

    while (a.disponibilidade == 0 && a.mat != mat) {
        pos = (pos + 1) % N;
        fseek(arq, pos * sizeof(registro), SEEK_SET);
        fread(&a, sizeof(registro), 1, arq);
    }

    fclose(arq);

    if (a.disponibilidade == 0 && a.mat == mat) {
        printf("Registro encontrado:\n");
        printf("Mat: %d | Nome: %s | Curso: %s\n", a.mat, a.nome, a.curso);
    } else {
        printf("Registro não encontrado.\n");
    }
}

void remover(char *nomeArq, int mat) {
    int pos = hash(mat);
    registro a;

    FILE *arq = fopen(nomeArq, "r+b");
    if (arq == NULL) {
        perror("Erro ao abrir arquivo");
        return;
    }

    fseek(arq, pos * sizeof(registro), SEEK_SET);
    fread(&a, sizeof(registro), 1, arq);

    while (a.disponibilidade == 0 && a.mat != mat) {
        pos = (pos + 1) % N;
        fseek(arq, pos * sizeof(registro), SEEK_SET);
        fread(&a, sizeof(registro), 1, arq);
    }

    if (a.disponibilidade == 0 && a.mat == mat) {
        a.disponibilidade = 1; // Marca como disponível
        fseek(arq, pos * sizeof(registro), SEEK_SET);
        fwrite(&a, sizeof(registro), 1, arq);
        printf("Registro removido com sucesso.\n");
    } else {
        printf("Registro não encontrado.\n");
    }

    fclose(arq);
}

// -------------------- MENU PRINCIPAL ----------------------

int main() {
    char nomeArquivo[] = "dados.bin";
    int opcao;

    do {
        printf("\n=== MENU ===\n");
        printf("1 - Inicializar arquivo\n");
        printf("2 - Inserir registro\n");
        printf("3 - Listar registros\n");
        printf("4 - Buscar por matrícula\n");
        printf("5 - Remover registro\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar(); // limpa buffer

        if (opcao == 1) {
            inicializar(nomeArquivo);
            printf("Arquivo inicializado com sucesso.\n");
        } else if (opcao == 2) {
            int mat;
            char nome[50], curso[50];
            printf("Matrícula: ");
            scanf("%d", &mat);
            getchar();
            printf("Nome: ");
            fgets(nome, sizeof(nome), stdin);
            nome[strcspn(nome, "\n")] = 0; // remover \n
            printf("Curso: ");
            fgets(curso, sizeof(curso), stdin);
            curso[strcspn(curso, "\n")] = 0;

            inserir(nomeArquivo, mat, nome, curso);
            printf("Registro inserido.\n");
        } else if (opcao == 3) {
            listar(nomeArquivo);
        } else if (opcao == 4) {
            int mat;
            printf("Digite a matrícula para buscar: ");
            scanf("%d", &mat);
            buscar(nomeArquivo, mat);
        } else if (opcao == 5) {
            int mat;
            printf("Digite a matrícula para remover: ");
            scanf("%d", &mat);
            remover(nomeArquivo, mat);
        } else if (opcao != 0) {
            printf("Opção inválida!\n");
        }
    } while (opcao != 0);

    printf("Programa finalizado.\n");
    return 0;
}
