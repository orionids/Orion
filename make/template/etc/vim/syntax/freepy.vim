if exists("b:current_syntax")
	finish
endif

if version < 600
  source <sfile>:p:h/python.vim
else
  runtime! syntax/python.vim
endif
