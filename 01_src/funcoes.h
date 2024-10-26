#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// FUNÇÕES DE POSICIONAMENTO
/*Converte posição relativa em posição absoluta*/
int posicao_absoluta(char *texto, int linha, int posicao_relativa)
{
    int j = 0, k = 0;
    for (int i = 0; j <= linha; i++)
    {
        if (j == linha)
        {
            if (k == posicao_relativa)
                return i;
            k++;
        }
        if (texto[i] == '\n')
            j++;
    }
}

/*Converte posição absoluta em posição relativa*/
int posicao_relativa(char *texto, int linha, int posicao_absoluta)
{
    int j = 0, k = 0;
    for (int i = 0; j <= linha; i++)
    {
        if (j == linha)
        {
            k++;
            if (i == posicao_absoluta)
                return k;
        }
        if (texto[i] == '\n')
            j++;
    }
}

/*Auxília na conversão posição absoluta em posição relativa*/
int descobre_linha(char *texto, int x)
{
    int linha = 0;
    for (int i = 0; i < strlen(texto); i++)
    {
        if (i == x)
            return linha;
        if (texto[i] == '\n')
            linha++;
    }
};

// FUNÇÕES DE CONTAGEM
/*Conta o número de caracteres de uma linha*/
int contagem_caracteres_linha(char *texto, int linha_atual)
{
    int j = 0, i = 0, contador = 0;
    while (1)
    {
        if (j == linha_atual)
            contador++;
        if (texto[i] == '\n')
            j++;
        if (j == linha_atual + 1)
            break;
        i++;
    }
    return contador - 1;
}

/*Conta o número de linhas totais de um texto*/
int contagem_linhas(char *texto)
{
    int numero_de_linhas = 0;
    for (int i = 0; i < strlen(texto); i++)
    {
        if (texto[i] == '\n')
            numero_de_linhas++;
    }
    return numero_de_linhas;
}

/*Conta o número de caracteres totais do texto*/
int contagem_caracteres(char *texto)
{
    int numero_caracteres = 0;
    for (int i = 0; i < strlen(texto); i++)
        numero_caracteres++;
    return numero_caracteres;
}

// FUNÇÕES DE EXIBIÇÃO E REFERÊNCIA
/*Apresenta a linha e posição relativa marcada e indica espaço para escrita*/
void referencia(int linha_marcada, int posicao_marcada)
{
    printf("\n%d,%d>", linha_marcada, posicao_marcada);
}

/*Apresenta dados de posição ao usuário */
void localizacao(char *texto, int pos, int linha, int numero_caracteres, int numero_linhas)
{
    printf("posição atual: %d|| linha atual: %d || caractere atual: %c\n", pos, linha, texto[posicao_absoluta(texto, linha, pos)]);
}

/*Responsável pelo posicionamento cursor ^*/
void posicao_cursor(int pos)
{
    for (int k = 0; k <= pos; k++)
    {
        if (k == pos)
            printf("^");
        else
            printf(" ");
    }
}

/*Exibe a linha e divide linhas maiores*/
void exibe_linha(char *texto, int linha_atual, int pos)
{
    int j = 0, quebra_linha = 0, aux_pos = 0, sinal = 0;
    for (int i = 0; j <= linha_atual; i++)
    {
        if (j == linha_atual)
        {
            if (pos == aux_pos) // indica que o cursor está na linha impressa
                sinal = 1;

            aux_pos++;
            printf("%c", texto[i]);
            quebra_linha++;
            if (quebra_linha == 120) // quebra a linha quando chegar ao limite estabelecido de 120 caracteres
            {
                if (sinal == 1) // impressão do cursos na trecho de linha dividido
                {
                    printf("\n");
                    if (pos > 119)
                        pos = pos % 120;
                    posicao_cursor(pos);
                    sinal = 0;
                }
                printf("\n");
                quebra_linha = 0;
            }
        }
        if (texto[i] == '\n')
            j++;
    }

    // Caso para trechos de linhas ou linhas inteiras menores que o limite
    if (aux_pos - pos < 120)
        posicao_cursor(pos % 120);
}

// FUNÇÕES DE MENSAGEM
/*Aviso de comando inválido*/
void comando_invalido(char comando[], int n)
{

    printf("O comando '%c' é inválido!\n\n", comando[n]);
}

/*Aviso de texto não inserido*/
void texto_nulo(int marca_linha, int marca_pos)
{
    printf("Nenhum texto foi inserido no editor. Insira um com o comando [A]\n");
    referencia(marca_linha, marca_pos);
}

// FUNÇÕES DE MANIPULAÇÃO
/*Devolve uma string com o conteúdo do texto e com a insersão entre os indíces x e x+tam_palavra-1 do texto o conteúdo da palavra.
Supõe que a palavra não seja maior que o texto*/
char *insere(char *texto, char *palavra, int novo_tam, int tam_palavra, int x)
{
    // Criação de variável auxiliar que será devolvida
    char *texto_aux = (char *)malloc(novo_tam * sizeof(char));

    // Transcrição
    int l = 0;
    for (int i = 0; i - tam_palavra < novo_tam; i++)
    {
        if (i < x)
            texto_aux[i] = texto[i];
        if (i >= x)
        {
            if (l < tam_palavra - 1)
            {
                texto_aux[i] = palavra[l];
                l++;
            }
            else
                texto_aux[i] = texto[i - tam_palavra + 1];
        }
    }
    return texto_aux;
}

/*Devolve uma string com o conteúdo desempilhado de uma pilha de tamanho t em um texto entre os indíces x e x+t.*/
char *desempilha(char *texto, char *pilha, int novo_tam, int t, int x)
{
    // Criação de variável auxiliar que será devolvida
    char *texto_aux = (char *)malloc(novo_tam * sizeof(char));

    // Transcrição
    int l = t;
    for (int i = 0; i - t < novo_tam; i++)
    {
        if (i <= x)
            texto_aux[i] = texto[i];
        if (i > x)
        {
            if (t > 0)
            {
                texto_aux[i] = pilha[--t];
            }
            else
                texto_aux[i] = texto[i - l];
        }
    }
    return texto_aux;
}

/*Remove o elemento contido no indice x de um texto[0..numero_de_caracteres-1] e remaneja os conteúdos dos indices posteriores*/
char *apaga(char *texto, int numero_de_caracteres, int x)
{
    for (int j = x; j < numero_de_caracteres; j++)
    {
        texto[j] = texto[j + 1];
    }
    return texto;
}

/*Recebe uma palavra[0 .. tam_palavra] e busca ela em um texto[1.. numero_de_caracteres].
Se houver ocorrência dessa palavra, devolve a posição absoluta do primeiro caractere da próxima ocorrência dela. Caso não, devolve o valor 0.
Supõe que tam_palavra > 0 e que texto > tam_palavra*/
int busca(char *texto, int numero_de_caracteres, char *palavra, int tam_palavra, int x)
{
    int inicio, r;
    // Busca no texto
    for (int k = x + 1; k <= numero_de_caracteres; k++)
    {
        r = 0;
        // Conferindo se há aparição da palavra
        while (r < tam_palavra && palavra[r] == texto[k + r])
        {
            inicio = k;
            r++;
        }

        // Devolução de resultados da busca
        if (r >= tam_palavra - 1)
        {
            return inicio;
        }
        if (k == numero_de_caracteres)
        {
            return 0;
        }
    }
};
