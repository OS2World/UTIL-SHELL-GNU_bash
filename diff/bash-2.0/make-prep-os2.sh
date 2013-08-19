#!bash

#
# This script file is an add-on to build BASH on and for OS/2
# It will do some minor modifications to makefiles
#


if ! [ -f .os2.ready ] || [ stamp-h -nt .os2.ready ] ; then
 for makefile in Makefile builtins/Makefile lib/readline/Makefile \
		lib/glob/Makefile lib/malloc/Makefile lib/termcap/Makefile \
		lib/tilde/Makefile doc/Makefile; do

  echo "Preparing $makefile ..."
  sed -e "s| = -l\([^t][a-z]*\)| = -llib\1|" -e "s|-ltilde|-llibtilde|" \
      -e "s|^SHELL|#SHELL|" -e "/^VPATH/s|:|;|g" -e "s|psize.aux|psizeaux|" \
      -e "s|^LOCAL_LIBS = .*|LOCAL_LIBS = -lsocket|" $makefile >$tmp$
  if [ $makefile == Makefile ]; then
   echo '$(Program): emx_supp.o' >>$tmp$
   echo 'OBJECTS += emx_supp.o' >>$tmp$
  fi
  mv $tmp$ $makefile

  sed -e "s|psize.aux|psizeaux|g" builtins/psize.sh >$tmp$
  mv $tmp$ builtins/psize.sh

 done;
 touch .os2.ready
fi
