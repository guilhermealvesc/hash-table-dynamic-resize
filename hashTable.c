#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

#define LOADFACTOR 0.75 //Fator de tamanho para dynamic resizing

//Item único da tabela
typedef struct item {
  int key;
  void* data;
} Item;

//Estrutura para tabela hash, variável item size adicionada
//para ter o tamanho dos itens na memória
struct hash {
  int qnt, TABLE_SIZE, ITEM_SIZE;
  Item **itens;
};

int keyFold(int chave, int TABLE_SIZE);
int sondagemLinear(int pos, int i, int TABLE_SIZE);
int dynamicResize(Hash* ha);

//Função para criar a tabela hash dado o tamanho dos tipos
Hash* criaHash(int TABLE_SIZE, int TAMANHO_TIPO) {
  Hash* ha = (Hash*) malloc(sizeof(Hash));
  if(ha == NULL) return NULL;
  int i;
  ha->TABLE_SIZE = TABLE_SIZE;
  ha->ITEM_SIZE = TAMANHO_TIPO;
  ha->itens = (Item**) malloc(TABLE_SIZE * sizeof(Item*));
  if(ha->itens == NULL) {
    free(ha);
    return NULL;
  }
  ha->qnt = 0;
  for(i = 0; i < TABLE_SIZE; i++) {
    ha->itens[i] = NULL;
  }
  return ha;
}

//Função para liberar a tabela
void liberaHash(Hash* ha) {
  if(ha == NULL) return;
  int i;
  for(i = 0; i < ha->TABLE_SIZE; i++) {
    if(ha->itens[i] != NULL) {
      free(ha->itens[i]->data);
      free(ha->itens[i]);
    }
  }
  free(ha->itens);
  free(ha);
  return;
}

//FALTA DYNAMIC RESIZING
//Função para inserir elemento na tabela
int insereHash(Hash* ha, int chave, void *dados) {
  if (ha == NULL || ha->qnt == ha->TABLE_SIZE)
    return 0;

  //Se a tabela precisar de ser redimensionada
  //e o redimensionamento falhar == erro
  if(ha->qnt/(float) ha->TABLE_SIZE > LOADFACTOR && !dynamicResize(ha))
    return 0;

  int i, newPos;
  int pos = keyFold(chave, ha->TABLE_SIZE);
  for(i = 0; i < ha->TABLE_SIZE; i++) {
    newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
    if(ha->itens[newPos] == NULL) {
      Item* item = (Item*) malloc(sizeof(Item));
      if(item == NULL) return 0;
      item->key = chave;
      item->data = (void *) malloc(ha->ITEM_SIZE);
      if(item->data == NULL) {
        free(item);
        return 0;
      }
      memcpy(item->data, dados, ha->ITEM_SIZE);
      ha->itens[newPos] = item;
      ha->qnt++;
      return 1;
    }
  }
  return 0;
}

//Função para buscar um elemento em uma tabela hash
int buscaHash(Hash* ha, int chave, void *dados) {
  if(ha == NULL) return 0;
  int i, newPos;
  int pos = keyFold(chave, ha->TABLE_SIZE);
  for(i = 0; i < ha->TABLE_SIZE; i++) {
    newPos = sondagemLinear(pos, i, ha->TABLE_SIZE);
    if(ha->itens[newPos] == NULL)
      return 0;
    if(ha->itens[newPos]->key == chave) {
      memcpy(dados, ha->itens[newPos]->data, ha->ITEM_SIZE);
      return 1;
    }
  }
  return 0;
}


//Metodo da dobra para hash
int keyFold(int chave, int TABLE_SIZE) {
  int num_bits = 10;
  int p1 = chave >> num_bits;
  int p2 = chave & (TABLE_SIZE - 1);
  return p1 ^ p2;
}

//Metodo de sondagem linear em enderaçamento aberto
int sondagemLinear(int pos, int i, int TABLE_SIZE) {
  return ((pos + i) & 0x7FFFFFFF) % TABLE_SIZE;
}

//Função para redimensionar a tabela com o tamanho dobrado
int dynamicResize(Hash* ha) {
  if(ha == NULL) return 0;
  //Cria tabela com tamanho dobrado
  int timesTwo = ha->TABLE_SIZE * 2;
  Item** newHa = (Item**) malloc(timesTwo * sizeof(Item*));
  if(newHa == NULL) return 0;

  //inializa newHa com NULL

    for(int i=0; i< timesTwo; i++)
    {
        newHa[i]=NULL;
    }

  //Passando os ponteiros da tabela anterior para a nova
  int i, j, pos, newPos;
  for(i = 0; i < ha->TABLE_SIZE; i++) {
    if(ha->itens[i] != NULL) {
      //Sondagem linear na nova tabela com dobro de tamanho
      pos = keyFold(ha->itens[i]->key, timesTwo);
      for(j = 0; j < timesTwo; j++) {
        newPos = sondagemLinear(pos, j, timesTwo);
        if(newHa[newPos] == NULL) {
          newHa[newPos] = ha->itens[i];
          break;
        }
      }
    }
  }
  free(ha->itens);
  ha->itens = newHa;
  ha->TABLE_SIZE = timesTwo;
  return 1;
}
