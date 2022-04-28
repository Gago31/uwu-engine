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
* Ejecutar `cmake ../source`
* Abrir `build/Project.sln` en Visual Studio y compilar `grafica` en modo Release
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
