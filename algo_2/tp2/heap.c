#include "heap.h"
#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>


#define TAMANIO_INICIAL 7

struct heap{
	void **datos;
	size_t cantidad;
	size_t capacidad;
	cmp_func_t cmp;
};

heap_t *heap_crear(cmp_func_t cmp){

	if(cmp==NULL) return NULL;
	heap_t *heap = malloc(sizeof(heap_t));
	if(heap == NULL) return NULL;
	heap->cantidad = 0;
	heap->capacidad = TAMANIO_INICIAL;
	heap->cmp = cmp;
	heap->datos = malloc(heap->capacidad*sizeof(void*));
	if(heap->datos == NULL){
		free(heap);
		return NULL;
	}
	
	return heap;
}

void swap(void **dato1, void **dato2){

	void *auxiliar = (*dato1);
	(*dato1) = (*dato2);
	(*dato2) = auxiliar;
}

void downheap(void **arreglo, size_t tamanio, int pos_padre, cmp_func_t cmp){

	int pos_hijo_izquierdo;
	int pos_hijo_derecho;
	int pos_minimo;
	if(pos_padre >= tamanio) return;
	pos_hijo_izquierdo = 2*pos_padre+1;
	pos_hijo_derecho = 2*pos_padre+2;
	pos_minimo = pos_padre;
	if( (pos_hijo_izquierdo < (int)tamanio) && (cmp(arreglo[pos_hijo_izquierdo], arreglo[pos_minimo]) > 0) ){
		pos_minimo = pos_hijo_izquierdo;
	}
	if( (pos_hijo_derecho < (int)tamanio) && (cmp(arreglo[pos_hijo_derecho], arreglo[pos_minimo]) > 0) ){
		pos_minimo = pos_hijo_derecho;
	}
	if(pos_minimo != pos_padre){
		swap(&arreglo[pos_minimo], &arreglo[pos_padre]);
		downheap(arreglo, tamanio, pos_minimo, cmp);
	}
}


void upheap(void **arreglo, int pos_hijo, cmp_func_t cmp){

	int pos_padre;
	if(pos_hijo <= 0) return;
	pos_padre = (pos_hijo-1)/2;
	if(cmp(arreglo[pos_hijo], arreglo[pos_padre]) > 0){
		swap(&arreglo[pos_hijo], &arreglo[pos_padre]);
		upheap(arreglo, pos_padre, cmp);
	}
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){

	heap_t *heap = heap_crear(cmp);
	if(heap == NULL) return NULL;
	if(n==0) return heap;
	for (int i = 0; i < n; i++){
		heap_encolar(heap,arreglo[i]);
	}
	heap->cantidad = n;
	heap->capacidad = n*2+1;
	heap->cmp = cmp;
	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){

	if(destruir_elemento != NULL){
		for(int i = 0; i < heap->cantidad; i++){
			destruir_elemento(heap->datos[i]);
		}
	}
	free(heap->datos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){

	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){

	if(heap_cantidad(heap) == 0) return true;
	return false;
}

bool heap_encolar(heap_t *heap, void *elem){

	int pos_elemento = (int)heap->cantidad;
	void **auxiliar;
	if(heap->capacidad == heap_cantidad(heap)){
		heap->capacidad = heap->capacidad*2+1; 
		auxiliar = realloc(heap->datos, heap->capacidad*sizeof(void*));
		if(auxiliar == NULL){
			heap->capacidad = (heap->capacidad-1)/2; 
			return false;
		}
		heap->datos = auxiliar;
	}
	heap->datos[pos_elemento] = elem;
	upheap(heap->datos, pos_elemento, heap->cmp);
	heap->cantidad++;
	return true;
}

void *heap_ver_max(const heap_t *heap){
	if(heap->cantidad==0) return NULL;
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){

	if(heap->cantidad==0) return NULL;
	int pos_elem = 0;
	void *elem_desencolado = heap->datos[pos_elem];
	void **auxiliar;
	heap->datos[pos_elem]=heap->datos[heap->cantidad-1];
	heap->cantidad--;
	downheap(heap->datos, heap->cantidad, pos_elem, heap->cmp);
	if(heap->capacidad == heap_cantidad(heap)/4){
		heap->capacidad = heap->capacidad/2; 
		auxiliar = realloc(heap->datos, heap->capacidad*sizeof(void*));
		if(auxiliar == NULL){
			heap->capacidad = heap->capacidad*2; 
			return false;
		}
		heap->datos = auxiliar;
	}
	return elem_desencolado;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){

	heap_t *heap = heap_crear_arr(elementos, cant, cmp);
	if(heap == NULL) return;
	size_t i=cant-1;
	while(heap_esta_vacio(heap)==false){
		elementos[i]=heap_desencolar(heap);
		i--;
	}
	heap_destruir(heap, NULL);
}
