//@@@@@@@@@@@@@@@@@@@*******************---------------------תתתתתתתתתתתתתתתתתתת
//                      OS2/EMX-specific portion of CONFIG.H
//@@@@@@@@@@@@@@@@@@@*******************---------------------תתתתתתתתתתתתתתתתתתת

/* include UNISTD and STDLIB first to exclude redefinition confilicts */
#include <unistd.h>
#define _MALLOC_H	/* Hack: avoid including malloc.h :-( */
#include <stdlib.h>
#undef _MALLOC_H	/* Really it has not been included */

/*-------------- Override some autodetected by configure values --------------*/

/* EMX select is quite uneffective */
#undef HAVE_SELECT
/* EMX getenv() is fine enough */
#undef CAN_REDEFINE_GETENV
/* EMX has not FIFO support but configure erroneously defines it */
#undef PROCESS_SUBSTITUTION
/* EMX getpgrp accepts no arguments */
#undef GETPGRP_VOID
/* emx_supp.c handles #! */
#undef HAVE_HASH_BANG_EXEC
#define HAVE_HASH_BANG_EXEC
/* EMX have gethostname() */
#undef HAVE_GETHOSTNAME
#define HAVE_GETHOSTNAME
/* EMX have getpeername() */
#undef HAVE_GETPEERNAME
#define HAVE_GETPEERNAME
/* configure fails to set this because sys/socket.h requires sys/types.h */
#undef HAVE_SYS_SOCKET_H
#define HAVE_SYS_SOCKET_H
/* EMX has -lsocket but has no -lnsl -- still configure fails to understand it */
#undef HAVE_LIBSOCKET
#define HAVE_LIBSOCKET
/* Change default mail directory to something more usual */
#undef DEFAULT_MAIL_DIRECTORY
#define DEFAULT_MAIL_DIRECTORY "/tcpip/etc/mail"

/* We`re providing a emulated shared-library layer */
#define HAVE_DLOPEN
#define HAVE_DLSYM
#define HAVE_DLCLOSE

/* We`ll use the POSIX signal handling scheme */
#ifdef HAVE_BSD_SIGNALS
#undef HAVE_BSD_SIGNALS
#endif
#ifndef HAVE_POSIX_SIGNALS
#define HAVE_POSIX_SIGNALS
#endif
#ifdef HAVE_USG_SIGHOLD
#undef HAVE_USG_SIGHOLD
#endif

#define O_NODELETETEMP 0x80000000

/* Kludge for finfo.c */
#define major(x) ((x >> 8) & 0xFF)
#define minor(x) (x & 0xFF)
/* Kludge for pathchk.c */
#define error warnx

extern int execve_async;

extern void emx_initialize(void);
extern char *emx_unixify(char *path);
extern char *emx_deunixify(char *path);
extern char *emx_getcwd(char *buffer, int size);
extern int  emx_chdir(char *dir);
extern int  emx_tempopen(char *name, int oflag, int mode);
extern int  emx_realpid(int pid);

#define PATH_SEPARATOR ';'
#define UNIXIFY_PATH(path) emx_unixify(path)

/* Default primary prompt string. */
#undef PPROMPT
#define PPROMPT "[\\h|\\w]"

#define EXEC_EXT_COUNT 7
#define EXEC_EXT_ENUM  {"", ".exe", ".cmd", ".sh",".com", ".bat", ".btm"}
#define EXEC_EXT_RUN   { 2,     0,       1,    2,      0,      1,      1}
/* 0 - run directly; 1 - through COMSPEC; 2 - through BASH */
