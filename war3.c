#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10

// Estruturas
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

typedef struct {
    char nome[20];
    char cor[MAX_COR];
    char *missao; // armazenada dinamicamente
} Jogador;

/* ---------- Funcoes de gerencia de territorios ---------- */

// cadastra territorios no vetor mapa (acesso por ponteiro)
void cadastrarTerritorios(Territorio *mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Territorio %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exercito: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Numero de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// exibe mapa (IDs iniciam em 1 para usuario)
void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n===== MAPA ATUAL =====\n");
    for (int i = 0; i < n; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* ---------- Funcoes de ataque / jogo ---------- */

// verifique se duas strings sao iguais ignorando case simples (assume entradas consistentes)
int coresIguais(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}

// simula ataque entre dois territorios (ponteiros)
// regras:
// - se mesmo dono => erro
// - rola dado 1-6 para cada lado
// - se atacante vence: defensor troca cor para atacante->cor e recebe metade das tropas do atacante
// - senao: atacante perde 1 tropa
void atacar(Territorio *atacante, Territorio *defensor) {
    if (coresIguais(atacante->cor, defensor->cor)) {
        printf("\n[ERRO] Voce nao pode atacar um territorio da mesma cor!\n");
        return;
    }

    if (atacante->tropas <= 0) {
        printf("\n[ERRO] Territorio atacante nao tem tropas suficientes!\n");
        return;
    }

    printf("\n>>> Ataque de %s (%s) contra %s (%s)\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    int dadoAt = (rand() % 6) + 1;
    int dadoDf = (rand() % 6) + 1;
    printf("Dado Atacante: %d | Dado Defensor: %d\n", dadoAt, dadoDf);

    if (dadoAt > dadoDf) {
        printf("Resultado: O atacante venceu!\n");
        // defensor troca de dono
        strcpy(defensor->cor, atacante->cor);
        // defensor recebe metade das tropas do atacante (inteiro)
        defensor->tropas = atacante->tropas / 2;
        if (defensor->tropas < 0) defensor->tropas = 0;
    } else {
        printf("Resultado: O defensor resistiu! O atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
    }
}

/* ---------- Funcoes de missoes ---------- */

// atribui uma missao aleatoria do vetor missoes[] para destino (destino deve ter espaco alocado por malloc)
void atribuirMissao(char **destino, char *missoes[], int totalMissoes) {
    int idx = rand() % totalMissoes;
    size_t tam = strlen(missoes[idx]) + 1;
    *destino = (char *) malloc(tam);
    if (*destino == NULL) {
        printf("Erro ao alocar memoria para missao!\n");
        exit(1);
    }
    strcpy(*destino, missoes[idx]);
}

// verifica se jogador cumpriu sua missao.
// retorno: 1 = cumpriu, 0 = nao cumpriu
// Implementacao simples: reconhece as 5 missoes predefinidas abaixo.
int verificarMissao(char *missao, Jogador *j, Territorio *mapa, int tamanho) {
    // MISSÕES POSSIVEIS (devem corresponder exatamente as strings definidas no vetor):
    // 1) "Conquistar 3 territorios"
    // 2) "Eliminar todas tropas da cor vermelho"
    // 3) "Ter no total 30 tropas"
    // 4) "Conquistar territorio RJ"
    // 5) "Conquistar 2 territorios consecutivos"

    // 1) Conquistar 3 territorios: contar territorios com cor == j->cor
    if (strcmp(missao, "Conquistar 3 territorios") == 0) {
        int cnt = 0;
        for (int i = 0; i < tamanho; i++) if (coresIguais(mapa[i].cor, j->cor)) cnt++;
        return (cnt >= 3) ? 1 : 0;
    }

    // 2) Eliminar todas tropas da cor vermelho: verificar se existe algum territorio com cor == "vermelho" e tropas >0
    if (strcmp(missao, "Eliminar todas tropas da cor vermelho") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (coresIguais(mapa[i].cor, "vermelho") && mapa[i].tropas > 0) return 0;
        }
        return 1;
    }

    // 3) Ter no total 30 tropas: somar tropas em territórios do jogador
    if (strcmp(missao, "Ter no total 30 tropas") == 0) {
        int soma = 0;
        for (int i = 0; i < tamanho; i++) if (coresIguais(mapa[i].cor, j->cor)) soma += mapa[i].tropas;
        return (soma >= 30) ? 1 : 0;
    }

    // 4) Conquistar territorio RJ: existe algum territorio com nome "RJ" e cor == jogador?
    if (strcmp(missao, "Conquistar territorio RJ") == 0) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].nome, "RJ") == 0 && coresIguais(mapa[i].cor, j->cor)) return 1;
        }
        return 0;
    }

    // 5) Conquistar 2 territorios consecutivos: existe indice i tal que i e i+1 pertencem ao jogador?
    if (strcmp(missao, "Conquistar 2 territorios consecutivos") == 0) {
        for (int i = 0; i < tamanho - 1; i++) {
            if (coresIguais(mapa[i].cor, j->cor) && coresIguais(mapa[i+1].cor, j->cor)) return 1;
        }
        return 0;
    }

    // Missao desconhecida: nao cumprida
    return 0;
}

// exibe uma missao (passagem por valor da string)
void exibirMissao(const char *missao) {
    printf("MISSAO: %s\n", missao);
}

/* ---------- Liberar memoria ---------- */

// libera mapa e missoes dos jogadores
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int numJogadores) {
    // libera missoes alocadas dinamicamente
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].missao != NULL) free(jogadores[i].missao);
    }
    // libera vetor de jogadores alocado no main (se alocado dinamicamente)
    free(jogadores);
    // libera mapa
    free(mapa);
}

/* ---------- Main (fluxo do jogo) ---------- */

int main() {
    srand(time(NULL));

    int nTerritorios;
    printf("Quantos territorios deseja cadastrar? ");
    scanf("%d", &nTerritorios);

    // aloca mapa dinamicamente
    Territorio *mapa = (Territorio *) calloc(nTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para mapa!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, nTerritorios);

    // numero de jogadores (simples: 2 ou mais)
    int numJogadores;
    printf("\nQuantos jogadores (2 recomendado)? ");
    scanf("%d", &numJogadores);
    if (numJogadores < 1) numJogadores = 2;

    // aloca jogadores dinamicamente
    Jogador *jogadores = (Jogador *) malloc(sizeof(Jogador) * numJogadores);
    if (jogadores == NULL) {
        printf("Erro ao alocar memoria para jogadores!\n");
        free(mapa);
        return 1;
    }

    // cadastra jogadores (nome e cor)
    for (int i = 0; i < numJogadores; i++) {
        printf("\n--- Jogador %d ---\n", i + 1);
        printf("Nome do jogador: ");
        scanf(" %[^\n]", jogadores[i].nome);
        printf("Cor do jogador (ex: azul, vermelho, verde): ");
        scanf(" %[^\n]", jogadores[i].cor);
        jogadores[i].missao = NULL; // inicializa
    }

    // vetor de missoes (descricoes)
    char *missoes[] = {
        "Conquistar 3 territorios",
        "Eliminar todas tropas da cor vermelho",
        "Ter no total 30 tropas",
        "Conquistar territorio RJ",
        "Conquistar 2 territorios consecutivos"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // atribui missoes para cada jogador (armazenadas dinamicamente)
    for (int i = 0; i < numJogadores; i++) {
        atribuirMissao(&jogadores[i].missao, missoes, totalMissoes);
        printf("\nJogador %s recebeu sua missao (sera exibida uma vez):\n", jogadores[i].nome);
        exibirMissao(jogadores[i].missao);
    }

    // loop de jogo basico
    int opcao;
    int vencedor = -1;
    do {
        exibirTerritorios(mapa, nTerritorios);
        printf("\nMENU\n1 - Atacar\n2 - Mostrar missoes (opcional)\n0 - Sair\nEscolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            int idAt, idDf;
            printf("Escolha o territorio atacante (ID): ");
            scanf("%d", &idAt);
            printf("Escolha o territorio defensor (ID): ");
            scanf("%d", &idDf);

            // ajuste IDs (usuario usa 1..n)
            if (idAt < 1 || idAt > nTerritorios || idDf < 1 || idDf > nTerritorios) {
                printf("\n[ERRO] IDs invalidos!\n");
            } else if (idAt == idDf) {
                printf("\n[ERRO] Nao pode escolher o mesmo territorio como atacante e defensor!\n");
            } else {
                atacar(&mapa[idAt - 1], &mapa[idDf - 1]);
            }

            // apos ataque, verificar missoes de todos jogadores
            for (int j = 0; j < numJogadores; j++) {
                if (jogadores[j].missao != NULL) {
                    if (verificarMissao(jogadores[j].missao, &jogadores[j], mapa, nTerritorios)) {
                        vencedor = j;
                        break;
                    }
                }
            }
            if (vencedor != -1) {
                printf("\n***** JOGADOR VENCEDOR: %s *****\n", jogadores[vencedor].nome);
                printf("Missao cumprida: %s\n", jogadores[vencedor].missao);
                break;
            }
        } else if (opcao == 2) {
            // opcional: exibir missoes de cada jogador (pode ser util para debug)
            for (int j = 0; j < numJogadores; j++) {
                printf("\nJogador: %s | Cor: %s\n", jogadores[j].nome, jogadores[j].cor);
                exibirMissao(jogadores[j].missao);
            }
        }

    } while (opcao != 0);

    // limpar memoria
    liberarMemoria(mapa, jogadores, numJogadores);
    printf("\nPrograma finalizado.\n");
    return 0;
}
