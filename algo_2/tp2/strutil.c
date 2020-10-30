#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FIN_CADENA '\0'
#define ESPACIADO '\n'

char *substr(const char *str, size_t n){

	char *str_final = malloc(sizeof(char)*(n+1));
	if(str == NULL) return NULL;

	if(n > 0) strncpy(str_final, str, n);
	
	str_final[n] = FIN_CADENA;
	return str_final;
}

char **split(const char *str, char sep){

	unsigned int cantidad_separadores = 0;
	unsigned int cantidad_caracteres = 0;
	unsigned int contador_strv = 0;
	size_t largo_subcadena = 0;
	size_t largo_cadena_anterior = 0;
	char **strv_final;

	if(str == NULL) return NULL;

	while(str[cantidad_caracteres] != FIN_CADENA){
		if(str[cantidad_caracteres] == sep) cantidad_separadores++;
		cantidad_caracteres++;
	}

	strv_final = malloc(sizeof(char*)*(cantidad_separadores+2));
	if(strv_final == NULL) return NULL;

	for(int i = 0; i <= cantidad_caracteres; i++){
		if( (str[i] == sep) || (str[i] == FIN_CADENA) ){
			if(str[i-1] == ESPACIADO) largo_subcadena--;
			strv_final[contador_strv] = substr(str + largo_cadena_anterior, largo_subcadena);
			largo_cadena_anterior += largo_subcadena+1;
			largo_subcadena = 0;
			contador_strv++; 
		} else{
			largo_subcadena++;
		}
	}
	strv_final[cantidad_separadores+1] = NULL;
	return strv_final;
}

char *join(char **strv, char sep){

	char *str_final; 
	size_t largo_strv = 0;
	unsigned int cantidad_caracteres = 0;
	unsigned int cantidad_separadores = 0;
	unsigned int contador_str = 0;

	if(strv == NULL) return NULL;

	while(strv[largo_strv] != NULL){
		cantidad_caracteres += (unsigned int)strlen(strv[largo_strv]);
		largo_strv++;
	}

	if(largo_strv > 0){
		cantidad_separadores = (unsigned int)largo_strv-1;
	}

	if(sep != FIN_CADENA){ 
		cantidad_caracteres += cantidad_separadores;
	}

	str_final = malloc(sizeof(char)*(cantidad_caracteres+1));
	if(str_final == NULL) return NULL;

	largo_strv = 0;
	for(int i = 0; i < cantidad_caracteres; i++){
		if(strv[largo_strv][contador_str] == FIN_CADENA){
			contador_str = 0;
			largo_strv++;
			if(sep != FIN_CADENA){
				str_final[i] = sep;
				continue;
			}
		}
		str_final[i] = strv[largo_strv][contador_str];
		contador_str++;
	}
	str_final[cantidad_caracteres] = FIN_CADENA;
	return str_final;
}

void free_strv(char *strv[]){

	unsigned int contador_strv = 0;

	while(strv[contador_strv] != NULL){
		free(strv[contador_strv]);
		contador_strv++;
	}
	free(strv);
}