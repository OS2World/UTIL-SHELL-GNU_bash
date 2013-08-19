/* uname.c (emx+gcc) -- Copyright (c) 1993 by Kai Uwe Rommel */
/*                      Copyright (c) 1993-1995 by Eberhard Mattes */

#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

#define SET(dst,src) _strncpy (name->dst, (src), sizeof (name->dst))
#define CHR(x)       (char)((x)+'0')

int _uname (struct utsname *name)
{
  char *host;
  int dos;
  int ver_hi = _osmajor, ver_lo = _osminor;

  if ((ver_hi == 20) && (ver_lo >= 30))
  {
   ver_hi = ver_lo - (ver_lo % 10);
   ver_lo = (ver_lo % 10) * 10;
  }

  dos = (_osmode == DOS_MODE && _osmajor < 10);
  host = getenv ("HOSTNAME");
  if (host == NULL)
    host = "standalone";

  SET (sysname, (dos ? "MS-DOS" : "OS/2"));
  SET (nodename, host);
  SET (release, "1");
  name->version[0] = (dos ? CHR (ver_hi) : CHR (ver_hi / 10));
  name->version[1] = '.';
  name->version[2] = CHR (ver_lo / 10);
  name->version[3] = CHR (ver_lo % 10);
  name->version[4] = 0;
  SET (machine, "i386");
  return 0;
}
