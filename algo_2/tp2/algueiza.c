#define _GNU_SOURCE

#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "strutil.h"
#include "pila.h"
#include "cola.h"

#define AGREGAR_ARCHIVO "agregar_archivo"
#define VER_TABLERO "ver_tablero"
#define INFO_VUELO "info_vuelo"
#define PRIORIDAD_VUELOS "prioridad_vuelos"
#define BORRAR "borrar"
#define FORMATO_READ "r"
#define ORDEN_ASC "asc"
#define ORDEN_DESC "desc"
#define TAMANIO_MAXIMO 2000

bool hash_esta_vacio(hash_t *hash){
	
	if(hash_cantidad(hash)==0){
		return true;
	}
	return false;
}
void destruir_arreglo(void* arr){
	for(int i=0;i<=2;i++){
		free(((char**)arr)[i]);
	}free(arr);
}

void destruir_arreglo_hash(void* arreglo){

	for(int i=0; i<=10;i++){
		free(((char**)arreglo)[i]);
	}
	free(arreglo);
}


int cmp_codigo_vuelo_desc(const char* codigo_de_vuelo_1,const char* codigo_de_vuelo_2){
	if(strcmp(codigo_de_vuelo_1,codigo_de_vuelo_2)>0){
		return -1;
	}
	else if(strcmp(codigo_de_vuelo_1,codigo_de_vuelo_2)<0){
		return 1;
	}
	return 0;
}
int cmp_codigo_vuelo(const char* codigo_de_vuelo_1,const char* codigo_de_vuelo_2){
	if(strcmp(codigo_de_vuelo_1,codigo_de_vuelo_2)>0){
		return 1;
	}else if(strcmp(codigo_de_vuelo_1,codigo_de_vuelo_2)<0){
		return -1;
	}
	return 0;
}
int cmp_horario_vuelo_aux(char* fraccion_de_tiempo_1, char* fraccion_de_tiempo_2){
	
	if(atoi(fraccion_de_tiempo_1)>atoi(fraccion_de_tiempo_2)) return 1;
	else if(atoi(fraccion_de_tiempo_1)<atoi(fraccion_de_tiempo_2)) return -1;
	return 0;
}

int cmp_horario_vuelo(const char * tiempo_1, const char *tiempo_2){ 
	
	int comparacion_parcial=0; 
	int contador_fecha=0;
	int contador_hora=0;
	char** arreglo_fecha_hora_1=split(tiempo_1, 'T');
	char** arreglo_fecha_hora_2=split(tiempo_2, 'T');
	char** arreglo_fecha_1=split(arreglo_fecha_hora_1[0], '-');
	char** arreglo_hora_1=split(arreglo_fecha_hora_1[1], ':');
	char** arreglo_fecha_2=split(arreglo_fecha_hora_2[0], '-');
	char** arreglo_hora_2=split(arreglo_fecha_hora_2[1], ':');

	while(contador_fecha<=2){

		comparacion_parcial=cmp_horario_vuelo_aux(arreglo_fecha_1[contador_fecha], arreglo_fecha_2[contador_fecha]);
        if(comparacion_parcial!=0) break;
		contador_fecha++;
	}
	if(comparacion_parcial==0){
		while(contador_hora<=2){
		
			comparacion_parcial=cmp_horario_vuelo_aux(arreglo_hora_1[contador_hora], arreglo_hora_2[contador_hora]);
        	if(comparacion_parcial!=0) break;
			contador_hora++;
		}
	}
	free_strv(arreglo_fecha_hora_1);
    free_strv(arreglo_fecha_hora_2);
    free_strv(arreglo_fecha_1);
    free_strv(arreglo_fecha_2);
    free_strv(arreglo_hora_1);
    free_strv(arreglo_hora_2);
	return comparacion_parcial;
}

int cmp_horario_vuelo_desc(const char * tiempo_1, const char *tiempo_2){ 
	
	int comparacion_parcial=0; 
	int contador_fecha=0;
	int contador_hora=0;
	char** arreglo_fecha_hora_1=split(tiempo_1, 'T');
	char** arreglo_fecha_hora_2=split(tiempo_2, 'T');
	char** arreglo_fecha_1=split(arreglo_fecha_hora_1[0], '-');
	char** arreglo_hora_1=split(arreglo_fecha_hora_1[1], ':');
	char** arreglo_fecha_2=split(arreglo_fecha_hora_2[0], '-');
	char** arreglo_hora_2=split(arreglo_fecha_hora_2[1], ':');

	while(contador_fecha<=2){

		comparacion_parcial=cmp_horario_vuelo_aux(arreglo_fecha_1[contador_fecha], arreglo_fecha_2[contador_fecha]);
        if(comparacion_parcial!=0) break;
		contador_fecha++;
	}
	if(comparacion_parcial==0){
		while(contador_hora<=2){
		
			comparacion_parcial=cmp_horario_vuelo_aux(arreglo_hora_1[contador_hora], arreglo_hora_2[contador_hora]);
        	if(comparacion_parcial!=0) break;
			contador_hora++;
		}
	}
	free_strv(arreglo_fecha_hora_1);
    free_strv(arreglo_fecha_hora_2);
    free_strv(arreglo_fecha_1);
    free_strv(arreglo_fecha_2);
    free_strv(arreglo_hora_1);
    free_strv(arreglo_hora_2);
	return (-1)*comparacion_parcial;
}

int comparacion_abb_asc(const char* clave_1, const char* clave_2){

	char** codigo_tiempo_1=split(clave_1,',');
	char** codigo_tiempo_2=split(clave_2,',');
	int comparacion_parcial=cmp_codigo_vuelo(codigo_tiempo_1[0],codigo_tiempo_2[0]);
	if(comparacion_parcial==0){
        free_strv(codigo_tiempo_1);
        free_strv(codigo_tiempo_2);
		return 0;
	} 
	comparacion_parcial=cmp_horario_vuelo(codigo_tiempo_1[1],codigo_tiempo_2[1]);
	if(comparacion_parcial!=0 || strcmp(codigo_tiempo_2[0],"CODIGO_AUXILIAR")==0){
        free_strv(codigo_tiempo_1);
        free_strv(codigo_tiempo_2);
        return comparacion_parcial;
    }
    comparacion_parcial=cmp_codigo_vuelo(codigo_tiempo_1[0],codigo_tiempo_2[0]);
    free_strv(codigo_tiempo_1);
    free_strv(codigo_tiempo_2);
    return comparacion_parcial;
}

int comparacion_abb_desc(const char* clave_1, const char* clave_2){
	
    char** codigo_tiempo_1=split(clave_1,',');
	char** codigo_tiempo_2=split(clave_2,',');
	int comparacion_parcial=cmp_codigo_vuelo_desc(codigo_tiempo_1[0],codigo_tiempo_2[0]);
	if(comparacion_parcial==0){
        free_strv(codigo_tiempo_1);
        free_strv(codigo_tiempo_2);
        return 0;
    }
	comparacion_parcial=cmp_horario_vuelo_desc(codigo_tiempo_1[1],codigo_tiempo_2[1]);
	if(comparacion_parcial!=0 || strcmp(codigo_tiempo_2[0],"CODIGO_AUXILIAR")==0){
        free_strv(codigo_tiempo_1);
        free_strv(codigo_tiempo_2);
        return comparacion_parcial;
    }
    comparacion_parcial=cmp_codigo_vuelo_desc(codigo_tiempo_1[0],codigo_tiempo_2[0]);
    free_strv(codigo_tiempo_1);
    free_strv(codigo_tiempo_2);
    return comparacion_parcial;
}

int cmp_prioridad_vuelo(const void *a, const void *b){
	
	char* prioridad_1=((char**)a)[0];
	char* prioridad_2=((char**)b)[0];

	if(atoi(prioridad_1)>atoi(prioridad_2)){
		return -1;
	}
	else if(atoi(prioridad_1)<atoi(prioridad_2)){
		return 1;
	}else{
		if(strcmp(((char**)a)[1],((char**)b)[1])>0){
			return 1;
		}else{
			return -1;
		}
	}
}

void agregar_archivo(FILE* archivo, hash_t* hash, abb_t* abb_asc, abb_t* abb_desc){

	char* linea=NULL;
	size_t capacidad=0;
	size_t leidos;

	while((leidos=getline(&linea,&capacidad,archivo))!=-1){
		
		char** arreglo_de_datos=split(linea,',');

		char* clave_hash=strdup(arreglo_de_datos[0]);

    	char** arreglo_hash=malloc(sizeof(char*)*11);
    	arreglo_hash[0]=strdup(arreglo_de_datos[0]);
    	arreglo_hash[1]=strdup(arreglo_de_datos[1]);
    	arreglo_hash[2]=strdup(arreglo_de_datos[2]);
    	arreglo_hash[3]=strdup(arreglo_de_datos[3]);
    	arreglo_hash[4]=strdup(arreglo_de_datos[4]);
    	arreglo_hash[5]=strdup(arreglo_de_datos[5]);
    	arreglo_hash[6]=strdup(arreglo_de_datos[6]);
    	arreglo_hash[7]=strdup(arreglo_de_datos[7]);
    	arreglo_hash[8]=strdup(arreglo_de_datos[8]);
    	arreglo_hash[9]=strdup(arreglo_de_datos[9]);
        arreglo_hash[10]=NULL;
    	

    	char** datos_abb=malloc(sizeof(char*)*3);
    	datos_abb[0]=strdup(arreglo_de_datos[0]);
    	datos_abb[1]=strdup(arreglo_de_datos[6]);
    	datos_abb[2]=NULL;

    	char** datos_abb_2=malloc(sizeof(char*)*3);
    	datos_abb_2[0]=strdup(arreglo_de_datos[0]);
    	datos_abb_2[1]=strdup(arreglo_de_datos[6]);
    	datos_abb_2[2]=NULL;
    
    	char* clave_abb=join(datos_abb,',');
    	char* clave_abb_2=join(datos_abb_2,',');

    	if(hash_pertenece(hash,clave_hash)){
    		char **arreglo_hash_obtenido = hash_obtener(hash,clave_hash);
    		char *arreglo_a_borrar[3];
    		arreglo_a_borrar[0] = clave_hash;
    		arreglo_a_borrar[1] = arreglo_hash_obtenido[6];
    		arreglo_a_borrar[2] = NULL;
    		char* clave_a_borrar = join(arreglo_a_borrar, ',');
    		destruir_arreglo(abb_borrar(abb_asc,clave_a_borrar));
    		destruir_arreglo(abb_borrar(abb_desc,clave_a_borrar));
    		free(clave_a_borrar);
    	}
    	abb_guardar(abb_asc, clave_abb, datos_abb);

    	abb_guardar(abb_desc, clave_abb_2, datos_abb_2);

		hash_guardar(hash, clave_hash, arreglo_hash);

		free(clave_hash);
		free(clave_abb);
		free(clave_abb_2);
		free_strv(arreglo_de_datos);
	}
	
	free(linea);
}

void ver_tablero(abb_t* abb, char* desde, char* hasta, int cantidad_a_imprimir, char* orden){

	int contador=0;
	char** datos_vuelo;
	
	abb_iter_t* iter=abb_iter_in_crear(abb,desde);
	
	while(contador<cantidad_a_imprimir && abb_iter_in_al_final(iter,hasta)==false){
		
		datos_vuelo=(char**)abb_obtener(abb, abb_iter_in_ver_actual(iter));
		
		printf("%s - ",datos_vuelo[1]);
		printf("%s\n",datos_vuelo[0]);
			
		abb_iter_in_avanzar(iter,hasta);
		contador++;
	}
	abb_iter_in_destruir(iter);
}

bool prioridad_vuelos(hash_t *hash, int k){
 	
 	int cantidad_a_imprimir=0;
 	heap_t *heap = heap_crear(cmp_prioridad_vuelo);
 	if (heap == NULL) return false;
 	hash_iter_t *iter = hash_iter_crear(hash);
 	if(iter == NULL){
 		heap_destruir(heap, NULL);
 		return false;
 	}
 	pila_t* pila=pila_crear();
 	if(pila==NULL){
 		heap_destruir(heap,NULL);
 		hash_iter_destruir(iter);
 		return false;
 	}

 	for(int i = 0; i < k; i++){
 		if(hash_iter_al_final(iter)) break;
 		cantidad_a_imprimir++;
 		char **arreglo_hash = (char**)hash_obtener(hash, hash_iter_ver_actual(iter));
 		char **arreglo_heap =malloc(sizeof(char*)*3);
 		arreglo_heap[0] = strdup(arreglo_hash[5]);
 		arreglo_heap[1] = strdup(arreglo_hash[0]);
 		arreglo_heap[2] =NULL;
 		heap_encolar(heap, arreglo_heap);
 		hash_iter_avanzar(iter);
 	}
 	while(!hash_iter_al_final(iter)){
 		char **arreglo_hash = (char**)hash_obtener(hash, hash_iter_ver_actual(iter));
 		char **arreglo_heap =malloc(sizeof(char*)*3);
 		arreglo_heap[0] = strdup(arreglo_hash[5]);
 		arreglo_heap[1] = strdup(arreglo_hash[0]);
 		arreglo_heap[2] =NULL;
 		if(cmp_prioridad_vuelo(arreglo_heap, heap_ver_max(heap)) < 0){
 			destruir_arreglo(heap_desencolar(heap));
 			heap_encolar(heap, arreglo_heap);
 		}else{
            destruir_arreglo(arreglo_heap);
        }
 		hash_iter_avanzar(iter);
 	}
 	for(int i = 0; i < cantidad_a_imprimir; i++){
 		pila_apilar(pila, heap_desencolar(heap));
 	}
 	while(pila_esta_vacia(pila)==false){
 		char** arreglo_pila=(char**)pila_desapilar(pila);
 		printf("%s - %s\n", arreglo_pila[0], arreglo_pila[1]);
 		destruir_arreglo(arreglo_pila);
 	}
 	pila_destruir(pila);
 	heap_destruir(heap,destruir_arreglo);
 	hash_iter_destruir(iter);
 	return true;
}

void informacion_vuelo(hash_t* hash, char* numero_de_vuelo){

	char** datos_vuelo=hash_obtener(hash, numero_de_vuelo);
    
	for(int i=0; i<9;i++){
		printf("%s ",datos_vuelo[i]);
	}
	printf("%s\n",datos_vuelo[9]);
}

bool borrado_correcto(abb_t* abb_asc,abb_t* abb_desc, hash_t *hash, char *desde, char *hasta){

	abb_iter_t* iter=abb_iter_in_crear(abb_asc, desde);
	if(iter==NULL){
		return false;
	}
	cola_t *cola = cola_crear();
	if(cola == NULL){
		free(iter);
		return false;
	} 

	while(abb_iter_in_al_final(iter,hasta)==false){

		char *clave_copia = strdup(abb_iter_in_ver_actual(iter));
		cola_encolar(cola, clave_copia);
		abb_iter_in_avanzar(iter, hasta);
	}

	while(!cola_esta_vacia(cola)){

		char *clave_copia = cola_desencolar(cola);
		destruir_arreglo(abb_borrar(abb_asc, clave_copia));
		destruir_arreglo(abb_borrar(abb_desc, clave_copia));
		
		char** arreglo_auxiliar_hash=split(clave_copia,',');
		char* clave_hash=arreglo_auxiliar_hash[0];
		char** datos_vuelo=hash_obtener(hash, clave_hash);
		
		for(int i=0; i<9;i++){
			printf("%s ",datos_vuelo[i]);
		}printf("%s\n",datos_vuelo[9]);

		destruir_arreglo_hash(hash_borrar(hash,clave_hash));
		free_strv(arreglo_auxiliar_hash);
		free(clave_copia);
	}
	abb_iter_in_destruir(iter);
	cola_destruir(cola,NULL);
	return true;	
}

void imprimir_interfaz(){

	printf("Lista de comandos:\n");
	printf("agregar_archivo <nombre_archivo>\n");
	printf("ver_tablero <K cantidad vuelos> <modo: asc/desc> <desde> <hasta>\n");
	printf("info_vuelo <código vuelo>\n");
	printf("prioridad_vuelos <K cantidad vuelos>\n");
	printf("borrar <desde> <hasta>\n");
}

int cantidad_parametros(char **arreglo_comando){

	int contador = 0;
	while(arreglo_comando[contador] != NULL){
		contador++;		
	}
	return contador;
}

bool parametros_en_orden_tablero(char **arreglo_comando){

	if(atoi(arreglo_comando[1]) > 0){;
		if( (strcmp(arreglo_comando[2], ORDEN_ASC) == 0) || (strcmp(arreglo_comando[2], ORDEN_DESC) == 0) ){	
			if(cmp_horario_vuelo(arreglo_comando[3], arreglo_comando[4]) <= 0){
				return true;
			}
		}
	}
	return false;
}


int main(int argc, char const *argv[]){

	char comando[TAMANIO_MAXIMO];
	char **arreglo_comando = NULL;
	bool finalizar_programa = false;
	hash_t *hash = hash_crear(destruir_arreglo_hash);
	if(hash == NULL) return -1;
	abb_t *abb_asc = abb_crear(comparacion_abb_asc, destruir_arreglo); 	
	if(abb_asc == NULL){
		hash_destruir(hash);
		return -1;
	}
	abb_t *abb_desc = abb_crear(comparacion_abb_desc, destruir_arreglo);
	if(abb_desc == NULL){
		hash_destruir(hash);
		abb_destruir(abb_asc);
		return -1;
	}
	//imprimir_interfaz();
	while(!finalizar_programa){
		bool ok = true;
		//printf("ingresa una operacion: \n");
		if(fgets(comando,TAMANIO_MAXIMO,stdin)==NULL){
			break;
		}
		arreglo_comando = split(comando,' ');

		if(strcmp(arreglo_comando[0], AGREGAR_ARCHIVO) == 0){
			if(cantidad_parametros(arreglo_comando) != 2){
				ok = false;
			} else {
				FILE* archivo = fopen(arreglo_comando[1], FORMATO_READ	);
				if(archivo == NULL){
					ok = false;
				} else{
					agregar_archivo(archivo, hash, abb_asc,abb_desc);
					fclose(archivo);
				}
			}
		} else if(strcmp(arreglo_comando[0], VER_TABLERO) == 0){
			if(cantidad_parametros(arreglo_comando) != 5){
				ok = false;
			} else if(!parametros_en_orden_tablero(arreglo_comando)){
				ok = false;
			} else{
				if(strcmp(arreglo_comando[2],ORDEN_ASC)==0){
					ver_tablero(abb_asc,arreglo_comando[3],arreglo_comando[4],atoi(arreglo_comando[1]),arreglo_comando[2]);
				}else{
					ver_tablero(abb_desc,arreglo_comando[4],arreglo_comando[3],atoi(arreglo_comando[1]),arreglo_comando[2]);
				}
			}
		} else if(strcmp(arreglo_comando[0], INFO_VUELO) == 0){
			if(cantidad_parametros(arreglo_comando) != 2){
				ok = false;
			} else if(!hash_pertenece(hash, arreglo_comando[1])){
				ok = false;
			} else{
				informacion_vuelo(hash, arreglo_comando[1]);
			}
		} else if(strcmp(arreglo_comando[0], PRIORIDAD_VUELOS) == 0){
			if(cantidad_parametros(arreglo_comando) != 2){
					ok = false;
		
			} else if(atoi(arreglo_comando[1]) <= 0){
				ok = false;
			} else if(!prioridad_vuelos(hash, atoi(arreglo_comando[1]))){
				ok = false;
			}
		} else if(strcmp(arreglo_comando[0], BORRAR) == 0){
			if(cantidad_parametros(arreglo_comando) != 3){
				ok = false;
			} else if(cmp_horario_vuelo(arreglo_comando[1], arreglo_comando[2]) > 0){
				ok = false;
			} else if(!borrado_correcto(abb_asc,abb_desc, hash, arreglo_comando[1], arreglo_comando[2])){
				ok = false;
			} else{
				if(hash_esta_vacio(hash)) finalizar_programa = true;
			}
		} else if(strcmp(arreglo_comando[0],"salir")==0){
			return 0;
		} else{
			ok = false;
		}
		if(!ok){ 
			fprintf(stderr, "Error en comando %s\n", arreglo_comando[0]);
		} else{
			printf("OK\n");
		}
		free_strv(arreglo_comando);
	}
	hash_destruir(hash);
    abb_destruir(abb_asc);
    abb_destruir(abb_desc);
	return 0;
}
