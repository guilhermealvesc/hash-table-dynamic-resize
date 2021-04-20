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

//Matricula vai ser passada como chave na tabela hash
struct aluno {
  char nome[50];
  int notas[3];
};

int main() {
  Hash* hashTable = criaHash(QNT_ALUNOS + 1, sizeof(struct aluno));
  if(hashTable == NULL) return 0;

  FILE* arq = fopen("alunos.txt", "r");
  if(arq == NULL) return 0;

  int mat, notas[3];
  char nome[50];
  struct aluno al;
  while(1) {
    fscanf(arq, "%d %s %d %d %d", &mat, nome, notas, notas + 1, notas + 2);
    if(feof(arq)) break;
    strcpy(al.nome, nome);
    al.notas[0] = notas[0];
    al.notas[1] = notas[1];
    al.notas[2] = notas[2];
    if(insereHash(hashTable, mat, &al)) {
      printf("Insercao do aluno %d feita com sucesso!\n", mat);
    } else {
      printf("Nao e possivel mais inserir na tabela!\n");
      break;
    }
  }
  //Código teste
  int i;
  struct aluno ibe; 
  for(i = 0; i < QNT_ALUNOS + 1; i++) {
    if(buscaHash(hashTable, i, &ibe)) {
      printf("Aluno(%d): %s // Notas: %d %d %d\n", i, ibe.nome, ibe.notas[0], ibe.notas[1], ibe.notas[2]);
    } else {
      printf("Aluno com matricula %d nao existe!\n", i);
    }
  }

  return 0;
}

