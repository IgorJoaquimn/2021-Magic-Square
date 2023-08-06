#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

typedef struct
{
    int valor_atual;
    int valor_referencia;
    int *possiveis_valores;
    int tamanho;
    int is_completamente_iterado;
} iterador_t;

typedef struct
{
    int ultimo_valor;
    char **permutacoes;

} lista_permutadores_t;

// Clona a matriz de origem com a matriz de destino
void clona_matrix(int n, int destino[n][n], int origem[n][n])
{
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            destino[i][j] = origem[i][j];
}

// ==================DEFINE METODOS DE ITERADOR==================
// Popula array possiveis_valores da estrutura iterador
void preenche_iterador(iterador_t *iterador)
{
    // Preenche de 0 até o valor_atual com 0
    for (int j = 0; j < iterador->valor_atual; j++)
        iterador->possiveis_valores[j] = 0;

    // Preenche do valor_atual até o final do array com o valor de J
    for (int j = iterador->valor_atual; j <= iterador->tamanho; j++)
    {
        iterador->possiveis_valores[j - 1] = j;
        if (j == iterador->valor_referencia)
            iterador->possiveis_valores[j - 1] = 0;
    }
    iterador->is_completamente_iterado = 0;
}

// Verifica se todas as posições do iterador sao 0
int is_iterador_nulo(iterador_t *iterador)
{
    for (int j = 0; j < iterador->tamanho - 1; j++)
        if (iterador->possiveis_valores[j] != 0)
            return 0;

    return 1;
}

// Move o valor_atual = J para J + 1 e torna possiveis_valores[J] = 0
void incrementa_iterador(iterador_t *iterador)
{
    iterador->possiveis_valores[iterador->valor_atual - 1] = 0;

    if (iterador->valor_atual >= iterador->tamanho)
    {
        iterador->is_completamente_iterado = 1;
        return;
    }

    iterador->valor_atual++;
    if (iterador->possiveis_valores[iterador->valor_atual - 1] == 0)
        incrementa_iterador(iterador);

    iterador->is_completamente_iterado = is_iterador_nulo(iterador);
}

// Método recursivo para gerar as somas
int incrementa_lista_iteradores(iterador_t *ultimo_iterador, iterador_t *primeiro_iterador)
{
    // Passo base
    if (ultimo_iterador == primeiro_iterador)
    {
        incrementa_iterador(primeiro_iterador);
        return primeiro_iterador->is_completamente_iterado;
    }

    else
    {
        int valor_atual = ultimo_iterador->valor_atual;
        int valor_anterior = (ultimo_iterador - 1)->valor_atual;
        int valor_maximo = ultimo_iterador->tamanho;

        // Enquanto o valor do iterador atual for menor que n^2, ele será incrementado
        if ((valor_atual) < valor_maximo)
            incrementa_iterador(ultimo_iterador);

        // Se o valor atual for igual ao valor maximo, então o iterador percorreu todos os valores
        if (valor_atual == valor_maximo)
        {
            // O novo valor atual será igual ao valor de seu anterior incrementado em 2 posições, porque o anterior será incrementado no prox passo da recursão
            if (valor_anterior + 2 < valor_maximo)
                ultimo_iterador->valor_atual = valor_anterior + 2;

            preenche_iterador(ultimo_iterador);
            return incrementa_lista_iteradores(ultimo_iterador - 1, primeiro_iterador);
        }

        return 0;
    }
}
//==================FIM METODOS DE ITERADOR/==================
// METODOS PARA ENCONTRAR SOMAS MAXIMAS
int get_combinacao_N(int valor, int n, int matriz[n][n], int index_linha)
{

    int soma_maxima = n * (1 + (n * n)) / 2;
    // Quantidade de possiveis numeros no quadrado magico = n^2
    int qtd_de_valores = n * n;

    // Inicializacao da lista de iteradores
    iterador_t lista_iteradores[n - 1];
    for (int i = 0; i < n - 1; i++)

    {
        lista_iteradores[i].valor_atual = valor + i + 1;
        // Inicializacao das variaveis
        lista_iteradores[i].possiveis_valores = (int *)malloc(qtd_de_valores * sizeof(int));
        lista_iteradores[i].tamanho = qtd_de_valores;
        lista_iteradores[i].valor_referencia = valor;
        preenche_iterador(&lista_iteradores[i]);
    }
    // Fim da inicializacao da lista de iteradores
    // Busca pelas combinacoes onde a soma = soma maxima

    int encontrou_soma = 1;
    while (1)
    {
        int soma = valor;
        for (int i = 0; i < n - 1; i++)
            soma += lista_iteradores[i].valor_atual;

        if (soma == soma_maxima)
        {
            int todos_unicos = 1;

            // Garante que todos os valores da soma são unicos
            for (int i = 1; i < n; i++)
                if (lista_iteradores[i].valor_atual == lista_iteradores[i - 1].valor_atual)
                    todos_unicos = 0;

            // Garante que os valores da soma não apareceram na matriz
            for (int i = 0; i < n; i++)
                for (int j = 0; j < n; j++)
                    for (int k = 0; k < n-1; k++)
                        if (matriz[i][j] == lista_iteradores[k].valor_atual)
                            todos_unicos = 0;

            if (todos_unicos)
            {
                matriz[index_linha][0] = valor;
                for (int i = 1; i < n; i++)
                    matriz[index_linha][i] = lista_iteradores[i - 1].valor_atual;

                encontrou_soma = 1;
                break;
            }
        }

        int todos_iteradores_nulos = incrementa_lista_iteradores(&lista_iteradores[n - 2], &lista_iteradores[0]);
        if (todos_iteradores_nulos)
        {
            encontrou_soma = 0;
            break;
        }
    }
    // Fim da busca, matriz saida possui todas as combinacoes

    // printa_iteradores(&lista_iteradores);
    for (int i = 0; i < n - 1; i++)
        free(lista_iteradores[i].possiveis_valores);
    return encontrou_soma;
}

int get_prox_menor_valor(int n, int matriz[n][n])
{
    int lista[n * n];
    for (int i = 0; i < n * n; i++)
        lista[i] = i + 1;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            lista[matriz[i][j] - 1] = 0;

    for (int i = 0; i < n * n; i++)
        if (lista[i] != 0)
            return lista[i];

    return -1;
}

void gera_quadrado(int n,int matrix[n][n])
{

    int matriz[n][n];
    int soma_maxima = n * (1 + (n * n)) / 2;
    // Parte onde são geradas e armazenadas todas as combinacoes que formam a soma maxima
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matriz[i][j] = 0;

    int saidas = 0;
    int i = rand() % (n) + 1;
    int anterior = 0;
    while (saidas != n)
    {
    // printf("Gerou");

        if (anterior != i)
        {

            saidas += get_combinacao_N(i, n, matriz, saidas);
            anterior = i;
            i = get_prox_menor_valor(n, matriz);
            if (i == -1)
                break;
        }
        else
        {

            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    matriz[j][k] = 0;

            i = rand() % (n * n / 2) + 1;

            saidas = 0;
            anterior = 0;
        }
    }
    clona_matrix(n, matrix, matriz);
}


// METODOS DE MATRIZ
int soma_linha_matrix(int n, int matrix[n][n], int posicao)
{
    int soma = 0;
    for (int j = 0; j < n; j++)
        soma += matrix[posicao][j];
    return soma;
}

int soma_coluna_matrix(int n, int matrix[n][n], int posicao)
{
    int soma = 0;
    for (int j = 0; j < n; j++)
        soma += matrix[j][posicao];
    return soma;
}

int soma_diagonal_principal_matrix(int n, int matrix[n][n])
{
    int soma = 0;
    for (int j = 0; j < n; j++)
        soma += matrix[j][j];
    return soma;
}

int soma_diagonal_secundaria_matrix(int n, int matrix[n][n])
{
    int soma = 0;
    for (int j = 0; j < n; j++)
        soma += matrix[j][n - j - 1];
    return soma;
}

// Verifica condições para ser um quadrado magico
int e_quadrado_magico(int n, int matrix[n][n])
{
    // CODIGOS PARA CONFERIR SE A MATRIZ É UM QUADRADO MAGICO
    int soma_maxima = n * (1 + (n * n)) / 2;

    int e_magico = 1;
    e_magico = e_magico && (soma_diagonal_principal_matrix(n, matrix) == soma_maxima);
    e_magico = e_magico && (soma_diagonal_secundaria_matrix(n, matrix) == soma_maxima);
    if (!e_magico)
        return 0;

    for (int i = 0; i < n; i++)
        e_magico = e_magico && (soma_linha_matrix(n, matrix, i) == soma_maxima);

    if (!e_magico)
        return 0;

    for (int i = 0; i < n; i++)
        e_magico = e_magico && (soma_coluna_matrix(n, matrix, i) == soma_maxima);

    if (!e_magico)
        return 0;

    return 1;
}


void imprime_matrix(int n, int matrix[n][n])
{
    if(e_quadrado_magico(n, matrix))
    {
        int soma_maxima = n * (1 + (n * n)) / 2;
        printf("%d %d\n", n, soma_maxima);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d ", matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }
    
}
// FIM METODOS DE MATRIZ



// METODOS PARA MANIPULAR PERMUTACOES
void permuta_elementos_linha(int n, int linha[n], char *string_permutacao)
{
    int nova_linha[n];
    for (int i = 0; i < n; i++)
    {
        // NA POSICAO I fica a linha string_permutacao[i]
        int posicao = (int)string_permutacao[i] - 48;
        nova_linha[i] = linha[posicao];
    }
    for (int i = 0; i < n; i++)
        linha[i] = nova_linha[i];
}

void permuta_linha_matrix(int n, int matrix[n][n], char *string_permutacao)
{
    int nova_matrix[n][n];
    for (int i = 0; i < n; i++)
    {
        // NA POSICAO I fica a linha string_permutacao[i]
        int linha = (int)string_permutacao[i] - 48;
        for (int j = 0; j < n; j++)
            nova_matrix[i][j] = matrix[linha][j];
    }
    clona_matrix(n, matrix, nova_matrix);
}

void permuta_colunas_matrix(int n, int matrix[n][n], char *string_permutacao)
{
    int nova_matrix[n][n];
    for (int i = 0; i < n; i++)
    {
        // NA POSICAO I fica a coluna string_permutacao[i]
        int coluna = (int)string_permutacao[i] - 48;
        for (int j = 0; j < n; j++)
            nova_matrix[j][i] = matrix[j][coluna];
    }
    clona_matrix(n, matrix, nova_matrix);
}

void swap(char *x, char *y)
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

void gera_lista_permutacoes(char *a, int l, int r, lista_permutadores_t *lista)
{
    if (l == r)
    {
        lista->permutacoes[lista->ultimo_valor] = (char *)malloc(sizeof(char) * (r + 2));
        strcpy(lista->permutacoes[lista->ultimo_valor], a);

        lista->permutacoes[lista->ultimo_valor][r + 1] = '\0';

        lista->ultimo_valor++;
    }

    else
    {
        for (int i = l; i <= r; i++)
        {
            swap((a + l), (a + i));
            gera_lista_permutacoes(a, l + 1, r, lista);

            //backtrack
            swap((a + l), (a + i));
        }
    }
}

void permuta_matriz(int n, int matrix[n][n], lista_permutadores_t *lista)
{
    for (int i = 0; i < lista->ultimo_valor; i++)
    {
        // printf("Permutou coluna,");
        if (e_quadrado_magico(n, matrix))
            break;
        permuta_colunas_matrix(n, matrix, lista->permutacoes[i]);

        for (int k = 0; k < n; k++)
        {
            if (e_quadrado_magico(n, matrix))
                break;

            for (int l = 0; l < lista->ultimo_valor; l++)
            {
                if (e_quadrado_magico(n, matrix))
                    break;
                permuta_elementos_linha(n, matrix[k], lista->permutacoes[l]);
                for (int j = 0; j < lista->ultimo_valor; j++)
                {
                    if (e_quadrado_magico(n, matrix))
                        break;
                    permuta_linha_matrix(n, matrix, lista->permutacoes[j]);
                }
            }
        }
    }
}

void permutador(int n, int matrix[n][n])
{
    int quantidade_permutacoes = 1;

    // Fatorial de n
    for (int i = 1; i <= n; i++)
        quantidade_permutacoes *= i;

    lista_permutadores_t lista;
    lista.permutacoes = (char **)malloc(sizeof(char *) * quantidade_permutacoes);
    lista.ultimo_valor = 0;

    char string_permutacao[n + 1];
    string_permutacao[n] = '\0';

    for (int i = 0; i < n; i++)
        string_permutacao[i] = i + '0';

    gera_lista_permutacoes(string_permutacao, 0, n - 1, &lista);

    int count = 0;
    while (!e_quadrado_magico(n, matrix))
    {
        permuta_matriz(n, matrix, &lista);
        count++;
        if (count == 10)
        {
            gera_quadrado(n, matrix);
            count = 0;
        }
    }

    for (int i = 0; i < lista.ultimo_valor; i++)
        free(lista.permutacoes[i]);
    free(lista.permutacoes);
}
// FIM DE METODOS PARA GERAR PERMUTACOES



// METODOS ANALITICOS PARA QUADRADO MAGICO GRANDES
void quadrado_magico_impar(int n)
{
    int matriz[n][n];
    // Inicializa matriz com 0
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            matriz[i][j] = 0;
 
    // Posicao inicial dos index (meio da ultima linha)
    int i = n / 2;
    int j = n - 1;
 
    // Preenche matriz
    for (int num = 1; num <= n * n;) {
        if (i == -1 && j == n)
        {
            j = n - 2;
            i = 0;
        }
        else {
            // Se index passar do limite da matriz
            if (j == n)
                j = 0;

            if (i < 0)
                i = n - 1;
        }
        if (matriz[i][j]) //Se já houver algum elemento nessa posicao
        {
            j -= 2;
            i++;
            continue;
        }
        else
            matriz[i][j] = num++;
 
        j++;
        i--; // 1st condition
    }
    imprime_matrix(n, matriz);
}

void inverte_valor(int n, int j, int k, int matriz1[n][n], int matriz2[n][n])
{
    int aux =0;
    aux = matriz1[j][k];
    matriz1[j][k] = matriz2[j][k];
    matriz2[j][k] = aux;
}

void rotaciona_matrix(int n, int matriz[n][n])
{

    permuta_linha_matrix(n, matriz,"210");
    permuta_colunas_matrix(n, matriz,"210");
}

void quadrado_magico_par_simples(int geracao)
{
    int n = geracao/2;
    int sub_quadrados[4][n][n];
    // PADRAO A B C D 

    for (int i = 0; i < 4; i++)
    {
        gera_quadrado(3, sub_quadrados[i]);
        permutador(3, sub_quadrados[i]);

    }
    for (int i = 0; i < 4; i++)
    {   
        while(sub_quadrados[i][0][0] != 8)
        {
            rotaciona_matrix(n, sub_quadrados[i]);
        }
        if(sub_quadrados[i][0][1] == 3)
        {    
            int clone[n][n];

            for (int j = 0; j < n; j++)
                for (int k = 0; k < n; k++)
                    clone[k][j] = sub_quadrados[i][j][k];
            clona_matrix(n, sub_quadrados[i], clone);
        }
        
    }


    int soma = n*n;
    for (int i = 1; i < 4; i++)
    {
        for (int j = 0; j < n; j++)
        {
            for (int k = 0; k < n; k++)
            {
                sub_quadrados[i][j][k] += soma;
            }
        }
        soma += n*n;
    }


    
    inverte_valor(n,0,0, sub_quadrados[0],sub_quadrados[3]);
    inverte_valor(n,1,1, sub_quadrados[0],sub_quadrados[3]);
    inverte_valor(n,2,0, sub_quadrados[0],sub_quadrados[3]);

    // imprime_matrix(3, sub_quadrados[0]);

    int index[4] = {0,2,3,1};


    int soma_maxima = n * (1 + (n * n)) / 2;
    printf("%d %d\n", n, soma_maxima);
    for (int i = 0; i < n; i++)
    {
        for (int k = 0; k < n-1; k++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d ", sub_quadrados[index[k]][i][j]);
            
            }
        }
        printf("\n");
    }

    for (int i = 0; i < n; i++)
    {
        for (int k = 2; k < 4; k++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d ", sub_quadrados[index[k]][i][j]);
            
            }
        }
        printf("\n");
    }
        // imprime_matrix(3, sub_quadrados[i]);
}
// FIM DE METODOS ANALITICOS PARA QUADRADO MAGICO GRANDES


int main()
{
    srand(time(NULL));
    for (int i = 3; i <= 4; i++)
    {
        int matriz[i][i];
        gera_quadrado(i, matriz);
        permutador(i, matriz);
        imprime_matrix(i, matriz);

    }

    quadrado_magico_impar(5);
    quadrado_magico_par_simples(6);
}
