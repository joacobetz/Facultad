import pedir_casilla
import tablero_visible
import tablero_invisible
from string import ascii_uppercase

def main(max_filas, max_columnas, cant_de_bombas):
  """recibe por parametro las dimensiones del tablero y la cantidad de bombas y ejecuta el buscaminas
  compilando funciones externas. ademas evalua los ingresos del usuario, decide si gano o perdio e imprime
  sus respectivos mensajes"""

  empezar=True
  victoria=False
  cont_de_signos=0
  cont_de_bombas_descubiertas=0
  cont_de_casillas=0
  contador_de_turnos=0 
  tablero_v= tablero_visible.crear_tablero_visible(max_filas, max_columnas)
  tablero_i=tablero_invisible.crear_tablero_invisible(max_filas,max_columnas,cant_de_bombas)

  while empezar:
    print("""
      Bienvenido a Algominas! tu objetivo como miembro del escuadron anti-minas de Barbara es 
      desvelar todas las celdas del campo de batalla, pero cuidado si pisas una mina tu mision y tu 
      vida habran acabado...
      """)
    print("""CONTROLES:
        -Para descubrir una celda indica el numero de fila su respectiva letra y su numero de columna con su numero (ejemplo c6)

        -Si sospechas que hay una mina en alguna celda puedes poner un signo de pregunta con el escribiendo [letra][numero][?] (ejemplo c6?)
          ((no te preocupes, puedes quitarlo en cualquier momento efectuando nuevamente la accion sobre la casilla)) 
          """)
    empezar=False
 
  for x in range(0, max_filas+1): 
        cont=0
        for y in tablero_v[x]:
            if cont//(max_columnas)==1:
                print(y, end="\n")
            else:
                print(y, end=" ")
                cont+=1
 
  while not victoria:
    casilla=pedir_casilla.pedir_casilla(max_filas,max_columnas)

    a=ascii_uppercase.index(casilla[0].upper())+1
    b=pedir_casilla.numero_en_ingreso(casilla)
    
    if not "?" in casilla:
      if tablero_i[a][b]=="#":
        if contador_de_turnos>0:
          print("has fracasado miserablemente")
          return
        else:
          while tablero_i[a][b]=="#":
            tablero_i=tablero_invisible.crear_tablero_invisible(max_filas, max_columnas,cant_de_bombas)
      contador_de_turnos+=1
      contador=0
      for f in range(a-1, a+2):
        for c in range(b-1, b+2):
          if not f>max_filas and not c> max_columnas :
            if tablero_i[f][c] == "#":
              contador += 1
      if not tablero_v[a][b]=="?" and not str(tablero_v[a][b]).isdigit():     
        tablero_v[a][b]=" "+str(contador)
        cont_de_casillas+=1
      else:
        continue
    else:
        if tablero_v[a][b]==" ?":
          tablero_v[a][b]=" *"
          cont_de_signos-=1
          cont_de_casillas+=1
        elif not str(tablero_v[a][b]).isdigit():
            tablero_v[a][b]= " ?"
            cont_de_signos+=1
            if tablero_i[a][b]=="#":
              cont_de_bombas_descubiertas+=1
              cont_de_casillas-=1
            if (cont_de_bombas_descubiertas==cant_de_bombas) and cont_de_bombas_descubiertas==cont_de_signos:
              victoria=True
        else:
          continue
    if cont_de_casillas>=(max_columnas*max_filas-cant_de_bombas):
      victoria=True
    for x in range(0, max_filas+1): 
        cont=0
        for y in tablero_v[x]:
            if cont//(max_columnas)==1:
                print(y, end="\n")
            else:
                print(y, end=" ")
                cont+=1
  
  if victoria:
    print("HAS SALVADO AL EJERCITO BARBARIANO")
