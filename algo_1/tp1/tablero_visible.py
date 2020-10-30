from string import ascii_uppercase

def crear_tablero_visible(max_filas, max_columnas):
    """recibe como parametro la cantidad de filas y columnas. luego crea y devuelve un tablero de
    esas proporciones"""
    tablero=[]
    for i in range(max_filas+1): 
        filas=[]
        if max_columnas < 9:
            for j in range(max_columnas+1):
                filas.append(" *") 
        else: 
            for j in range(max_columnas+1):
                filas.append(" *")
        tablero.append(filas)
    for x in range(max_columnas):
        if x<9:
            tablero[0][x+1]=str(x+1)+" "
        else:
            tablero[0][x+1]=x+1
    tablero[0][0]="  "   
        
    for l in range(max_filas):
        tablero[l+1][0]=ascii_uppercase[l] 

    return tablero
    

