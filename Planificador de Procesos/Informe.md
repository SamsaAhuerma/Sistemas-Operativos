# INFORME LAB 3 - GRUPO SOmatizando
### Integrantes:
* Martin Salinas
* Samsa Ahuerma
* Ezequiel Ludueña
* Consuelo Moyano

## Introducción
En este laboratorio se investigaron las politicas de planificación de la CPU por parte del sistema operativo xv6-riscv (Round-Robin) y se compararon las mismas con la política "multi level feedback queue" (MLFQ), implementada en el mismo sistema. Para ello se realizaron mediciones de rendimiento en operaciones de tipo CPU e IO en ambos casos y luego se contrastaron los resultados.


## Estudio de xv6
* El sistema operativo utiliza la política de planificaicón Round Robin. El sistema recorre un arreglo con todos los procesos buscando al primero que no esté disponible y lo selecciona para correr. Una vez que el proceso termine su ejecución o llegue al time interrupt (lo que suceda primero), se coloca al final del arreglo y se repite el proceso.
* La duración del quantum en xv6 es de 1000000 (un millón) de ciclos del CPU.
* El cambio de contexto en xv6 dura 28 operaciones, ya que en el archivo swtch.s hay 28 instrucciones de store y load de registros.
* El cambio de contexto en xv6 consume tiempo del quantum.
* No hay forma de asignar menor tiempo a un proceso durante la ejecución del sistema, ya que el quantum se asigna globalmente al inicio.  
* Los estados en los que un proceso puede permanecer en xv6-riscv son:
    * **UNUSED**: Es un estado transicional, se asigna cuando se crea un proceso o cuando se finalizó su ejecución.  
    * **USED**: Este estado representa que ya fue reservada memoria para este proceso y esta listo para ser utilizado.  
    * **SLEEPING**: Este estado se le asigna a un proceso a través de la syscall sleep( ) y representa que todavia no puede ser ejecutado.  
    * **RUNNABLE**: Representa que el proceso se encuentra en el arreglo de procesos a la espera de ser ejecutado por el planificador. Se pasa a este estado luego de ejecutar yield( ) si todavia no finalizó el proceso y terminó el quantum de ejecución, con fork( ) cuando se crea el mismo y con wakeup( ) para salir del estado sleeping.  
    * **RUNNING**: Este estado se asigna cuando el proceso es seleccionado por el planificador y se está ejecutando el mismo.
    * **ZOMBIE**: Este estado se asigna cuando un proceso finalizó su ejecución pero todavia se está esperando la finalización de un proceso padre del mismo.



## Mediciones Round-Robin
Se ejecutaron pruebas de poder de cómputo (cpubench) y de tiempos de respuestas de I/O (iobench), simulando distintos casos según que y cuantas pruebas se ejecutaron al mismo tiempo, sobre xv6-riscv con su política de planificación Round-Robin.  
Las pruebas se realizaron en una computadora con un procesador Intel Core i5-4440 y la version del software Qemu 6.2.0 para emular xv6-riscv.  
Se ejecutaron las pruebas con un quantum de 1000000 (un millón) de ticks y luego con un quantum 10 veces menor, de 100000 (cien mil) ticks.   
Los resultados de las mismas son:
### Caso 1: 1 iobench solo.
| Quantum  | Promedio OPW/100T | Promedio OPR/100T | Cant. select IO |
|:--------:|:-----------------:|:-----------------:|:---------------:|
|  1000000 |      1234,1578    |     1234,1578     |      133854     |
|  100000  |      998,3157     |     998,3157      |      167727      |

### Caso 2: 1 cpubench solo.
| Quantum  | Promedio MFLOPS /100T | Cant. select CPU |
|:--------:|:---------------------:|:----------------:|
|  1000000 |       321,3333        |       2126       |
|  100000  |       295,3333        |       20990      |

### Caso 3: 1 iobench con 1 cpubench.
| Quantum  |Promedio MFLOPS /100T |Promedio OPW/100T |Promedio OPR/100T   | Cant. select IO |  Cant. select CPU|
|:--------:|:--------------------:|:----------------:|:------------------:|:---------------:|:----------------:|
| 1000000  |       569,7692       |     33,1     |     33,1       |      2358      |      2448        |
| 100000   |       469,8461       |     335,4   |     335,4     |      20120      |      21653       |
### Caso 4: 1 cpubench con 1 cpubench.
| Quantum  | Promedio MFLOPS /100T 1er cpubench| Promedio MFLOPS /100T 2do cpubench| Cant. select 1er CPU |Cant. select 2do CPU |
|:--------:|:---------------------------------:|:---------------------------------:|:--------------------:|:-------------------:|
| 1000000  |             472,2666              |              473,1333             |         2122         |         2133        |
| 100000   |             562,2500              |              562,8750             |         20770        |         21142       |
### Caso 5: 1 cpubench con 1 cpubench y 1 iobench.
| Quantum  |Promedio MFLOPS /100T 1er cpubench |Promedio MFLOPS /100T 2do cpubench |Promedio OPW/100T   | Promedio OPR/100T | Cant. select IO|Cant. select 1er CPU | Cant. select 2do CPU |
|:----------:|:----------:|:----------:|:----------:|:----------:|:----------:|:----------:|:----------:|
| 1000000  | 551,8333 | 551,1111 |   19,2      | 19,2       |1221| 2144|2150|
| 100000   | 554,4444| 552,7222|    156,63 |156,63|10502|20801|20508|

## Mediciones MLFQ
 
### Caso 1: 1 iobench solo.
| Quantum  | Promedio OPW/100T | Promedio OPR/100T | Cant. select IO |
|:--------:|:-----------------:|:-----------------:|:---------------:|
|  1000000 |       2158        |        2158       |     163108    |
|  100000  |     2949,2105     |      2949,2105    |     193692    |

### Caso 2: 1 cpubench solo.
| Quantum  | Promedio MFLOPS /100T | Cant. select CPU |
|:--------:|:---------------------:|:----------------:|
|  1000000 |       1261,0555       |        2101      |
|  100000  |          558          |       22571      |

### Caso 3: 1 iobench con 1 cpubench.
| Quantum  |Promedio MFLOPS /100T |Promedio OPW/100T |Promedio OPR/100T   | Cant. select IO |  Cant. select CPU|
|:--------:|:--------------------:|:----------------:|:------------------:|:---------------:|:----------------:|
| 1000000  |       1090,0588      |       35,7       |        35,7        |      12833      |       2102       |
| 100000   |        518,2352      |     334,7647     |      334,7647      |      21181      |      21026       |

### Caso 4: 1 cpubench con 1 cpubench.
| Quantum  | Promedio MFLOPS /100T 1er cpubench| Promedio MFLOPS /100T 2do cpubench| Cant. select 1er CPU |Cant. select 2do CPU |
|:--------:|:---------------------------------:|:---------------------------------:|:--------------------:|:-------------------:|
|  1000000 |              679,4375             |            676,1875               |        1056          |         1062        |
|  100000  |              656,9375             |              654,625              |        10587         |         10625       |

### Caso 5: 1 cpubench con 1 cpubench y 1 iobench.
| Quantum  |Promedio MFLOPS /100T 1er cpubench |Promedio MFLOPS /100T 2do cpubench |Promedio OPW/100T   | Promedio OPR/100T | Cant. select IO|Cant. select 1er CPU | Cant. select 2do CPU |
|:----------:|:----------:|:----------:|:----------:|:----------:|:----------:|:----------:|:----------:|
| 1000000  | 694,6471 | 696,7647 |    36,1     |    36,1    |10638|1050 |1062|
| 100000   | 631,9375 | 635,9333 |  280   | 280 |10601|10484|12499|


## Conclusiones

### Política RR  
Con respecto al caso de IObench solo, no existen grandes diferencias cuando cambia el quantum. Esto se debe a que la mayor cantidad de llamadas al scheduler se producen por operaciones IO y no por timer interrupt.  

En el caso de CPUbench solo, podemos notar que cuando el quantum es 10 veces más chico, se hacen 10 veces más llamadas al scheduler. Esto sucede, ya que al no tener operaciones de lectura y escritura, la gran mayoría de las interrupciones son por el timer interrupt. Al haber un quantum más chico, va a haber más interrupciones por tiempo, y por lo tanto más llamadas al scheduler.  

En el caso de IObench y CPUbench ejecutándose en paralelo, se puede ver en los resultados que tienen una cantidad similar de llamadas al scheduler. Cuando el quantum es 10 veces mas chico, en el proceso CPUbench tiene 10 veces más llamadas al scheduler como se veía en el caso 2, y el proceso IObench tiene 10 veces mas operaciones de lectura y escritura, por lo que las llamadas al scheduler aumentan también 10 veces más. Comparten equitativamente el control del CPU.    

En el caso de dos CPUbench ejecutándose en paralelo, pasa algo muy similar al CPUbench ejecutándose solo, pues cuando el quantum es 10 veces más chico, hacen 10 veces más llamadas al scheduler. Además notar que los datos obtenidos se puede ver un uso equitativo del CPU para ambos procesos.   

Para el caso de dos CPUbench y un IObench ejecutándose en paralelo, también es muy similar al tercer caso, no hay muchas diferencias. Se puede notar que el IObench tiene la mitad de llamadas al scheduler que los procesos de CPUbench, creemos que esto ocurre ya que el proceso IObench interrumpe su ejecución justo antes del quantum, dejándole poco tiempo a los procesos de CPUbench para correr. Por lo tanto, los procesos de CPUbench tienen el doble de llamadas al scheduler que el IObench.  
  
  ### Política MLFQ  

Los dos primeros casos de IObench y CPUbench ejecutándose solos son muy parecidos al RR como se esperaba, ya que la prioridad no se modifica al haber solo un proceso en ejecución.  

En el caso de 1 IObench y 1 CPUbench corriendo en paralelo, los resultados son muy similares a los de RR. Esto sucede ya que el IObench no puede tomar todo el control del CPU porque solo lo comparte con un proceso y no con más, como sucede en el último caso.  
  
En el caso de 2 CPUbench ejecutándose en paralelo, no existen grandes diferencias con la política RR. Cuando el quantum es 10 veces mas chico, la cantidad de llamadas al scheduler es 10 veces más grande como sucedía en la otra política. Esto es porque la prioridad disminuye cada vez que hay un timer interrupt, y es la única forma de interrumpir estos procesos ya que no tienen operaciones IO. Por lo tanto se alternan entre estos dos procesos por la cantidad de llamadas al scheduler y se mantienen iguales. 

En el caso de 2 CPUbench y 1 IObench, podemos ver que se mantiene el promedio de MFLOPS en ambos CPUbench mientras que la cantidad de operaciones de lectura y escritura aumenta notablemente. Esto se debe a que al hacer operaciones IO, se hace una llamada al scheduler sin disminuir su prioridad, pues solo disminuye cuando hay un timer interrupt. De esta manera, el proceso de IObench, toma control del CPU. 


## Reflexiones  
La política de MLFQ, favorece a los procesos que tienen operaciones de IO, como es el caso de IObench. Esto puede llevar, a que los procesos como CPUbench, que no tienen operaciones de IO, sufran starvation. Esto ocurre ya que los procesos como IObench, no disminuyen su prioridad, pues no hacen interrupciones de tiempo. 