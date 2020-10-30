#include "pila.h"
#include <stdlib.h>

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

#define CAPACIDAD_INICIAL_DE_LA_PILA 10

bool pila_redimensionar(pila_t* pila){

	void* datos_nuevos;
	size_t capacidad_inicial=pila->capacidad;
	if((pila->cantidad)<=((pila->capacidad)/4) && (pila->capacidad)>CAPACIDAD_INICIAL_DE_LA_PILA){
		pila->capacidad/=2;
	}
	else if((pila->cantidad)==((pila->capacidad))){
		pila->capacidad*=2;
	}
	if(capacidad_inicial>pila->capacidad || capacidad_inicial<pila->capacidad){
		datos_nuevos=realloc(pila->datos, (pila->capacidad) * sizeof(void*));
		if(datos_nuevos==NULL){
			return false;
		}
		pila->datos=datos_nuevos;
	}
	return true;
}

pila_t* pila_crear(void) {
	
	pila_t* pila = malloc(sizeof(pila_t));
	if(pila == NULL ){
		return NULL;
	}
	pila->capacidad=CAPACIDAD_INICIAL_DE_LA_PILA;
	pila->cantidad=0;
	pila->datos=malloc((pila->capacidad)*sizeof(void*));
	return pila;
}

void pila_destruir(pila_t *pila){

	free(pila->datos);
	free(pila);
}

bool pila_esta_vacia(const pila_t *pila){
	
	if(pila->cantidad==0){
		return true;
	}
	return false;
}

bool pila_apilar(pila_t *pila, void* valor){

	if(pila_redimensionar(pila)==false){
		return false;
	}
	pila->datos[(pila->cantidad)]=valor;
	pila->cantidad=(pila->cantidad)+1;
	return true;
}

void* pila_ver_tope(const pila_t *pila){

	if((pila->cantidad)==0){
		return NULL;
	}
	void* tope=pila->datos[(pila->cantidad)-1];
	return tope;
}

void* pila_desapilar(pila_t *pila){

	if((pila->cantidad)==0){
		return NULL;
	}
	void* tope=pila->datos[(pila->cantidad)-1];
	pila->datos[(pila->cantidad)-1]=NULL;
	pila->cantidad=(pila->cantidad)-1;
	pila_redimensionar(pila);
	return tope;
}