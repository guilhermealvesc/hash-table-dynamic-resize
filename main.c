/*
  Integrantes:
  Guilherme Alves Carvalho - 11921BCC016
  João Batista de Sousa Paula - 11911BCC008
  Pedro Leale - 11921BCC008
*/

#include <stdio.h>
#include <string.h>
#include "hashTable.h"
#define QNT_ALUNOS 2287
#include <ctype.h>


//Matricula vai ser passada como chave na tabela hash
struct aluno
{
    char nome[50];
    int notas[3];
};

int main()
{
    char resposta;
    Hash* hashTable = criaHash(QNT_ALUNOS, sizeof(struct aluno));
    if(hashTable == NULL) return 0;

    FILE* arq = fopen("alunos.txt", "r");
    if(arq == NULL) return 0;

    int mat, notas[3];
    char nome[50];
    struct aluno al;
    while(1)
    {
        if(feof(arq)) break;
        fscanf(arq, "%d %s %d %d %d", &mat, nome, notas, notas + 1, notas + 2);
        strcpy(al.nome, nome);
        al.notas[0] = notas[0];
        al.notas[1] = notas[1];
        al.notas[2] = notas[2];
        if(insereHash(hashTable, mat, &al))
        {
            printf("Insercao do aluno %d feita com sucesso!\n", mat);
        }
        else
        {
            printf("Nao e possivel mais inserir na tabela!\n");
            break;
        }
    }
    do
    {
        printf("\n\nquer consultar um aluno na tabela [s,n]");
        scanf(" %c",&resposta);
        struct aluno ibe;
        int matricula;
        resposta=tolower(resposta);
        if(resposta=='s')
        {
            printf("diga a mateicula do aluno\n");
            scanf("%d",&matricula);
            if(buscaHash(hashTable, matricula, &ibe))
            {
                printf("\n\nAluno (%d)   nome %s  nota 1 =  %d  | nota 2 = %d | nota 3 = %d \n",matricula,ibe.nome,ibe.notas[0],ibe.notas[1],ibe.notas[2]);
            }
            else
                printf("\n\n o aluno com a matricula  %d nao existe ",matricula);



        }
        else
        break;


    }while(resposta!='n');









    return 0;
}

