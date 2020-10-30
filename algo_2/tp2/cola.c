#include <stdlib.h>
#include <stdbool.h>
#include "cola.h"

typedef struct nodo{
	struct nodo* prox;
	void* dato;
}nodo_t;

nodo_t* crear_nodo(){
	nodo_t* nodo=malloc(sizeof(nodo_t));
	if(nodo==NULL){
		return NULL;
	}
	nodo->dato=NULL;
	nodo->prox=NULL;
	return nodo;
}

struct cola{
	nodo_t* prim;
	nodo_t* ult;
};

cola_t* cola_crear(void){
	cola_t* cola=malloc(sizeof(cola_t));
	if(cola == NULL){
		return NULL;
	}
	cola->prim=NULL;
	cola->ult=NULL;
	return cola;
}

bool cola_esta_vacia(const cola_t *cola){
	if(cola->prim==NULL){
		return true;
	}
	return false;
}

void cola_destruir(cola_t *cola, void destruir_dato(void*)){
	while(cola_esta_vacia(cola) == false){
		if(destruir_dato!=NULL){
			destruir_dato(cola->prim->dato);
		}
		cola_desencolar(cola);
	}
	free(cola);
}

bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo_nuevo=crear_nodo();
	if(nodo_nuevo==NULL){
		return false;
	}
	nodo_nuevo->dato=valor;

	if(cola_esta_vacia(cola)==true){
		cola->prim=nodo_nuevo;
		cola->ult=nodo_nuevo;
	}
	else{
		cola->ult->prox=nodo_nuevo;
		cola->ult=nodo_nuevo;
	}
	return true;
}

void* cola_ver_primero(const cola_t *cola){
	if(cola_esta_vacia(cola)==true){
		return NULL;
	}
	return cola->prim->dato;
}

void* cola_desencolar(cola_t *cola){
	if(cola_esta_vacia(cola)==true){
		return NULL;
	}
	void* dato=cola->prim->dato;
	nodo_t* nuevo_prim=cola->prim->prox;
	free(cola->prim);
	cola->prim=nuevo_prim;
	return dato;
}