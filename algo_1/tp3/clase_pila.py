class _Nodo:
    '''representa un nodo con atributos de dato y el elemento a continuacion'''
    def __init__(self, dato, prox=None):
        '''recibe un dato y un proximo nodo, y crea un nodo con
        ellos. si no recibe proximo este en None por defecto'''
        self.dato = dato
        self.prox = prox

class Pila:
    '''representa una pila'''
    def __init__(self):
        '''crea una pila vacia'''
        self.tope = None
        
    def apilar(self, dato):
        '''recibe un dato y lo apila'''
        nodo = _Nodo(dato)
        if self.tope:
            nodo.prox = self.tope
        self.tope = nodo
    
    def desapilar(self):
        '''desapila el primer elemento de la pila y devuelve su valor'''
        if self.tope is None:
            raise PilaVaciaError
        dato = self.tope.dato
        self.tope = self.tope.prox
        return dato
    
    def esta_vacia(self):
        '''devuelve si la pila esta vacia o no'''
        if self.tope is None:
            return True
        return False
    
    def ver_tope(self):
        '''devuelve el valor del elemento arriba de la pila'''
        if self.tope is None:
            raise PilaVaciaError
        return self.tope.dato
        
class PilaVaciaError(Exception):
    pass