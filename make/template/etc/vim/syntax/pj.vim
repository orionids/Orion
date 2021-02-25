if exists("b:current_syntax")
	finish
endif

if version < 600
  source <sfile>:p:h/javacc.vim
  source <sfile>:p:h/preproc.vim
else
  runtime! syntax/javacc.vim
  runtime! syntax/preproc.vim
endif

syn clear  javaError2

" these parenthesis rules prevent to correctly render preprocessor inside parenthesis
syn clear  javaParenT
syn clear  javaParenT1
syn clear  javaParenT2
syn clear  javaParenError

