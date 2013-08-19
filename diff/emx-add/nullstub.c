/*
 * This stub file is used by LD and EMXOMFLD as a null file when run with -Zexe
 * switch. This program does nothing except running another program with
 * same name, but appending the .exe extension.
 */

#include <string.h>
#include <alloca.h>
#include <process.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/dirent.h>

int main (int argc, char *argv[])
{
 char name[MAXNAMLEN + 4];
 _execname (name, MAXNAMLEN);
 execv (strcat(name, ".exe"), argv);
 return 127;
}
