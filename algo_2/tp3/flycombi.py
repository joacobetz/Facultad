#!/usr/bin/python3

import heapq
import random
import sys
import math

def eliminar_iguales(lista):
    nueva=[]
    elementos=set()
    for e in lista:
        if not e in elementos:
            elementos.add(e)
            elementos.add((e[1],e[0],e[2]))
            nueva.append(e)
    return nueva

class cola:

    def __init__(self):
        self.items = []

    def encolar(self, x):
        self.items.append(x)

    def desencolar(self):
        if self.esta_vacia():
            raise ValueError("La cola está vacía")
        return self.items.pop(0)

    def esta_vacia(self):
        return len(self.items) == 0

class pila:

    def __init__(self):
        self.items = []
    
    def esta_vacia(self):
        return len(self.items) == 0

    def apilar(self, x):
        self.items.append(x)
    
    def desapilar(self):
        if self.esta_vacia():
            raise IndexError("La pila está vacía")
        return self.items.pop()

class grafo:
    
    def __init__(self,vertices=None):

        self.adyacencia = {};
        if vertices!=None:
            for v in vertices:
                self.adyacencia[v]={}

    def agregar_arista(self,vertice_1,vertice_2,peso=1):

        if(not vertice_1 in self.adyacencia or not vertice_2 in self.adyacencia):
            raise ValueError("Los vertices seleccionados son invalidos");
    
        adyacencia_1=self.adyacencia.get(vertice_1,{});
        adyacencia_2=self.adyacencia.get(vertice_2,{});

        adyacencia_1[vertice_2]=peso
        adyacencia_2[vertice_1]=peso

        self.adyacencia[vertice_1]=adyacencia_1
        self.adyacencia[vertice_2]=adyacencia_2

    def borrar_arista(self,vertice_1,vertice_2):

        if(vertice_1 not in self.adyacencia or vertice_2 not in self.adyacencia):
            raise ValueError("Los vertices seleccionados son invalidos");
        if(vertice_2 in self.adyacencia[vertice_1]):
            self.adyacencia[vertice_1].pop(vertice_2)
            self.adyacencia[vertice_2].pop(vertice_1)

    def agregar_vertice(self,vertice):

        self.adyacencia[vertice]={}

    def borrar_vertice(self,vertice):

        if(vertice in self.adyacencia):
            adyacencia=self.adyacencia.get(vertice,{})
            for clave in adyacencia:
                self.adyacencia[clave].pop(vertice)
            self.adyacencia.pop(vertice)

    def estan_unidos(self, vertice_1,vertice_2):

        if(vertice_1 in self.adyacencia[vertice_2]):
            return True
        return False

    def vertice_random(self):

        if(len(self.adyacencia.keys())==0):
            raise ValueError("No hay ningun vertice en el grafo")
        vertices=list(self.adyacencia.keys())
        vertice_random=random.choice(vertices)
        return vertice_random

    def obtener_vertices(self):

        return self.adyacencia.keys()

    def obtener_adyacentes(self, vertice):

        return self.adyacencia[vertice].keys()

    def pertenece(self,vertice):

        if(vertice in self.adyacencia):
            return True
        return False

    def peso_arista(self,vertice_1,vertice_2):

        if(not vertice_1 in self.adyacencia[vertice_2]):
            raise ValueError("vertices no unidos")
        return self.adyacencia[vertice_1].get(vertice_2,0)

    def obtener_aristas(self):
        lista=[]
        tuplas=self.adyacencia.items()
        for e in tuplas:
            tuplas_2=e[1].items()
            for t in tuplas_2:
                lista.append((e[0],t[0],t[1]))
        aristas=eliminar_iguales(lista)
        return aristas



class grafo_dirigido:
    
    def __init__(self,vertices=None):

        self.adyacencia = {};
        if vertices!=None:
            for v in vertices:
                self.adyacencia[v]={}

    def agregar_arista(self,vertice_1,vertice_2,peso=1):

        if(not vertice_1 in self.adyacencia or not vertice_2 in self.adyacencia):
            raise ValueError("Los vertices seleccionados son invalidos");
    
        adyacencia_1=self.adyacencia.get(vertice_1,{});
        adyacencia_1[vertice_2]=peso

        self.adyacencia[vertice_1]=adyacencia_1

    def borrar_arista(self,vertice_1,vertice_2):

        if(vertice_1 not in self.adyacencia or vertice_2 not in self.adyacencia):
            raise ValueError("Los vertices seleccionados son invalidos");
        if(vertice_2 in self.adyacencia[vertice_1]):
            self.adyacencia[vertice_1].pop(vertice_2)

    def agregar_vertice(self,vertice):

        self.adyacencia[vertice]={}

    def borrar_vertice(self,vertice):

        if(vertice in self.adyacencia):
            adyacencia=self.adyacencia.get(vertice,{})
            for clave in adyacencia:
                self.adyacencia[clave].pop(vertice)
            self.adyacencia.pop(vertice)

    def estan_unidos(self, vertice_1,vertice_2):

        if(vertice_2 in self.adyacencia[vertice_1]):
            return True
        return False

    def vertice_random(self):

        if(len(self.adyacencia.keys())==0):
            raise ValueError("No hay ningun vertice en el grafo")
        vertices=list(self.adyacencia.keys())
        vertice_random=random.choice(vertices)
        return vertice_random

    def obtener_vertices(self):

        return self.adyacencia.keys()

    def obtener_adyacentes(self, vertice):

        return self.adyacencia[vertice].keys()

    def pertenece(self,vertice):

        if(vertice in self.adyacencia):
            return True
        return False

    def peso_arista(self,vertice_1,vertice_2):

        if(not vertice_2 in self.adyacencia[vertice_1]):
            raise ValueError("vertices no unidos")
        return self.adyacencia[vertice_1].get(vertice_2,0)

    def obtener_aristas(self):
        lista=[]
        tuplas=self.adyacencia.items()
        for e in tuplas:
            tuplas_2=e[1].items()
            for t in tuplas_2:
                lista.append((e[0],t[0],t[1]))
        aristas=eliminar_iguales(lista)
        return aristas

class conjuntos:

    def __init__(self,elementos=None):
        self.conj=[]
        if elementos!=None:
            for e in elementos:
                c=set()
                c.add(e)
                self.conj.append(c)

    def union(self,v,w):
        for e in self.conj:
            if v in e and w in e:
                return
            if v in e:
                c_v=e
            if w in e:
                c_w=e
        c_nuevo=c_v.union(c_w)
        self.conj.remove(c_w)
        self.conj.remove(c_v)
        self.conj.append(c_nuevo)

    def find(self,v):
        for e in self.conj:
            if v in e:
                return e
        return None

def camino_minimo(grafo,origen):
    
    dist={}
    padre={}
    for v in grafo.obtener_vertices(): dist[v]=math.inf
    dist[origen]=0
    padre[origen]=None  
    h=[]
    heapq.heappush(h,(origen,0))
    while not len(h)==0:
        v=heapq.heappop(h)
        for w in grafo.obtener_adyacentes(v[0]):
            if dist[v[0]]+int(grafo.peso_arista(v[0],w))<dist[w]:
                dist[w]=dist[v[0]]+int(grafo.peso_arista(v[0],w))
                padre[w]=v[0]
                heapq.heappush(h,(w,dist[w]))
    return padre,dist

def camino_minimo_frecuencias(grafo,origen):
    
    dist={}
    padre={}
    for v in grafo.obtener_vertices(): dist[v]=math.inf
    dist[origen]=0
    padre[origen]=None  
    h=[]
    heapq.heappush(h,(origen,0))
    while not len(h)==0:
        v=heapq.heappop(h)
        for w in grafo.obtener_adyacentes(v[0]):
            if dist[v[0]]+1/int(grafo.peso_arista(v[0],w))<dist[w]:
                dist[w]=dist[v[0]]+1/int(grafo.peso_arista(v[0],w))
                padre[w]=v[0]
                heapq.heappush(h,(w,dist[w]))
    return padre,dist


def prim(grafo_):

    vertice=grafo_.vertice_random()
    visitados=set()
    visitados.add(vertice)
    heap=[]
    arbol=grafo(grafo_.obtener_vertices())
    for w in grafo_.obtener_adyacentes(vertice):
        heapq.heappush(heap,(grafo_.peso_arista(vertice,w),(vertice,w)))
    while not len(heap)==0:
        (peso,tupla)=heapq.heappop(heap)
        if tupla[1] in visitados:
            continue
        arbol.agregar_arista(tupla[0],tupla[1],peso)
        visitados.add(tupla[1])
        for u in grafo_.obtener_adyacentes(tupla[1]):
            if u not in visitados: heapq.heappush(heap,(grafo_.peso_arista(tupla[1],u),(tupla[1],u)))
    return arbol


def ordenar_por_tercer_elemento(tupla):
    return tupla[2]

def kruskal(grafo_):

    conjuntos_=conjuntos(grafo_.obtener_vertices())
    aristas=grafo_.obtener_aristas()
    aristas.sort(key=ordenar_por_tercer_elemento)

    arbol=grafo(grafo_.obtener_vertices())
    for a in aristas:
        v,w,peso=a
        if conjuntos_.find(v)==conjuntos_.find(w):
            continue
        arbol.agregar_arista(v,w,peso)
        conjuntos_.union(v,w)
    return arbol

def bfs(grafo,origen):

    visitados=set()
    padres={}
    orden={}
    cola_=cola()
    visitados.add(origen)
    padres[origen]=None
    orden[origen]=0
    cola_.encolar(origen)
    while not cola_.esta_vacia():
        v=cola_.desencolar()
        for w in grafo.obtener_adyacentes(v):
            if not w in visitados:
                visitados.add(w)
                cola_.encolar(w)
                padres[w]=v
                orden[w]=orden[v]+1
    return padres, orden

def camino_mas(grafo,ciudad_aeropuerto,origen,destino):
    
    aeropuertos_origen=ciudad_aeropuerto[origen]    
    aeropuertos_destino=ciudad_aeropuerto[destino]
    dist_min=math.inf
    for aeropuerto_o in aeropuertos_origen:
        padre_act,dist_act=camino_minimo(grafo, aeropuerto_o)
        for aeropuerto_d in aeropuertos_destino:
            if dist_act[aeropuerto_d]<dist_min:
                dist_min=dist_act[aeropuerto_d]
                padre_min=padre_act
                destino_min=aeropuerto_d
    pila_ = pila()
    vertice = destino_min
    pila_.apilar(vertice)
    while padre_min[vertice] != None:
        vertice = padre_min[vertice]
        pila_.apilar(vertice)
    lista = []
    while not pila_.esta_vacia():
        lista.append(pila_.desapilar())
    cadena = (' -> '.join(lista))
    print(cadena)

def camino_escalas(grafo,ciudad_aeropuertos ,origen,destino):
    
    aeropuertos_origen=ciudad_aeropuertos[origen]    
    aeropuertos_destino=ciudad_aeropuertos[destino]
    orden_min=math.inf
    for aeropuerto_o in aeropuertos_origen:
        padre_act,orden_act=bfs(grafo, aeropuerto_o)
        for aeropuerto_d in aeropuertos_destino:
            if orden_act[aeropuerto_d]<orden_min:
                orden_min=orden_act[aeropuerto_d]
                padre_min=padre_act
                destino_min=aeropuerto_d
    pila_ = pila()
    vertice = destino_min
    pila_.apilar(vertice)
    while padre_min[vertice] != None:
        vertice = padre_min[vertice]
        pila_.apilar(vertice)
    lista = []
    while not pila_.esta_vacia():
        lista.append(pila_.desapilar())
    cadena = (' -> '.join(lista))
    print(cadena)

def dfs_ciclo_largo_n(grafo,vertice,origen,n,visitados,camino_actual):

    n=int(n)
    visitados.add(vertice)
    if len(camino_actual) == n:
        if origen in grafo.obtener_adyacentes(vertice):
            return camino_actual
        return []
    for w in grafo.obtener_adyacentes(vertice):
        if w in visitados: continue
        solucion=dfs_ciclo_largo_n(grafo,w,origen,n,visitados,camino_actual+[w])
        if len(solucion) == n:
            return solucion
        if len(solucion) > n: 
            visitados.remove(vertice)
            camino_actual.pop()
    visitados.remove(vertice)
    return []

def vacaciones(grafo,ciudad_aeropuerto,origen,n):

    aeropuertos_origen=ciudad_aeropuerto[origen]
    visitados=set()
    for aeropuerto_o in aeropuertos_origen:
        lista=dfs_ciclo_largo_n(grafo,aeropuerto_o,aeropuerto_o,n,visitados,[aeropuerto_o])
        if len(lista) != 0:
            lista.append(aeropuerto_o)
            break
    if len(lista) == 0:
        print("No se encontro recorrido")
    else:
        cadena = (' -> '.join(lista))
        print(cadena)

def obtener_lista_vuelos(arbol):
    vertice=arbol.vertice_random()
    cola_=cola()
    visitados=set()
    cola_.encolar(vertice)
    lista=[]
    while cola_.esta_vacia()==False:
        v=cola_.desencolar()
        visitados.add(v)
        for w in arbol.obtener_adyacentes(v):
            if not w in visitados:
                lista.append((v,w))
                cola_.encolar(w)
    return lista

def nueva_aerolinea(grafo,grafo_linea,ruta):

    arbol_tendido_minimo=kruskal(grafo)
    vuelos=obtener_lista_vuelos(arbol_tendido_minimo)
    
    with open(ruta,'w') as ruta_minima: 
        for i in range (len(vuelos)):
            linea=str(grafo_linea.peso_arista(vuelos[i][0],vuelos[i][1]))
            ruta_minima.write(linea)
    print("OK")

def ordenar_por_segundo_elemento(tupla):
    return tupla[1]  

def ordenar_vertices(diccionario):
    
    lista_tuplas=list(diccionario.items())
    lista_tuplas.sort(key = ordenar_por_segundo_elemento, reverse = True)
    lista_vertices_ordenados=[]
    for t in lista_tuplas:
        if(t[1]!=math.inf):
            lista_vertices_ordenados.append(t[0])
    return lista_vertices_ordenados

def centralidad(grafo,cantidad_a_imprimir):
    
    cent = {}
    contador=0
    for v in grafo.obtener_vertices(): cent[v] = 0
    for v in grafo.obtener_vertices():
        padre, distancias = camino_minimo_frecuencias(grafo, v)
        cent_aux = {}
        for w in grafo.obtener_vertices(): cent_aux[w] = 0
        vertices_ordenados = ordenar_vertices(distancias) 
        for w in vertices_ordenados:
            if padre[w] is not None:
                cent_aux[padre[w]] += 1 + cent_aux[w]
        for w in grafo.obtener_vertices():
            if w == v: continue
            cent[w] += cent_aux[w]
    lista_centralidad=ordenar_vertices(cent)
    while contador<len(lista_centralidad)-1 and contador<int(cantidad_a_imprimir)-1:
        print(lista_centralidad[contador],end=", ")
        contador+=1
    print(lista_centralidad[contador])


def crear_orden_topologico(archivo):
    
    with open(archivo,'r') as orden:
        arbol=grafo_dirigido(orden.readline().rstrip().split(','))
        for linea in orden:
            prioridad=linea.split(',')
            arbol.agregar_arista(prioridad[0],prioridad[1].rstrip())
    return arbol

def orden_topologico(grafo):

    grados={}
    for vertice in grafo.obtener_vertices(): grados[vertice]=0
    for vertice in grafo.obtener_vertices():
        for vertice_ady in grafo.obtener_adyacentes(vertice):
            grados[vertice_ady]+=1
    cola_=cola()
    for vertice in grafo.obtener_vertices():
        if grados[vertice]==0: cola_.encolar(vertice)
    lista = []
    while not cola_.esta_vacia():
        vertice=cola_.desencolar()
        lista.append(vertice)
        for vertice_ady in grafo.obtener_adyacentes(vertice):
            grados[vertice_ady]-=1
            if grados[vertice_ady]==0:
                cola_.encolar(vertice_ady)
    if len(grafo.obtener_vertices())==len(lista):
        return lista
    return None

def itinerario_cultural(grafo,ciudad_aeropuerto,archivo):
    
    arbol = crear_orden_topologico(archivo)
    orden = orden_topologico(arbol)
    cadena = (', '.join(orden))
    print(cadena)
    for i in range(len(orden)-1):
        camino_escalas(grafo,ciudad_aeropuerto,orden[i],orden[i+1])
    

def cargar_datos():

    grafo_precios=grafo()
    grafo_cantidad_vuelos=grafo()
    grafo_tiempo=grafo()
    grafo_no_pesado=grafo()
    grafo_linea=grafo()
    ubicacion={}
    ciudad_aereopuerto={}
    if len(sys.argv)!=3:
        raise ValueError("cantidad incorrecta archivos")

    with open(sys.argv[1],'r') as aeropuertos:
        for linea in aeropuertos:
            datos=linea.rstrip().split(',')
            grafo_cantidad_vuelos.agregar_vertice(datos[1])
            grafo_precios.agregar_vertice(datos[1])
            grafo_tiempo.agregar_vertice(datos[1])
            grafo_no_pesado.agregar_vertice(datos[1])
            grafo_linea.agregar_vertice(datos[1])

            ubicacion[datos[1]]=[datos[2],datos[3]]
            aux=ciudad_aereopuerto.get(datos[0],[])
            ciudad_aereopuerto[datos[0]]=list(aux)+[datos[1]]
        
    with open(sys.argv[2],'r') as vuelos: 
        for linea in vuelos:
            datos=linea.rstrip().split(',')
            grafo_cantidad_vuelos.agregar_arista(datos[0],datos[1],datos[4])
            grafo_precios.agregar_arista(datos[0],datos[1],datos[3])
            grafo_tiempo.agregar_arista(datos[0],datos[1],datos[2])
            grafo_no_pesado.agregar_arista(datos[0],datos[1],1)
            grafo_linea.agregar_arista(datos[0],datos[1],linea)

    return grafo_tiempo, grafo_cantidad_vuelos,grafo_precios,grafo_no_pesado,grafo_linea,ciudad_aereopuerto,ubicacion

def flycombi():
    
    sys.setrecursionlimit(10000000)
    ok=True
    grafo_tiempo,grafo_cantidad_vuelos,grafo_precios,grafo_no_pesado,grafo_linea,ciudad_aereopuerto,ubicacion=cargar_datos()
    for linea in sys.stdin:
        datos_linea=linea.split()
        if datos_linea[0]=="listar_operaciones":
            if len(datos_linea) !=1:
                ok=False
            else:
                print("camino_mas")
                print("camino_escalas")
                print("centralidad")
                print("nueva_aerolinea")
                print("vacaciones")
                print("itinerario")
        elif datos_linea[0]=="camino_mas":
            linea_camino_mas=linea.rstrip().split(',')
            modo=linea_camino_mas[0].split()[1]
            if len(linea_camino_mas)!=3:
                ok=False
            else:
                if modo=="barato":
                    camino_mas(grafo_precios,ciudad_aereopuerto,linea_camino_mas[1],linea_camino_mas[2])
                elif modo=="rapido":
                    camino_mas(grafo_tiempo,ciudad_aereopuerto,linea_camino_mas[1],linea_camino_mas[2])
        elif datos_linea[0]=="camino_escalas":
            
            linea_camino_escalas=linea.rstrip().split(',')
            origen=linea_camino_escalas[0].split("camino_escalas ")[1]
            if len(linea_camino_escalas)!=2:
                ok=False
            else:
                camino_escalas(grafo_no_pesado,ciudad_aereopuerto, origen,linea_camino_escalas[1])
        elif datos_linea[0]=="centralidad":
            if len(datos_linea)!=2:
                ok=False
            else:
                centralidad(grafo_cantidad_vuelos,datos_linea[1])
        elif datos_linea[0]=="nueva_aerolinea":
            if len(datos_linea)!=2:
                ok=False
            else:
                nueva_aerolinea(grafo_precios,grafo_linea, datos_linea[1])
        elif datos_linea[0]=="vacaciones":
            linea_vacaciones=linea.rstrip().split(',')
            origen=linea_vacaciones[0].split("vacaciones ")[1]
            if len(linea_vacaciones)!=2:
                ok=False
            else:
                vacaciones(grafo_no_pesado,ciudad_aereopuerto, origen,linea_vacaciones[1])
        elif datos_linea[0]=="itinerario":
            if len(datos_linea)!=2:
                ok=False
            else:
                itinerario_cultural(grafo_no_pesado,ciudad_aereopuerto,datos_linea[1])
        else:
            raise ValueError("Funcionalidad no disponible")
        if(ok==False):
            raise ValueError("cantidad de parametros incorrecta")

flycombi()