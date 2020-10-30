import copy
import sys
from clase_pila import Pila
from clase_tortuga import Tortuga

ORIENTACION_INI = 90 #Direccion en que se desarrollara el gráfico

MSJ_PARAM = 'El parámetro ingresado es inválido'
MSJ_IO = 'Ocurrió un problema durante la ejecución del archivo'
MSJ_SIST_L = 'El archivo no contiene un sistema L'
MSJ_GUIA_USUARIO= """El formato a seguir es "python sistemas_L.py <Nombre del archivo .sl> <Cantidad de iteraciones> <Nombre del nuevo archivo .svg>" """

def main():
    '''evalúa los parámetros ingresados por la línea de comandos,
    generando un archivo de tipo SVG según estas instrucciones,
    muestra un mensaje en caso de ser inválidos.'''
    if len(sys.argv) != 4:
        print(MSJ_PARAM)
        print(MSJ_GUIA_USUARIO)
        return
    ruta_sist_L = sys.argv[1]
    try:
        iteraciones = int(sys.argv[2])
    except ValueError:
        print(MSJ_PARAM)
        print(MSJ_GUIA_USUARIO)
        return
    ruta_svg = sys.argv[3]
    try:
        angulo, axioma, reglas = procesar_sist_l(ruta_sist_L)
    except TypeError:
        return
    instrucciones = generar_instr(axioma, reglas, iteraciones)
    escribir_svg(ruta_svg, instrucciones, angulo)

def procesar_sist_l(ruta):
    '''recibe una ruta y devuelve el angulo, el axioma y las reglas
    de transformacion dentro de el, muestra un mensaje si ocurre un
    problema en el procesamineto del archivo'''
    reglas = {}
    try:
        with open(ruta, 'r') as archivo:
            angulo = float(next(archivo))
            axioma = next(archivo)
            for linea in archivo:
                predecesor, sucesor = linea.split()
                reglas[predecesor] = sucesor
        return angulo, axioma, reglas
    except IOError:
        print(MSJ_IO)      
    except TypeError:
        print(MSJ_SIST_L)
    return
    
def generar_instr(axioma, reglas, iteraciones):
    '''recibe un axioma, las reglas de transformacion y la cantidad
    de iteraciones y devuelve las intrucciones para la tortuga'''
    if iteraciones < 2:
        return axioma
    res = ''
    for e in axioma:
        res += reglas.get(e, e)
    return generar_instr(res, reglas, iteraciones -1)

def escribir_svg(ruta, instrucciones, angulo):
    '''recibe una ruta, unas instrucciones y un angulo y escribe en la
    ruta dada una representacion de las instrucciones en formato svg'''
    camino, min_x, min_y, ancho, alto, ttga = generar_dibujo(instrucciones, angulo)
    try:
        with open(ruta, 'w') as arch:
            arch.write(f'<svg viewBox="{min_x} {min_y} {ancho} {alto}" xmlns="http://www.w3.org/2000/svg">\n')
            arch.write(f'\t<path d="{camino}" stroke-width="{ttga.ancho()}" stroke="{ttga.color()}" fill="none"/>\n')
            arch.write('</svg>\n')
    except IOError:
        print(MSJ_IO)
    print(f'La imagen generada se guardó en {ruta} exitosamente')

def generar_dibujo(instrucciones, angulo):
    '''recibe unas instrucciones y un angulo y devuelve el dibujo que
    forman, las dimensiones del cuadro que ocupa el dibujo, y la tortuga
    que se utilizó para generar el dibujo'''
    tortugas = Pila()
    tortuga = Tortuga()
    tortuga.izquierda(ORIENTACION_INI)
    tortugas.apilar(tortuga)
    dibujo = 'M 0 0'
    x_min, y_min, x_max, y_max = 0, 0, 0, 0
    for e in instrucciones:
        mover = ejecutar_instruccion(e, angulo, tortugas)
        if mover:
            tortuga = tortugas.ver_tope()
            accion = 'M' if not tortuga.pluma_esta_abajo() or e == ']' else 'L'
            x, y = tortuga.ver_posicion()
            x_min = x if x < x_min else x_min
            x_max = x if x > x_max else x_max
            y_min = -y if -y < y_min else y_min #(-y) porque en SVG se invierte
            y_max = -y if -y > y_max else y_max
            dibujo += ' ' + f'{accion} {x} {-y}'
    borde_x = (5*x_min - x_max)/4
    borde_y = (5*y_min - y_max)/4
    ancho = 3*(x_max - x_min)/2
    alto = 3*(y_max - y_min)/2
    return dibujo, borde_x, borde_y, ancho, alto, tortuga

def ejecutar_instruccion(elem, angulo, pila):
    '''recibe una instruccion, un angulo y una pila de tortugas y
    aplica la instruccion sobre la pila de tortugas, devuelve si
    dicha instruccion cambia la posicion de la tortuga en el tope'''
    tortuga = pila.ver_tope()
    if elem == 'F' or elem == 'G':
        tortuga.pluma_abajo()
        tortuga.adelantar()
        return True
    if elem == 'f' or elem == 'g':
        tortuga.pluma_arriba()
        tortuga.adelantar()
        return True
    if elem == '+':
        tortuga.derecha(angulo)
        return False
    if elem == '-':
        tortuga.izquierda(angulo)
        return False
    if elem == '[':
        pila.apilar(copy.deepcopy(tortuga))
        return False
    if elem == ']':
        pila.desapilar()
        return True
    if elem == '|':
        tortuga.izquierda(180)
        return False

main()