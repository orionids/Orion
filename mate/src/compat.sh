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

indirect()
{
	if [ -z "$BASH" ] && [ -n "$ZSH_VERSION" ]; then
		echo ${(P)1}
	else
		echo ${!1}
	fi
}

