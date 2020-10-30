#define _GNU_SOURCE


#include "pila.h"
#include "strutil.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#define ERROR -1

bool es_un_numero_valido(char* elemento){
	size_t comienzo=0;
	size_t fin=strlen(elemento);
	if(fin==0) return false;
	if(elemento[0]=='-'){
		comienzo=1;
	}
	for(size_t i=comienzo; i<fin; i++){
		if(isdigit(elemento[i])==false){
			return false;
		}
	}return true;
}
bool es_cero_f(char* elemento){
	size_t comienzo=0;
	size_t fin=strlen(elemento);
	if(fin==0) return false;
	if(elemento[0]=='-'){
		comienzo=1;
	}
	for(size_t i=comienzo; i<fin; i++){
		if(elemento[i]!='0'){
			return false;
		}
	}return true;
}

int suma(int a, int b){
	return a+b;
}

int raiz(int n){ 
	if(n==1) return 1;
	int inicio=0;
	int fin =n-1;
	int medio=(inicio+fin)/2;
	while(inicio<=fin){
		medio=(inicio+fin)/2;
		if(medio*medio<=n && n<((medio+1)*(medio+1))){
			return medio;
		}
		if(medio*medio>n){
			fin=medio-1;
		}
		else{
			inicio=medio+1;
		}
	}
	return medio;
}

int resta(int a, int b){
	return a-b;
}

int multiplicacion(int a, int b){
	return a*b;
}

int division(int a, int b){
	return a/b;
}

int potencia(int a, int b){
	
    if (b == 0) return 1;
    int resultado_parcial = potencia(a,b/2);
    if(b%2 == 0) return resultado_parcial*resultado_parcial;
    else return a*resultado_parcial*resultado_parcial;
}

int logaritmo(int a, int b){  

	if(a<b) return 0;
	int resultado=1;
	int inicio=0;
	int fin =a; 
	int medio=(inicio+fin)/2; 
	while(inicio<=fin){
		resultado=1;
		medio=(inicio+fin)/2;
		for(int i=0;i<medio;i++){
			resultado*=b;
			if(resultado>a) break;
		}		
		if(resultado<=a && a<resultado*b){
			return medio;
		}
		if(resultado>a){
			fin=medio-1;
		}
		else{
			inicio=medio+1;
		}
	}
	return medio;

}

int operador_ternario(int a, int b, int c){
	if(a==0) return c;
	else return b;
}

int* pedir_dos_elementos(pila_t* pila, bool* ok, bool* es_cero){

	int* arreglo_de_elementos=malloc(sizeof(int)*2);
	if(arreglo_de_elementos==NULL){
		*ok=false;
		return NULL;
	}
	arreglo_de_elementos[0]=0;
	arreglo_de_elementos[1]=0;
	char* a=  pila_desapilar(pila);
	if(a==NULL) *ok=false;
	char* b=  pila_desapilar(pila);
	if(b==NULL && *ok==true){ 
		free(a); 
		*ok=false;
	}
	if(es_cero!=NULL && *ok==true){
		if(es_cero_f(b)) *es_cero=true;
	}if(*ok==true){
		arreglo_de_elementos[0]=atoi(a);
		arreglo_de_elementos[1]=atoi(b);
		free(a);
		free(b);
	}
	return arreglo_de_elementos;
}


bool procesar_operaciones(pila_t* pila, char* elemento, int* resultado_de_operacion, bool* ok){
	
	int resultado=0;
	bool se_opero=false;

	if(strcmp(elemento,"+")==0 || strcmp(elemento, "+\n")==0){
		int* arreglo_de_numeros=pedir_dos_elementos(pila, ok, NULL);
		if(*ok!=false){
			resultado=suma(arreglo_de_numeros[0],arreglo_de_numeros[1]);
		}
		se_opero=true;
		if(arreglo_de_numeros!=NULL) free(arreglo_de_numeros);

	}if(strcmp(elemento,"-")==0 || strcmp(elemento, "-\n")==0){
		int* arreglo_de_numeros=pedir_dos_elementos(pila, ok, NULL);

		if(*ok!=false){

			resultado=resta(arreglo_de_numeros[0],arreglo_de_numeros[1]);
		}
		se_opero=true;
		if(arreglo_de_numeros!=NULL) free(arreglo_de_numeros);

	}if(strcmp(elemento,"log")==0 || strcmp(elemento, "log\n")==0){

		int* arreglo_de_numeros=pedir_dos_elementos(pila, ok, NULL);
		if(arreglo_de_numeros[1]<=1 || *ok==false) *ok=false;
		else{
			resultado=logaritmo(arreglo_de_numeros[0],arreglo_de_numeros[1]);
		}
		se_opero=true;
		if(arreglo_de_numeros!=NULL) free(arreglo_de_numeros);

	}if(strcmp(elemento,"^")==0 || strcmp(elemento, "^\n")==0){
		int* arreglo_de_numeros=pedir_dos_elementos(pila, ok, NULL);
		if(arreglo_de_numeros[1]<0 || *ok==false || (arreglo_de_numeros[0]==0 && arreglo_de_numeros[1]==0)) *ok=false;
		else{
			resultado=potencia(arreglo_de_numeros[0],arreglo_de_numeros[1]);
		}

		se_opero=true;
		if(arreglo_de_numeros!=NULL) free(arreglo_de_numeros);

	}if(strcmp(elemento,"sqrt" )==0 || strcmp(elemento, "sqrt\n")==0){
		char* n=pila_desapilar(pila);
		if(n==NULL) *ok=false;
		else if(atoi(n)<0){
			free(n);
			*ok=false;
		}
		else{
			resultado=raiz(atoi(n));
			free(n);
		}
		se_opero=true;

	}if(strcmp(elemento,"/")==0 || strcmp(elemento, "/\n")==0){
		bool es_cero=false;
		int* arreglo_de_numeros=pedir_dos_elementos(pila, ok,&es_cero);
		if(es_cero || *ok==false) *ok=false;
		else{
			resultado=division(arreglo_de_numeros[0],arreglo_de_numeros[1]);
		} se_opero=true;
		if(arreglo_de_numeros!=NULL) free(arreglo_de_numeros);

	}if(strcmp(elemento,"?")==0 || strcmp(elemento, "?\n")==0){

		char* a= pila_desapilar(pila);
		if(a==NULL) *ok=false;
		char* b= pila_desapilar(pila);
		if(b==NULL && *ok==true){
			free(a); 
			*ok=false;
		}
		char* c= pila_desapilar(pila);
		if(c==NULL && *ok==true){
			free(a); 
			free(b); 
			*ok=false;
		}
		if(*ok==true){
			resultado=operador_ternario( atoi(a),atoi(b),atoi(c));
			free(a);
			free(b);
			free(c);
		}
		se_opero=true;

	}if(strcmp(elemento,"*")==0 || strcmp(elemento, "*\n")==0){
		int* arreglo_de_numeros=pedir_dos_elementos(pila, ok,NULL);
		if(*ok!=false){
			resultado=multiplicacion(arreglo_de_numeros[0],arreglo_de_numeros[1]);
		}
		se_opero=true;
		if(arreglo_de_numeros!=NULL) free(arreglo_de_numeros);
	}
	if(se_opero){
		*resultado_de_operacion= resultado;
	}
	return se_opero;
}

int main(){ 

	bool se_opero=false;
	char* linea=NULL;
	size_t capacidad=0;
	size_t leidos; 


	while ((leidos= getline(&linea,&capacidad,stdin))!=-1){

		int resultado_de_operacion=0;
		bool ok=true;
		size_t i=0;
		size_t contador=0;

		pila_t* pila=pila_crear();
        if(pila==NULL) return ERROR;

		char** linea_spliteada=split(linea,' ');
		if(!linea_spliteada) return ERROR;

		for(int y=0; linea_spliteada[y]!=NULL; y++){
			contador++;
		}
		
		
		while(linea_spliteada[i]!=NULL){

			if(strlen(linea_spliteada[i])==0 || (strstr(linea_spliteada[i],"\n")!=NULL && strlen(linea_spliteada[i])==1)){
				free(linea_spliteada[i]);
				i++;
				continue;
			}
			se_opero=procesar_operaciones(pila, linea_spliteada[i], &resultado_de_operacion, &ok);

			if ((es_un_numero_valido(linea_spliteada[i])==false && !se_opero )|| !ok ){
  				ok = false;
  				break;

			} else if(se_opero){

				char* cadena=NULL;
   				if(asprintf(&cadena, "%d", resultado_de_operacion)<0){
   					ok=false;
   					free(cadena);
   					break;
   				}
				pila_apilar(pila, cadena);
				resultado_de_operacion=0;
			} else{

				char* copia=malloc(sizeof(char)*strlen(linea_spliteada[i])+1);
				strcpy(copia, linea_spliteada[i]);
				pila_apilar(pila, copia);
			}
			free(linea_spliteada[i]);
			i++;	
		}

		if(ok==false){
			printf("ERROR\n");
		}
		else{
			char* elemento=pila_desapilar(pila);
			
			if(pila_esta_vacia(pila)==false){
				printf("ERROR\n");
			}
			else if(elemento==NULL){
				printf("ERROR\n");
			}
			else{
				int resultado=atoi(elemento);
				printf("%d\n", resultado);
			}
			free(elemento);
		}
		for(size_t x=i; x<contador; x++){
  			free(linea_spliteada[x]);
  		}		
		free(linea_spliteada);

		while(pila_esta_vacia(pila)==false){
  			free(pila_desapilar(pila));
  		}
		pila_destruir(pila);
	}
	free(linea);
	return  0;
}
