### Laboratorio 4: BigBrother FS

#### Objetivo General:
En este laboratorio, el objetivo es trabajar con un sistema de archivos FAT y ampliar su funcionalidad. La implementación base es un sistema de archivos FAT parcial en espacio de usuario. Se busca completar partes de la interfaz del sistema de archivos para añadir funcionalidades y también agregar una monitorización transparente de la actividad dentro del sistema de archivos.

#### Detalles Relevantes:
- **FAT-FUSE:** Se trabaja con un sistema de archivos FAT en espacio de usuarios utilizando FUSE (Filesystem in USErspace). Esto permite implementar filesystems a nivel de espacio de usuarios.
  
- **Parte 1: Vigilancia:**
  - Se crea un archivo secreto (/fs.log) para registrar todas las operaciones de lectura y escritura de archivos realizadas por el usuario.
  - Debe ser invisible a comandos como `ls`, pero accesible a comandos como `cat` o redirecciones.

- **Parte 2: Ocultando el archivo en otros FS:**
  - La vigilancia debe funcionar en otras implementaciones de FAT sin alterar el formato FAT.
  - El archivo fs.log debe tener ciertas características que permitan su ignorancia en otras herramientas sin ser sobreescrito.

- **Parte 3: Unlink y rmdir:**
  - Agregar funcionalidad al filesystem para borrar archivos y directorios.
  - Marcar los clusters correspondientes como libres y actualizar la entrada de directorio y el árbol de directorios en memoria.

- **Parte 4: Escribiendo nuevos clusters:**
  - Modificar el filesystem para permitir la escritura completa de archivos en nuevos clusters si es necesario, ampliando la capacidad de almacenamiento.

#### Puntos Extras Propuestos:
- Agregar una opción al montar el filesystem para mostrar/ocultar el archivo de logs.
- Escribir tests para asegurar la correcta implementación de las partes 1 y 2.
- Agregar registro de palabras censuradas en el archivo de logs.
- Implementar una caché en memoria para evitar recorrer la tabla FAT por cada operación de lectura o escritura.
- Soporte para nombres de archivo de más de 8 caracteres.
- Modificar el TAD fat_table para utilizar correctamente la segunda copia de la tabla FAT.
- Agregar un nuevo cluster al directorio cuando se llena de entradas.

#### Instrucciones de Compilación y Ejecución:
- Instalación de dependencias: `libfuse-dev`, `fs`, `unittest`.
- Compilación: `make`.
- Generación de imagen para montar: `make image`.
- Montaje del filesystem: `make mount` o `./fat-fuse test.img mnt`.
- Desmontaje: `make unmount` o `umount mnt`.
- Ejecución de tests: `make testfs`.

El laboratorio implica extender un sistema de archivos FAT en espacio de usuario, implementando funcionalidades específicas y garantizando su correcto funcionamiento y pruebas exhaustivas. Las partes del trabajo abordan desde la vigilancia hasta la ampliación de capacidades de almacenamiento, con la posibilidad de implementar puntos extras para una funcionalidad más avanzada.
