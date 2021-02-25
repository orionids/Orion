if exists("b:current_syntax")
	finish
endif

if version < 600
  source <sfile>:p:h/javascript.vim
  source <sfile>:p:h/preproc.vim
else
  runtime! syntax/javascript.vim
  runtime! syntax/preproc.vim
endif
