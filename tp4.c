
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"
#include "grafo.h"
/*
 * tp3.c
 *
 *  Created on: 16/06/2013
 *      Author: rafaelramon
 */
typedef struct parametros{
	int threadId;
	LinkGrafo grafo;

}Parametros;
void criaParametro(int t, Grafo *grafo,Parametros *para){

	para->threadId = t;
	para->grafo = grafo;

}
#define NUM_THREADS    3
///VARIAVEIS
int numCores = 0;
int corAtual = 0;
int ultimoColorido = 0;
void *colorePorCor(void *threadid){
   Parametros* par;
   par = (Parametros*)threadid;
   while(!ultimoColorido || corAtual<ultimoColorido){
	   permute(par->grafo->vertices, 0, par->grafo->len-1,par->threadId);
	   //SESSÂO CRITICA
	   pthread_mutex_t mutex;
	   pthread_mutex_lock(&mutex);
	   corAtual++;
	   pthread_mutex_unlock(&mutex);
	   par->threadId =corAtual;
   }
   pthread_exit(NULL);

}
int main(int argc, char** argv) {
   pthread_t threads[NUM_THREADS];
   int rc;
   long t;
   int numInstancias	;
   	int count, numErbs;

   	char *ArquivoInput = argv[1];  //Primeiro Parametro:Arquivo de Entrada.
   	char *ArquivoOutput = argv[2]; //Segundo Parametro:Arquivo de Saída.
   	Grafo grafo;


   	FILE *fp;
   	fp = fopen(ArquivoInput,"r"); //Abre arquivo para escrever
   	FILE *arquivoSaida;
   	arquivoSaida = fopen(ArquivoOutput,"w"); //Abre arquivo para escrever
   	if(!fp){
   		printf("Erro na leitura de entrada!");
   		return 0;
   	}


   	fscanf (fp,"%d",&numInstancias);
   	//Passa por todas as Instancias
   	for(count=0;count<numInstancias;count++){
   		fscanf (fp,"%d",&numErbs);
   		incializaGrafo(&grafo, numErbs);
   		carregaGrafo(&grafo,fp);
   		numCores = grafo.len;
   		int countThread=0;
   		while(countThread <NUM_THREADS){
			Parametros param ;
			criaParametro(countThread, &grafo,&param);
			rc = pthread_create(&threads[countThread], NULL, colorePorCor, (void *)&param);
   			countThread++;

   		}
   		corAtual = NUM_THREADS;
   		for(t=0 ; t<NUM_THREADS; t++)
   		   pthread_join(threads[t], NULL);
   		fprintf(arquivoSaida,"%d ",ultimoColorido);
   	}

	  printf("In main: creating thread %ld\n", t);
	  if (rc){
		 printf("ERROR; return code from pthread_create() is %d\n", rc);
		 exit(-1);
	  }


    return (0);
}
