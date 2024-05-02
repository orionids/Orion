case "$OSTYPE" in
	cygwin)
	USER=$USERNAME
	ifconfig()
	{
		ipconfig "$@"
	}
	term()
	{
		mintty -t $MATE_TERM_TITLE -e "$@" &
	}
	MATE_TERM=term
	;;
	darwin*)
	term()
	{
		osascript -e '
		on run argv

		set arglist to {}
		repeat with i from 1 to length of argv
			set end of arglist to " \"" & item i of argv & "\""
		end repeat

		tell app "Terminal"
			do script "echo -n -e \"\\033]0;TITLE\\007\";" & arglist & ";exit 0"
		end tell
		end run' "$@"
	}
	MATE_TERM=term
	;;
	*)
	MATE_TERM="lxterminal -t $MATE_TERM_TITLE -e"
esac

srcname()
{
	if [ -z "$__SRC" ]; then
		if [ -z "$BASH" ] && [ -n "$ZSH_VERSION" ]; then
			export __SRC='echo ${(%):-%x}'
		else
			export __SRC='echo ${BASH_SOURCE[0]}'
		fi
	fi
	eval $__SRC
}

# splitstr arrayName string splitter
splitstr()
{
	eval "$1=(${2//$3/ })"
}

if [ -z "$BASH" ] && [ -n "$ZSH_VERSION" ]; then #{
	elem0=1

	indirect()
	{
		echo - ${(P)1}
	}

	argument()
	{
		local i=$1
		shift 1
		echo - $@[i]
	}

	xalias()
	{
		setopt aliases
	}
else #}{
	elem0=0

	indirect()
	{
		echo ${!1}
	}

	argument()
	{
		local i=$1
		shift 1
		echo ${!i}
	}

	xalias()
	{
		shopt -s expand_aliases
	}
fi #}
