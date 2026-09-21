#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <string.h>

/*
Equipo 09
Gerardo Josue Rubio Calderon
Argenis Adan Gutierrez Hurtado
*/

using namespace std;

class OperacionesArreglos {
public:
    static void crearArregloMPI(int* sub_A, int* sub_B, int size_local, const char* nombre_equipo, int rank_mpi, int imprimir_detalles, int offset_global) {
        #pragma omp parallel for
        for (int i = 0; i < size_local; i++) {
            int pos_global = offset_global + i;
            sub_A[i] = pos_global + 1;
            sub_B[i] = (pos_global + 1) * 2;

            if (imprimir_detalles) {
                int hilo_id = omp_get_thread_num();
                #pragma omp critical
                {
                    printf("[Equipo: %s] [Proceso MPI: %d] [Hilo OpenMP: %d] [Posicion: %d] [Operacion: Creacion]\n",
                           nombre_equipo, rank_mpi, hilo_id, pos_global);
                }
            }
        }
    }

    static void sumar(int* sub_A, int* sub_B, int* sub_C, int size_local, const char* nombre_equipo, int rank_mpi, int imprimir_detalles, int offset_global) {
        #pragma omp parallel for
        for (int i = 0; i < size_local; i++) {
            sub_C[i] = sub_A[i] + sub_B[i];
            if (imprimir_detalles) {
                int hilo_id = omp_get_thread_num();
                int pos_global = offset_global + i;
                #pragma omp critical
                {
                   printf("[Equipo: %s] [Proceso MPI: %d] [Hilo OpenMP: %d] [Posicion: %d] [Operacion: Suma | Resultado: %d]\n",
                          nombre_equipo, rank_mpi, hilo_id, pos_global, sub_C[i]);
                }
            }
        }
    }

    static void restar(int* sub_A, int* sub_B, int* sub_C, int size_local, const char* nombre_equipo, int rank_mpi, int imprimir_detalles, int offset_global) {
        #pragma omp parallel for
        for (int i = 0; i < size_local; i++) {
            sub_C[i] = sub_A[i] - sub_B[i];
            if (imprimir_detalles) {
                int hilo_id = omp_get_thread_num();
                int pos_global = offset_global + i;
                #pragma omp critical
                {
                    printf("[Equipo: %s] [Proceso MPI: %d] [Hilo OpenMP: %d] [Posicion: %d] [Operacion: Resta | Resultado: %d]\n",
                           nombre_equipo, rank_mpi, hilo_id, pos_global, sub_C[i]);
                }
            }
        }
    }

    static void multiplicar(int* sub_A, int* sub_B, int* sub_C, int size_local, const char* nombre_equipo, int rank_mpi, int imprimir_detalles, int offset_global) {
        #pragma omp parallel for
        for (int i = 0; i < size_local; i++) {
            sub_C[i] = sub_A[i] * sub_B[i];
            if (imprimir_detalles) {
                int hilo_id = omp_get_thread_num();
                int pos_global = offset_global + i;
                #pragma omp critical
                {
                    printf("[Equipo: %s] [Proceso MPI: %d] [Hilo OpenMP: %d] [Posicion: %d] [Operacion: Multiplicacion | Resultado: %d]\n",
                           nombre_equipo, rank_mpi, hilo_id, pos_global, sub_C[i]);
                }
            }
        }
    }

    static void cuadrado(int* sub_A, int* sub_C, int size_local, const char* nombre_equipo, int rank_mpi, int imprimir_detalles, int offset_global) {
        #pragma omp parallel for
        for (int i = 0; i < size_local; i++) {
            sub_C[i] = sub_A[i] * sub_A[i];
            if (imprimir_detalles) {
                int hilo_id = omp_get_thread_num();
                int pos_global = offset_global + i;
                #pragma omp critical
                {
                    printf("[Equipo: %s] [Proceso MPI: %d] [Hilo OpenMP: %d] [Posicion: %d] [Operacion: Cuadrado de A | Resultado: %d]\n",
                           nombre_equipo, rank_mpi, hilo_id, pos_global, sub_C[i]);
                }
            }
        }
    }
};

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int num_procesos, rank_mpi, long_name;
    char nombre_equipo[MPI_MAX_PROCESSOR_NAME];

    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank_mpi);
    MPI_Get_processor_name(nombre_equipo, &long_name);

    int N = 40;
    int imprimir_detalles = (N == 4000000) ? 1 : 0;

    if (N % num_procesos != 0) {
        if (rank_mpi == 0) printf("Error: El size del arreglo (%d) debe ser divisible exactamente entre el numero de procesos (%d).\n", N, num_procesos);
        MPI_Finalize();
        return -1;
    }

    int size_local = N / num_procesos;
    int offset_global = rank_mpi * size_local;

    int* sub_A = (int*)malloc(size_local * sizeof(int));
    int* sub_B = (int*)malloc(size_local * sizeof(int));
    int* sub_C = (int*)malloc(size_local * sizeof(int));

    int *A = NULL, *B = NULL, *C = NULL;
    if (rank_mpi == 0) {
        A = (int*)malloc(N * sizeof(int));
        B = (int*)malloc(N * sizeof(int));
        C = (int*)malloc(N * sizeof(int));
    }

    int opcion_menu = 0;

    do {
        if (rank_mpi == 0) {
            printf("\n=== MENU PRINCIPAL SCATTER/GATHER (Nodo Maestro: %s) ===\n", nombre_equipo);
            printf("1. Crear arreglos\n");
            printf("2. Sumar arreglos\n");
            printf("3. Restar arreglos\n");
            printf("4. Multiplicar arreglos\n");
            printf("5. Cuadrado de arreglo A\n");
            printf("0. Salir\n");
            printf("Seleccione una opcion: ");
            fflush(stdout); // PARCHE: Obliga a imprimir el menú inmediatamente
            scanf("%d", &opcion_menu);
        }

        MPI_Bcast(&opcion_menu, 1, MPI_INT, 0, MPI_COMM_WORLD);
        double tiempo_inicio = MPI_Wtime();

        switch(opcion_menu) {
            case 1:
                OperacionesArreglos::crearArregloMPI(sub_A, sub_B, size_local, nombre_equipo, rank_mpi, imprimir_detalles, offset_global);
                // El maestro recolecta todo de golpe usando Gather
                MPI_Gather(sub_A, size_local, MPI_INT, A, size_local, MPI_INT, 0, MPI_COMM_WORLD);
                MPI_Gather(sub_B, size_local, MPI_INT, B, size_local, MPI_INT, 0, MPI_COMM_WORLD);
                break;

            case 2: case 3: case 4: case 5:
                // El maestro reparte de golpe usando Scatter
                MPI_Scatter(A, size_local, MPI_INT, sub_A, size_local, MPI_INT, 0, MPI_COMM_WORLD);
                if (opcion_menu != 5) {
                    MPI_Scatter(B, size_local, MPI_INT, sub_B, size_local, MPI_INT, 0, MPI_COMM_WORLD);
                }

                // Ejecución local con OpenMP
                if (opcion_menu == 2) OperacionesArreglos::sumar(sub_A, sub_B, sub_C, size_local, nombre_equipo, rank_mpi, imprimir_detalles, offset_global);
                else if (opcion_menu == 3) OperacionesArreglos::restar(sub_A, sub_B, sub_C, size_local, nombre_equipo, rank_mpi, imprimir_detalles, offset_global);
                else if (opcion_menu == 4) OperacionesArreglos::multiplicar(sub_A, sub_B, sub_C, size_local, nombre_equipo, rank_mpi, imprimir_detalles, offset_global);
                else if (opcion_menu == 5) OperacionesArreglos::cuadrado(sub_A, sub_C, size_local, nombre_equipo, rank_mpi, imprimir_detalles, offset_global);

                // El maestro recolecta los resultados C de golpe usando Gather
                MPI_Gather(sub_C, size_local, MPI_INT, C, size_local, MPI_INT, 0, MPI_COMM_WORLD);
                break;

            case 0:
                if (rank_mpi == 0) printf("\nSaliendo del programa y liberando memoria.\n");
                break;
            default:
                if (rank_mpi == 0) printf("\nOpcion no valida chaval.\n");
                break;
        }

        double tiempo_fin = MPI_Wtime();

        if (rank_mpi == 0 && opcion_menu >= 1 && opcion_menu <= 5) {
            printf("\n--- RESULTADOS FINALES ---\n");
            printf("Tiempo de ejecucion: %f segundos.\n", tiempo_fin - tiempo_inicio);
            if (imprimir_detalles) {
                printf("Arreglo resultado:\n");
                for (int i = 0; i < N; i++) {
                    if (opcion_menu == 1) printf("A[%d]=%d B[%d]=%d | ", i, A[i], i, B[i]);
                    else printf("%d ", C[i]);
                }
                printf("\n");
            }
            printf("---------------------\n");
            fflush(stdout); // PARCHE: Vaciado final de los resultados para que se muestren de golpe
        }
    } while (opcion_menu != 0);

    free(sub_A); free(sub_B); free(sub_C);
    if (rank_mpi == 0) { free(A); free(B); free(C); }

    MPI_Finalize();
    return 0;
}
