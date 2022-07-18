# uwu-engine

Motor de videojuegos desarrollado para el curso CC5512.

(nombre no final, no soy muy creativo para los nombres)

Este motor estará orientado a juegos del género RPG/Dungeon crawler, donde se recorre un mapa que existe en una grilla en primera persona 3D, con combates estilo RPG por turnos en el camino.

## Instrucciones de compilación

Para compilar es necesario utilizar CMake y Visual Studio. Los pasos a seguir son:

* Clonar el repositorio
* Hacer `cd` dentro del repositorio
* Ejecutar `git submodule init`
* Ejecutar `git submodule update`
* Hacer `cd libs/grafica_cpp`
* Crear carpeta `build`
* Hacer `cd build`
* Ejecutar `cmake ../`
* Abrir `build/grafica.sln` en Visual Studio y compilar `grafica` en modo Release

Con esto se compilaron las dependencias del motor.

Para compilar el motor:
* Crear la carpeta `engine/build`
* Hacer `cd engine/build`
* Ejecutar `cmake ../source`
* Abrir `engine/build/UWUengine.sln` en Visual Studio
* Compilar `uwuEngine` en modo Release

## Para ejecutar:
* Dirigirse a `build/Release`
* Ejecutar `uwuEngine.exe`

## Controles
* Flechas Izquierda/Derecha, D-pad izquierda/derecha: Girar a la izquierda/derecha
* Flechas Arriba/Abajo, D-pad arriba/abajo: Avanzar/Retroceder
* Z - Agregar o eliminar delfín al centro del mapa
* X - Mostrar u ocultar diálogo

# Crear una escena
Para poder armar una escena básica basta usar los nodos disponibles por defecto modificando el archivo main.cpp. A continuación se explica el proceso de armado de escena.

## Cargar recursos
Es necesario cargar los shaders, modelos, sonidos y texturas que se van a utilizar. Para esto se puede usar las funciones que se hayan por defecto en la función loadResources(). Todas las funciones de cargar recursos toman primero la ruta relativa al archivo main.cpp del recurso a cargar (o en caso de ser un shader del vertex, fragment y opcionalmente geometry shader), y luego el nombre que se le asignará a ese recurso.

## Modificar controles
En la función setInputBindings() se encuentran las asociaciones de las acciones por defecto con las teclas del teclado y los botones y ejes de un joystick. Se puede modificar los controles asociados a gusto, así como agregar o eliminar opciones agregando o quitando pares de acción-botón.

## Armar la escena
En la función buildScene() se arma el árbol de nodos, utilizando los distintos nodos disponibles. En la escena por defecto hay un ejemplo de casi todos los nodos disponibles en que se observa cómo construirlos y configurarlos. Al final de dicha función se encuentra la línea `scene.save(scene.json)`, esto guardará la escena armada como un json que luego puede cargarse desde la función main con `Scene::load(ruta_archivo)`.

Notar que es necesario que haya un nodo Player y un nodo GridNode para que el juego funcione correctamente.

## Correr la escena

Para correr una escena basta con entregarla como argumento a la función `uwu::setMainScene(scene)` antes de ejecutar `uwu::run()`. Las funciones `uwu::initialize()`, `setInputBindings()`, `uwu::run()` y `uwu::kill()` son necesarias para que funcione el juego y deberían permanecer en su lugar por defecto.!

# Diagrama de arquitectura de UWUengine

[diagramaUwUEngine](https://user-images.githubusercontent.com/53621382/179604094-f1c7a6a2-60f0-44d2-86de-9f8e200d066b.png)
