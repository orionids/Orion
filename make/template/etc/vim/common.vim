let winos=0

if has("unix")
	let _='/'
	let __=':'
else
if has("win16") || has("win32") || has("win64")
	let winos=1
	let _='\'
	let __=';'
	let mate_small_font="Lucida_Console:w6:h11:cANSI"
	let mate_medium_font="Lucida_Console:w7.5:h13:cANSI"
	let mate_large_font="Lucida_Console:w9.4:h16:cANSI"
	"let mate_large_font="Lucida_Console:w9.5:h16:cANSI"
else
	let winos=0
endif
endif

" This function set the origin of gvim window to the top-left of the
" desktop window : called initially by vimrc and manually by ORG command
function! MATE_Org()
	if has("gui_running")
		winpos 0 0
		if g:winos
			set lines=35 columns=80
		endif
	endif
endfunction
