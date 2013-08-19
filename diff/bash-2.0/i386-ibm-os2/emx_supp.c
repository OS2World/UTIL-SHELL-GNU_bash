/*============================================================================*/
/*                                                                            */
/* BASH/EMX support routines                                                  */
/*                                                                            */
/*============================================================================*/
/*                                                                            */
/* Copyright (C) 1987, 1989, 1992 Free Software Foundation, Inc.              */
/*                                                                            */
/* This file is part of the GNU Readline Library, a library for               */
/* reading lines of text with interactive input and history editing.          */
/*                                                                            */
/* The GNU Readline Library is free software; you can redistribute it         */
/* and/or modify it under the terms of the GNU General Public License         */
/* as published by the Free Software Foundation; either version 1, or         */
/* (at your option) any later version.                                        */
/*                                                                            */
/* The GNU Readline Library is distributed in the hope that it will be        */
/* useful, but WITHOUT ANY WARRANTY; without even the implied warranty        */
/* of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           */
/* GNU General Public License for more details.                               */
/*                                                                            */
/* The GNU General Public License is often shipped with GNU software, and     */
/* is generally kept in a file called COPYING or LICENSE.  If you do not      */
/* have a copy of the license, write to the Free Software Foundation,         */
/* 675 Mass Ave, Cambridge, MA 02139, USA.                                    */
/*                                                                            */
/*============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <io.h>
#include <fcntl.h>
#include <sys/kbdscan.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <share.h>
#include <stdarg.h>

#include <emx/startup.h>
#include <emx/syscalls.h>
#include <emx/io.h>

#define INCL_DOS
#define INCL_DOSERRORS
#define INCL_DOSSESMGR
#define INCL_WINPROGRAMLIST
#define INCL_WINFRAMEMGR
#include <os2.h>

#include "command.h"
#include "config.h"
#include "general.h"
#include "externs.h"
#include "execute_cmd.h"

static char *k_up      = "\EOA";
static char *k_down    = "\EOB";
static char *k_right   = "\EOC";
static char *k_left    = "\EOD";
static char *k_f1      = "\EOP";
static char *k_f2      = "\EOQ";
static char *k_f3      = "\EOR";
static char *k_f4      = "\EOS";
static char *k_f5      = "\EOT";
static char *k_f6      = "\EOU";
static char *k_f7      = "\EOV";
static char *k_f8      = "\EOW";
static char *k_f9      = "\EOX";
static char *k_f10     = "\EOY";
static char *k_c_back  = "\E[M";
static char *k_ins     = "\E[@";
static char *k_del     = "\E[P";
static char *k_c_home  = "\E[2J";
static char *k_c_end   = "\E[K";
static char *k_home    = "\E]7";
static char *k_end     = "\E]1";
static char *k_pgup    = "\E]9";
static char *k_pgdn    = "\E]3";
static char *k_a_tab   = "\E]at";
static char *k_c_tab   = "\E]ct";
static char *k_s_tab   = "\E]st";
static char *k_c_pgup  = "\E]c9";
static char *k_c_pgdn  = "\E]c3";
static char *k_c_up    = "\E]c8";
static char *k_c_down  = "\E]c2";
static char *k_c_left  = "\E]c4";
static char *k_c_right = "\E]c6";
static char *k_c_ins   = "\E]ci";
static char *k_c_del   = "\E]cd";
static char *k_s_ins   = "\E]si";
static char *k_s_del   = "\E]sd";
static char *k_c_space = "\E]c ";
static char *k_a_lbrck = "\E][";
static char *k_a_rbrck = "\E]]";
static char *k_a_enter = "\E]ae";
static char *k_a_a     = "\EaA";
static char *k_a_b     = "\EaB";
static char *k_a_c     = "\EaC";
static char *k_a_d     = "\EaD";
static char *k_a_e     = "\EaE";
static char *k_a_f     = "\EaF";
static char *k_a_g     = "\EaG";
static char *k_a_h     = "\EaH";
static char *k_a_i     = "\EaI";
static char *k_a_j     = "\EaJ";
static char *k_a_k     = "\EaK";
static char *k_a_l     = "\EaL";
static char *k_a_m     = "\EaM";
static char *k_a_n     = "\EaN";
static char *k_a_o     = "\EaO";
static char *k_a_p     = "\EaP";
static char *k_a_q     = "\EaQ";
static char *k_a_r     = "\EaR";
static char *k_a_s     = "\EaS";
static char *k_a_t     = "\EaT";
static char *k_a_u     = "\EaU";
static char *k_a_v     = "\EaV";
static char *k_a_w     = "\EaW";
static char *k_a_x     = "\EaX";
static char *k_a_y     = "\EaY";
static char *k_a_z     = "\EaZ";

int enh_read(int handle, void *var, size_t size)
{
 if (_isterm(handle))
 {
  static char *curseq = NULL;                   /* Current key sequence */
  int key;

again:
  if (curseq)
  {
   *(char *)var = *curseq;
   curseq++;
   if (*curseq == 0) curseq = NULL;
   return sizeof(char);
  }

  key = _read_kbd(0, 1, 0);

  switch (key)
  {
   case -1:
    goto again;
   case 0:
   {
    key = _read_kbd(0, 0, 0);
    switch (key)
    {
     case -1:
      goto again;
     case K_UP:            curseq = k_up;      goto again;
     case K_DOWN:          curseq = k_down;    goto again;
     case K_RIGHT:         curseq = k_right;   goto again;
     case K_LEFT:          curseq = k_left;    goto again;
     case K_F1:            curseq = k_f1;      goto again;
     case K_F2:            curseq = k_f2;      goto again;
     case K_F3:            curseq = k_f3;      goto again;
     case K_F4:            curseq = k_f4;      goto again;
     case K_F5:            curseq = k_f5;      goto again;
     case K_F6:            curseq = k_f6;      goto again;
     case K_F7:            curseq = k_f7;      goto again;
     case K_F8:            curseq = k_f8;      goto again;
     case K_F9:            curseq = k_f9;      goto again;
     case K_F10:           curseq = k_f10;     goto again;
     case K_HOME:          curseq = k_home;    goto again;
     case K_END:           curseq = k_end;     goto again;
     case K_INS:           curseq = k_ins;     goto again;
     case K_DEL:           curseq = k_del;     goto again;
     case K_CTRL_HOME:     curseq = k_c_home;  goto again;
     case K_CTRL_END:      curseq = k_c_end;   goto again;
     case K_CTRL_UP:       curseq = k_c_up;    goto again;
     case K_CTRL_DOWN:     curseq = k_c_down;  goto again;
     case K_CTRL_LEFT:     curseq = k_c_left;  goto again;
     case K_CTRL_RIGHT:    curseq = k_c_right; goto again;
     case K_CTRL_INS:      curseq = k_c_ins;   goto again;
     case K_CTRL_DEL:      curseq = k_c_del;   goto again;
     case K_SHIFT_INS:     curseq = k_s_ins;   goto again;
     case K_SHIFT_DEL:     curseq = k_s_del;   goto again;
     case K_PAGEUP:        curseq = k_pgdn;    goto again;
     case K_PAGEDOWN:      curseq = k_pgup;    goto again;
     case K_CTRL_PAGEUP:   curseq = k_c_pgdn;  goto again;
     case K_CTRL_PAGEDOWN: curseq = k_c_pgup;  goto again;
     case K_CTRL_TAB:      curseq = k_c_tab;   goto again;
     case K_CTRL_SPACE:    curseq = k_c_space; goto again;
     case K_ALT_TAB:       curseq = k_a_tab;   goto again;
     case K_ALT_LEFT_BRACKET:
                           curseq = k_a_lbrck; goto again;
     case K_ALT_RIGHT_BRACKET:
                           curseq = k_a_rbrck; goto again;
     case K_ALT_RETURN:    curseq = k_a_enter; goto again;
     case K_ALT_A:         curseq = k_a_a;     goto again;
     case K_ALT_B:         curseq = k_a_b;     goto again;
     case K_ALT_C:         curseq = k_a_c;     goto again;
     case K_ALT_D:         curseq = k_a_d;     goto again;
     case K_ALT_E:         curseq = k_a_e;     goto again;
     case K_ALT_F:         curseq = k_a_f;     goto again;
     case K_ALT_G:         curseq = k_a_g;     goto again;
     case K_ALT_H:         curseq = k_a_h;     goto again;
     case K_ALT_I:         curseq = k_a_i;     goto again;
     case K_ALT_J:         curseq = k_a_j;     goto again;
     case K_ALT_K:         curseq = k_a_k;     goto again;
     case K_ALT_L:         curseq = k_a_l;     goto again;
     case K_ALT_M:         curseq = k_a_m;     goto again;
     case K_ALT_N:         curseq = k_a_n;     goto again;
     case K_ALT_O:         curseq = k_a_o;     goto again;
     case K_ALT_P:         curseq = k_a_p;     goto again;
     case K_ALT_Q:         curseq = k_a_q;     goto again;
     case K_ALT_R:         curseq = k_a_r;     goto again;
     case K_ALT_S:         curseq = k_a_s;     goto again;
     case K_ALT_T:         curseq = k_a_t;     goto again;
     case K_ALT_U:         curseq = k_a_u;     goto again;
     case K_ALT_V:         curseq = k_a_v;     goto again;
     case K_ALT_W:         curseq = k_a_w;     goto again;
     case K_ALT_X:         curseq = k_a_x;     goto again;
     case K_ALT_Y:         curseq = k_a_y;     goto again;
     case K_ALT_Z:         curseq = k_a_z;     goto again;
    }
    goto again;
   }
   case 15: /* Shift+Tab */
    curseq = k_s_tab;  goto again;
   case 127:
    curseq = k_c_back; goto again;
   default:
    *(char *)var = key; return 1;
  }
 }
 else
  return read(handle, var, size);
}

extern int interactive_shell;

static void to_first_column()
{
 USHORT Row, Column;

 if (interactive_shell)
 {
  VioGetCurPos(&Row, &Column, 0);
  if (Column > 0)
  {
   putchar('\n');
   fflush(stdout);
  }
 }
}

// Return the length of the string, adding a 1 for each '"'
static int q_strlen (char const *s)
{
 char *tmp = (char *)s;
 int len = strlen(s);
 char prev = 0;

 while (*tmp)
 {
  if ((*tmp == '"') || ((*tmp == '\\') && (prev != '\\')))
   len += 2;   // A bit of overkill, but anyway its alloca()`ted
  else
   len++;
  tmp++;
 }
 return (len);
}

// Copy src into dst, char by char, backquoting '"'s if neccessary
static void q_strcat (char const *d, char const *s)
{
 char *src = (char *)s;
 char *dst = (char *)d;
 int bs = 0;

 dst += strlen (dst);

 while (*src != 0)
 {
  switch (*src)
  {
   case '"':
   {
    memset (dst, '\\', ++bs);
    dst += bs; bs = 0;
    break;
   }
   case '\\':
    ++bs;
    break;
   default:
    bs = 0;
  }
  *dst++ = *src++;
 }
 *dst = 0;
}

/* BASH executable pathname */
extern char *shell_name;
/* Existing extensions for executables */
extern char *exec_ext[];
/* Interpreters for different extensions */
static char exec_run[EXEC_EXT_COUNT] = EXEC_EXT_RUN;
/* Get a variable by name */
extern char *get_string_value (char *);

/*----------------------------------------------------------------------------*/
/*-------- A BASH'ized version of spawnve which is used by exec* calls -------*/
/*----------------------------------------------------------------------------*/

/* Use for runflags instead of 'mode' */
#define spawn_SESSION		0x0001
#define spawn_WAIT		0x0002
#define spawn_OVERLAY		0x0004
#define spawn_DETACH		0x0008
#define spawn_TYPE		0x000F0000
#define spawn_TYPE_DEFAULT	SSF_TYPE_DEFAULT << 16
#define spawn_TYPE_FULLSCREEN	SSF_TYPE_FULLSCREEN << 16
#define spawn_TYPE_WINDOWED	SSF_TYPE_WINDOWABLEVIO << 16
#define spawn_TYPE_PM		SSF_TYPE_PM << 16
#define spawn_TYPE_VDM		SSF_TYPE_VDM << 16
#define spawn_TYPE_WINDOWEDVDM	SSF_TYPE_WINDOWEDVDM << 16

/* Shared memory management */
#define PID_RING_LEN 256    /* Must be a step of two! */

static struct
{
 HMTX inuse;
 struct { int read,ppid,cpid; } pidring[PID_RING_LEN];
 int ringhead,ringtail;
} *ShMem, *pShMem;

int spawnve(int mode, const char *name, char * const argv[],
            char * const envp[])
{
  int i, j, l, n, prefix_len = 0;
  char *ext, *tmp, *arg1, *execname, *p_arg, *p_env, *prefix = NULL;
  char runtype, freeexec = 0, hswValid = 0, quoteargs = 1;
  ULONG appflags;
  const char * const *p;
  unsigned int runflags = 0;
  int retcode = -1;
  char errbuf[MAXNAMLEN + 1], queue[MAXNAMLEN + 1];
  SWCNTRL swc;
  HSWITCH vioHSW;

  errno = ENOENT;

  /* EMX crashes on very long filenames... */
  if (strlen (name) > MAXNAMLEN - 4)
  {
   errno = ENAMETOOLONG;
   return -1;
  }

  /* Find extension ordinal in exec_ext array */
  ext = _getext2(name);
  for (i = 0; i < EXEC_EXT_COUNT; i++)
  {
   if (strcmp(ext, exec_ext[i]) == 0)
    goto found;
  }
  if (access(name, F_OK) == 0)
   errno = ENOEXEC;
  return -1;

found:
 {
  PTIB tb;
  PPIB pb;

  /* Determine our session type */
  if ((DosGetInfoBlocks(&tb, &pb) == NO_ERROR) &&
      (vioHSW = WinQuerySwitchHandle(NULLHANDLE, pb->pib_ulpid)) &&
      (WinQuerySwitchEntry(vioHSW, &swc) == NO_ERROR))
   hswValid = 1;
 }

 switch (mode & 0xFF)
 {
  case P_WAIT:
   runflags |= spawn_WAIT; break;
  case P_PM:
   runflags |= spawn_SESSION | spawn_TYPE_PM; break;
  case P_OVERLAY:
   runflags |= spawn_WAIT | spawn_OVERLAY; break;
  case P_DETACH:
   runflags |= spawn_DETACH; break;
 }

 if (((runtype = exec_run[i]) == 0) &&
     (DosQueryAppType((PSZ)name, &appflags) == NO_ERROR) &&
     ((runflags & spawn_DETACH) == 0) &&
     (hswValid))
 {
  /* Compare to application type */
  switch (appflags & 7)
  {
   case FAPPTYP_NOTSPEC:  /* Methinks its a DOS proggy */
   {
    if (appflags & FAPPTYP_DOS)
     if (swc.bProgType == PROG_FULLSCREEN)
      runflags |= (spawn_SESSION | spawn_TYPE_VDM);
     else
      runflags |= (spawn_SESSION | spawn_TYPE_WINDOWEDVDM);
    else
     runflags |= (spawn_SESSION | spawn_TYPE_DEFAULT);
    break;
   }
   case FAPPTYP_NOTWINDOWCOMPAT:
   {
    if (swc.bProgType != PROG_FULLSCREEN)
     runflags |= (spawn_SESSION | spawn_TYPE_FULLSCREEN);
    break;
   }
   case FAPPTYP_WINDOWCOMPAT:
   {
    /* That'll do it */
    break;
   }
   case FAPPTYP_WINDOWAPI:
   {
    runflags |= (spawn_SESSION | spawn_TYPE_PM);
    break;
   }
   default:
   {
    runtype = 1; /* Let $COMSPEC decide what to do */
    break;
   }
  }
 }

fallback:
 switch (runtype)
 {
  case 0:
directrun:
   if ((runflags & spawn_SESSION) &&
       (((runflags & spawn_TYPE) == spawn_TYPE_VDM) ||
        ((runflags & spawn_TYPE) == spawn_TYPE_WINDOWEDVDM)))
   {
    /* DOS command interpreter does not understand '/'s */
    execname = savestring((char *)name);
    freeexec = 1;
    for (tmp = execname; *tmp; tmp++)
     if (*tmp == '/') *tmp = '\\';
   } else
    execname = (char *)name;
   break;
  case 1:
   execname = get_string_value ("COMSPEC");
   if (!execname)
   {
    internal_error("COMSPEC variable not defined");
    errno = EACCES;
    return -1;
   }
   prefix_len = strlen(execname) + 1;
   prefix = alloca(prefix_len + 2 + 1 + strlen(name) + 1);
   strcpy(prefix, execname);
   emx_deunixify(prefix);
   strcpy((char *)&prefix[prefix_len - 1], " /c ");
   prefix_len += 2 + 1;
   strcpy((char *)&prefix[prefix_len], name);
   prefix_len += strlen(name) + 1;
   break;
  case 2:
  default:
  {
   /* We must define HAVE_HASH_BANG_EXEC since we`re processing #! here */
   FILE *f;
   char line[MAXNAMLEN + 3];
   line[0] = 0;
   if (!(f = fopen(name, "rt")))
   {
    errno = ENOENT;
    return -1;
   };
   fgets((char *)&line, sizeof(line), f);
   fclose(f);
   if (line[0] != '#')
    /* Try to run as regular executable */
    goto directrun;
   if (line[1] != '!')
   {
    /* Run script using current shell */
    strcpy((char *)&line, "#! ");
    strcat((char *)&line, shell_name);
   }
   n = strlen(line);
   while ((n > 0) && ((line[n - 1] < ' ') || (whitespace(line[n])))) n--;
   for (i = 2; whitespace(line[i]) && i < n; i++)
    ;
   for (j = i; (!whitespace(line[j])) && j < n; j++)
    ;
   l = i; j++;
   tmp = xmalloc(j - i);
   _strncpy(tmp, (char *)&line[i], j - i);

   execname = find_user_command(tmp);
   free(tmp);
   freeexec = 1;

   prefix_len = n - l + 1 + 1 + strlen(name);
   prefix = alloca(prefix_len + 1);
   _strncpy(prefix, (char *)&line[l], n - l + 1);
   strcat(prefix, " ");
   strcat(prefix, name);
   break;
  }
 }

 if ((execname == NULL) || (access(execname, F_OK) != 0))
 {
  if (execname == NULL)
  {
   errno = ENOEXEC;
   return -1;
  }
  execname = savestring(execname);
  freeexec = 1;
  if ((ext_file_status(&execname) & FS_EXISTS) == 0)
  {
   free(execname);
   errno = ENOENT;
   return -1;
  }
 }

 {
  char *qlist = get_string_value ("BASH_NOQUOTEARGS");
  char *tmp, *name = _getname (execname);
  int namelen;

  tmp = _getext (name);
  namelen = (tmp ? (int) (tmp - name) : strlen (name));

  while (qlist)
  {
   tmp = strchr (qlist, ';');
   if (!(j = tmp ? (int) (tmp - qlist) : strlen (qlist)))
    break;
   if ((namelen == j) &&
       (memicmp (qlist, name, namelen) == 0))
   {
    quoteargs = 0;
    break;
   }
   qlist += j;
   while (qlist[0] == ';')
    qlist++;
  }
 }

 /* Prepare command-line string */
 j = prefix_len + 2;
 for (i = 0; i <= 1; i++)
 {
  for (p = (const char **)argv; *p != NULL; ++p)
  {
   if (p != (const char **)argv)
    if (i == 0)			// Add a space before args starting from 1
     j++;
    else
     strcat(p_arg, " ");
   else
    if (prefix_len)
     continue;			// Prefix already contains argv[0]

   // If argv[i] contains a space or tab, we should put it in quotes
   if (strchr(*p, ' ') || strchr(*p, '\t'))
    if (i == 0)
     j += 1 + (quoteargs ? q_strlen(*p) : strlen(*p)) + 1;
    else
    {
     strcat(p_arg, "\"");
     if (quoteargs)
      q_strcat (p_arg, *p);
     else
      strcat (p_arg, *p);
     strcat(p_arg, "\"");
    }
   else
    // If we`re running a EMX program, we should backquote all '"'s in argv[i]
    if (i == 0)
     j += (quoteargs ? q_strlen(*p) : strlen(*p));
    else
     if (quoteargs)
      q_strcat(p_arg, *p);
     else
      strcat(p_arg, *p);
  }
  if (i == 0)
  {
   p_arg = alloca(j);
   if (prefix_len)
   {
    memcpy(p_arg, prefix, prefix_len);
    p_arg[prefix_len] = ' ';
    p_arg[prefix_len + 1] = 0;
   } else
    *p_arg = 0;
  }
 }
 p_arg[strlen(p_arg) + 1] = 0;

#if 0
 printf("quoteargs = %d\n", quoteargs);
 printf("exec{%s}\n", execname);
 printf("args{%s}\n", p_arg);
#endif

 for (arg1 = p_arg; !whitespace(*arg1) && *arg1; arg1++) ;
 *arg1++ = 0;

 /* Prepare environment */
 j = 1;
 for (p = (const char * const *)envp; *p != NULL; ++p)
  j += strlen (*p) + 1;
 p_env = tmp = alloca(j);
 for (p = (const char * const *)envp; *p != NULL; ++p)
 {
  i = strlen (*p);
  memcpy(tmp, *p, i+1);
  tmp += i+1;
 }
 *tmp = 0;

 if (runflags & spawn_SESSION) /* Use DosStartSession */
 {
  HQUEUE hq;

  /* Termination queue management */
  sprintf((char *)&queue, "\\QUEUES\\bash%d", getpid());
  if (DosCreateQueue(&hq, QUE_FIFO | QUE_CONVERT_ADDRESS, (PSZ)queue) == NO_ERROR)
  {
   STARTDATA sd;
   ULONG sid,pid;
   APIRET r;

   memset(&sd, 0, sizeof(sd));
   sd.Length = sizeof(sd);
   sd.Related = (runflags & spawn_WAIT ? SSF_RELATED_CHILD : SSF_RELATED_INDEPENDENT);
   sd.FgBg = SSF_FGBG_FORE;
   sd.PgmName = execname;
   sd.PgmInputs = arg1;
   sd.TermQ = (PBYTE)&queue;
   sd.Environment = p_env;
   sd.InheritOpt = SSF_INHERTOPT_PARENT;
   sd.SessionType = (runflags & spawn_TYPE) >> 16;
   sd.ObjectBuffer = (PSZ)&errbuf;
   sd.ObjectBuffLen = sizeof(errbuf);
#if 0
   sd.PgmControl = SSF_CONTROL_NOAUTOCLOSE;
#endif

   r = DosStartSession(&sd, &sid, &pid);

   if (r == NO_ERROR || r == ERROR_SMG_START_IN_BACKGROUND)
   {
    if (runflags & spawn_WAIT)
    {
     REQUESTDATA rd;
     ULONG Length;
     PUSHORT Info = NULL;
     BYTE Priority;
     ULONG oldVis, oldJump;
     SWP oldpos;

     if (hswValid)
     {
      /* Exclude session from task list */
      oldVis = swc.uchVisibility; swc.uchVisibility = SWL_INVISIBLE;
      oldJump = swc.fbJump; swc.fbJump = SWL_NOTJUMPABLE;
      WinChangeSwitchEntry(vioHSW, &swc);

      /* Minimize session */
      WinQueryWindowPos(swc.hwnd, &oldpos);
      if ((oldpos.fl & SWP_MINIMIZE) == 0)
       WinPostMsg(swc.hwnd, WM_SYSCOMMAND, (MPARAM)SC_MINIMIZE, MPFROM2SHORT(CMDSRC_MENU, FALSE));
     }

     DosReadQueue(hq, &rd, &Length, (PPVOID)&Info, 0,
                  DCWW_WAIT, &Priority, 0);

     if (hswValid)
     {
      /* Restore jumpable & visible status */
      swc.uchVisibility = oldVis;
      swc.fbJump = oldJump;
      WinChangeSwitchEntry(vioHSW, &swc);

      /* Restore session */
      if ((oldpos.fl & SWP_MINIMIZE) == 0)
      {
       WinPostMsg(swc.hwnd, WM_SYSCOMMAND, (MPARAM)SC_RESTORE, MPFROM2SHORT(CMDSRC_MENU, FALSE));
       WinSetWindowPos(swc.hwnd, oldpos.hwndInsertBehind, oldpos.x, oldpos.y,
                       oldpos.cx, oldpos.cy, oldpos.fl);
      }
     }

     retcode = Info[1];
    } else
     retcode = pid;
   } else
    internal_error("Execution failed because of module \"%s\"", (char *)&errbuf);
   DosCloseQueue(hq);
  } else
   internal_error("Cannot create a queue between processes");
 } else
 {
  RESULTCODES rc;

  if (DosExecPgm((PCHAR)&errbuf, sizeof(errbuf),
      ((runflags & (spawn_DETACH | spawn_WAIT)) ? EXEC_SYNC : EXEC_ASYNC) |
      (runflags & spawn_DETACH ? EXEC_BACKGROUND : 0),
      p_arg, p_env, &rc, execname) == NO_ERROR)
  {
   if (runflags & spawn_WAIT)
    retcode = rc.codeResult;
   else
    retcode = 0;
   if (runflags & spawn_DETACH)
   {
    /* Put child PID into parent`s PID chain */
    HMTX sem = pShMem->inuse;
    if (DosOpenMutexSem(NULL, &sem) == NO_ERROR)
    {
     if (DosRequestMutexSem(sem, SEM_INDEFINITE_WAIT) == NO_ERROR)
     {
      pShMem->pidring[pShMem->ringhead].read = 0;
      pShMem->pidring[pShMem->ringhead].ppid = getpid();
      pShMem->pidring[pShMem->ringhead].cpid = rc.codeTerminate;/* child PID */
      pShMem->ringhead++;
      DosReleaseMutexSem(sem);
     }
     DosCloseMutexSem(sem);
    }
   }
  }
  else
   internal_error("Execution failed because of module \"%s\"", (char *)&errbuf);
 }

 if (runflags & spawn_WAIT)
  to_first_column();

 if (freeexec) free(execname);

 if (/*(runflags & spawn_OVERLAY) && */(retcode >= 0))
  exit(retcode);

 return retcode;
}

/* Set to 1 to execve() in a detached process */
int execve_async = 0;

int execve(const char *name, char * const argv[], char * const envp[])
{
 return spawnve(execve_async ? P_DETACH : P_OVERLAY, name, argv, envp);
}

char *emx_unixify(char *path)
{
 char *p;

 for (p = path; *p; p++)
  if (*p == '\\') *p = '/';

 if ((path[1] == ':') && (path[0]))
  path[0] = tolower(path[0]);
 return path;
}

char *emx_deunixify(char *path)
{
 char *p;

 for (p = path; *p; p++)
  if (*p == '/') *p = '\\';
 return path;
}

int chdir(__const__ char *dir)
{
 return _chdir2(dir);
}

char *getcwd(char *buffer, size_t size)
{
 char *res = _getcwd2(buffer, size);

 if (res && res[0] && res[1] == ':')
  res[0] = tolower(res[0]);
 return res;
}

char *tempdir(void)
{
 char *tmpdir = get_string_value ("TMP");
 if (!tmpdir) tmpdir = get_string_value ("TEMP");
 if (!tmpdir) tmpdir = "/tmp";
 return tmpdir;
}

int emx_tempopen(char *name, int oflag, int mode)
{
 char *tmpdir = tempdir();
 int try, fd;

 for (try = 1000; try; try--)
 {
  sprintf(name, "%s/%c%d-sh.%d", tmpdir, (oflag & O_NODELETETEMP ? '#' : 't'),
          getpid(), rand() % 1000);
  fd = open(name, (oflag | O_EXCL) & ~O_NODELETETEMP, mode);
  if ((fd != -1) || (errno != EEXIST))
  {
   emx_unixify(name);
   return fd;
  }
 }
 errno = EMFILE;
 return -1;
}

void removetmp(void)
{
 char fullname[MAXNAMLEN + 1];
 char *tmpdir = tempdir();
 DIR *dh = opendir(tmpdir);
 struct dirent *de;
 int fnlen;

 if (dh)
 {
  strcpy((char *)fullname, tmpdir);
  strcat((char *)fullname, "/");
  fnlen = strlen((char *)&fullname);

  while (de = readdir(dh))
  {
   char *s = (char *)&de->d_name;
   if (*s != 't') continue; s++;
   while ((*s >= '0') && (*s <= '9')) s++;
   if (memcmp(s, "-sh.", 4) != 0) continue; s += 4;
   while ((*s >= '0') && (*s <= '9')) s++;
   if (*s != 0) continue;
   strcat((char *)&fullname[fnlen], (char *)&de->d_name);
   unlink((char *)&fullname);
  }
  closedir(dh);
 }
}

static void emx_exit()
{
 removetmp();
 DosCloseMutexSem(ShMem->inuse);
 DosFreeMem((PVOID)ShMem);
}

static void init_shmem()
{
 DosAllocSharedMem((PPVOID)&ShMem, NULL, sizeof(*ShMem),
  OBJ_GETTABLE | PAG_COMMIT | PAG_READ | PAG_WRITE);
 DosCreateMutexSem(NULL, &ShMem->inuse, DC_SEM_SHARED, FALSE);
}

void emx_initialize(void)
{
 init_shmem();
 atexit(emx_exit);
 removetmp();
}

int emx_realpid(int pid)
{
 char *shname = alloca(MAXNAMLEN);
 int i, tries, found = 0;

 if (pid <= 0)
  return (pid);

 for (tries = 0; (!found) && (tries < 10); tries++)
 {
  if (DosRequestMutexSem(ShMem->inuse, SEM_INDEFINITE_WAIT) == NO_ERROR)
  {
   for (i = ShMem->ringtail; i != ShMem->ringhead; i = (i + 1) & (PID_RING_LEN - 1))
    if (ShMem->pidring[i].ppid == pid)
    {
     pid = ShMem->pidring[i].cpid;
     ShMem->pidring[i].read = 1;
     found = 1;
     break;
    }

   for (i = ShMem->ringtail; i != ShMem->ringhead; ShMem->ringtail = i = (i + 1) & (PID_RING_LEN - 1))
    if (!ShMem->pidring[i].read)
     break;
   DosReleaseMutexSem(ShMem->inuse);
  }
  if (!found)
   DosSleep(32);
 }
 return (pid);
}

/* A slightly modified version of fork() */
extern void _rmutex_fork (void);

int fork (void)
{
 int pid;

 pid = __fork ();
 if (pid == 0)
 {
  pShMem = ShMem;
  init_shmem();
  if (DosGetSharedMem((PVOID)pShMem, PAG_READ | PAG_WRITE) != NO_ERROR)
   pShMem = NULL;
#if defined (__MT__)
  _rmutex_fork ();
#endif
 }
 return pid;
}

/*-------------------------------------- Unix-like dynamic linking emulation -*/

typedef struct _track_rec
{
 char *name;
 HMODULE handle;
 void *id;
 struct _track_rec *next;
} tDLLchain, *DLLchain;

static DLLchain dlload = NULL;		/* A simple chained list of DLL names */
static char dlerr[256];				    /* last error text string */
static void *last_id;

static DLLchain find_id(void *id)
{
 DLLchain tmp;

 for (tmp = dlload; tmp; tmp = tmp->next)
  if (id == tmp->id)
   return (tmp);

 return (NULL);
}

/* load a dynamic-link library and return handle */
void *dlopen (char *filename, int flags)
{
 HMODULE hm;
 DLLchain tmp;
 char err[256];
 char *errtxt;
 int rc, set_chain = 0;

 for (tmp = dlload; tmp; tmp = tmp->next)
  if (strnicmp(tmp->name, filename, 999) == 0)
   break;

 if (!tmp)
 {
  tmp = (DLLchain)xmalloc(sizeof(tDLLchain));
  if (!tmp) goto nomem;
  tmp->name = savestring(filename);
  tmp->next = dlload;
  set_chain = 1;
 }

 switch (rc = DosLoadModule((PSZ)&err, sizeof(err), filename, &hm))
 {
  case NO_ERROR:
   tmp->handle = hm;
   if (set_chain)
   {
    do { last_id++; } while ((last_id == 0) || (find_id(last_id)));
    tmp->id = last_id;
    dlload = tmp;
   }
   return (tmp->id);
  case ERROR_FILE_NOT_FOUND:
  case ERROR_PATH_NOT_FOUND:
   errtxt = "module `%s' not found";
   break;
  case ERROR_TOO_MANY_OPEN_FILES:
  case ERROR_NOT_ENOUGH_MEMORY:
  case ERROR_SHARING_BUFFER_EXCEEDED:
nomem:
   errtxt = "out of system resources";
   break;
  case ERROR_ACCESS_DENIED:
   errtxt = "access denied";
   break;
  case ERROR_BAD_FORMAT:
  case ERROR_INVALID_SEGMENT_NUMBER:
  case ERROR_INVALID_ORDINAL:
  case ERROR_INVALID_MODULETYPE:
  case ERROR_INVALID_EXE_SIGNATURE:
  case ERROR_EXE_MARKED_INVALID:
  case ERROR_ITERATED_DATA_EXCEEDS_64K:
  case ERROR_INVALID_MINALLOCSIZE:
  case ERROR_INVALID_SEGDPL:
  case ERROR_AUTODATASEG_EXCEEDS_64K:
  case ERROR_RELOCSRC_CHAIN_EXCEEDS_SEGLIMIT:
   errtxt = "invalid module format";
   break;
  case ERROR_SHARING_VIOLATION:
  case ERROR_LOCK_VIOLATION:
   errtxt = "sharing violation";
   break;
  case ERROR_INIT_ROUTINE_FAILED:
   errtxt = "module initialization failed";
   break;
  default:
   errtxt = "cause `%s', error code = %d";
   break;
 }
 snprintf((char *)&dlerr, sizeof(dlerr), errtxt, &err, rc);
 if (tmp) { if (tmp->name) free(tmp->name); free(tmp); }
 return (0);
}

/* return a pointer to the `symbol' in DLL */
void *dlsym(void *handle, char *symbol)
{
 int rc;
 PFN addr;
 char *errtxt;
 DLLchain tmp = find_id(handle);

 if (!tmp) goto inv_handle;

 switch (rc = DosQueryProcAddr(tmp->handle, 0, symbol, &addr))
 {
  case NO_ERROR:
   return ((void *)addr);
  case ERROR_INVALID_HANDLE:
inv_handle:
   errtxt = "invalid module handle";
   break;
  case ERROR_PROC_NOT_FOUND:
  case ERROR_INVALID_NAME:
   errtxt = "no symbol `%s' in module";
   break;
  default:
   errtxt = "symbol `%s', error code = %d";
   break;
 }
 snprintf((char *)&dlerr, sizeof(dlerr), errtxt, symbol, rc);
 return (NULL);
}

/* free dynamicaly-linked library */
int dlclose(void *handle)
{
 int rc;
 DLLchain tmp = find_id(handle);

 if (!tmp) goto inv_handle;

 switch (rc = DosFreeModule(tmp->handle))
 {
  case NO_ERROR:
   free(tmp->name);
   dlload = tmp->next;
   free(tmp);
   return (0);
  case ERROR_INVALID_HANDLE:
inv_handle:
   strcpy((char *)&dlerr, "invalid module handle");
   return (-1);
  case ERROR_INVALID_ACCESS:
   strcpy((char *)&dlerr, "access denied");
   return (-1);
  default:
   return (-1);
 }
}

/* return a string describing last occured dl error */
char *dlerror()
{
 return ((char *)&dlerr);
}

void dlreload()
{
 int i;
 DLLchain tmp;

 for (tmp = dlload; tmp; tmp = tmp->next)
  dlopen(tmp->name, 0);
 return;
}

/*---------------------------------------------- Directory names replacement -*/

/*
 * These routines are replacement for EMX`s own open(), fopen(), fdopen()
 * taking care of converting such unix-specific things like '/tmp/',
 * '/dev/null' etc into their os/2 equivalents:
 *
 * starting part of filename | will be converted to
 * --------------------------+----------------------------------
 * /dev/null                 | /dev/nul
 * /tmp/                     | {$TMP:-/tmp}
 * /etc/                     | {$ETC:-/etc}
 *
 */

static struct
{
 char *path;
 int len;
 int mode;
 char *repl;
} conv_path[] =
{
 {"/dev/null", 10, 0, "/dev/nul"},
 {"/tmp/", 5, 1, "TMP"},
 {"/etc/", 5, 2, "ETC"},
 {NULL, 0, 0, NULL}
};

static char *convert_filename(const char *name, char *dest)
{
 char *s;
 int i;

 if (strlen (name) > MAXNAMLEN - 3)
 {
  errno = ENAMETOOLONG;
  return NULL;
 }

 for (i = 0; conv_path[i].path; i++)
 {
  if (strncmp(name, conv_path[i].path, conv_path[i].len) == 0)
   switch (conv_path[i].mode)
   {
    case 0:
     strcpy(dest, conv_path[i].repl);
     return (dest);
    case 1:
     s = tempdir();
     goto copys;
    case 2:
     s = get_string_value (conv_path[i].repl);
copys:
     if (!s) break;
     strcpy(dest, s);
     s = &dest[strlen(dest) - 1];
     if ((*s != '/') && (*s != '\\'))
      strcat(dest, "/");
     strcat(dest, (char *)&name[conv_path[i].len]);
     return (dest);
   }
 }
 strcpy(dest, name);
 return (dest);
}

int open (const char *name, int oflag, ...)
{
  va_list va;
  int h;
  char *new_name = convert_filename(name, alloca(MAXNAMLEN + 1));

  if (!new_name)
   return (-1);

  va_start (va, oflag);
  h = _vsopen (new_name, oflag, SH_DENYNO, va);
  va_end (va);
  return h;
}

FILE *fopen (const char *fname, const char *mode)
{
  char *new_fname = convert_filename(fname, alloca(MAXNAMLEN + 1));
  FILE *f;

  if (!new_fname)
   return (NULL);

  f = _newstream ();
  if (f != NULL)
    f = _fopen (f, new_fname, mode, SH_DENYNO, 1);
  return f;
}
