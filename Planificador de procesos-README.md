### Laboratorio 3: Planificador de Procesos

Este laboratorio se enfoca en el planificador apropiativo de xv6-riscv, analizando su funcionamiento, evaluando los procesos beneficiados o perjudicados por este planificador y desarrollando una implementación que reemplace la política de planificación original.

#### Objetivos Principales:
1. **Estudio del Planificador Original:**
   - Comprender la política de planificación utilizada por xv6-riscv.
   - Investigar la duración del quantum y del cambio de contexto.
   - Evaluar si el cambio de contexto consume tiempo de un quantum y si un proceso puede recibir menos tiempo.

2. **Contabilización de Selecciones:**
   - Modificar la estructura de proceso para registrar cuántas veces un proceso es elegido por el planificador.
   - Modificar la función `procdump` para mostrar esta información y crear una syscall `pstat(pid)` que devuelva la prioridad, selección por el planificador y última ejecución de un proceso.

3. **Evaluación de Rendimiento:**
   - Integrar programas de usuario `iobench` y `cpubench` en xv6-riscv para medir I/O y potencia de cálculo.
   - Realizar mediciones de estos programas solos y en combinación para entender cómo afecta el planificador a cada tipo de proceso.

4. **Implementación de MLFQ:**
   - Gradualmente, rastrear y mantener la prioridad de los procesos y el número de selecciones por el planificador.
   - Implementar el algoritmo MLFQ (Multi-Level Feedback Queue) en el planificador, utilizando reglas específicas para seleccionar el próximo proceso.

5. **Análisis del Nuevo Planificador:**
   - Realizar mediciones nuevamente para entender las propiedades y el rendimiento del nuevo planificador MLFQ.
   - Investigar la posibilidad de que se produzca starvation en el nuevo planificador y justificar esta posibilidad.

#### Puntos Extra (Rama "extras"):
- Varias mejoras al planificador, como la política de ascenso de prioridad, ajuste de longitudes de quantum por nivel de prioridad, optimización del uso del procesador cuando no hay procesos, entre otros.
- Investigar cómo la localidad de los procesos influye en el rendimiento si el planificador intenta mantenerlos en el mismo procesador.

La realización del laboratorio implica entender el funcionamiento del planificador original, desarrollar mejoras graduales y realizar mediciones para evaluar el rendimiento y comportamiento de los procesos bajo el nuevo planificador. Además, se plantean puntos extra para explorar y profundizar sobre aspectos específicos del planificador.
