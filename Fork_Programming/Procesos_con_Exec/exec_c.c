/***********************************************************
   exec_c.c
   Demuestra el uso de algunas de las funciones exec.
***********************************************************/
#include <unistd.h>
#include <stdio.h>

void usar_execvp();
void usar_execlp();

int main () {
   char c;
   printf("Quieres usar execvp(v) o execlp(l)?: ");
   scanf("%c",&c);
   switch(c) {
      case 'v':   usar_execvp(); break;
      case 'l':   usar_execlp(); break;
   }

   return 0;
}

void usar_execvp()
{
   // La consola xterm ejecutará el editor VI
   char *v[]={"xterm", "-bg", "yellow", "-e", "vi", NULL};
   execvp("xterm",v);
}

void usar_execlp()
{
   // La consola xterm ejecutará el editor VI
   execlp("xterm", "xterm", "-bg", "skyblue", "-e", "vi", NULL);
}

