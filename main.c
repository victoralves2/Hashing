#include <stdio.h>
#include <stdlib.h> 

#define N 50

typedef struct registro {
    int mat;
    char nome[50];
    char curso[50];
	int info;
	int disponibilidade; 		//0 – não está disponível (em uso); 1 – disponível
}registro;

void inicializar(char *nomeArq){
    FILE*arq = fopen(nomeArq, "wb");
    registro a;
    int i;
    a.disponibilidade = 1;
    for(i=0; i<N;i++)
        fwrite(&a,sizeof(registro),1,arq);
    fclose(arq);
}

void inserir(char *nomeArq, int x){
    int pos = AcharPosicao(nomeArq, x);
    FILE *arq = fopen(nomeArq, "r+b");
    registro a;
    a.info = x;
    a.disponibilidade = 0;
    fseek(arq, pos*sizeof(registro),SEEK_SET);
    fwrite(&a,sizeof(registro),1,arq);
    fclose(arq);
}

int AcharPosicao (char *nomeArq, int x) {
   int pos = hash(x) ;
   registro a;
   FILE *arq = fopen(nomeArq, "b");   
   fseek (arq, pos*sizeof(registro), SEEK_SET);
   fread (&a, sizeof(registro), 1, arq);
   while (a.disponibilidade == 0) {
       pos = (pos+1)%N;
       fseek (arq, pos*sizeof(registro), SEEK_SET);
       fread (&a, sizeof(registro), 1, arq);
   }
   fclose(arq);
   return pos;
}


int main(){
    
    return 0;
}