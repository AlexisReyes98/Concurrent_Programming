/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Actividad_3.c -o actividad3
**                    ./actividad3
*/

#include <stdio.h>  /* funciones de I/O standard */
#include <unistd.h> /* func standard de unix como alarm()*/
#include <signal.h> /* nombres de macros de señales, y el prototipo de signal() */
#include <stdlib.h>
#include <time.h>

#define M 4

void imprimeMatriz(int m[M][M]);
void escribeMatrizA(int mA[M][M]);
void escribeMatrizB(int mB[M][M]);
void escribeInicio(int rinicio, int cinicio);
void multiplica(int rinicio, int cinicio);
void cacha_alarma(int sig_num);

int matriz_A[M][M];
int matriz_B[M][M];
int matriz_C[M][M];

int main() {
    srand(time(NULL));
    FILE *input_file;
    FILE *input_file1;
    FILE *input_file2;
    int rinicio;
    int cinicio;

    /* 
        Si hay archivos temporales.
        Se llenan las matrices con los valores de los archivos temporales.
        rinicio y cinicio igual toman valores del archivo
    */
    if ((input_file1 = fopen("MatA.txt","r")) && (input_file2 = fopen("MatB.txt","r")))
	{
		/* Se genera la Matriz A */
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                fscanf(input_file1,"%d", matriz_A[i][j]);
            }
        }

        /* Se genera la Matriz B */
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                fscanf(input_file2,"%d", matriz_B[i][j]);
            }
        }

        if (!(input_file = fopen("inicio.txt","w")))
        {
            printf("Error al abrir el fichero");
        }
        else
        {
            fscanf(input_file,"%d %d", rinicio, cinicio);
        }

        //signal(SIGALRM, cacha_alarma); /* Cacha la señal ALRM */

        multiplica(rinicio, cinicio);
	}
    /*
        Si no hay archivos temporales
        Se llenan las matrices con enteros aleatorios.
        rinico y cinicio inician en 0.
        Se escriben los archivos temporales.
    */
    else if ((input_file1 = fopen("MAtA.txt","w")) && (input_file2 = fopen("MAtA.txt","w")))
    {
        /* Se genera la Matriz A */
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                matriz_A[i][j] = 10+rand()%50;
            }
        }

        /* Se genera la Matriz B */
        for (int i = 0; i < M; i++)
        {
            for (int j = 0; j < M; j++)
            {
                matriz_B[i][j] = 10+rand()%50;
            }
        }
        rinicio = 0; cinicio = 0;

        escribeInicio(rinicio, cinicio);
        escribeMatrizA(matriz_A);
        escribeMatrizB(matriz_B);

        //signal(SIGALRM, cacha_alarma); /* Cacha la señal ALRM */

        multiplica(rinicio, cinicio);
    }
    else
    {
        printf("\nError al abrir los ficheros\n");
    }

    //alarm(30); /* arranca la alarma de 30 segundos */
    
    //alarm(0); /* apaga la alarma */
    

    printf("\nMatriz A:");
    imprimeMatriz(matriz_A);
    printf("\n\nMatriz B:");
    imprimeMatriz(matriz_B);
    printf("\n\nResultado Matriz C:");
    imprimeMatriz(matriz_C);
    printf("\n");
    

    fclose(input_file);
    fclose(input_file1);
    fclose(input_file2);

    //remove("inicio.txt");
    //remove("MatA.txt");
    //remove("MatB.txt");

    return 0;
}

void multiplica(int rinicio, int cinicio) {
    /* Se realiza la multiplicación de matrices A*B y se obtiene como resultado la matriz C */
    for (int i = rinicio; i < M; i++) /* Filas de A */
    {
        for (int j = cinicio; j < M; j++) /* Columnas de B */
        {
            matriz_C[i][j] = 0;
            for (int k = rinicio; k < M; k++) /* Columnas de A */
            {
                matriz_C[i][j] += matriz_A[i][k] * matriz_B[k][j];
            }
        }
    }
}

void imprimeMatriz(int m[M][M]) {
    for (int i = 0; i < M; i++)
    {
        printf("\n");
        for (int j = 0; j < M; j++)
        {
            printf("%d ", m[i][j]);
        }
    }
}

/*
    EStas funciones sirven para escribir en los archivos temporales.
*/
void escribeMatrizA(int mA[M][M]) {
    FILE *input_file1 = fopen("MAtA.txt","w");
    for (int i = 0; i < M; i++)
    {
        fprintf(input_file1,"\n");
        for (int j = 0; j < M; j++)
        {
            fprintf(input_file1,"%d",mA[i][j]);
        }
    }
    fclose(input_file1);
}

void escribeMatrizB(int mB[M][M]) {
    FILE *input_file2 = fopen("MAtB.txt","w");
    for (int i = 0; i < M; i++)
    {
        fprintf(input_file2,"\n");
        for (int j = 0; j < M; j++)
        {
            fprintf(input_file2,"%d",mB[i][j]);
        }
    }
    fclose(input_file2);
}

void escribeInicio(int rinicio, int cinicio) {
    FILE *input_file;
	if (!(input_file = fopen("inicio.txt","w")))
	{
		printf("Error al abrir el fichero");
	}
	else 
	{
		fprintf(input_file,"%d %d", rinicio, cinicio);
		fclose(input_file);
	}
}

/* Define el manejador de la alarma */
void cacha_alarma(int sig_num)
{
    exit(0);
}
