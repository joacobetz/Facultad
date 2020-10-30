import math

class _Pluma:
    '''representa una pluma con atributos de tinta, ancho de trazo
    y si esta levantada o no'''
    def __init__(self):
        '''crea una pluma con ancho igual 1, color negro y en posicion
        de escritura'''
        self.ancho = 1
        self.color = 'black'
        self.escribe = True

class Tortuga:
    '''representa una tortuga con atributos de posicion, orientacion y pluma'''
    def __init__(self):
        '''crea una tortuga con posicion en (0,0), orientacion de 0
        radianes y pluma con los atrubutos que vengan por defecto'''
        self.posicion = (0,0)
        self.orientacion = 0
        self.pluma = _Pluma()
    
    def derecha(self, angulo):
        '''recibe un angulo y cambia la orientacion de la tortuga
        el ángulo dado hacia la derecha'''
        self.orientacion -= math.radians(angulo)

    def izquierda(self, angulo):
        '''recibe un angulo y cambia la orientacion de la tortuga
        el ángulo dado hacia la izquierda'''
        self.orientacion += math.radians(angulo)

    def pluma_arriba(self):
        '''cambia el atributo pluma de la tortuga para que no escriba'''
        self.pluma.escribe = False
    
    def pluma_abajo(self):
        '''cambia el atributo pluma de la tortuga para que escriba'''
        self.pluma.escribe = True
    
    def adelantar(self,n=1):
        '''recibe la cantidad (1 por defecto) de espacio a recorrer y avanza
        dicha cantidad en la direccion en que esté orientada la Tortuga'''
        x, y = self.posicion
        x += n*math.cos(self.orientacion)
        y += n*math.sin(self.orientacion)
        self.posicion = (x,y)
    
    def angulo(self):
        '''devuelve la orientacion en grados de la tortuga'''
        return math.degrees(self.orientacion)

    def ver_posicion(self):
        '''devuelve la posicion de la tortuga'''
        return self.posicion

    def pluma_esta_abajo(self):
        '''devuelve si la pluma escribe o no'''
        return self.pluma.escribe

    def color(self):
        '''devuelve el color de la pluma'''
        return self.pluma.color

    def ancho(self):
        '''devuelve el ancho de la pluma'''
        return self.pluma.ancho