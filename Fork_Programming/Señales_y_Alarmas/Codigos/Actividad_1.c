/*
**      Giovanny ALexis Reyes Vilchis
**      2163031812
**      Compilar con: gcc Actividad_1.c -o actividad1
**                    ./actividad1
*/

#include <stdio.h>  /* funciones standard de I/O */
#include <unistd.h> /* funciones unix, como getpid() */
#include <signal.h> /* signal macros, y el prot. signal()*/

void cacha_int(int sig_num);

int main(int argc, char* argv[]) {
    /* poner cacha_int como manejador de la señal INT (ctrl-c)*/
    signal(SIGINT, cacha_int);
    int removerArch1, removerArch2;
    char cad[] = "Guardando lo que sea en los archivos...";
    
    FILE *input_file1 = fopen("temp1.txt", "w+");
    FILE *input_file2 = fopen("temp2.txt", "w+");

    fputs(cad,input_file1);
    fputs(cad,input_file2);
    fclose(input_file1);
    fclose(input_file2);
    sleep(20);

    removerArch1 = remove("temp1.txt");
    removerArch2 = remove("temp2.txt");
    
    if(removerArch1 == 0 && removerArch2 == 0) {
        printf("\nArchivos borrados\n\n");
    }

    return 0;
}

/* Esta rutina es el manejador de la señal */
void cacha_int(int sig_num) {
    int removerArch1, removerArch2;
    /* reinstala el manejador de nuevo para la siguiente vez */
    signal(SIGINT, cacha_int);
    removerArch1 = remove("temp1.txt");
    removerArch2 = remove("temp2.txt");
    if(removerArch1 == 0 && removerArch2 == 0) {
        printf("\nArchivos borrados\n\n");
    }
    fflush(stdout);
}
