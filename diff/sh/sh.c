#include <stdlib.h>
#include <process.h>

int main (int argc, char *argv[])
{
 argv[0] = "sh";
 execvp("bash.exe", argv);
 return 127;
}
