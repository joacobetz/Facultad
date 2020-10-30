#include "hash.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#define CAPACIDAD_INICIAL 35
#define VACIO 'V'
#define OCUPADO 'O'
#define BORRADO 'B'
#define MAXIMO_FACTOR_CARGA 0.4

typedef struct campo{
	char *clave;
	void *valor;
	char estado;
}campo_t;

struct hash {
	size_t cantidad;
	size_t capacidad;
	hash_destruir_dato_t destruir_dato;
	campo_t *campos;
};

struct hash_iter {
	const hash_t *hash;
	size_t posicion_actual;
};

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){

	hash_t* hash = malloc(sizeof(hash_t));
	if(hash == NULL) return NULL;
	hash->cantidad = 0;
	hash->capacidad = CAPACIDAD_INICIAL;
	hash->destruir_dato = destruir_dato;
	hash->campos = malloc(sizeof(campo_t)*CAPACIDAD_INICIAL);
	if(hash->campos == NULL){
		free(hash);
		return NULL;
	}
	for(int i = 0; i < CAPACIDAD_INICIAL; i++){
		campo_t campo;
		campo.estado = VACIO;
		campo.clave = NULL;
		campo.valor = NULL;
		hash->campos[i] = campo;
	}
	return hash;
}

unsigned long hashing(const char *clave, size_t largo){
	
	size_t i = 0;
  	unsigned long hash = 0;
  	while(i != largo){
    	hash += clave[i++];
    	hash += hash << 10;
   		hash ^= hash >> 6;
  	}
  	hash += hash << 3;
  	hash ^= hash >> 11;
  	hash += hash << 15;
  	return hash;
}

size_t posicion_clave(const hash_t* hash, const char* clave, bool *clave_encontrada){

	size_t posicion = hashing(clave, strlen(clave))%hash->capacidad;
	while( (hash->campos[posicion].estado != VACIO) && (!(*clave_encontrada)) ){
		if(strcmp(hash->campos[posicion].clave,clave)==0 && hash->campos[posicion].estado != BORRADO){
			(*clave_encontrada) = true;
		} else if(hash->capacidad == posicion+1){
			posicion = 0;
		} else{
			posicion++;
		}
	}
	return posicion;
}

void reacomodar(hash_t* hash, campo_t* arreglo_nuevo){
	
	size_t posicion;
	for(size_t i = 0; i < hash->capacidad; i++){
		campo_t campo;
		campo.estado = VACIO;
		campo.clave = NULL;
		campo.valor = NULL;
		arreglo_nuevo[i] = campo;
	}
	for(size_t i = 0; i < hash->capacidad/2; i++){
		if(hash->campos[i].estado == OCUPADO){
			posicion = (hashing(hash->campos[i].clave, strlen(hash->campos[i].clave)))%hash->capacidad;
			while(arreglo_nuevo[posicion].estado != VACIO){
				if(i == hash->capacidad-1){
					posicion = 0;
				} else{
					posicion++;
				}
			}
			arreglo_nuevo[posicion].estado = OCUPADO;
			arreglo_nuevo[posicion].clave = hash->campos[i].clave;
			arreglo_nuevo[posicion].valor = hash->campos[i].valor;
		}
		else if(hash->campos[i].estado==BORRADO){
			free(hash->campos[i].clave);
		}
	}
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato){

	char* copia;
	bool hay_cambio_valor = false;
	size_t posicion = posicion_clave(hash, clave, &hay_cambio_valor);
	if(!hay_cambio_valor){
		copia = malloc(strlen(clave)+1);
		strcpy(copia,clave);
		hash->campos[posicion].clave = copia;
		hash->campos[posicion].estado = OCUPADO;
		hash->cantidad++;

	}else{

		if(hash->destruir_dato != NULL){
			hash->destruir_dato(hash->campos[posicion].valor);
		}
	}
	hash->campos[posicion].valor = dato;

	float factor_de_carga = (float)hash->cantidad/(float)hash->capacidad;
	if(factor_de_carga >= MAXIMO_FACTOR_CARGA){
		
		campo_t* arreglo_nuevo = malloc(hash->capacidad*2*sizeof(campo_t));
		if(arreglo_nuevo == NULL) return false;
		hash->capacidad *= 2;
		reacomodar(hash, arreglo_nuevo);
		free(hash->campos);									
		hash->campos = arreglo_nuevo;
	}
	return true;
}	

void *hash_borrar(hash_t *hash, const char *clave){
	
	if(hash->cantidad==0) return NULL;
	bool clave_encontrada = false;
	size_t posicion = posicion_clave(hash, clave, &clave_encontrada);
	if(!clave_encontrada) return NULL;
	hash->campos[posicion].estado = BORRADO;
	hash->cantidad--;
	return hash->campos[posicion].valor;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	
	bool clave_encontrada = false;
	size_t posicion = posicion_clave(hash, clave, &clave_encontrada);
	if(!clave_encontrada) return NULL;
	return hash->campos[posicion].valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){

	bool clave_encontrada = false;
	posicion_clave(hash, clave, &clave_encontrada);
	if(!clave_encontrada) return false;
	return clave_encontrada;
}

size_t hash_cantidad(const hash_t *hash){
	
	return hash->cantidad;
}

void hash_destruir(hash_t *hash){

	for(size_t i = 0; i < hash->capacidad; i++){
		if( (hash->destruir_dato != NULL) && (hash->campos[i].estado ==OCUPADO) ){
			hash->destruir_dato(hash->campos[i].valor);
		}
		if(hash->campos[i].estado !=VACIO){
			free(hash->campos[i].clave);
		}	
	}
	free(hash->campos);
	free(hash);
}

hash_iter_t *hash_iter_crear(const hash_t *hash){
	
	size_t contador = 0;
	hash_iter_t *iter = malloc(sizeof(hash_iter_t));
	if(iter == NULL) return NULL;
	iter->hash = hash;
	if(iter->hash->cantidad != 0){
		while(contador<iter->hash->capacidad && iter->hash->campos[contador].estado != OCUPADO){
			contador++;
		}
	}
	iter->posicion_actual = contador;
	return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
	
	bool siguiente_encontrado = false;
	size_t contador = iter->posicion_actual+1;

	if(contador>=iter->hash->capacidad){
		iter->posicion_actual=iter->hash->capacidad;
		return false;
	}
	while( (contador < iter->hash->capacidad) && (!siguiente_encontrado) ){
		if(iter->hash->campos[contador].estado == OCUPADO){
			siguiente_encontrado = true;
			iter->posicion_actual = contador;
		}else{
			contador++;
		}
	}
	if(contador==iter->hash->capacidad){
		iter->posicion_actual=iter->hash->capacidad;
		return false;
	}
	return siguiente_encontrado;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
	if(iter->posicion_actual==iter->hash->capacidad) return NULL;
	return iter->hash->campos[iter->posicion_actual].clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){

	if(iter->posicion_actual==iter->hash->capacidad) return true;
	if(iter->hash->cantidad==0) return true;
	return false;
}
	
void hash_iter_destruir(hash_iter_t* iter){
	
	free(iter);
}
