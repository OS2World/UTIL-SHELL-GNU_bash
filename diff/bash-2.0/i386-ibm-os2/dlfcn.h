/* dlfcn.h -- header file for Unix-like dynamic-link functions */

/* Copyright (C) 1987,1991 Free Software Foundation, Inc.               */
/*                                                                      */
/* This file is part of GNU Bash, the Bourne Again SHell.               */
/*                                                                      */
/* Bash is free software; you can redistribute it and/or modify it      */
/* under the terms of the GNU General Public License as published by    */
/* the Free Software Foundation; either version 1, or (at your option)  */
/* any later version.                                                   */
/*                                                                      */
/* Bash is distributed in the hope that it will be useful, but WITHOUT  */
/* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY   */
/* or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public     */
/* License for more details.                                            */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Bash; see the file COPYING.  If not, write to the Free    */
/* Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.         */

/* load a dynamic-link library and return handle */
void *dlopen (char *filename, int flags);

/* return a pointer to the `symbol' in DLL */
void *dlsym(void *handle, char *symbol);

/* free dynamicaly-linked library */
int dlclose(void *handle);

/* return a string describing last occured dl error */
char *dlerror();

/* reload all loaded dynamic-link libraries -- used after fork() */
void dlreload();
