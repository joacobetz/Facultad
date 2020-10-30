#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

char *substr(const char *str, size_t n){
	size_t contador=0;
	size_t i=0;
	while(i<n){
		if(str[i]=='\0'){
			break;
		}
		i++;
	}
	char* cadena_nueva=malloc(sizeof(char)*(i+1));
	if(cadena_nueva==NULL){
		return NULL;
	}
	while(contador<n && str[contador]!='\0'){
		cadena_nueva[contador]=str[contador];
		contador++;
	}
	cadena_nueva[contador]='\0';
	return cadena_nueva;
}

char **split(const char *str, char sep){
	size_t contador=0;
	size_t contador_interno_arreglo=0;
	size_t inicio=0;
	size_t tamanio=0;

	for(size_t i=0; i<strlen(str);i++){
		if(str[i]==sep) contador++;
	}

	char** arreglo_de_cadenas=malloc(sizeof(char*)*(contador+2));
	if(arreglo_de_cadenas==NULL) return NULL;
	
	for(size_t x=0; x<=strlen(str);x++){
		if(str[x]==sep || str[x]=='\0'){
			arreglo_de_cadenas[contador_interno_arreglo]=substr(str+inicio,tamanio);
			contador_interno_arreglo++;
			inicio+=tamanio+1;
			tamanio=0;
		}else{
			tamanio++;
		}
	}
	arreglo_de_cadenas[contador_interno_arreglo]=NULL;
	return arreglo_de_cadenas;
}

char *join(char **strv, char sep){
	
	size_t i=0;
	size_t x=0;
	size_t largo_cadena_joineada=0;
	size_t contador_de_separadores=0;
	size_t contador_de_caracteres=0;
	size_t tamanio_del_malloc;

	while(strv[i]!=NULL){
		contador_de_caracteres+=strlen(strv[i]);
		contador_de_separadores++;
		i++;
	}

	if(sep!='\0' && contador_de_separadores>1) contador_de_caracteres+=contador_de_separadores-=1;
	tamanio_del_malloc=contador_de_caracteres+1;

	char* cadena_joineada=malloc(sizeof(char)*(tamanio_del_malloc));
	if(!cadena_joineada) return NULL;
	
	while(strv[x]!=NULL){

		size_t largo_sub_cadena=0;
		size_t tamanio_de_la_cadena=strlen(strv[x]);

		for(size_t y=largo_cadena_joineada; y<largo_cadena_joineada+tamanio_de_la_cadena; y++){
			if(strv[x][largo_sub_cadena]!='\0'){
				cadena_joineada[y]=strv[x][largo_sub_cadena];
			}
			largo_sub_cadena++;
		}
		if(x!=contador_de_separadores && sep!='\0'){
			cadena_joineada[largo_cadena_joineada+tamanio_de_la_cadena]=sep;
		}
		if(sep!='\0'){
			largo_cadena_joineada+=(tamanio_de_la_cadena+1);	
		}else{
			largo_cadena_joineada+=tamanio_de_la_cadena;
		}
		x++;
	}
	if(largo_cadena_joineada==0){
		cadena_joineada[0]='\0';
		return cadena_joineada;
	}
	if(sep=='\0'){
		cadena_joineada[largo_cadena_joineada]='\0';
	}else{
		cadena_joineada[largo_cadena_joineada-1]='\0';
	}
	return cadena_joineada;
}

void free_strv(char *strv[]){
	int x=0;
	while(strv[x]!=NULL){
		free(strv[x]);
		x++;
	}
	free(strv);
}