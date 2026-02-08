#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// Código da Ilha – Edição Free Fire
// Nível: Mestre
// Este programa simula o gerenciamento avançado de uma mochila com componentes coletados durante a fuga de uma ilha.
// Ele introduz ordenação com critérios e busca binária para otimizar a gestão dos recursos.

    //Constantes e definições
#define MAX_ITENS 10
#define MAX_NOME 50
    //Variáveis globais
int totalItens = 0; // Controla a quantidade atual de itens na mochila

// Struct para representar um item
struct Item{
    char nome[MAX_NOME];
    char tipo[20];
    int quantidade;
    int prioridade; // 1 a 5
};

    //Funçoes para o menu e manipulação dos itens
void limparTela();
void exibirMenu();
void inserirItem(struct Item *mochila, int *totalItens);
void removerItem(struct Item *mochila, int *totalItens);
void listarItens(struct Item *mochila, int totalItens);
void menuDeOrdenacao(struct Item *mochila, int totalItens);
void insertionSort(struct Item *mochila, int totalItens, int criterio);
void buscaBinariaPorNome(struct Item *mochila, int totalItens);

int main() {   
    struct Item *mochila = (struct Item*) calloc(MAX_ITENS, sizeof(struct Item));
    if(mochila == NULL) return 1; 

    //verificações de alocação
    if(mochila == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1; //retorna 1 para indicar um erro.
    }
    int opcao; // Menu principal com opções

    do {
        exibirMenu(totalItens);
        if (scanf("%d", &opcao) != 1) { 
            opcao = -1;
        }
        limparTela(); 

        switch (opcao) {
            case 1:
                inserirItem(mochila, &totalItens);
                listarItens(mochila, totalItens);
                break;
            case 2:
                removerItem(mochila, &totalItens);
                listarItens(mochila, totalItens);
                break;
            case 3:
                listarItens(mochila, totalItens);
                break;
            case 4:
                menuDeOrdenacao(mochila, totalItens);
                break;
            case 5:
                buscaBinariaPorNome(mochila,totalItens);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);
    
    // A estrutura switch trata cada opção chamando a função correspondente.
    // A ordenação e busca binária exigem que os dados estejam bem organizados.
    free(mochila);
    return 0;
}

void limparTela() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void exibirMenu(int totalItens) {
    printf("Itens na mochila: %d/%d\n", totalItens, MAX_ITENS);
    printf("\n1. Adicionar item (LOOT)\n");
    printf("2. Remover item\n");
    printf("3. Listar itens na mochila\n");
    printf("4. Ordenar os itens por critério (nome, tipo, prioridade)\n");
    printf("5. Realizar busca binária por nome\n");
    printf("0. Sair\n");
    printf("\nOpcao: ");
}

void inserirItem(struct Item *mochila, int *totalItens) {
    if (*totalItens >= MAX_ITENS) {
        printf("Mochila cheia!\n");
        return;
    }
    int i = *totalItens;
    printf("Nome do item: ");
    fgets(mochila[i].nome, MAX_NOME, stdin);
    mochila[i].nome[strcspn(mochila[i].nome, "\n")] = 0;

    printf("Tipo: ");
    fgets(mochila[i].tipo, 20, stdin);
    mochila[i].tipo[strcspn(mochila[i].tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &mochila[i].quantidade);
    printf("Prioridade (1-5): ");
    scanf("%d", &mochila[i].prioridade);
    limparTela();

    (*totalItens)++;
    printf("Item adicionado!\n");
}

void removerItem(struct Item *mochila, int *totalItens) {
    if (*totalItens == 0) { printf("Nada para remover.\n"); 
        return; 
    }
    
    char nomeBusca[MAX_NOME];
    printf("Nome do item para descartar: ");
    fgets(nomeBusca, MAX_NOME, stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = 0;

    int pos = -1;
    for (int i = 0; i < *totalItens; i++) {
        if (strcmp(mochila[i].nome, nomeBusca) == 0) {
            pos = i;
            break;
        }
    }

    if (pos != -1) {
        for (int i = pos; i < (*totalItens) - 1; i++) {
            mochila[i] = mochila[i + 1];
        }
        (*totalItens)--;
        printf("Item descartado para abrir espaço!\n");
    } else printf("Item não encontrado na mochila.\n");
}

void listarItens(struct Item *mochila, int totalItens) {
    if (totalItens == 0) { 
        printf("\nMochila vazia. Vá saquear!\n"); return; 
    }
    printf("\n--------------------------------------------\n");
    printf("--- CONTEÚDO DA MOCHILA (%d/%d) ---\n", totalItens, MAX_ITENS);
    printf("--------------------------------------------\n");
    for (int i = 0; i < totalItens; i++) {
        printf("[%d] %-15s | Tipo: %-10s | Qtd: %02d | Prio: %d\n", 
               i+1, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
    printf("--------------------------------------------\n");
}

void menuDeOrdenacao(struct Item *mochila, int totalItens) {
    if (totalItens < 2) {
        printf("Itens insuficientes para ordenar.\n");
        return;
    }
    int criterio;
    printf("\n--- ESCOLHA O CRITÉRIO ---\n");
    printf("1. Por Nome (A-Z)\n");
    printf("2. Por Prioridade (Maior primeiro)\n");
    printf("3. Por tipo");
    scanf("%d", &criterio);
    limparTela();

    insertionSort(mochila, totalItens, criterio);
}

void insertionSort(struct Item *mochila, int totalItens, int criterio) {
    for (int i = 1; i < totalItens; i++) {
        struct Item chave = mochila[i];
        int j = i - 1;
        
        if (criterio == 1) {
            // Ordenando por NOME (A-Z)
            while (j >= 0 && strcmp(mochila[j].nome, chave.nome) > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            }
        } else if (criterio == 2) {
            // Ordenando por PRIORIDADE (do maior para o menor)
            while (j >= 0 && mochila[j].prioridade < chave.prioridade) {
                mochila[j + 1] = mochila[j];
                j--;
            }
        } else if (criterio == 3) {
            // Ordenando por TIPO (A-Z)
            while (j >= 0 && strcmp(mochila[j].tipo, chave.tipo) > 0) {
                mochila[j + 1] = mochila[j];
                j--;
            }
        }
        mochila[j + 1] = chave;
    }
    printf("Mochila ordenada!\n");
}

void buscaBinariaPorNome(struct Item *mochila, int totalItens) {
    // A busca binária EXIGE que a lista esteja ordenada por NOME antes
    // Para simplificar, vamos avisar que precisa estar ordenada ou implementar uma ordenação rápida aqui
    printf("Aviso: A busca binaria so funciona se os nomes estiverem em ordem alfabetica.\n");
    char busca[MAX_NOME];
    printf("Nome do item: ");
    fgets(busca, MAX_NOME, stdin);
    busca[strcspn(busca, "\n")] = 0;

    int inicio = 0, fim = totalItens - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int res = strcmp(mochila[meio].nome, busca);
        if (res == 0) {
            printf("Item encontrado! Tipo: %s | Prioridade: %d\n", mochila[meio].tipo, mochila[meio].prioridade);
            return;
        } else if (res < 0) inicio = meio + 1;
        else fim = meio - 1;
    }
    printf("Item nao encontrado.\n");
}

// Struct Item:
// Representa um componente com nome, tipo, quantidade e prioridade (1 a 5).
// A prioridade indica a importância do item na montagem do plano de fuga.

// Enum CriterioOrdenacao:
// Define os critérios possíveis para a ordenação dos itens (nome, tipo ou prioridade).

// Vetor mochila:
// Armazena até 10 itens coletados.
// Variáveis de controle: numItens (quantidade atual), comparacoes (análise de desempenho), ordenadaPorNome (para controle da busca binária).

// limparTela():
// Simula a limpeza da tela imprimindo várias linhas em branco.

// exibirMenu():
// Apresenta o menu principal ao jogador, com destaque para status da ordenação.

// inserirItem():
// Adiciona um novo componente à mochila se houver espaço.
// Solicita nome, tipo, quantidade e prioridade.
// Após inserir, marca a mochila como "não ordenada por nome".

// removerItem():
// Permite remover um componente da mochila pelo nome.
// Se encontrado, reorganiza o vetor para preencher a lacuna.

// listarItens():
// Exibe uma tabela formatada com todos os componentes presentes na mochila.

// menuDeOrdenacao():
// Permite ao jogador escolher como deseja ordenar os itens.
// Utiliza a função insertionSort() com o critério selecionado.
// Exibe a quantidade de comparações feitas (análise de desempenho).

// insertionSort():
// Implementação do algoritmo de ordenação por inserção.
// Funciona com diferentes critérios de ordenação:
// - Por nome (ordem alfabética)
// - Por tipo (ordem alfabética)
// - Por prioridade (da mais alta para a mais baixa)

// buscaBinariaPorNome():
// Realiza busca binária por nome, desde que a mochila esteja ordenada por nome.
// Se encontrar, exibe os dados do item buscado.
// Caso contrário, informa que não encontrou o item.
