#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Struct que representa cada territorio
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Função para cadastrar territorios
void cadastrarTerritorios(Territorio *mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome); // lê string com espaço
        printf("Cor do exercito: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Exibir todos os territorios
void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < n; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Função que simula o ataque
void atacar(Territorio *atacante, Territorio *defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("\n[ERRO] Voce nao pode atacar um territorio da mesma cor!\n");
        return;
    }

    printf("\n>>> Ataque de %s (%s) contra %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAtacante = (rand() % 6) + 1; // de 1 a 6
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado Atacante: %d | Dado Defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: O atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor);     // troca dono
        defensor->tropas = atacante->tropas / 2; // transfere metade das tropas
    } else {
        printf("Resultado: O defensor resistiu! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0; // evita negativo
    }
}

// Liberar memoria alocada
void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nMemoria liberada com sucesso!\n");
}

int main() {
    srand(time(NULL)); // inicializa aleatoriedade

    int n;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &n);

    // Alocacao dinamica
    Territorio *mapa = (Territorio *) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria!\n");
        return 1;
    }

    // Cadastro inicial
    cadastrarTerritorios(mapa, n);

    int opcao;
    do {
        exibirTerritorios(mapa, n);
        printf("\nMENU\n");
        printf("1 - Atacar\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int iAtacante, iDefensor;
            printf("Escolha o territorio atacante (ID): ");
            scanf("%d", &iAtacante);
            printf("Escolha o territorio defensor (ID): ");
            scanf("%d", &iDefensor);

            // Corrigido para IDs começarem em 1
            if (iAtacante >= 1 && iAtacante <= n && iDefensor >= 1 && iDefensor <= n) {
                atacar(&mapa[iAtacante - 1], &mapa[iDefensor - 1]);
            } else {
                printf("\n[ERRO] IDs invalidos!\n");
            }
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}
