from string import ascii_uppercase

def numero_en_ingreso(casilla):
    """recibe una casilla y decide si esta esta en las dimensiones de columnas del tablero"""
    numeros=""
    numero_ingresado=0

    for e in casilla:
        if e.isdigit():
            numeros+=e
    for x in range(len(numeros)):
        numero_ingresado += int(numeros[x])*(10**(len(numeros)-(x+1)))
    return numero_ingresado

def margen_de_numeros(numero_ingresado, max_numero):    
    if numero_ingresado <= max_numero and numero_ingresado>0:
        return True
    return False

def pedir_casilla(max_filas, max_columnas):
    """le pide una celda al usuario, si es correcta la duevuelve y si no lo es 
        le vuelve a pedir una celda valida"""

    seguir_preguntando=True
    rango_letra= str(["A", ascii_uppercase[max_filas-1]])
    rango_numero= str([1, max_columnas])
    
    while seguir_preguntando:
        casilla=input(f"Inserte una fila entre {rango_letra} y una columna entre {rango_numero}(Por ejemplo: c2): ") 
        posicion_signo=0
        if casilla=="":
            continue
        if "ñ" in casilla or "Ñ" in casilla:
            continue
        elif not casilla[0].isalpha():
            continue
        if "?" in casilla:
            posicion_signo=casilla.index("?")
            if posicion_signo+1 !=len(casilla):
                continue
        for e in casilla:
            if e!=casilla[0] and e!=casilla[posicion_signo]:
                if not e.isdigit():
                    continue
        numero_ingresado=numero_en_ingreso(casilla)
        if ascii_uppercase.index(casilla[0].upper())>max_filas or margen_de_numeros(numero_ingresado,max_columnas)==False:
            continue
        else:
            seguir_preguntando=False
    return casilla

