#include <stdio.h>

int soma_intervalo(int vetor[], int n, int inicio, int fim)
{
    int soma = 0;
    for (int i = inicio; i <= fim; i++)
        soma += vetor[i];

    return soma;
}

int main()
{
    //Leitura do vetor
    int n;
    scanf("%d", &n);

    int vector[n];
    for (int i = 0; i < n; i++)
        scanf("%d", &vector[i]);


    // Computaçao das somas maximas
    int inicio = 0;
    int final = 0;
    int soma_max = 0;

    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            int soma = soma_intervalo(vector, n, i, j);
            if (soma > soma_max)
            {
                inicio = i + 1;
                final = j + 1;
                soma_max = soma;
            }
        }
    }

    // Resultado
    printf("%d\n", soma_max);
    printf("%d %d\n", inicio, final);
}