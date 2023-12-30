MyBash es un intérprete de línea de comandos (shell) diseñado para reflejar la arquitectura y estructura interna de las primitivas de comunicación y concurrencia proporcionadas por UNIX. Este proyecto se enfoca en implementar un shell similar al bash, ofreciendo funcionalidades generales como la ejecución de comandos en modo foreground y background, redirección de entrada y salida estándar, así como la creación de pipes entre comandos.

Objetivos de Implementación
Ejecución de Comandos: Capacidad para ejecutar comandos en modo foreground y background.
Redirección de Entrada/Salida: Implementación de redirección de entrada y salida estándar.
Pipes entre Comandos: Habilitación de pipes para conectar múltiples comandos.
Comandos Internos: Implementación de comandos internos como cd, help, y exit.
Robustez y Pruebas: Manejo robusto de entradas incompletas o inválidas y pruebas unitarias.
Características Implementadas
Ejecución de comandos en primer y segundo plano.
Redirección de entrada y salida estándar.
Pipes para la comunicación entre comandos.
Implementación de comandos internos cd, help, y exit.
Soporte para terminación con CTRL-D (EOT).
Robustez ante entradas inválidas o incompletas.
Estructura del Proyecto
El proyecto está organizado en 6 módulos:

mybash: Módulo principal.
command: Definiciones de los TADs para representar comandos.
parsing: Procesamiento de la entrada del usuario usando un parser.
parser: Implementación del TAD parser.
execute: Módulo ejecutor de comandos.
builtin: Implementación de los comandos internos del shell.
Uso del Proyecto
Para compilar y ejecutar el proyecto:

Clona este repositorio.
Compila el código usando el comando make.
Ejecuta el shell con ./mybash.
Asegúrate de tener instaladas las dependencias de GLib y Check.

Kickstart y Testeo
El repositorio proporciona un conjunto de pruebas unitarias para los módulos command y parsing. Para ejecutarlas:

Pruebas de command.c (scommand y pipeline): $ make test-command
Pruebas de parsing.c: $ make test-parsing
Pruebas para todos los módulos: $ make test
