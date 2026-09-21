# Procesamiento Distribuido y Paralelo con MS-MPI y OpenMP 🚀

Este repositorio contiene la implementación de un sistema distribuido en C++ para realizar operaciones aritméticas sobre arreglos de gran escala (hasta 4,000,000 de elementos) utilizando un clúster de computadoras. 

El proyecto combina **MS-MPI** para la distribución de la carga de trabajo a través de la red (paso de mensajes) y **OpenMP** para el procesamiento paralelo local aprovechando los procesadores multinúcleo de cada nodo.

## 👥 Integrantes (Equipo 09)
* Gutiérrez Hurtado Argenis Adán
* Rubio Calderón Gerardo Josué
* **Institución:** Universidad de Guadalajara (CUTonalá) - Ingeniería en Ciencias Computacionales.

## ⚙️ Características del Proyecto
* **Dos implementaciones de MPI:** 
  1. `version1_send.cpp`: Implementación punto a punto utilizando `MPI_Send` y `MPI_Recv`.
  2. `version2_scatter.cpp`: Implementación optimizada mediante comunicaciones colectivas con `MPI_Scatter` y `MPI_Gather`.
* **Paralelismo Híbrido:** Distribución de datos entre computadoras (MPI) y cálculo paralelo en hilos (OpenMP).
* **Escalabilidad:** Soporta ejecuciones detalladas (40 elementos) para trazabilidad de hilos/nodos y pruebas de estrés (4,000,000 de elementos) utilizando generación aleatoria *thread-safe* (`<random>`).
* **Manejo de Buffer:** Integración de comandos de vaciado de memoria (`fflush`) para visualización de salida en tiempo real a través de la red.

## 🛠️ Requisitos del Sistema
Para compilar y ejecutar este proyecto en un entorno de red, se requiere:
* **Sistema Operativo:** Windows 10/11.
* **Red:** Las computadoras deben estar conectadas a la misma red local (LAN/WLAN) configurada como **"Red Privada"**.
* **Dependencias:** 
  * [Microsoft MPI (MS-MPI)](https://docs.microsoft.com/en-us/message-passing-interface/microsoft-mpi) (v10.1.x o superior).
  * Compilador C++ con soporte para OpenMP (Ej. MinGW / GCC).
  * Entorno de desarrollo recomendado: Code::Blocks.

## 🚀 Guía de Ejecución

### 1. Configuración del Entorno de Red
Antes de ejecutar el clúster, asegúrese de que el demonio de MS-MPI esté corriendo en todos los nodos involucrados y que el Firewall de Windows permita conexiones entrantes en los puertos de MPI (o desactívelo temporalmente en redes privadas).
En la consola (CMD) de cada computadora como Administrador, levante el servicio:

smpd -d
2. Compilación
Compile el archivo .cpp deseado asegurándose de vincular las librerías de MPI y habilitar OpenMP en los flags del compilador (Ej. -fopenmp en GCC). Las DLLs de MS-MPI (msmpi.dll) deben estar accesibles o en la misma carpeta que el ejecutable generado.

3. Ejecución Distribuida
El programa se lanza desde el nodo maestro apuntando a las direcciones IP de los equipos que conforman el clúster. Utilice el comando mpiexec inyectando las credenciales del usuario de red local configurado para MPI:

DOS
mpiexec -hosts 2 [IP_NODO_MAESTRO] 1 [IP_NODO_ESCLAVO] 1 -user mpiuser -password [CONTRASEÑA] \\[IP_NODO_MAESTRO]\Ruta\Compartida\ejecutable.exe
Sustituya las IPs y la ruta compartida según su configuración de red local.

📊 Operaciones Soportadas
El programa despliega un menú interactivo que permite realizar las siguientes operaciones sobre los arreglos particionados:

Crear y llenar arreglos dinámicamente.

Suma de arreglos (A + B).

Resta de arreglos (A - B).

Multiplicación de arreglos (A * B).

Cuadrado del arreglo A (A^2).

En pruebas de alto rendimiento (N=4,000,000), el menú suprimirá las salidas individuales en consola para mostrar únicamente los tiempos de ejecución reales (Benchmarks).
