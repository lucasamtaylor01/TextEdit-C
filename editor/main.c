#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "funcoes.h"

int main()
{
    // String de comando
    char comando[100];

    // String de manipução
    char *texto;

    // Inteiros de referência e posição
    int pos, linha_atual, numero_de_caracteres, numero_de_linhas, marca_pos = 0, marca_linha = 0;

    // Indicadores
    int sem_texto = 0, ind = 0;

    // Variáveis de Pilhas
    char *pilha;
    int t = 0, desempilhado = 1;

    printf("\n\n* * * EDITOR DE TEXTO * * *\n\n>");
    while (ind == 0)
    {
        fgets(comando, 100, stdin);
        for (int i = 0; i <= strlen(comando); i++)
        {

            /*Realiza a abertura do arquivo .txt e realiza a transcrição para o char texto*/
            if (comando[0] == 'A')
            {

                pos = linha_atual = numero_de_caracteres = numero_de_linhas = 0;

                // Leitura do nome do arquivo
                char *nome_arq;
                int tam_nome = 0;
                for (int j = 1; j < strlen(comando); j++)
                    tam_nome++;

                nome_arq = (char *)malloc(tam_nome * sizeof(char));
                for (int j = 1; j < strlen(comando) - 1; j++)
                    nome_arq[j - 1] = comando[j];

                // Manipulação do arquivo de texto
                FILE *arq;
                char c;
                int ch;
                arq = fopen(nome_arq, "r");

                if (arq == NULL)
                {
                    free(nome_arq);
                    printf("Problemas na abertura do arquivo. Tente novamente.\n\n>");
                    break;
                }
                else
                {
                    // Alocação dinâmica do texto
                    for (c = getc(arq); c != EOF; c = getc(arq))
                        numero_de_caracteres++;
                    fclose(arq);
                    texto = (char *)malloc(numero_de_caracteres * sizeof(char));

                    // Transcrição do arquivo para uma string
                    arq = fopen(nome_arq, "r");
                    int aux = 0;
                    while ((ch = fgetc(arq)) != EOF)
                    {
                        texto[aux] = ch;
                        aux++;
                    }
                    free(nome_arq);
                    fclose(arq);

                    // Atualização de dados
                    numero_de_linhas = contagem_linhas(texto);
                    pos = 0;
                    sem_texto = 1;
                    marca_pos = 0, marca_linha = 0;

                    localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_caracteres);
                    exibe_linha(texto, linha_atual, pos);
                    referencia(marca_linha, marca_pos);
                    break;
                }
            }

            /*Finaliza o programa*/
            if (comando[i] == '!')
            {
                if (sem_texto == 0)
                    free(texto);
                printf("Fechando o editor . . .\n");
                ind = 1;
                break;
            }

            /*Verificação se o texto foi inserido ou não*/
            if (sem_texto == 0)
            {
                texto_nulo(marca_linha, marca_pos);
                break;
            }
            else
            {
                /*Sobrescreve o conteúdo do arquivo "salva" para um arq de nome escolhido pelo usuário*/
                if (comando[0] == 'E')
                {
                    // Leitura e alocaçaão dinâmica do nome inserido
                    int tam_nome = 0;
                    for (int j = 1; j < strlen(comando); j++)
                        tam_nome++;
                    char *nome_arq = (char *)malloc(tam_nome * sizeof(char));
                    for (int j = 1; j < strlen(comando) - 1; j++)
                        nome_arq[j - 1] = comando[j];

                    // Validação da extensão do arquivo
                    if (nome_arq[strlen(comando) - 1] != 't' && nome_arq[strlen(comando) - 2] != 'x' && nome_arq[strlen(comando) - 3] != 't' && nome_arq[strlen(comando) - 4] != '.')
                    {
                        free(nome_arq);
                        printf("Aceita-se apenas nomes de arquivo com extenção .txt\nTente novamente.\n\n>");
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        break;
                    }

                    // Transcrição e validação do arquivo sobrescrito
                    FILE *arq;
                    arq = fopen(nome_arq, "w");
                    if (arq == NULL)
                    {
                        free(nome_arq);
                        printf("Problemas na criação do arquivo. Tente novamente.\n\n>");
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                    int salvo = fputs(texto, arq);
                    if (salvo == EOF)
                    {
                        free(nome_arq);
                        printf("Erro de gravação. Tente novamente.\n\n>");
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                    fclose(arq);
                    printf("\nO arquivo intitulado como %s foi salvo com sucesso!\n\n", nome_arq);
                    free(nome_arq);

                    // Exibição padrão de final de comando
                    localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_caracteres);
                    exibe_linha(texto, linha_atual, pos);
                    referencia(marca_linha, marca_pos);
                    break;
                }

                /*Insere uma palavra solicitada pelo usuário na posição atual*/
                else if (comando[0] == 'I')
                {

                    // Alocação dinâmica da palavra
                    int tam_palavra = 0;
                    for (int j = 1; j < strlen(comando); j++)
                        tam_palavra++;
                    char *palavra = (char *)malloc(tam_palavra * sizeof(char));
                    for (int j = 1; j < strlen(comando) - 1; j++)
                        palavra[j - 1] = comando[j];
                    int novo_tam = numero_de_caracteres + tam_palavra;
                    int x = posicao_absoluta(texto, linha_atual, pos);
                    char *texto_aux = insere(texto, palavra, novo_tam, tam_palavra, x);

                    // Transcrição da variável auxiliar
                    free(texto);
                    texto = (char *)malloc(novo_tam * sizeof(char));
                    for (int i = 0; i < novo_tam; i++)
                        texto[i] = texto_aux[i];


                    // Liberação de memória e variáveis utilizadas
                    free(palavra);
                    free(texto_aux);

                    // Atualização dos dados
                    numero_de_linhas = contagem_caracteres(texto);
                    numero_de_caracteres = contagem_caracteres(texto);

                    // Exibição padrão de final de comando
                    localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                    exibe_linha(texto, linha_atual, pos);
                    referencia(marca_linha, marca_pos);
                    break;
                }

                /*Comandos de movimentação na mesma linha (avanço, volta, inicio da linha e fim da linha, respectivamente)*/
                else if (comando[i] == 'T' || comando[i] == 'F' || comando[i] == 'O' || comando[i] == '$')
                {

                    int fim = contagem_caracteres_linha(texto, linha_atual) - 1;
                    if (comando[i] == 'F')
                        pos--;
                    if (comando[i] == 'T')
                        pos++;
                    if (comando[i] == 'O')
                        pos = 0;
                    if (comando[i] == '$')
                        pos = fim;

                    // Restrições
                    if (pos <= 0)
                        pos = 0;
                    if (pos >= fim)
                        pos = fim;
                }

                /*Comando P:Avança para o inicio da próxima palavra || Comando Q: Vai para o inicio da palavra atual*/
                else if (comando[i] == 'P' || comando[i] == 'Q')
                {
                    int x = posicao_absoluta(texto, linha_atual, pos);

                    // procura o início da próxima palavra
                    if (comando[i] == 'P')
                    {
                        for (int j = x; texto[j] != '\n'; j++)
                        {
                            if (texto[j + 1] == ' ')
                            {
                                pos = posicao_relativa(texto, linha_atual, j) + 1;
                                break;
                            }
                        }
                    }

                    // procura o início da palavra atual
                    if (comando[i] == 'Q')
                    {
                        for (int j = x;; j--)
                        {
                            if (texto[j - 1] == ' ')
                            {
                                pos = posicao_relativa(texto, linha_atual, j) - 1;
                                break;
                            }
                            // Restrição
                            if (texto[j - 1] == '\n')
                            {
                                pos = 0;
                                break;
                            }
                        }
                    }
                }

                /*Direciona a uma linha especifica ou ao final do texto*/
                else if (comando[0] == ':')
                {
                    pos = 0;
                    if (comando[1] == 'F')
                    {
                        // direcionamento para o final do texto
                        linha_atual = numero_de_linhas - 1;

                        // Exibição padrão de final de comando
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                    else
                    {
                        // Alocação dinâmica da linha destino
                        int tam_linha_destino = 0;
                        char *linha_destino;
                        for (int j = 1; j < strlen(comando); j++)
                            tam_linha_destino++;

                        linha_destino = (char *)malloc(tam_linha_destino * sizeof(char));
                        for (int j = 1; j < strlen(comando) - 1; j++)
                            linha_destino[j - 1] = comando[j];

                        int x = atoi(linha_destino); // Conversão char->int
                        // Validação do dado inserido
                        if (x >= 0 && x < numero_de_linhas)
                        {
                            linha_atual = x;

                            // Exibição padrão de final de comando
                            localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                            exibe_linha(texto, linha_atual, pos);
                            referencia(marca_linha, marca_pos);
                            break;
                        }

                        // Tratamento para os casos de erro
                        else if (x < 0 || x >= numero_de_linhas)
                        {
                            printf("A linha %d não existe no texto. O texto vai de 0 até %d.\n\n", x, numero_de_linhas);
                            // Exibição padrão de final de comando
                            localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                            exibe_linha(texto, linha_atual, pos);
                            referencia(marca_linha, marca_pos);
                            break;
                        }
                        else
                        {
                            printf("Ocorreu um erro. Tente novamente\n\n");
                            // Exibição padrão de final de comando
                            localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                            exibe_linha(texto, linha_atual, pos);
                            referencia(marca_linha, marca_pos);
                            break;
                        }
                        free(linha_destino);
                    }
                }

                /*Exclusão do caractere atual*/
                else if (comando[i] == 'D')
                {
                    // Exclusão do caractere atual
                    apaga(texto, numero_de_caracteres, posicao_absoluta(texto, linha_atual, pos));

                    // Atualização de dados
                    numero_de_caracteres = contagem_caracteres(texto);
                    numero_de_linhas = contagem_linhas(texto);
                }

                /*Marcação de linha e caractere atuais*/
                else if (comando[i] == 'M')
                {
                    marca_pos = pos;
                    marca_linha = linha_atual;
                }

                /*Desempilha conteúdo da pilha (colar)*/
                else if (comando[i] == 'V')
                {
                    // Verificação da validade comando
                    if (desempilhado == 0)
                    {
                        printf("O texto já foi desempilhado!\n");
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                    else
                    {

                        // Alocação dinâmica do texto com conteúdo desempilhado
                        int novo_tam = t + numero_de_caracteres;
                        int x = posicao_absoluta(texto, linha_atual, pos);
                        char *novo_texto = desempilha(texto, pilha, novo_tam, t, x);

                        // Transcrição para a a variável texto
                        free(texto);
                        texto = (char *)malloc(novo_tam * sizeof(char));
                        for (int i = 0; i < novo_tam; i++)
                            texto[i] = novo_texto[i];

                        // Atualização de dados
                        numero_de_linhas = contagem_linhas(texto);
                        numero_de_caracteres = contagem_caracteres(texto);
                        free(pilha);
                        desempilhado = 0;
                        t = 0;
                    }
                }

                /*Empilha o conteúdo da pilha (copiar/recortar)*/
                else if (comando[i] == 'C' || comando[i] == 'X')
                {
                    // Definição dos intervalos para o empilhamento
                    int pos_pilha = posicao_absoluta(texto, linha_atual, pos);
                    int pos_marcada = posicao_absoluta(texto, marca_linha, marca_pos);
                    int capacidade = abs(pos_marcada - pos_pilha);
                    pilha = (char *)malloc(capacidade * sizeof(char));

                    // Empilhamento
                    if (pos_pilha <= pos_marcada)
                    {
                        for (int j = pos_marcada; j >= pos_pilha; j--)
                        {
                            pilha[t++] = texto[i];
                            if (comando[j] == 'X')
                                apaga(texto, numero_de_caracteres, j);
                        }
                    }

                    if (pos_pilha > pos_marcada)
                    {
                        for (int j = pos_pilha; j >= pos_marcada; j--)
                        {
                            pilha[t++] = texto[j];
                            if (comando[i] == 'X')
                                apaga(texto, numero_de_caracteres, j);
                        }
                    }

                    // Atualização de dados pós exclusão de caracteres
                    if (comando[i] == 'X')
                    {

                        numero_de_linhas = contagem_linhas(texto);
                        numero_de_caracteres = contagem_caracteres(texto);
                    }
                    desempilhado = 1; // indicação liberando o desempilhamento

                    printf("Empilhado com sucesso!\n\n");
                }

                /*Une duas linhas*/
                else if (comando[i] == 'U')
                {
                    // Verificação da validade comando
                    if (linha_atual == numero_de_linhas - 1)
                    {
                        printf("Não há mais linhas para unir. Tente outro comando.\n");
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                    else
                    {
                        // Exclusão da quebra de linha (\n) para promover união
                        for (int j = posicao_absoluta(texto, linha_atual, pos);; j++)
                        {
                            if (texto[j] == '\n')
                            {
                                apaga(texto, numero_de_caracteres, j);
                                break;
                            }
                        }
                        // Atualização de dados
                        numero_de_linhas = contagem_linhas(texto);
                        numero_de_caracteres = contagem_caracteres(texto);
                    }
                }

                /*Movimentação entre linhas (avanço ou volta)*/
                else if (comando[i] == 'J' || comando[i] == 'H')
                {
                    pos = 0;
                    if (comando[i] == 'J')
                        linha_atual++;
                    if (comando[i] == 'H')
                        linha_atual--;

                    // Restrições
                    if (linha_atual >= numero_de_linhas)
                    {
                        printf("Você está na última linha do texto\n\n");
                        linha_atual = numero_de_linhas - 1;

                        // Exibição padrão de final de comando
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                    if ((linha_atual) <= 0)
                    {
                        printf("Você está na primeira linha do texto\n\n");
                        linha_atual = 0;

                        // Exibição padrão de final de comando
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                }

                /*Exibe o conteúdo empilhado*/
                else if (comando[i] == 'Z')
                {
                    // Verificação da validade comando
                    if (t == 0)
                        printf("pilha vazia\n");
                    else
                    {
                        printf("Exibindo a pilha de memória: \n");
                        for (int j = t - 1; j >= 0; j--)
                            printf("%c", pilha[j]);
                        printf("\n");
                        referencia(marca_linha, marca_pos);
                        break;
                    }
                }

                /*Cria uma nova linha a partir da posição atual*/
                else if (comando[i] == 'N')
                {
                    // Definição de posições absolutas
                    int x = posicao_absoluta(texto, linha_atual, pos);

                    // Laço para inserção do caractere \n
                    for (int j = numero_de_caracteres; j > x; j--)
                    {
                        texto[j] = texto[j - 1];
                    }
                    texto[x] = '\n';

                    // Atualização de dados
                    numero_de_caracteres = contagem_caracteres(texto);
                    numero_de_linhas = contagem_linhas(texto);
                    pos = contagem_caracteres_linha(texto, linha_atual) - 1;
                }

                /*Busca de próxima ocorrência da palavra*/
                else if (comando[0] == 'B')
                {

                    // Alocação dinâmica da palavra buscada
                    int tam_palavra = 0;
                    for (int j = 1; j < strlen(comando); j++)
                        tam_palavra++;
                    char *palavra = (char *)malloc(tam_palavra * sizeof(char));
                    for (int j = 1; j < strlen(comando) - 1; j++)
                        palavra[j - 1] = comando[j];

                    // busca palavra
                    int y = posicao_absoluta(texto, linha_atual, pos);
                    int x = busca(texto, numero_de_caracteres, palavra, tam_palavra, y);

                    if (x == 0)
                    {
                        printf("A palavra %s não foi encontrada em nenhum lugar do texto\n\n", palavra);

                        // Exibição padrão de final de comando
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        free(palavra);
                    }
                    else
                    {
                        // Conversão para posição relativa
                        linha_atual = descobre_linha(texto, x);
                        pos = posicao_relativa(texto, linha_atual, x) - 1;

                        // Exibição padrão de final de comando
                        localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                        exibe_linha(texto, linha_atual, pos);
                        referencia(marca_linha, marca_pos);
                        free(palavra);
                    }
                    break;
                }

                /*Exibição padrão de final de comando*/
                else if (comando[i] == '\0')
                {
                    localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                    exibe_linha(texto, linha_atual, pos);
                    referencia(marca_linha, marca_pos);
                    break;
                }

                /*Substitui próxima ocorrência da palavra*/
                else if (comando[0] == 'S')
                {
                    // Identificação da posição do divisor
                    int divisor;
                    for (int j = 1;; j++)
                    {
                        if (comando[j] == '/')
                        {
                            divisor = j;
                            break;
                        }
                    }

                    // Definição do tamanho das palavras
                    int tam_bus = 0, tam_subs = 0;
                    for (int j = 1; j < strlen(comando); j++)
                    {
                        if (j < divisor)
                            tam_bus++;
                        if (j > divisor)
                            tam_subs++;
                    }

                    // Criação e definição de conteúdo das strings
                    char buscada[50], subs[50];
                    for (int j = 1; j < strlen(comando) - 1; j++)
                    {
                        if (j < divisor)
                            buscada[j - 1] = comando[j];
                        if (j > divisor)
                            subs[j - divisor - 1] = comando[j];
                    }

                    // Busca
                    int y = posicao_absoluta(texto, linha_atual, pos);
                    int x = busca(texto, numero_de_caracteres, buscada, tam_bus + 1, y);

                    // Conversão para posição relativa
                    linha_atual = descobre_linha(texto, x);
                    pos = posicao_relativa(texto, linha_atual, x) - 1;

                    // Apagando o conteúdo anterior
                    int k = 0;
                    while (k < tam_bus)
                    {
                        apaga(texto, numero_de_caracteres, x);
                        numero_de_caracteres = contagem_caracteres(texto);
                        numero_de_linhas = contagem_linhas(texto);
                        k++;
                    }

                    // Alocação dinâmica do novo conteúdo
                    int novo_tam = tam_subs + numero_de_caracteres;
                    char *texto_aux = insere(texto, subs, novo_tam, tam_subs + 1, x);

                    // Transcrição do novo conteúdo
                    free(texto);
                    texto = (char *)malloc(novo_tam * sizeof(char));
                    for (int i = 0; i < novo_tam; i++)
                        texto[i] = texto_aux[i];

                    // Atualização dos dados
                    numero_de_linhas = contagem_caracteres(texto);
                    numero_de_caracteres = contagem_caracteres(texto);
                    free(texto_aux);

                    // Exibição padrão de final de comando
                    localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                    exibe_linha(texto, linha_atual, pos);
                    referencia(marca_linha, marca_pos);
                    break;
                }

                /*Comandos inválidos*/
                else if (comando[i] == 'G' || comando[i] == 'K' || comando[i] == 'L' || comando[i] == 'W' || comando[i] == 'Y' || comando[i] >= 97 && comando[i] <= 122)
                {
                    comando_invalido(comando, i);

                    // Exibição padrão de final de comando
                    localizacao(texto, pos, linha_atual, numero_de_caracteres, numero_de_linhas);
                    exibe_linha(texto, linha_atual, pos);
                    referencia(marca_linha, marca_pos);
                    break;
                }
            }
        }
    }
    return 0;
}