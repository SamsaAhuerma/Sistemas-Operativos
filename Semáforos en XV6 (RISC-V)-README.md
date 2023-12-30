### Semáforos para Espacio de Usuario - XV6

Este proyecto de laboratorio consiste en implementar un sistema de semáforos nombrados en el espacio de usuario del sistema operativo XV6. Estos semáforos ofrecen sincronización entre procesos y se gestionan a nivel del kernel. Deberán desarrollarse cuatro llamadas al sistema (syscalls) y un programa de usuario llamado "pingpong".

#### Características de los Semáforos Nombrados:
- Administrados por el kernel.
- Disponibles globalmente para todos los procesos del sistema operativo.
- Conservan su estado mientras el sistema esté activo (se pierden entre reinicios).
- Identificados por un "nombre", números enteros entre 0 y un límite máximo.

#### Syscalls a Implementar:
- `int sem_open(int sem, int value)`: Abre e inicializa el semáforo con un valor arbitrario.
- `int sem_close(int sem)`: Libera el semáforo.
- `int sem_up(int sem)`: Incrementa el semáforo, desbloqueando procesos cuando su valor es 0.
- `int sem_down(int sem)`: Decrementa el semáforo, bloqueando procesos cuando su valor es 0.

**Observaciones sobre las Syscalls:**
- Definir posibles errores y políticas del sistema para casos como cerrar un semáforo en uso o modificar su valor inicial.

#### Programa "pingpong":
- Sincroniza la impresión de "ping" y "pong" usando semáforos.
- Toma un argumento N que representa la cantidad de veces que se imprimirán "ping" y "pong".
- Genera dos procesos que alternan impresiones: uno imprime "ping" y el otro "pong".

**Detalles de Entrega:**
- Ejecutar `make grade` para autocorrección y `make clean` para limpieza antes de la entrega.
- El estilo de codificación debe seguir las convenciones de XV6.

Este proyecto busca implementar de manera libre de problemas de concurrencia y condiciones de carrera, además de realizar una validación adecuada de los argumentos de las syscalls.
