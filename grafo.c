#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include<math.h>

#define FALSE 0
#define TRUE  1

#include "grafo.h"

extern int corAtual;
extern int ultimoColorido;
int coloriu=0;
void incializaGrafo(LinkGrafo grafo,int tam){
	//Cria um vetor de ponteiros
	grafo -> vertices = (LinkVertice*) calloc (tam,sizeof(LinkVertice));
	grafo->len = tam;
	grafo->inseridos = 0;
}

float distancia(Coordenadas a, Coordenadas b){
	float difX,difY;
	difX = a.x - b.x;
	difY = a.y - b.y;
	difX = pow(difX,2);
	difY = pow(difY,2);


	return sqrt(difX + difY);
}
void carregaGrafo(LinkGrafo grafo, FILE * p){
	Coordenadas ptInsercao;
	LinkVertice linkVertice;
	int j;
	float raio,x,y;

	for( j= 0; j < grafo->len; j++){
			fscanf (p,"%f %f %f \n", &x,&y,&raio);
			ptInsercao.x = x;
			ptInsercao.y = y;
			linkVertice = criaVertice(ptInsercao,raio);
			insereVertice(linkVertice,grafo,j);
		}
}

int possuiInterferencia(LinkVertice a, LinkVertice b){
	float dist = distancia(a->coord,b->coord);
	float somaRaios = a->raio + b->raio;
	if(dist<=somaRaios)
		return TRUE;
	return FALSE;

}
void criaAresta(LinkVertice a, LinkVertice b){
	inserePrimeiro(a,&b->arestas);
	inserePrimeiro(b,&a->arestas);

}

//busca todas os vértices do grafo e insere quando necessário
void insereArestas(LinkVertice vertice,LinkGrafo grafo){
	int i;
	for(i=0; i<grafo->inseridos; i++){
		if(possuiInterferencia(grafo->vertices[i],vertice))
			criaAresta(vertice, grafo->vertices[i]);
	}
}
void insereVertice (LinkVertice vertice, LinkGrafo grafo, int pos){
	insereArestas(vertice, grafo);
	grafo->vertices[pos] = vertice;
	grafo->inseridos++;

}
void imprimeVertice(LinkVertice vertice){
	printf("[%.1f ,%.1f ];",vertice->coord.x,
			vertice->coord.y);
}
int verificaCorMinimaLivreVizinho(Lista vertices,int menor){
	int i;
	LinkCelula aux = vertices.primeiro;

	for(i=0;i < vertices.len; i++){
		if(menor == aux->vertice->cor ){
			menor = verificaCorMinimaLivreVizinho(vertices, menor+1);
			break;
		}
		else{
			aux = aux->prox;
		}
	}

	return menor;
}
void coloreVertice(LinkVertice vertice){
	vertice->cor = verificaCorMinimaLivreVizinho(vertice->arestas,1);
}



void imprimeVetorVertices(LinkVertice *vertices, int tam){
	int count;
	for(count=0;count<tam;count++){
			   imprimeVertice(vertices[count]);

	}
	printf("\n");
}
int coloreGrafo(LinkVertice *vertices, int tam){
	int count, coresGrafo = 0;
	for(count=0;count<tam;count++){
		coloreVertice(vertices[count]);
		if(coresGrafo < vertices[count]->cor )
			coresGrafo = vertices[count]->cor;

	}
	return coresGrafo;
}
void limpaGrafo(LinkVertice *vertices, int tam){
	int count;
	for(count=0;count<tam-1;count++)
		vertices[count]->cor = 0;
}
void imprimeVerticesGrafo(Grafo grafo){
	 imprimeVetorVertices(grafo.vertices, grafo.len);
}

LinkVertice criaVertice(Coordenadas coord, float raio){
	LinkVertice insert  = (LinkVertice) malloc(sizeof(Vertice));
	insert->coord = coord;
	insert ->raio = raio;
	criaLista(&insert->arestas);
	return insert;

}
/* Function to swap values at two pointers */
void swap (LinkVertice *x, LinkVertice *y)
{
    LinkVertice temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

/* Function to print permutations of string
   This function takes three parameters:
   1. String
   2. Starting index of the string
   3. Ending index of the vertices.
   4. numLimite limit of this thread */
void permute(LinkVertice *a, int i, int n, int numLimite){
   int j,valor;
   if (i == n){
	   valor =  (a,n+1);
//	   imprimeVetorVertices(a,i+1);
	  // printf("TERMINOU %d cores \n",valor);
	   pthread_mutex_t mutex;
	   //Se ainda não coloriu nenhuma vez
	   //SESSÂO CRITICA
	   if (!coloriu||valor< ultimoColorido){
		   pthread_mutex_lock(&mutex);
		   ultimoColorido = valor;
		   pthread_mutex_unlock(&mutex);
	   }
	   if(!coloriu)
		   coloriu=1;
	  ///
//	   pthread_exit(NULL);
   }
   else {
	   limpaGrafo(a,n);
	   valor = coloreGrafo(a,i);
//	   imprimeVetorVertices(a, i);
	   if((!coloriu)||(numLimite <ultimoColorido && valor<=numLimite) ){
		   for (j = i; j <= n; j++){
			  swap((a+i), (a+j));
			  permute(a, i+1, n, numLimite);
			  swap((a+i), (a+j)); //backtrack
		   }
	   }
   }
}
