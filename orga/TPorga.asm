;Se pide desarrollar un programa en assembler Intel 80x86 que cargue desde teclado una cantidad X
;de matrices (con X <= 5) de NxM elementos (con N y M <= 8) y luego permita realizar las siguientes
;operaciones entre ellas:
;● Resta de 2 o más matrices.
;● Igualdad entre 2 matrices.
;● Multiplicar una matriz por un valor escalar leído desde teclado.
;● Modificar o consultar el valor de un elemento (Mij) de una matriz.
;Nota: Los elementos que componen a cada matriz (Mij) son números enteros entre -99 y 99

global main
extern  puts
extern  printf
extern  sscanf                  
extern  gets

section     .data

    msgPedirN       db     "Inserte el numero de filas(1 a 8): ",0
    msgPedirM       db     "Inserte el numero de columnas(1 a 8): ",0
    msgPedirElemento         db      "Inserte un numero (de -99 a 99): ",0
    msgPreguntarPorOtraMatriz   db "¿Desea crear  otra matriz?(S para continuar, otra tecla para finalizar) ",0
    msgLimiteDeCreadas      db  "Ya se llego al limite de matrices",0
    msgNumeroIncorrectoFilCol     db  "El numero ingresado es incorrecto debe ser de 1 a 8, intente nuevamente",0
    msgNumeroIncorrectoElem     db  "El numero ingresado es incorrecto debe ser de -99 a 99, intente nuevamente",0
    msgNumeroIncorrectoOperacion db  "No ha elegido una operacion valida. intente nuevamente",10,0
    msgNumeroIncorrecto     db  "Ha ingresado un numero incorrecto. intente nuevamente",10,0
    msgMenu                 db  "Elija una operacion:",10,"1 Para restar",10,"2 Para verificar igualdad",10,"3 Para  multiplicar por un escalar",10,"4 Para consultar un valor",10,"5 Para modificar un valor",10,0
    msgPedirMatrizAOperar   db  "Elija una matriz (1 a 5): ",10,0
    msgSonIguales           db  "Las matrices son iguales",10,0
    msgSonDistintas         db  "Las matrices son distintas",10,0
    msgPedirEscalar         db  "Ingrese un escalar: ",0
    msgComenzarACrear       db  "Cargue las matrices que va a usar. Las matrices quedaran numeradas segun el orden en el que fueron creadas.",10,0
    msgPedirReemplazo       db  "Ingrese un numero para reemplazarlo por el de la posicion elegida: ",0
    msgOtraOperacion        db  "¿Desea realizar otra operacion? (N para terminar, otra tecla para seguir): ",0
    msgTamanioIncorrectoMatriz  db  "El tamaño de las matrices no permite realizar la operacion",10,0
    msgCantidadMatricesResta    db  "Ingrese la cantidad de matrices que va a restar: ",0
    msgNumeroIncorrectoRango    db  "El numero ingresado es incorrecto ya que se encuentra fuera del rango de la matriz elegida. Intente nuevamente",10,0
    msgOK                       db   "Todo OK",10,0
    consultaFormato             db   "El elemento es: %li",10,0
    msgExplicacionCarga         db   "Se cargaran los numeros completando las filas",0

    matriz1     times  64  dq 0      
    matriz2     times  64  dq 0
    matriz3     times  64  dq 0      
    matriz4     times  64  dq 0
    matriz5     times  64  dq 0

    matrizAoperar1 times 64 dq 0
    matrizAoperar2 times 64 dq 0
    matrizAoperar3 times 64 dq 0
    matrizAoperar4 times 64 dq 0
    matrizAoperar5 times 64 dq 0
    matrizResultado     times  64  dq 0
    matrizAux       times 64 dq 0

    contadorDePedidos    db  0

    numFormat            db  "%li",0

    primerElemFormat    db  "( %li ",0
    elemFormat          db  "%li ",0
    finDeFilaFormat     db  "%li )",10,0
    formato1x1          db  "( %li )",10,0

    esPrimerElem         db   1
    tamanioMatriz        dw   1
    
section     .bss
    plusRsp		        resq	1
    operacionElegida    resw    1
    elementoMatriz_num  resq    1
	
    numeroDeMatrizAOperar   resb 1 
    cantidadDeMatricesResta_str resw 1
    cantidadDeMatricesResta_num resq 1
    
    otraMatriz          resb    1
    otraOperacion       resb    1

    Naux_str        resb    1
    Maux_str        resb    1

    Maux_num        resq    1
    Naux_num        resq    1

    N1          resq    1
    M1          resq    1

    N2          resq    1
    M2          resq    1

    N3          resq    1
    M3          resq    1

    N4          resq    1
    M4          resq    1

    N5          resq    1
    M5          resq    1

    NaOperar1   resq    1
    MaOperar1   resq    1

    NaOperar2   resq    1
    MaOperar2   resq    1

    NaOperar3   resq    1
    MaOperar3   resq    1

    NaOperar4   resq    1
    MaOperar4   resq    1

    NaOperar5   resq    1
    MaOperar5   resq    1

    Naux        resq    1
    Maux        resq    1

    reemplazo_str    resw 1
    reemplazo_num    resq 1
    Nresul      resq    1
    Mresul      resq    1

    elementoMatriz_str  resq 1

    matricesApedir      resq 1
    cantidadPedida      resq 1

    identidadMatrizAOperar1 resw 1

    escalar_str             resw 1
    escalar_num             resq 1

section     .text
main:
    call    crearMatrices
    call    menu
finProg:
    ret

crearMatrices:
    mov     rdi,msgComenzarACrear
    call    puts
pedirOtraMatriz:
    mov     rdi,msgPedirN
    call    puts

    mov     rdi,Naux_str
    call    gets

    mov     rdi,Naux_str
    mov     rsi,numFormat
    mov     rdx,Naux_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[Naux_num],0
    jle     numeroIncorrectoFilCol
    cmp     qword[Naux_num],8
    jg      numeroIncorrectoFilCol

    mov     rdi,msgPedirM
    call    puts

    mov     rdi,Maux_str
    call    gets

    mov     rdi,Maux_str
    mov     rsi,numFormat
    mov     rdx,Maux_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[Maux_num],0
    jle     numeroIncorrectoFilCol
    cmp     qword[Maux_num],8
    jg      numeroIncorrectoFilCol

    mov     r11,1
    imul    r11,qword[Naux_num]                                
    imul    r11,qword[Maux_num]                     
    mov     qword[tamanioMatriz],r11

guardarDatosMatriz:
    mov     rdi,msgExplicacionCarga
    call    puts
    mov     rcx,qword[tamanioMatriz]
    mov     rbx,0
guardarValor:
    
    push    rcx

volverAPreguntar:

    mov     rdi,msgPedirElemento
    call    puts

    mov     rdi,elementoMatriz_str
    call    gets

    mov     rdi,elementoMatriz_str
    mov     rsi,elemFormat
    mov     rdx,elementoMatriz_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[elementoMatriz_num],-99
    jl      numeroIncorrectoElem
    cmp     qword[elementoMatriz_num],99
    jg      numeroIncorrectoElem

    mov     r8,qword[elementoMatriz_num]

    cmp     byte[contadorDePedidos],0
    jne     probarSegundaMatriz

    mov     r9,qword[Naux_num]
    mov     qword[N1],r9
    mov     r9,qword[Maux_num]
    mov     qword[M1],r9

    mov     qword[matriz1+rbx],r8
    jmp     elementoColocado

probarSegundaMatriz:
    cmp     byte[contadorDePedidos],1
    jne     probarTerceraMatriz

    mov     r9,qword[Naux_num]
    mov     qword[N2],r9
    mov     r9,qword[Maux_num]
    mov     qword[M2],r9

    mov     qword[matriz2+rbx],r8
    jmp     elementoColocado
    
probarTerceraMatriz:
    cmp     byte[contadorDePedidos],2
    jne     probarCuartaMatriz

    mov     r9,qword[Naux_num]
    mov     qword[N3],r9
    mov     r9,qword[Maux_num]
    mov     qword[M3],r9

    mov     qword[matriz3+rbx],r8
    jmp     elementoColocado
    
probarCuartaMatriz:
    cmp     byte[contadorDePedidos],3
    jne     esQuintaMatriz

    mov     r9,qword[Naux_num]
    mov     qword[N4],r9
    mov     r9,qword[Maux_num]
    mov     qword[M4],r9

    mov     qword[matriz4+rbx],r8
    jmp     elementoColocado
    
esQuintaMatriz:
    mov     r9,qword[Naux_num]
    mov     qword[N5],r9
    mov     r9,qword[Maux_num]
    mov     qword[M5],r9

    mov     qword[matriz5+rbx],r8

elementoColocado:
    add     rbx,8
    pop     rcx
    dec     rcx
    JNZ     guardarValor

    inc     byte[contadorDePedidos] 

    cmp     byte[contadorDePedidos],5
    jne     preguntarPorOtraMatriz
    mov     rdi,msgLimiteDeCreadas
    call    puts
    ret

preguntarPorOtraMatriz:

    mov     rdi,msgPreguntarPorOtraMatriz
    call    puts

    mov     rdi,otraMatriz
    call    gets

    cmp     byte[otraMatriz],"S"
    je      pedirOtraMatriz
    
    ret

numeroIncorrectoFilCol:

    mov     rdi,msgNumeroIncorrectoFilCol
    call    puts
    jmp     pedirOtraMatriz

numeroIncorrectoElem:
    mov     rdi, msgNumeroIncorrectoElem
    call    puts
    jmp     volverAPreguntar

menu:
    mov     rdi,msgMenu
    call    puts

    mov     rdi,operacionElegida
    call    gets

    cmp    word[operacionElegida],"1"
    je     restar

    cmp    word[operacionElegida],"2"
    je     verificarIgualdad

    cmp     word[operacionElegida],"3"
    je      multiplicar

    cmp     word[operacionElegida],"4"
    je     consultarValor

    cmp     word[operacionElegida],"5"
    je     modificarValor

    mov     rdi,msgNumeroIncorrectoOperacion
    call    puts
    jmp     menu
terminarOperacion:
    mov     rdi,msgOtraOperacion
    call    puts

    mov     rdi,otraOperacion
    call    gets

    cmp     byte[otraOperacion],"N"
    je      finProg
    jmp     menu

pedirMatricesAOPerar:

    mov     rdi,msgPedirMatrizAOperar
    call    puts

    mov     rdi,numeroDeMatrizAOperar
    call    gets

    cmp     qword[cantidadPedida],0
    jne     vaPorLaSegunda

    cmp     byte[numeroDeMatrizAOperar],"1"
    jne     VerSiPidioLaSegunda1

    mov     r8,qword[N1]
    mov     qword[NaOperar1],r8
    mov     r8,qword[M1]
    mov     qword[MaOperar1],r8

    mov     word[identidadMatrizAOperar1],1

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

copiarElemento11:
    mov     r8,qword[matriz1+rbx]
    mov     qword[matrizAoperar1+rbx],r8
    add     rbx,8
    loop    copiarElemento11
    jmp     avanzarIteracion

VerSiPidioLaSegunda1:
    cmp     byte[numeroDeMatrizAOperar],"2"
    jne     VerSiPidioLaTercera1

    mov     r8,qword[N2]
    mov     qword[NaOperar1],r8
    mov     r8,qword[M2]
    mov     qword[MaOperar1],r8

    mov     word[identidadMatrizAOperar1],2

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

copiarElemento12:
    mov     r8,qword[matriz2+rbx]
    mov     qword[matrizAoperar1+rbx],r8
    add     rbx,8
    loop    copiarElemento12
    jmp     avanzarIteracion

VerSiPidioLaTercera1:
    
    cmp     byte[numeroDeMatrizAOperar],"3"
    jne     VerSiPidioLaCuarta1

    mov     r8,qword[N3]
    mov     qword[NaOperar1],r8
    mov     r8,qword[M3]
    mov     qword[MaOperar1],r8

    mov     word[identidadMatrizAOperar1],3

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

copiarElemento13:
    mov     r8,qword[matriz3+rbx]
    mov     qword[matrizAoperar1+rbx],r8
    add     rbx,8
    loop    copiarElemento13
    jmp     avanzarIteracion

VerSiPidioLaCuarta1:
    cmp     byte[numeroDeMatrizAOperar],"4"
    jne     VerSiPidioLaQuinta1

    mov     r8,qword[N4]
    mov     qword[NaOperar1],r8
    mov     r8,qword[M4]
    mov     qword[MaOperar1],r8

    mov     word[identidadMatrizAOperar1],4

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

copiarElemento14:
    mov     r8,qword[matriz4+rbx]
    mov     qword[matrizAoperar1+rbx],r8
    add     rbx,8
    loop    copiarElemento14
    jmp     avanzarIteracion

VerSiPidioLaQuinta1:
    cmp     byte[numeroDeMatrizAOperar],"5"
    jne     esIncorrecto

    mov     r8,qword[N5]
    mov     qword[NaOperar1],r8
    mov     r8,qword[M5]
    mov     qword[MaOperar1],r8

    mov     word[identidadMatrizAOperar1],5

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

copiarElemento15:
    mov     r8,qword[matriz5+rbx]
    mov     qword[matrizAoperar1+rbx],r8
    add     rbx,8
    loop    copiarElemento15
    jmp     avanzarIteracion

vaPorLaSegunda:
    cmp     qword[cantidadPedida],1
    jne     vaPorLaTercera

    cmp     byte[numeroDeMatrizAOperar],"1"
    jne     VerSiPidioLaSegunda2

    mov     r8,qword[N1]
    mov     qword[NaOperar2],r8
    mov     r8,qword[M1]
    mov     qword[MaOperar2],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar2]                                
    imul    r11,qword[MaOperar2]                     
    mov     rcx,r11

copiarElemento21:
    mov     r8,qword[matriz1+rbx]
    mov     qword[matrizAoperar2+rbx],r8
    add     rbx,8
    loop    copiarElemento21
    jmp     avanzarIteracion

VerSiPidioLaSegunda2:
    cmp     byte[numeroDeMatrizAOperar],"2"
    jne     VerSiPidioLaTercera2

    mov     r8,qword[N2]
    mov     qword[NaOperar2],r8
    mov     r8,qword[M2]
    mov     qword[MaOperar2],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar2]                                
    imul    r11,qword[MaOperar2]                     
    mov     rcx,r11

copiarElemento22:
    mov     r8,qword[matriz2+rbx]
    mov     qword[matrizAoperar2+rbx],r8
    add     rbx,8
    loop    copiarElemento22
    jmp     avanzarIteracion

VerSiPidioLaTercera2:
    
    cmp     byte[numeroDeMatrizAOperar],"3"
    jne     VerSiPidioLaCuarta2

    mov     r8,qword[N3]
    mov     qword[NaOperar2],r8
    mov     r8,qword[M3]
    mov     qword[MaOperar2],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar2]                                
    imul    r11,qword[MaOperar2]                     
    mov     rcx,r11

copiarElemento23:
    mov     r8,qword[matriz3+rbx]
    mov     qword[matrizAoperar2+rbx],r8
    add     rbx,8
    loop    copiarElemento23
    jmp     avanzarIteracion

VerSiPidioLaCuarta2:
    cmp     byte[numeroDeMatrizAOperar],"4"
    jne     VerSiPidioLaQuinta2

    mov     r8,qword[N4]
    mov     qword[NaOperar2],r8
    mov     r8,qword[M4]
    mov     qword[MaOperar2],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar2]                                
    imul    r11,qword[MaOperar2]                     
    mov     rcx,r11

copiarElemento24:
    mov     r8,qword[matriz4+rbx]
    mov     qword[matrizAoperar2+rbx],r8
    add     rbx,8
    loop    copiarElemento24
    jmp     avanzarIteracion

VerSiPidioLaQuinta2:
    cmp     byte[numeroDeMatrizAOperar],"5"
    jne     esIncorrecto

    mov     r8,qword[N5]
    mov     qword[NaOperar2],r8
    mov     r8,qword[M5]
    mov     qword[MaOperar2],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar2]                                
    imul    r11,qword[MaOperar2]                     
    mov     rcx,r11

copiarElemento25:
    mov     r8,qword[matriz5+rbx]
    mov     qword[matrizAoperar2+rbx],r8
    add     rbx,8
    loop    copiarElemento25
    jmp     avanzarIteracion
vaPorLaTercera:
    cmp     qword[cantidadPedida],2
    jne     vaPorLaCuarta

    cmp     byte[numeroDeMatrizAOperar],"1"
    jne     VerSiPidioLaSegunda3

    mov     r8,qword[N1]
    mov     qword[NaOperar3],r8
    mov     r8,qword[M1]
    mov     qword[MaOperar3],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar3]                                
    imul    r11,qword[MaOperar3]                     
    mov     rcx,r11

copiarElemento31:
    mov     r8,qword[matriz1+rbx]
    mov     qword[matrizAoperar3+rbx],r8
    add     rbx,8
    loop    copiarElemento31
    jmp     avanzarIteracion

VerSiPidioLaSegunda3:
    cmp     byte[numeroDeMatrizAOperar],"2"
    jne     VerSiPidioLaTercera3

    mov     r8,qword[N2]
    mov     qword[NaOperar3],r8
    mov     r8,qword[M2]
    mov     qword[MaOperar3],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar3]                                
    imul    r11,qword[MaOperar3]                     
    mov     rcx,r11

copiarElemento32:
    mov     r8,qword[matriz2+rbx]
    mov     qword[matrizAoperar3+rbx],r8
    add     rbx,8
    loop    copiarElemento32
    jmp     avanzarIteracion

VerSiPidioLaTercera3:
    
    cmp     byte[numeroDeMatrizAOperar],"3"
    jne     VerSiPidioLaCuarta3

    mov     r8,qword[N3]
    mov     qword[NaOperar3],r8
    mov     r8,qword[M3]
    mov     qword[MaOperar3],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar3]                                
    imul    r11,qword[MaOperar3]                     
    mov     rcx,r11

copiarElemento33:
    mov     r8,qword[matriz3+rbx]
    mov     qword[matrizAoperar3+rbx],r8
    add     rbx,8
    loop    copiarElemento33
    jmp     avanzarIteracion

VerSiPidioLaCuarta3:
    cmp     byte[numeroDeMatrizAOperar],"4"
    jne     VerSiPidioLaQuinta3

    mov     r8,qword[N4]
    mov     qword[NaOperar3],r8
    mov     r8,qword[M4]
    mov     qword[MaOperar3],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar3]                                
    imul    r11,qword[MaOperar3]                     
    mov     rcx,r11

copiarElemento34:
    mov     r8,qword[matriz4+rbx]
    mov     qword[matrizAoperar3+rbx],r8
    add     rbx,8
    loop    copiarElemento34
    jmp     avanzarIteracion

VerSiPidioLaQuinta3:
    cmp     byte[numeroDeMatrizAOperar],"5"
    jne     esIncorrecto

    mov     r8,qword[N5]
    mov     qword[NaOperar3],r8
    mov     r8,qword[M5]
    mov     qword[MaOperar3],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar3]                                
    imul    r11,qword[MaOperar3]                     
    mov     rcx,r11

copiarElemento35:
    mov     r8,qword[matriz5+rbx]
    mov     qword[matrizAoperar3+rbx],r8
    add     rbx,8
    loop    copiarElemento35
    jmp     avanzarIteracion

    
vaPorLaCuarta:
    cmp     qword[cantidadPedida],3
    jne     vaPorLaQuinta

    cmp     byte[numeroDeMatrizAOperar],"1"
    jne     VerSiPidioLaSegunda4

    mov     r8,qword[N1]
    mov     qword[NaOperar4],r8
    mov     r8,qword[M1]
    mov     qword[MaOperar4],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar4]                                
    imul    r11,qword[MaOperar4]                     
    mov     rcx,r11

copiarElemento41:
    mov     r8,qword[matriz1+rbx]
    mov     qword[matrizAoperar4+rbx],r8
    add     rbx,8
    loop    copiarElemento41
    jmp     avanzarIteracion

VerSiPidioLaSegunda4:
    cmp     byte[numeroDeMatrizAOperar],"2"
    jne     VerSiPidioLaTercera4

    mov     r8,qword[N2]
    mov     qword[NaOperar4],r8
    mov     r8,qword[M2]
    mov     qword[MaOperar4],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar4]                                
    imul    r11,qword[MaOperar4]                     
    mov     rcx,r11

copiarElemento42:
    mov     r8,qword[matriz2+rbx]
    mov     qword[matrizAoperar4+rbx],r8
    add     rbx,8
    loop    copiarElemento42
    jmp     avanzarIteracion

VerSiPidioLaTercera4:
    
    cmp     byte[numeroDeMatrizAOperar],"3"
    jne     VerSiPidioLaCuarta4

    mov     r8,qword[N3]
    mov     qword[NaOperar4],r8
    mov     r8,qword[M3]
    mov     qword[MaOperar4],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar4]                                
    imul    r11,qword[MaOperar4]                     
    mov     rcx,r11

copiarElemento43:
    mov     r8,qword[matriz3+rbx]
    mov     qword[matrizAoperar4+rbx],r8
    add     rbx,8
    loop    copiarElemento43
    jmp     avanzarIteracion

VerSiPidioLaCuarta4:
    cmp     byte[numeroDeMatrizAOperar],"4"
    jne     VerSiPidioLaQuinta4

    mov     r8,qword[N4]
    mov     qword[NaOperar4],r8
    mov     r8,qword[M4]
    mov     qword[MaOperar4],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar4]                                
    imul    r11,qword[MaOperar4]                     
    mov     rcx,r11

copiarElemento44:
    mov     r8,qword[matriz4+rbx]
    mov     qword[matrizAoperar4+rbx],r8
    add     rbx,8
    loop    copiarElemento44
    jmp     avanzarIteracion

VerSiPidioLaQuinta4:
    cmp     byte[numeroDeMatrizAOperar],"5"
    jne     esIncorrecto

    mov     r8,qword[N5]
    mov     qword[NaOperar4],r8
    mov     r8,qword[M5]
    mov     qword[MaOperar4],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar4]                                
    imul    r11,qword[MaOperar4]                     
    mov     rcx,r11

copiarElemento45:
    mov     r8,qword[matriz5+rbx]
    mov     qword[matrizAoperar4+rbx],r8
    add     rbx,8
    loop    copiarElemento45
    jmp     avanzarIteracion
vaPorLaQuinta:
    cmp     byte[numeroDeMatrizAOperar],"1"
    jne     VerSiPidioLaSegunda5

    mov     r8,qword[N1]
    mov     qword[NaOperar5],r8
    mov     r8,qword[M1]
    mov     qword[MaOperar5],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar5]                                
    imul    r11,qword[MaOperar5]                     
    mov     rcx,r11

copiarElemento51:

    mov     r8,qword[matriz1+rbx]
    mov     qword[matrizAoperar5+rbx],r8
    add     rbx,8
    loop    copiarElemento51
    jmp     avanzarIteracion

VerSiPidioLaSegunda5:
    cmp     byte[numeroDeMatrizAOperar],"2"
    jne     VerSiPidioLaTercera5

    mov     r8,qword[N2]
    mov     qword[NaOperar5],r8
    mov     r8,qword[M2]
    mov     qword[MaOperar5],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar5]                                
    imul    r11,qword[MaOperar5]                     
    mov     rcx,r11

copiarElemento52:
    mov     r8,qword[matriz2+rbx]
    mov     qword[matrizAoperar5+rbx],r8
    add     rbx,8
    loop    copiarElemento52
    jmp     avanzarIteracion

VerSiPidioLaTercera5:
    
    cmp     byte[numeroDeMatrizAOperar],"3"
    jne     VerSiPidioLaCuarta5

    mov     r8,qword[N3]
    mov     qword[NaOperar5],r8
    mov     r8,qword[M3]
    mov     qword[MaOperar5],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar5]                                
    imul    r11,qword[MaOperar5]                     
    mov     rcx,r11

copiarElemento53:
    mov     r8,qword[matriz5+rbx]
    mov     qword[matrizAoperar5+rbx],r8
    add     rbx,8
    loop    copiarElemento53
    jmp     avanzarIteracion

VerSiPidioLaCuarta5:
    cmp     byte[numeroDeMatrizAOperar],"4"
    jne     VerSiPidioLaQuinta5

    mov     r8,qword[N4]
    mov     qword[NaOperar5],r8
    mov     r8,qword[M4]
    mov     qword[MaOperar5],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar5]                                
    imul    r11,qword[MaOperar5]                     
    mov     rcx,r11

copiarElemento54:
    mov     r8,qword[matriz4+rbx]
    mov     qword[matrizAoperar5+rbx],r8
    add     rbx,8
    loop    copiarElemento54
    jmp     avanzarIteracion

VerSiPidioLaQuinta5:
    cmp     byte[numeroDeMatrizAOperar],"5"
    jne     esIncorrecto

    mov     r8,qword[N5]
    mov     qword[NaOperar5],r8
    mov     r8,qword[M5]
    mov     qword[MaOperar5],r8

    mov     rbx,0
    mov     r11,1
    imul    r11,qword[NaOperar5]                                
    imul    r11,qword[MaOperar5]                     
    mov     rcx,r11

copiarElemento55:
    mov     r8,qword[matriz5+rbx]
    mov     qword[matrizAoperar5+rbx],r8
    add     rbx,8
    loop    copiarElemento55
    jmp     avanzarIteracion

avanzarIteracion:
    add     qword[cantidadPedida],1
    mov     r8,qword[cantidadPedida]
    cmp     qword[matricesApedir],r8
    jg      pedirMatricesAOPerar
    ret
esIncorrecto:
    mov     rdi,msgNumeroIncorrecto
    call    puts
    jmp     pedirMatricesAOPerar


consultarValor:
    mov     qword[cantidadPedida],0
    mov     qword[matricesApedir],1
    call    pedirMatricesAOPerar

    mov     rdi,msgPedirN
    call    puts

    mov     rdi,Naux_str
    call    gets

    mov     rdi,Naux_str
    mov     rsi,numFormat
    mov     rdx,Naux_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    
    cmp     qword[Naux_num],0
    jle     numeroIncorrectoConsultarValor
    mov     r9,qword[NaOperar1]
    cmp     qword[Naux_num],r9
    jg      numeroIncorrectoConsultarValor

    mov     rdi,msgPedirM
    call    puts

    mov     rdi,Maux_str
    call    gets

    mov     rdi,Maux_str
    mov     rsi,numFormat
    mov     rdx,Maux_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[Maux_num],0
    jle     numeroIncorrectoConsultarValor
    mov     r9,qword[MaOperar1]
    cmp     qword[Maux_num],r9
    jg      numeroIncorrectoConsultarValor

    ;calcular desplazamiento ((i-1)*cant_col+(j-1))*8
    dec     qword[Naux_num]
    dec     qword[Maux_num]

    mov     r14,1
    imul    r14,qword[Naux_num]
    imul    r14,qword[MaOperar1]
    add     r14,qword[Maux_num]
    imul    r14,8

    mov     rdi,consultaFormato
    mov     rsi,qword[matrizAoperar1+r14]
    sub     rax,rax
    call    printf
    jmp     terminarOperacion

numeroIncorrectoConsultarValor:
    mov     rdi,msgNumeroIncorrectoRango
    CALL    puts
    jmp     consultarValor
modificarValor:
    mov     qword[cantidadPedida],0
    mov     qword[matricesApedir],1
    call    pedirMatricesAOPerar
vapMOD:
    mov     rdi,msgPedirReemplazo
    call    puts

    mov     rdi,reemplazo_str
    call    gets

    mov     rdi,reemplazo_str
    mov     rsi,numFormat
    mov     rdx,reemplazo_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[reemplazo_num],-99
    jl      volverAPreguntarMOD
    cmp     qword[reemplazo_num],99
    jg      volverAPreguntarMOD

    mov     rdi,msgPedirN
    call    puts

    mov     rdi,Naux_str
    call    gets

    mov     rdi,Naux_str
    mov     rsi,numFormat
    mov     rdx,Naux_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[Naux_num],0
    jle     numeroIncorrectoModificarValor
    mov     r9,qword[NaOperar1]
    cmp     qword[Naux_num],r9
    jg      numeroIncorrectoModificarValor

    mov     rdi,msgPedirM
    call    puts

    mov     rdi,Maux_str
    call    gets

    mov     rdi,Maux_str
    mov     rsi,numFormat
    mov     rdx,Maux_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[Maux_num],0
    jle     numeroIncorrectoModificarValor
    mov     r9,qword[MaOperar1]
    cmp     qword[Maux_num],r9
    jg      numeroIncorrectoModificarValor

    ;calcular desplazamiento ((i-1)*cant_col+(j-1))*8
    dec     qword[Naux_num]
    dec     qword[Maux_num]

    mov     r14,1
    imul    r14,qword[Naux_num]
    imul    r14,qword[MaOperar1]
    add     r14,qword[Maux_num]
    imul    r14,8

    cmp     word[identidadMatrizAOperar1],1
    jne     esLaSegunda
    mov     rdi,qword[reemplazo_num]
    mov     qword[matriz1+r14],rdi
    mov     rdi,msgOK
    call    puts
    jmp     terminarOperacion
esLaSegunda:
    cmp     word[identidadMatrizAOperar1],2
    jne     esLaTercera
    mov     rdi,qword[reemplazo_num]
    mov     qword[matriz2+r14],rdi
    mov     rdi,msgOK
    call    puts
    jmp     terminarOperacion
esLaTercera:
    cmp     word[identidadMatrizAOperar1],3
    jne     esLaCuarta
    mov     rdi,qword[reemplazo_num]
    mov     qword[matriz3+r14],rdi
    mov     rdi,msgOK
    call    puts
    jmp     terminarOperacion
esLaCuarta:
    cmp     word[identidadMatrizAOperar1],4
    jne     esLaQuinta
    mov     rdi,qword[reemplazo_num]
    mov     qword[matriz4+r14],rdi
    mov     rdi,msgOK
    call    puts
    jmp     terminarOperacion
esLaQuinta:
    mov     rdi,qword[reemplazo_num]
    mov     qword[matriz5+r14],rdi
    mov     rdi,msgOK
    call    puts
    jmp     terminarOperacion

volverAPreguntarMOD:
    mov     rdi,msgNumeroIncorrecto
    call    puts
    jmp     vapMOD
numeroIncorrectoModificarValor:
    mov     rdi,msgNumeroIncorrectoRango
    CALL    puts
    jmp     modificarValor

multiplicar:
    mov     qword[cantidadPedida],0
    mov     qword[matricesApedir],1
    call    pedirMatricesAOPerar
    
    mov     rdi,msgPedirEscalar
    call    puts

    mov     rdi,escalar_str
    call    gets

    mov     rdi,escalar_str
    mov     rsi,numFormat
    mov     rdx,escalar_num
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

    mov     rbx,0
multiplicarElemento:
    push    rcx
    mov     r13,qword[matrizAoperar1+rbx]
    imul    r13,qword[escalar_num]
    mov     qword[matrizResultado+rbx],r13
    add     rbx,8
    pop     rcx
    loop    multiplicarElemento
    mov     r10,qword[NaOperar1]
    mov     qword[Nresul],r10 
    mov     r10,qword[MaOperar1]
    mov     qword[Mresul],r10    
    call    imprimirMatriz
    jmp     terminarOperacion
restar:

    mov     rdi,msgCantidadMatricesResta
    call    puts

    mov     rdi,cantidadDeMatricesResta_str
    call    gets

    mov     rdi,cantidadDeMatricesResta_str
    mov     rsi,numFormat
    mov     rdx,cantidadDeMatricesResta_num    
    call    checkAlign
    sub     rsp,[plusRsp]
    call    sscanf
    add     rsp,[plusRsp]

    cmp     qword[cantidadDeMatricesResta_num],5
    jle     esValido
    mov     rdi,msgNumeroIncorrecto
    call    puts
    jmp     restar

esValido:
    mov     qword[cantidadPedida],0
    mov     r8,qword[cantidadDeMatricesResta_num]
    mov     qword[matricesApedir],r8 

    call    pedirMatricesAOPerar

    mov     r10,qword[NaOperar1]
    cmp     qword[NaOperar2],r10
    jne     tamanioIncorrecto

    mov     r10,qword[MaOperar1]
    cmp     qword[MaOperar2],r10
    jne      tamanioIncorrecto

    cmp     qword[cantidadDeMatricesResta_num],3
    jl      comenzarAOperar
    
    mov     r10,qword[NaOperar1]
    cmp     qword[NaOperar3],r10
    jne     tamanioIncorrecto

    mov     r10,qword[MaOperar1]
    cmp     qword[MaOperar3],r10
    jne     tamanioIncorrecto

    cmp     qword[cantidadDeMatricesResta_num],4
    jl      comenzarAOperar

    mov     r10,qword[NaOperar1]
    cmp     qword[NaOperar4],r10
    jne     tamanioIncorrecto

    mov     r10,qword[MaOperar1]
    cmp     qword[MaOperar4],r10
    jne     tamanioIncorrecto

    cmp     qword[cantidadDeMatricesResta_num],5
    jl      comenzarAOperar

    mov     r10,qword[NaOperar1]
    cmp     qword[NaOperar5],r10
    jne     tamanioIncorrecto

    mov     r10,qword[MaOperar1]
    cmp     qword[MaOperar5],r10
    jne     tamanioIncorrecto

comenzarAOperar:
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

    mov     rbx,0
restarElemento1:
    push    rcx
    mov     r13,qword[matrizAoperar1+rbx]
    sub     r13,qword[matrizAoperar2+rbx]
    mov     qword[matrizResultado+rbx],r13
    add     rbx,8
    pop     rcx
    loop    restarElemento1
    cmp     qword[cantidadDeMatricesResta_num],3
    jl      imprimirResta
operarConTresMatrices:
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

    mov     rbx,0
restarElemento2:
    push    rcx
    mov     r13,qword[matrizResultado+rbx]
    sub     r13,qword[matrizAoperar3+rbx]
    mov     qword[matrizResultado+rbx],r13
    add     rbx,8
    pop     rcx
    loop    restarElemento2
    cmp     qword[cantidadDeMatricesResta_num],4
    jl      imprimirResta
operarConCuatroMatrices:
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

    mov     rbx,0
restarElemento3:
    push    rcx
    mov     r13,qword[matrizResultado+rbx]
    sub     r13,qword[matrizAoperar4+rbx]
    mov     qword[matrizResultado+rbx],r13
    add     rbx,8
    pop     rcx
    loop    restarElemento3
    cmp     qword[cantidadDeMatricesResta_num],5
    jl      imprimirResta
operarConCincoMatrices:
    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

    mov     rbx,0
restarElemento4:
    push    rcx
    mov     r13,qword[matrizResultado+rbx]
    sub     r13,qword[matrizAoperar5+rbx]
    mov     qword[matrizResultado+rbx],r13
    add     rbx,8
    pop     rcx
    loop    restarElemento4
imprimirResta:
    mov     r10,qword[NaOperar1]
    mov     qword[Nresul],r10 
    mov     r10,qword[MaOperar1]
    mov     qword[Mresul],r10    
    call    imprimirMatriz
    jmp     terminarOperacion
tamanioIncorrecto:
    mov     rdi,msgTamanioIncorrectoMatriz
    call    puts
    jmp     terminarOperacion

verificarIgualdad:
    mov     qword[cantidadPedida],0
    mov     qword[matricesApedir],2
    call    pedirMatricesAOPerar
    
    mov     r11,qword[NaOperar1]
    cmp     r11,qword[NaOperar2]
    jne     noSonIguales

    mov     r11,qword[MaOperar1]
    cmp     r11,qword[MaOperar2]
    jne     noSonIguales

    mov     r11,1
    imul    r11,qword[NaOperar1]                                
    imul    r11,qword[MaOperar1]                     
    mov     rcx,r11

    mov     rbx,0
compararElemento:
    push    rcx
    mov     r13,qword[matrizAoperar1+rbx]
    cmp     r13,qword[matrizAoperar2+rbx]
    pop     rcx
    jne     noSonIguales
    add     rbx,8
    loop    compararElemento

    mov     rdi,msgSonIguales
    call    puts
    jmp     terminarOperacion
    
noSonIguales:
    mov     rdi,msgSonDistintas
    call    puts
    jmp     terminarOperacion

imprimirMatriz:
    

    cmp     qword[Nresul],1
    jne     noEs1X1
    cmp     qword[Mresul],1
    jne     noEs1X1
    
    mov     rdi,formato1x1
    mov     rsi,qword[matrizResultado]
    sub     rax,rax
    call    printf
    jmp     finImprimir

noEs1X1:
    mov     rbx,1
    mov     r15,0

    mov     r11,1
    imul    r11,qword[Nresul]                                
    imul    r11,qword[Mresul]                
    imul    r11,8       
    mov     qword[tamanioMatriz],r11            

imprimirElemento:
    
    mov     r12,qword[tamanioMatriz]
    cmp     r15,r12
    je      finImprimir

    cmp     byte[esPrimerElem],1
    jne     noEsPrimerElem

    mov     rdi,primerElemFormat
    mov     rsi,qword[matrizResultado+r15]
    sub     rax,rax
    call    printf

    add     r15,8
    inc     rbx
    mov     byte[esPrimerElem],0

noEsPrimerElem:

    mov     r10,qword[Mresul]
    cmp     rbx,r10
    je      llegueAlFinalDeLaFila

    mov     rdi,elemFormat
    mov     rsi,qword[matrizResultado+r15]
    sub     rax,rax
    call    printf

    add     r15,8
    inc     rbx

llegueAlFinalDeLaFila:

    cmp     rbx,r10
    jne     imprimirElemento

    mov     rdi,finDeFilaFormat
    mov     rsi,qword[matrizResultado+r15]
    sub     rax,rax
    call    printf

    add     r15,8
    mov     rbx,1
    mov     byte[esPrimerElem],1
    jmp     imprimirElemento

finImprimir:
    ret 

;******************
checkAlign:
	push rax
	push rbx
;	push rcx
	push rdx
	push rdi

	mov   qword[plusRsp],0
	mov		rdx,0

	mov		rax,rsp		
	add   rax,8		;para sumar lo q restó la CALL 
	add		rax,32	;para sumar lo que restaron las PUSH
	
	mov		rbx,16
	idiv	rbx			;rdx:rax / 16   resto queda en RDX

	cmp  rdx,0		;Resto = 0?
	je		finCheckAlign
;mov rdi,msj
;call puts
	mov   qword[plusRsp],8
finCheckAlign:
	pop rdi
	pop rdx
;	pop rcx
	pop rbx
	pop rax
	ret