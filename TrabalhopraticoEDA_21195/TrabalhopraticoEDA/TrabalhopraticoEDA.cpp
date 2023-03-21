/*
* Project: TrabalhopraticoEDA
* Purpose: Licenciatura de Engenharia Informática Médica
* Company: IPCA
* Created by: Paulo Pires
* Created on: 02/03/2023
* Email: a21195@alunos.ipca.pt
**/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LOCALIZACAO 20
#define MAX_MEIOS_MOBILIDADE 100

// Definindo a estrutura dos registos de aluguer
struct registo_aluguer {
    int dia;
    int mes;
    int ano;
    int duracao_horas;
    float custo_total;
    struct registo_aluguer* prox;
};

struct meio_mobilidade {
    int tipo; // 1 para bicicleta, 2 para scooter, etc.
    int carga_bateria;
    float custo_hora;
    char localizacao[MAX_LOCALIZACAO];
    int indice;
    struct meio_mobilidade* prox;
    struct registo_aluguer* registo_alugueres;
    int autonomia_km;
    int id; 
    float autonomia; 
    float preco; 
    int geocodigo;
};

// Função para ordenar os meios de mobilidade elétrica por ordem decrescente de autonomia
void ordenar_por_autonomia(struct meio_mobilidade* lista_meios, int num_meios) {
    struct meio_mobilidade temp;
    struct meio_mobilidade* atual;
    int i, j;

    for (i = 0; i < num_meios - 1; i++) {
        atual = lista_meios;
        for (j = 0; j < num_meios - i - 1; j++) {
            if (atual->autonomia_km < atual->prox->autonomia_km) {
                temp = *atual;
                *atual = *atual->prox;
                *atual->prox = temp;
            }
            atual = atual->prox;
        }
    }
}

// Função para contar o número de meios de mobilidade elétrica
int contar_meios_mobilidade(struct meio_mobilidade* lista_meios) {
    int contador = 0;
    struct meio_mobilidade* atual = lista_meios;

    while (atual != NULL) {
        if (atual->tipo == 1 || atual->tipo == 2) {
            contador++;
        }
        atual = atual->prox;
    }

    return contador;
}

// Função para adicionar um novo meio de mobilidade elétrica à lista
void adicionar_meio_mobilidade(struct meio_mobilidade** inicio, int tipo, int carga_bateria, float custo_hora, const char* localizacao) {
    struct meio_mobilidade* novo_meio = (struct meio_mobilidade*)malloc(sizeof(struct meio_mobilidade));
    if (novo_meio == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    novo_meio->tipo = tipo;
    novo_meio->carga_bateria = carga_bateria;
    novo_meio->custo_hora = custo_hora;
    strcpy(novo_meio->localizacao, localizacao);

    if (*inicio == NULL) {
        novo_meio->indice = 0;
        *inicio = novo_meio;
    }
    else {
        struct meio_mobilidade* atual = *inicio;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        novo_meio->indice = atual->indice + 1;
        atual->prox = novo_meio;
    }
}

// Escrever dados em arquivo
void escrever_arquivo(struct meio_mobilidade* inicio, const char* nome_arquivo) {
    FILE* arquivo = fopen("meios_mobilidade.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para escrita.\n");
        return;
    }

    struct meio_mobilidade* atual = inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d %d %.2f %s\n", atual->tipo, atual->carga_bateria, atual->custo_hora, atual->localizacao);
        atual = atual->prox;
    }

    fclose(arquivo);
}

// Ler dados de arquivo
void ler_arquivo(struct meio_mobilidade** inicio, const char* nome_arquivo) {
    FILE* arquivo = fopen("meios_mobilidade.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para leitura.\n");
        return;
    }

    int tipo, carga_bateria;
    float custo_hora;
    char localizacao[20];
    while (fscanf(arquivo, "%d %d %f %s", &tipo, &carga_bateria, &custo_hora, localizacao) != EOF) {
        adicionar_meio_mobilidade(inicio, tipo, carga_bateria, custo_hora, localizacao);
    }

    fclose(arquivo);
}

// Função para atualizar os índices dos meios de mobilidade elétrica na lista
void atualizar_indices(struct meio_mobilidade* inicio) {
    int indice = 0;
    struct meio_mobilidade* atual = inicio;
    while (atual != NULL) {
        atual->indice = indice;
        indice++;
        atual = atual->prox;
    }
}

// Função para exibir a lista de meios de mobilidade elétrica
void exibir_meios_mobilidade(struct meio_mobilidade* inicio) {
    printf("Lista de meios de mobilidade eletrica:\n");

    struct meio_mobilidade* atual = inicio;
    while (atual != NULL) {
        printf("Indice: %d\n", atual->indice);
        printf("Tipo: %d\n", atual->tipo);
        printf("Carga da bateria: %d\n", atual->carga_bateria);
        printf("Custo por hora: %.2f\n", atual->custo_hora);
        printf("Localizacao: %s\n", atual->localizacao);
        printf("\n");
        atual = atual->prox;
    }
}

// Função para remover um meio de mobilidade elétrica da lista
void remover_meio_mobilidade(struct meio_mobilidade** inicio, int indice) {
    if (*inicio == NULL) {
        printf("Lista vazia.\n");
        return;
    }
    struct meio_mobilidade* atual = *inicio;
    struct meio_mobilidade* anterior = NULL;

    while (atual != NULL) {
        if (atual->indice == indice) {
            if (anterior == NULL) {
                *inicio = atual->prox;
            }
            else {
                anterior->prox = atual->prox;
            }
            free(atual);
            atualizar_indices(*inicio);
            printf("Meio de mobilidade eletrica removido com sucesso.\n");
            return;
        }
        anterior = atual;
        atual = atual->prox;
    }
    free(atual);

    atualizar_indices(*inicio);

    printf("Indice invalido.\n");
}

// Função para buscar um meio de mobilidade elétrica na lista pelo tipo
void buscar_meio_mobilidade_por_tipo(struct meio_mobilidade* inicio, int tipo) {
    printf("Meios de mobilidade eletrica do tipo %d:\n", tipo);
    struct meio_mobilidade* atual = inicio;
    while (atual != NULL) {
        if (atual->tipo == tipo) {
            printf("Indice: %d\n", atual->indice);
            printf("Tipo: %d\n", atual->tipo);
            printf("Carga da bateria: %d\n", atual->carga_bateria);
            printf("Custo por hora: %.2f\n", atual->custo_hora);
            printf("Localizacao: %s\n", atual->localizacao);
            printf("\n");
        }
        atual = atual->prox;
    }
}

//Função para alterar um meio de mobilidade elétrica
void alterar_meio_mobilidade(struct meio_mobilidade* inicio, int indice, int tipo, int carga_bateria, float custo_hora, const char* localizacao) {
    struct meio_mobilidade* atual = inicio;
    while (atual != NULL) {
        if (atual->indice == indice) {
            atual->tipo = tipo;
            atual->carga_bateria = carga_bateria;
            atual->custo_hora = custo_hora;
            strcpy(atual->localizacao, localizacao);
            printf("Meio de mobilidade eletrica alterado com sucesso.\n");
            return;
        }
        atual = atual->prox;
    }
    printf("Indice invalido.\n");
}

// Função para adicionar um novo registo de aluguer para um meio de mobilidade elétrica específico
void adicionar_registo_aluguer(struct meio_mobilidade* meio, int dia, int mes, int ano, int duracao_horas, float custo_total) {
    struct registo_aluguer* novo_registo = (struct registo_aluguer*)malloc(sizeof(struct registo_aluguer));
    if (novo_registo == NULL) {
        printf("Erro ao alocar memoria.\n");
        return;
    }

    novo_registo->dia = dia;
    novo_registo->mes = mes;
    novo_registo->ano = ano;
    novo_registo->duracao_horas = duracao_horas;
    novo_registo->custo_total = custo_total;

    if (meio->registo_alugueres == NULL) {
        meio->registo_alugueres = novo_registo;
    }
    else {
        struct registo_aluguer* atual = meio->registo_alugueres;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        atual->prox = novo_registo;
    }

    printf("Registo de aluguer adicionado com sucesso.\n");
}

void exibir_meios_mobilidade_eletricos_localizacao(struct meio_mobilidade* inicio, int geocodigo) {
    struct meio_mobilidade* p = inicio;
    char codigo_str[16];

    sprintf(codigo_str, "%d", geocodigo); // converter inteiro para string

    while (p != NULL) {
        if (p->tipo == 3 && strcmp(p->localizacao, codigo_str) == 0) {
            printf("ID: %d | Autonomia: %.1f | Preço: %.2f | Localização: %s\n", p->id, p->autonomia, p->preco, p->localizacao);
        }
        p = p->prox;
    }
}

void exibir_menu() {
    printf("\nMenu:\n");
    printf("1 - Adicionar meio de mobilidade eletrica\n");
    printf("2 - Exibir lista de meios de mobilidade eletrica\n");
    printf("3 - Ordenar meios de mobilidade eletrica por autonomia\n");
    printf("4 - Remover meio de mobilidade eletrica\n");
    printf("5 - Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int escolha;
    struct meio_mobilidade* lista_meios = NULL;

    do {
        exibir_menu();
        scanf("%d", &escolha);

        switch (escolha) {
        case 1:
            // Adicionar meio de mobilidade eletrica
            int tipo, carga_bateria;
            float custo_hora;
            char localizacao[20];

            printf("Digite o tipo (1 para bicicleta, 2 para scooter, etc.): ");
            scanf("%d", &tipo);

            printf("Digite a carga da bateria: ");
            scanf("%d", &carga_bateria);

            printf("Digite o custo por hora: ");
            scanf("%f", &custo_hora);

            printf("Digite a localizacao: ");
            scanf("%s", localizacao);

            adicionar_meio_mobilidade(&lista_meios, tipo, carga_bateria, custo_hora, localizacao);
            printf("Meio de mobilidade eletrica adicionado com sucesso.\n");

            break;

        case 2:
            // Exibir lista de meios de mobilidade eletrica
            exibir_meios_mobilidade(lista_meios);

            break;

        case 3:
            // Ordenar meios de mobilidade eletrica por autonomia
            ordenar_por_autonomia(lista_meios, contar_meios_mobilidade(lista_meios));
            printf("Lista de meios de mobilidade eletrica ordenada com sucesso.\n");

            break;

        case 4:
            // Remover meio de mobilidade eletrica
            int indice;

            printf("Digite o indice do meio de mobilidade eletrica que deseja remover: ");
            scanf("%d", &indice);

            remover_meio_mobilidade(&lista_meios, indice);

            break;

        case 5:
            // Sair
            printf("Saindo...\n");

            break;

        default:
            printf("Opcao invalida. Tente novamente.\n");

            break;
        }
    } while (escolha != 5);

    return 0;
}

int main2() {
    struct meio_mobilidade* inicio = NULL;
    adicionar_meio_mobilidade(&inicio, 1, 100, 10.5, "Centro");
    adicionar_meio_mobilidade(&inicio, 2, 75, 8.0, "Zona Sul");
    adicionar_meio_mobilidade(&inicio, 1, 50, 7.5, "Zona Norte");
    adicionar_meio_mobilidade(&inicio, 3, 80, 12.0, "Centro");
    adicionar_meio_mobilidade(&inicio, 2, 20, 5.0, "Zona Oeste");

    exibir_meios_mobilidade(inicio);

    buscar_meio_mobilidade_por_tipo(inicio, 1);

    remover_meio_mobilidade(&inicio, 2);

    exibir_meios_mobilidade(inicio);

    alterar_meio_mobilidade(inicio, 2, 2, 90, 9.0, "Zona Sul");
    exibir_meios_mobilidade(inicio);

    adicionar_registo_aluguer(inicio->prox, 19, 3, 2023, 3, 10.0);

    int num_meios = contar_meios_mobilidade(inicio);
    ordenar_por_autonomia(inicio, num_meios);
    exibir_meios_mobilidade(inicio);

    exibir_meios_mobilidade_eletricos_localizacao(inicio, 100);

    return 0;
}