#include <stdio.h>

// Struct que representa um Territorio
typedef struct {
    char nome[30];   // nome do território
    char cor[10];    // cor do exército
    int tropas;      // quantidade de tropas
} Territorio;

int main() {
    Territorio territorios[5]; // vetor com 5 territórios

    // Cadastro dos 5 territórios
    for (int i = 0; i < 5; i++) {
        printf("Digite o nome do territorio %d: ", i+1);
        scanf("%s", territorios[i].nome); // lê até espaço

        printf("Digite a cor do exercito: ");
        scanf("%s", territorios[i].cor);

        printf("Digite o numero de tropas: ");
        scanf("%d", &territorios[i].tropas);

        printf("\n");
    }

    // Exibe os territórios cadastrados
    printf("\n=== Territorios cadastrados ===\n");
    for (int i = 0; i < 5; i++) {
        printf("Territorio %d: Nome=%s | Cor=%s | Tropas=%d\n",
               i+1, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }

    return 0;
}