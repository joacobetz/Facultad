#define _GNU_SOURCE

#include "strutil.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <stdio.h>
#include "cola.h"
#include <ctype.h>

bool es_un_numero_valido(const char* elemento){
	size_t comienzo=0;
	size_t fin=strlen(elemento);
	for(size_t i=comienzo; i<fin; i++){
		if(isdigit(elemento[i])==false){
			return false;
		}
	}return true;
}

int main(int  argc , const  char * argv []){
	
	FILE* archivo;
	int cantidad_lineas_contexto;
	bool pasaron_archivos=false;
	if(argc<3  || argc >4){
		fprintf(stderr,"%s\n","Cantidad de parametros erronea");
		return -1;
	}
	if(es_un_numero_valido(argv[2])==false){
		fprintf(stderr,"%s\n","Tipo de parametro incorrecto");
		return -1;
	}else{
		cantidad_lineas_contexto=atoi(argv[2]);
	}
	if(argc==3){
		archivo=stdin;
	}
	if(argc==4){
		archivo=fopen(argv[3],"r");
		pasaron_archivos=true;
		if(archivo==NULL){
			fprintf(stderr,"%s\n","No se pudo leer el archivo indicado");
			return -1;
		}
	}
	
	const char* patron=argv[1];
	cola_t* cola=cola_crear();
	if(cola==NULL) return -1;
	int i=0;
	char* linea=NULL;
	size_t capacidad=0;
	size_t leidos;
	char* linea_para_usar;
	bool se_stripeo=true;


	bool stripear(char** nueva, char* linea_para_stripear){
		if(linea[strlen(linea_para_stripear)-1]=='\n'){
			*nueva=substr(linea_para_stripear,(strlen(linea_para_stripear)-1));
			if(nueva== NULL) return false;
			return true;
		}else{
			return false;
		}	
	}

	while((leidos=getline(&linea,&capacidad,archivo))!=-1){
		
		if(stripear(&linea_para_usar, linea)==false){
			linea_para_usar=linea;
			se_stripeo=false;
		}

		char* copia=malloc(sizeof(char)*(strlen(linea_para_usar)+1));
		if(copia==NULL){
			cola_destruir(cola,free);
			return -1;
		} 
		strcpy(copia,linea_para_usar);
		if(se_stripeo) free(linea_para_usar);
		
		if(cola_encolar(cola,copia)==true){
			i++;
		}else return -1;
		
		if(strstr(linea,patron)!=NULL){
			while(cola_esta_vacia(cola)==false){
				char* elemento=(char*) cola_desencolar(cola);
				fprintf(stdout,"%s\n",elemento);
				free(elemento);
			}
			i=0;	
		}else if(i>cantidad_lineas_contexto){
			free(cola_desencolar(cola));
			i--;
		}
	}
	free(linea);
	cola_destruir(cola, free);

	if(pasaron_archivos) fclose(archivo);
	return 0;
}
