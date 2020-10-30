import random
from string import ascii_uppercase

def crear_tablero_invisible(max_fila, max_columna, cant_de_bombas):
    """recibe las deimensiones del tablero y la cantidad de bombas, y crea un tablero colocando las
    bombas en posiciones aleatorias"""

    tablero=[]
    cont_de_bombas_colocadas=0

    for i in range(max_fila+1):
        filas=[] 
        for j in range(max_columna+1): 
            filas.append("·")
        tablero.append(filas)

    while cont_de_bombas_colocadas!=cant_de_bombas:
        a=random.randint(1,max_fila)
        b=random.randint(1,max_columna)
        if tablero[a][b]!="#":
            tablero[a][b]="#"
            cont_de_bombas_colocadas+=1

    for x in range(max_columna):
        tablero[0][x+1]=x+1

    tablero[0][0]=""   
    
    for u in range(max_fila):
        tablero[u+1][0]=ascii_uppercase[u]      
    
    return tablero


    