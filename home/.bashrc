#------------------------------------------------------------------------------#
#                        bash/2 interactive startup file                       #
#                                                                              #
# This script file is loaded only when bash does not mimic /bin/sh (when it is #
# run through sh.exe). If BASH_ENV variable points to this script, it is run   #
# even for non-interactive BASH sessions (i.e. when running scripts).          #
#------------------------------------------------------------------------------#

# if this is not an interactive shell -- skip the following
if [ -z "$PS1" ]; then
	return
fi

#--------------------------------------------------[ For interactive shell ]----

# for os/2 terminal display a nifty logo and set a colored prompt
if [ $TERM = "os2" ] && [ $SHLVL = "1" ] ; then
	echo -e "\E[1;31mฤฤอออออออออออออออออออออออออออออออออออออออออฤฤ"
	echo -e "\E[1;33mGNU BASH for ${OSTYPE} version "${BASH_VERSION}
	echo -e "\E[1;32m          Running under ${OSTYPE} v"`uname -v`
	echo -e "\E[1;31mฤฤอออออออออออออออออออออออออออออออออออออออออฤฤ"

# Since BASH cannot assume something about terminals, we have to point him
# whenever characters in prompt are "visible" or "invisible" by using
# \[ (start invisible) and \] (end invisible) characters
	export PS1="\[\e[1;37m\][\[\e[33m\]${SHLVL}\[\e[37m\]|\[\e[32m\]\h\[\e[37m\]|\[\e[36m\]\w\[\e[37m\]]\[\e[0;36m\]"
	export PS2="\[\e[1;37m\]>\[\e[0;36m\]"
fi

# Limit history file to 200 lines
export HISTFILESIZE=200

# bogus
alias ls='ls -CF --color=auto'
alias ll='ls -l --color=auto'
alias dir='ls -ba --color=auto'
alias dot='ls .[a-zA-Z0-9_]* --color=auto'
alias /='cd /'
alias '~'='cd ~'
alias ..='cd ..'

alias m='more'
alias r='fc -s'

#------------------------------------------------------------- OS/2-specific ---
alias cls='clear'
alias os2help='help.cmd'
alias start='$COMSPEC /c start /c'

# You should use this only for simple commands
detach()
{
 { $@ & } 2>/dev/null
 echo -e "The Process Identification Number is $!."
}

alias a:='cd a:'
alias b:='cd b:'
alias c:='cd c:'
alias d:='cd d:'
alias e:='cd e:'
alias f:='cd f:'
alias g:='cd g:'
alias h:='cd h:'
alias i:='cd i:'
alias j:='cd j:'
alias k:='cd k:'
alias l:='cd l:'
alias m:='cd m:'
alias n:='cd n:'
alias o:='cd o:'
alias p:='cd p:'
alias q:='cd q:'
alias r:='cd r:'
alias s:='cd s:'
alias t:='cd t:'
alias u:='cd u:'
alias v:='cd v:'
alias w:='cd w:'
alias x:='cd x:'
alias y:='cd y:'
alias z:='cd z:'

# A semicolon-separated list of shell patterns which
# matches lines that are not to be added to the history
HISTIGNORE="&;[ ]*;?"

# 4os/2-like (and even better :-)
eset()
{
	tmpfile="${TMP-/tmp}/eset"$$".sh"
	for tmp in $@; do
	 echo export ${tmp}=\'${!tmp}\' >>${tmpfile}
	done
	${EDITOR:epm} ${tmpfile}
	. ${tmpfile}
	rm -f ${tmpfile}
}

# Show tasks matching given substring
gogrep()
{
	go | grep -e "P-ID PPID" -e "---- ----" -e "$@" -i
}

# Set terminal type
term()
{
	grep "^[^:]*$1[:|]" ${TERMCAP} >/dev/null
	if [ $? == 0 ] ; then
	        TERM=$1
		export TERM
	else
		echo "Terminal type \""$1"\" is missing from TERMCAP file ${TERMCAP}"
	fi
}

rot13()
{
	if [ $# = 0 ] ; then
		tr "[a-m][n-z][A-M][N-Z]" "[n-z][a-m][N-Z][A-M]"
	else
		tr "[a-m][n-z][A-M][N-Z]" "[n-z][a-m][N-Z][A-M]" < $1
	fi
}

function setenv()
{
	if [ $# -ne 2 ] ; then
		echo "setenv: Too few arguments"
	else
		export $1="$2"
	fi
}
