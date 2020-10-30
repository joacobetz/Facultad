#include "abb.h"
#include "pila.h"
#include <stdlib.h>
#include <string.h>
#include "strutil.h"
#include <stdio.h>

#define SIN_HIJOS 0
#define UN_HIJO 1
#define DOS_HIJOS 2

typedef struct nodo_abb {
	struct nodo_abb *izquierda;
	struct nodo_abb *derecha;
	char *clave;
	void *dato;
} nodo_abb_t;

struct abb {
	nodo_abb_t *raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
};

struct abb_iter {
	const abb_t *arbol;
	pila_t *nodos; 
};

abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){

	abb_t *arbol = malloc(sizeof(abb_t));
	if(arbol == NULL) return NULL;
	arbol->raiz = NULL;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	arbol->cantidad = 0;
	return arbol;
}

nodo_abb_t *nodo_buscado(nodo_abb_t *nodo, const char *clave, abb_comparar_clave_t cmp, bool *clave_encontrada){

	nodo_abb_t *buscado = NULL;
	if(nodo == NULL) return NULL;
	if(cmp(nodo->clave, clave) == 0){
		(*clave_encontrada) = true;
	} else if(cmp(nodo->clave, clave) > 0){
		buscado = nodo_buscado(nodo->izquierda, clave, cmp, clave_encontrada);
	} else{
		buscado = nodo_buscado(nodo->derecha, clave, cmp, clave_encontrada);
	}
	if(buscado == NULL) return nodo;
	return buscado;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato){

	bool clave_encontrada = false;
	nodo_abb_t *nodo = nodo_buscado(arbol->raiz, clave, arbol->cmp, &clave_encontrada);
	nodo_abb_t *nuevo;
	char *copia;
	copia = malloc(strlen(clave) + 1);
	if(copia == NULL) return false;
	strcpy(copia, clave);
	nuevo = malloc(sizeof(nodo_abb_t));
	if(nuevo == NULL){
		free(copia);
		return false;
	}
	nuevo->izquierda = NULL;
	nuevo->derecha = NULL;
	nuevo->clave = copia;
	nuevo->dato = dato;
	if(arbol->raiz==NULL){
		arbol->raiz = nuevo;
	} else if(arbol->cmp(nodo->clave,clave) > 0){
		nodo->izquierda = nuevo;
	} else{
		nodo->derecha = nuevo;
	}
	arbol->cantidad++;
	return true;
}

unsigned int nodo_cantidad_hijos(nodo_abb_t *nodo){

	if( (nodo->derecha == NULL) && (nodo->izquierda == NULL) ) return SIN_HIJOS;
	if( (nodo->derecha == NULL) || (nodo->izquierda == NULL) ) return UN_HIJO;
	return DOS_HIJOS;
}

nodo_abb_t *nodo_reemplazo(nodo_abb_t *nodo, nodo_abb_t *padre){

	nodo_abb_t *reemplazo = NULL;
	if(nodo == NULL) return NULL;
	if(nodo->izquierda != NULL){	
		if(nodo->izquierda->izquierda == NULL) padre = nodo;
	} 

	reemplazo = nodo_reemplazo(nodo->izquierda, padre);
	if(reemplazo == NULL){
		if(padre != NULL){
			if(nodo->derecha != NULL){
				if(padre->derecha == nodo){
					padre->derecha = nodo->derecha;
				} else{
					padre->izquierda = nodo->derecha;
				}
			} else if(padre->derecha == nodo){
				padre->derecha = NULL;
			} else{
				padre->izquierda = NULL;
			}
		}
		return nodo;
	}
	return reemplazo;
}

void abb_borrar_2(nodo_abb_t *nodo_a_borrar, nodo_abb_t* padre, nodo_abb_t **nodo_a_conectar){ //cambiar nombres

	nodo_abb_t *reemplazo;
	nodo_abb_t *padre_reemplazo;
	if(nodo_cantidad_hijos(nodo_a_borrar) == DOS_HIJOS){
		padre_reemplazo = nodo_a_borrar;
		reemplazo = nodo_reemplazo(nodo_a_borrar->derecha, padre_reemplazo);
		free(nodo_a_borrar->clave);
		nodo_a_borrar->clave = reemplazo->clave;
		nodo_a_borrar->dato = reemplazo->dato;
        free(reemplazo);
	} else{
		if(nodo_cantidad_hijos(nodo_a_borrar) == SIN_HIJOS){
			if(padre != NULL){
				if(padre->derecha == nodo_a_borrar){ 
					padre->derecha = NULL;
				} else{
					padre->izquierda = NULL;
				}
			} else{
				(*nodo_a_conectar) = NULL;
			} 
		} else{
			if(nodo_a_borrar->derecha != NULL){
				(*nodo_a_conectar) = nodo_a_borrar->derecha;
			} else{
				(*nodo_a_conectar) = nodo_a_borrar->izquierda;
			}
		}
		free(nodo_a_borrar->clave);
		free(nodo_a_borrar);
	}
}

nodo_abb_t *padre_buscado(nodo_abb_t *nodo, const char *clave, abb_comparar_clave_t cmp){

	if(nodo == NULL) return NULL;
	if( (cmp(nodo->clave, clave) > 0) && (nodo->izquierda != NULL) ){
		if(cmp(nodo->izquierda->clave, clave) == 0){
			return nodo;
		}
		return padre_buscado(nodo->izquierda, clave, cmp);
	}
	if( (cmp(nodo->clave, clave) < 0) && (nodo->derecha != NULL) ){
		if(cmp(nodo->derecha->clave, clave) == 0){
			return nodo;
		}	
		return padre_buscado(nodo->derecha, clave, cmp);
	}
	return NULL;
}

void *abb_borrar(abb_t *arbol, const char *clave){

	bool clave_encontrada = false;
	void *dato_borrado;
	nodo_abb_t *nodo = nodo_buscado(arbol->raiz, clave, arbol->cmp, &clave_encontrada);
	nodo_abb_t *padre = padre_buscado(arbol->raiz, clave, arbol->cmp);
	if(!clave_encontrada) return NULL;
	dato_borrado = nodo->dato;
	if(padre == NULL){
		abb_borrar_2(nodo, padre, &arbol->raiz);
	} else if(padre->derecha == nodo){
		abb_borrar_2(nodo, padre, &padre->derecha);
	} else{
		abb_borrar_2(nodo, padre, &padre->izquierda);
	}
	arbol->cantidad--;
	return dato_borrado;
}

void *abb_obtener(const abb_t *arbol, const char *clave){

	bool clave_encontrada = false;
	nodo_abb_t *nodo = nodo_buscado(arbol->raiz, clave, arbol->cmp, &clave_encontrada);
	if(!clave_encontrada) return NULL;
	return nodo->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave){

	bool clave_encontrada = false;
	nodo_buscado(arbol->raiz, clave, arbol->cmp, &clave_encontrada);
	return clave_encontrada;
}

size_t abb_cantidad(abb_t *arbol){

	return arbol->cantidad;
}

void abb_destruir_rec(nodo_abb_t *nodo, abb_destruir_dato_t destruir_dato){

	if(nodo == NULL) return;
	abb_destruir_rec(nodo->izquierda, destruir_dato);
	abb_destruir_rec(nodo->derecha, destruir_dato);
	if(destruir_dato != NULL) destruir_dato(nodo->dato);
	free(nodo->clave);
	free(nodo);
}

void abb_destruir(abb_t *arbol){

	abb_destruir_rec(arbol->raiz, arbol->destruir_dato);
	free(arbol);
}

bool abb_in_order_rec(nodo_abb_t *nodo, bool visitar(const char *, void *, void *), void *extra){

	bool ok = false;
	if(nodo == NULL) return true;
	ok = abb_in_order_rec(nodo->izquierda, visitar, extra);
	if(!ok) return ok;
	ok = visitar(nodo->clave, nodo->dato, extra);
	if(!ok) return ok;
	ok = abb_in_order_rec(nodo->derecha, visitar, extra);
	return ok;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra){

	abb_in_order_rec(arbol->raiz, visitar, extra);
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol, char *inicio){

	nodo_abb_t *auxiliar = arbol->raiz;
	abb_iter_t *iter = malloc(sizeof(abb_iter_t));
	if(iter == NULL) return NULL;
	iter->arbol = arbol;
	iter->nodos = pila_crear();
	if(iter->nodos == NULL){
		free(iter);
		return NULL;
	}
	char* arreglo_aux[3];
	arreglo_aux[0]="CODIGO_AUXILIAR";
	arreglo_aux[1]=inicio;
	arreglo_aux[2]=NULL;
	char* inicio_cmp=join(arreglo_aux,',');

	while(auxiliar != NULL){
		if(iter->arbol->cmp(auxiliar->clave, inicio_cmp) >= 0){
			pila_apilar(iter->nodos, auxiliar);
			auxiliar = auxiliar->izquierda;
		} else{
			auxiliar = auxiliar->derecha;
		}
	}
	free(inicio_cmp);
	return iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter, char *fin){

	char* fin_cmp=NULL;
	nodo_abb_t *auxiliar = pila_desapilar(iter->nodos);
	if(auxiliar == NULL) return false;
	if(auxiliar->derecha != NULL){
		auxiliar = auxiliar->derecha;
		char* arreglo_aux[3];
		arreglo_aux[0]="CODIGO_AUXILIAR";
		arreglo_aux[1]=fin;
		arreglo_aux[2]=NULL;
		fin_cmp=join(arreglo_aux,',');
		if(iter->arbol->cmp(auxiliar->clave, fin_cmp) <= 0){
			pila_apilar(iter->nodos, auxiliar);
		}
		while(auxiliar->izquierda != NULL){
			auxiliar = auxiliar->izquierda;
			if(iter->arbol->cmp(auxiliar->clave, fin_cmp) <= 0){
				pila_apilar(iter->nodos, auxiliar);
			}
		}
	}	
	free(fin_cmp);
	return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter){

	nodo_abb_t *auxiliar = pila_ver_tope(iter->nodos);
	if(auxiliar != NULL){
		return auxiliar->clave;
	}
	return NULL;
}	

bool abb_iter_in_al_final(const abb_iter_t *iter, char *fin){

	if(pila_esta_vacia(iter->nodos)){
		return true;
	}
	nodo_abb_t *auxiliar = pila_ver_tope(iter->nodos);
	char* arreglo_aux[3];
	arreglo_aux[0]="CODIGO_AUXILIAR";
	arreglo_aux[1]=fin;
	arreglo_aux[2]=NULL;
	char* fin_cmp=join(arreglo_aux,',');
	if(iter->arbol->cmp(auxiliar->clave, fin_cmp) > 0){
		free(fin_cmp);
		return true;
	}
	free(fin_cmp);
	return false;
}

void abb_iter_in_destruir(abb_iter_t* iter){

	pila_destruir(iter->nodos);
	free(iter);
}
