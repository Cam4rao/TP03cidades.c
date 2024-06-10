#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cidades.h"

Estrada *getEstrada(const char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "r");
    if (!file) return NULL;

    Estrada *estrada = (Estrada *)malloc(sizeof(Estrada));
    if (!estrada) {
        fclose(file);
        return NULL;
    }

    if (fscanf(file, "%d", &estrada->T) != 1 || estrada->T < 3 || estrada->T > 1000000) {
        free(estrada);
        fclose(file);
        return NULL;
    }
    if (fscanf(file, "%d", &estrada->N) != 1 || estrada->N < 2 || estrada->N > 10000) {
        free(estrada);
        fclose(file);
        return NULL;
    }

    estrada->C = (Cidade *)malloc(estrada->N * sizeof(Cidade));
    if (!estrada->C) {
        free(estrada);
        fclose(file);
        return NULL;
    }

    int *posicoes = (int *)malloc(estrada->N * sizeof(int));
    if (!posicoes) {
        free(estrada->C);
        free(estrada);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < estrada->N; ++i) {
        if (fscanf(file, "%d", &estrada->C[i].Posicao) != 1 || estrada->C[i].Posicao <= 0 || estrada->C[i].Posicao >= estrada->T) {
            free(posicoes);
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }

        for (int j = 0; j < i; ++j) {
            if (posicoes[j] == estrada->C[i].Posicao) {
                free(posicoes);
                free(estrada->C);
                free(estrada);
                fclose(file);
                return NULL;
            }
        }

        posicoes[i] = estrada->C[i].Posicao;

        if (fscanf(file, " %[^\n]", estrada->C[i].Nome) != 1) {
            free(posicoes);
            free(estrada->C);
            free(estrada);
            fclose(file);
            return NULL;
        }
    }

    free(posicoes);
    fclose(file);
    return estrada;
}

void swap(Cidade *a, Cidade *b) {
    Cidade temp = *a;
    *a = *b;
    *b = temp;
}

// Função de particionamento para o quicksort.
int partition(Cidade arr[], int low, int high) {
    int pivot = arr[high].Posicao;
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (arr[j].Posicao < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

// Implementação do quicksort.
void quicksort(Cidade arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);

        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

// Função para calcular a menor vizinhança.
double calcularMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return -1.0;

    // Ordenar as cidades com base na posição
    quicksort(estrada->C, 0, estrada->N - 1);

    double menorVizinhanca = (double)(estrada->T);

    for (int i = 0; i < estrada->N; ++i) {
        double vizinhanca;

        if (i == 0) {
            // Primeira cidade (borda esquerda)
            vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0 + estrada->C[i].Posicao;
        } else if (i == estrada->N - 1) {
            // Última cidade (borda direita)
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i -1].Posicao) / 2.0 + (estrada->T - estrada->C[i].Posicao);
        } else {
            // Cidades no meio
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0 + (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
        }
    }

    free(estrada->C);
    free(estrada);
    return menorVizinhanca;
}

char *cidadeMenorVizinhanca(const char *nomeArquivo) {
    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) return NULL;

    quicksort(estrada->C, 0, estrada->N - 1);

    double menorVizinhanca = (double)(estrada->T);
    int cidadeIndex = -1;

    for (int i = 0; i < estrada->N; ++i) {
        double vizinhanca;

        if (i == 0) {
            vizinhanca = (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0 + estrada->C[i].Posicao;
        } else if (i == estrada->N - 1) {
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i -1].Posicao) / 2.0 + (estrada->T - estrada->C[i].Posicao);
        } else {
            vizinhanca = (estrada->C[i].Posicao - estrada->C[i - 1].Posicao) / 2.0 + (estrada->C[i + 1].Posicao - estrada->C[i].Posicao) / 2.0;
        }

        if (vizinhanca < menorVizinhanca) {
            menorVizinhanca = vizinhanca;
            cidadeIndex = i;
        }
    }

    char *cidadeNome = (char *)malloc((strlen(estrada->C[cidadeIndex].Nome) + 1) * sizeof(char));
    if (cidadeNome) {
        strcpy(cidadeNome, estrada->C[cidadeIndex].Nome);
    }

    free(estrada->C);
    free(estrada);
    return cidadeNome;
}

int main(int argc, char *argv[]) {

    const char *nomeArquivo = "txt.txt";

    Estrada *estrada = getEstrada(nomeArquivo);
    if (!estrada) {
        printf("Erro ao ler o arquivo ou alocar memória.\n");
        return 1;
    }
    for (int i = 0; i < estrada->N; ++i){
        printf("%d %s\n", estrada->C[i].Posicao, estrada->C[i].Nome);
    }

    double menorVizinhanca = calcularMenorVizinhanca(nomeArquivo);
    char *cidade = cidadeMenorVizinhanca(nomeArquivo);

    printf("Menor vizinhança: %.2f\n", menorVizinhanca);
    printf("Cidade com a menor vizinhança: %s\n", cidade);

    free(estrada->C);
    free(estrada);
    free(cidade);

    return 0;
}
