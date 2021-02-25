syn region	ppPreCondit	start="^\s*\(%:\|#\)\s*\(if\|ifdef\|ifndef\|elif\)\>" skip="\\$" end="$" keepend contains=ppComment,ppCommentL,ppString,ppCharacter,ppParen,ppParenError,ppNumbers,ppCommentError,ppSpaceError
if !exists("c_no_if0")
  syn cluster	ppOutInGroup	contains=ppInIf,ppInElse,ppInElse2,ppOutIf,ppOutIf2,ppOutElse,ppInSkip,ppOutSkip
  syn region	ppOutWrapper	start="^\s*\(%:\|#\)\s*if\s\+0\+\s*\($\|//\|/\*\|&\)" end=".\@=\|$" contains=ppOutIf,ppOutElse,@NoSpell fold
  syn region	ppOutIf	contained start="0\+" matchgroup=ppOutWrapper end="^\s*\(%:\|#\)\s*endif\>" contains=ppOutIf2,ppOutElse
  if !exists("c_no_if0_fold")
    syn region	ppOutIf2	contained matchgroup=ppOutWrapper start="0\+" end="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0\+\s*\($\|//\|/\*\|&\)\)\@!\|endif\>\)"me=s-1 contains=ppSpaceError,ppOutSkip,@Spell fold
  else
    syn region	ppOutIf2	contained matchgroup=ppOutWrapper start="0\+" end="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0\+\s*\($\|//\|/\*\|&\)\)\@!\|endif\>\)"me=s-1 contains=ppSpaceError,ppOutSkip,@Spell
  endif
  syn region	ppOutElse	contained matchgroup=ppOutWrapper start="^\s*\(%:\|#\)\s*\(else\|elif\)" end="^\s*\(%:\|#\)\s*endif\>"me=s-1 contains=TOP,ppPreCondit
  syn region	ppInWrapper	start="^\s*\(%:\|#\)\s*if\s\+0*[1-9]\d*\s*\($\|//\|/\*\||\)" end=".\@=\|$" contains=ppInIf,ppInElse fold
  syn region	ppInIf	contained matchgroup=ppInWrapper start="\d\+" end="^\s*\(%:\|#\)\s*endif\>" contains=TOP,ppPreCondit
  if !exists("c_no_if0_fold")
    syn region	ppInElse	contained start="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0*[1-9]\d*\s*\($\|//\|/\*\||\)\)\@!\)" end=".\@=\|$" containedin=ppInIf contains=ppInElse2 fold
  else
    syn region	ppInElse	contained start="^\s*\(%:\|#\)\s*\(else\>\|elif\s\+\(0*[1-9]\d*\s*\($\|//\|/\*\||\)\)\@!\)" end=".\@=\|$" containedin=ppInIf contains=ppInElse2
  endif
  syn region	ppInElse2	contained matchgroup=ppInWrapper start="^\s*\(%:\|#\)\s*\(else\|elif\)\([^/]\|/[^/*]\)*" end="^\s*\(%:\|#\)\s*endif\>"me=s-1 contains=ppSpaceError,ppOutSkip,@Spell
  syn region	ppOutSkip	contained start="^\s*\(%:\|#\)\s*\(if\>\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" contains=ppSpaceError,ppOutSkip
  syn region	ppInSkip	contained matchgroup=ppInWrapper start="^\s*\(%:\|#\)\s*\(if\s\+\(\d\+\s*\($\|//\|/\*\||\|&\)\)\@!\|ifdef\>\|ifndef\>\)" skip="\\$" end="^\s*\(%:\|#\)\s*endif\>" containedin=ppOutElse,ppInIf,ppInSkip contains=TOP,ppPreProc
endif
syn region	ppIncluded	display contained start=+"+ skip=+\\\\\|\\"+ end=+"+
syn match	ppIncluded	display contained "<[^>]*>"
syn match	ppInclude	display "^\s*\(%:\|#\)\s*include\>\s*["<]" contains=ppIncluded

syn region	ppDefine		start="^\s*\(%:\|#\)\s*\(define\|undef\)\>" skip="\\$" end="$" keepend
" below is excluded from ppDefine inadequate for java syntax
" contains=ALLBUT,@ppPreProppGroup,@Spell
syn region	ppPreProc	start="^\s*\(%:\|#\)\s*\(pragma\>\|line\>\|warning\>\|warn\>\|error\>\)" skip="\\$" end="$" keepend contains=ALLBUT,@ppPreProppGroup,@Spell
syn cluster	ppPreProppGroup	contains=ppPreCondit,ppIncluded,ppInclude,ppDefine,ppErrInParen,ppErrInBracket,ppUserLabel,ppSpecial,ppOctalZero,ppOutWrapper,ppInWrapper,@ppOutInGroup,ppFormat,ppNumber,ppFloat,ppOctal,ppOctalError,ppNumbersCom,ppString,ppCommentSkip,ppCommentString,ppComment2String,@ppCommentGroup,ppCommentStartError,ppParen,ppBracket,ppMulti,ppBadBlock
hi def link ppDefine		Macro
hi def link ppPreProc		PreProc
hi def link ppInclude		PreProc
hi def link ppIncluded		String
hi def link ppOutSkip		ppOutIf2
hi def link ppInElse2		ppOutIf2
hi def link ppOutIf2		ppOut2  " Old syntax group for #if 0 body
hi def link ppOut2		ppOut  " Old syntax group for #if of #if 0
hi def link ppOut		Comment
