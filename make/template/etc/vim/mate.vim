" vim: ts=4 sw=4 :

if !exists("vimenter")
	let vimenter=0
endif
if !exists("g:winos")
	source $MK_BASE\/devel/make/template/etc/vim/common.vim
endif

if $MK_RELATIVE == ""
	let $MK_RELATIVE = "devel"
endif

func! MATE_SessionFile()
	if exists("g:mate_session")
		if g:mate_session=="default"
			return $MK_DEVEL."/etc/current.vim"
		endif
		return g:mate_session
	endif
	return ""
endfunc

func! MATE_Start()
	if argc()==0
		let sessfile = MATE_SessionFile()
		if filereadable(sessfile)
			exe "silent source ".sessfile
		endif
		let g:vimenter=2
	else
		let g:vimenter=1
	endif
endfunc

func! MATE_End()
	if g:vimenter==2
		let sessfile = MATE_SessionFile()
		if sessfile != ""
			exe "silent mksession! ".sessfile
"		exe "redir >> ".sessfile
"		echo "COMPILER ".$MK_COMPILER." ".$MK_OS
"		if $MK_MAKEFILE != ""
"			echo "MAKE ".$MK_MAKEFILE." ".$MK_MODE
"		endif
"		if $MK_PROJECT != ""
"			echo "PROJECT '".$MK_PROJECT
"		endif
"		redir END
		endif
	endif
endfunc

au VimEnter * call MATE_Start()
au VimLeave * call MATE_End()


if !exists("g:oldefm")
	let g:oldefm = &errorformat
endif
let &errorformat=g:oldefm


" This function must be called in the proper place to load the config file and
" set the output path
function! MATE_InitializeOutput()
	let g:resolution = "HIGH"
	if filereadable(g:MK_DEVEL."/etc/config.vim")
		exe "source ".g:MK_DEVEL."/etc/config.vim"
	endif

	if !exists("g:output_base_path")
		let g:output_base_path=$MK_RELATIVE
	endif

	call SetOutput()
endfunction


" inspect that "win32unix" and "win95" should be evaluated
if has("win16") || has("win32") || has("win64")
	let winos=1
	" ftp delivered with windows cd can be used in vim version 7
	if version>=700
		if has("win95")
			let netrw_ftp_cmd=$windir.'\ftp'
		else
			let netrw_ftp_cmd=$windir.'\system32\ftp'
		endif
	endif
else
	let winos=0
endif

function! MATE_Command(cmd,nparam,... )
	if a:0<a:nparam
		echo "At least ".a:nparam." parameter(s) are needed."
		return
	endif


	let cmdindex = stridx(a:cmd,",")
	if cmdindex>0
		let cmdcnt = strpart(a:cmd,-cmdindex,cmdindex*2)
	else
		let cmdcnt = 1
	endif

	let i=1
	let param=""
	while i<=a:nparam
		let param=param." ".a:{i}
		let i = i+1
	endwhile

	let cmdstart=1+a:nparam
	let i=cmdstart
	let filelist=""
	if !exists("a:{i}")
		let i=i-1
	else
		let curparam=a:{i}
		while strpart(curparam,0,1) == "@"
			let param=param." ".strpart(curparam,1)
			let cmdstart=cmdstart+1
			if exists("a:{i+1}")
				let i = i+1
			endif
		endwhile
	endif
	while i<=a:0
		if i<cmdstart || a:{i}=="."
			let curparam=expand("%:p")
		else
			let curparam=a:{i}
		endif
		let filelist = filelist." ".MATE_RelativeToDevelPath(GetCygwinEnv(curparam))
		let i=i+1
	endwhile
	let newpath=$MK_DEVEL

	echo filelist
	if filelist!=""
		let curpath=getcwd()
		exe "cd" newpath
		let filelist = param." ".filelist
		let makeprg=&makeprg
		if cmdindex<0
			let &makeprg=a:cmd." ".filelist
		else
			redir! > $MK_DEVEL/etc/matecmd
			while 1
				let i = cmdindex
				let i=i+1
				let cmdindex=match(a:cmd,",",i)
				if cmdindex<0
					echo strpart(a:cmd,i).filelist
				else
					echo strpart(a:cmd,i,cmdindex-i).filelist
				endif
				if cmdindex<0
					break
				endif
			endwhile
			redir END
			let &makeprg="bash ".$MK_DEVEL."/etc/matecmd"
		endif

		let oldefm=&errorformat
		let &errorformat="*"
		make
		let &errorformat=oldefm
		let &makeprg=makeprg
		exe "cd" curpath
	else
		echo "File was not opened."
	endif
endfunction



function! ResetNetRW()
	let g:netrw_server=""
	let g:netrw_sync=""
endfunction
call ResetNetRW()
let $TOOLPATH=''

function! DefinePath(path)
	if $TOOLPATH==''
		let $PATH=a:path
	elseif $MK_COMPILER=='gcc'
		let $PATH=a:path.g:__.$TOOLPATH
	else
		" there is some command link 'link' in cygwin which has the same name with vc
		" so TOOLPATH need to have priority for non-gcc compilers
		let $PATH=$TOOLPATH.g:__.a:path
	endif
endfunction

function! MATE_GetOutputPath(root,os,suffix)
	let path = a:os.g:_.$MK_CONFIG.g:_.$MK_DEVICE
	if a:root != ""
		let path = a:root.g:_.path
	endif
	if a:suffix != ""
		let path = path.g:_.a:suffix
	endif
	return path
endfunction


"function! MATE_ResolveBlank(s)
"	return "\\\"".substitute(a:s," ","\\\\ ", "g")."\\\""
"endfunction

function! MATE_RelativePath(basepath,filename)
	return substitute(a:filename,a:basepath."/","","")
endfunction

" 'filename' must follow the cygwin path rules
function! MATE_RelativeToDevelPath(filename)
	let ic=&ic
	set ic
	let relpath=MATE_RelativePath(GetCygwinEnv($MK_DEVEL),a:filename)
	if !ic
		set noic
	endif
	return relpath
endfunction

function! MATE_SetMakePrg()
	" To resolve blanks correctly, let &makeprg is used instead of exe "set makeprg"
	if exists("g:mate_makeprg_postfix")
"		exe "set makeprg=".g:mate_makeprg.MATE_ResolveBlank(g:mate_makeprg_postfix)
		let &makeprg=g:mate_makeprg.g:mate_makeprg_postfix
	else
"		exe "set makeprg=".g:mate_makeprg
		let &makeprg=g:mate_makeprg
	endif
endfunction

if exists("g:freepy")
	let g:freepy = substitute(g:freepy,"/",g:_,"g")
endif
function! Autopep8Formatted()
	let filename=expand("%")
	if &filetype == "freepy"
		return 0
	endif
	if exists("g:freepy")
		let start = 0
		let end = 0
		while end >=0
			let end = stridx(g:freepy,",",start)
			if end>=0
				let p = strpart(g:freepy,start,end)
			else
				let p = strpart(g:freepy,start)
			endif
			if stridx(filename,p)>=0
				return 0
			endif
			let start = end + 1
		endwhile
	endif
	return 1
endfunction

function! MATE_AutoPEP8(auto)
	let g:autopep8_disable_show_diff=1
	let g:autopep8_cmd=$MK_TEMPLATE_ROOT.g:_.'etc'.g:_.'cmd'.g:_.'autopep8.sh'
	au! BufWinEnter *.py if Autopep8Formatted() | setlocal sw=4 ts=4 et| endif
	if a:auto
		au! BufWritePre *.py if Autopep8Formatted() | Autopep8 | endif
	else
		au! BufWritePre *.py
	endif
endfunction

function! SetDevel(...)
	if a:0==0
		echo $MK_DEVEL
	else
		call ResetNetRW()
"		let $CVSROOT=''
		if a:1=="default"
			call InitDevel()
		else
" TODO : platform free directory name
			if( isdirectory(a:1) )
				let $MK_DEVEL=a:1
			else
				echo "Invalid directory"
				return
			endif
		endif

		if g:winos && filereadable($MK_DEVEL."/etc/win32/cygwin/gpath.vim" )
			exe "source ".$MK_DEVEL."/etc/win32/cygwin/gpath.vim"
		endif

		if filereadable($MK_DEVEL."/devel.vim")
			exe "source ".$MK_DEVEL."/devel.vim"
		endif

"		if $WINDEV != ""
"			let $WINDEV=""
"		echo $WINDEV
"		endif

		if $MK_CONFIG==''
			let $MK_CONFIG='generic'
		endif

		call SetCompiler( $MK_COMPILER )
		call MATE_SetMakePrg()

		cd $MK_DEVEL
		if g:vimenter
			echo $MK_DEVEL
		endif
	endif
endfunction

function! MATE_makeRedirect(redirected,s)
	if !a:redirected
		redir! > $MK_DEVEL/etc/$MK_OS/$MK_COMPILER.new
	endif
	echo a:s
	return 1
endfunction

function! MATE_ErrorMessage(msg)
	echo "ERROR ! : ".a:msg
endfunction

function! MATE_PrivateMakeFile(devel,name)
	let mkfile=a:devel."/".a:name."/".a:name.".mk"
	if !filereadable(mkfile)
		return ""
	endif
	let $MK_PRIVATE_MAKEFILE='1'
	return mkfile
endfunction

function! MATE_MakeFileName(name)

	if $MK_ALTERED_MAKEFILE == ""
		let mkpath=$MK_DEVEL
		let vendor=""
	else
		let mkpath=$MK_ALTERED_DEVEL
		if $MK_VENDOR_DEVEL != ""
			let vendor="/".$MK_VENDOR_DEVEL
		endif
	endif

	if $MK_PRIVATE_MAKEFILE == ""
		let mkpath=mkpath.vendor."/make"
	elseif $MK_PRIVATE_MAKEFILE == "."
		let mkpath=mkpath."/".$MK_MAKEFILE
	else
		let mkpath=mkpath."/".$MK_MAKEFILE."/".$MK_PRIVATE_MAKEFILE
	endif

	return mkpath."/".$MK_MAKEFILE.".mk"

	return

	" altered devel has priority
	let $MK_PRIVATE_MAKEFILE=''
	let $MK_ALTERED_MAKEFILE='1'
"	if $MK_ALTERED_DEVEL != ""
		let mkpath = $MK_ALTERED_DEVEL
"	else
"		let mkpath = $MK_DEVEL
"	endif
	if $MK_VENDOR_DEVEL != ""
		let mkpath=mkpath."/".$MK_VENDOR_DEVEL
	endif
	let mkfile=mkpath."/make/".a:name.".mk"
	if filereadable(mkfile)
		return mkfile
	endif

	let mkfile=MATE_PrivateMakeFile($MK_ALTERED_DEVEL,a:name)
	if mkfile != ""
		return mkfile
	endif

	let $MK_ALTERED_MAKEFILE=''
	let mkfile= $MK_DEVEL."/make/".a:name.".mk"
	if filereadable(mkfile)
		return mkfile
	endif

	return MATE_PrivateMakeFile($MK_DEVEL,a:name)
endfunction

function! MATE_MakeCurrent(...)

	if a:0<1
		echo "MAKE : [".$MK_MAKEFILE."] [".$MK_MODE."] [altered=".$MK_ALTERED_MAKEFILE.",private=".$MK_PRIVATE_MAKEFILE."]"
		return
	endif

	let argindex=1
	let redirected=0
	while argindex<=a:0
		if stridx(a:{argindex},"=") <0
			break
		endif
		let redirected = MATE_makeRedirect( redirected, a:{argindex} )
		let argindex = argindex + 1
	endwhile

	let cfg = $MK_DEVEL."/etc/".$MK_OS."/".$MK_COMPILER.".cfg"
	if argindex>a:0
		if redirected
			redir END
			let redirected=0
		endif
		if !filereadable(cfg)
			exe "redir > ".cfg
			redir END
		endif
		make change_option
	else

"if g:winos
		call call("MATE_Invoke", [ "mtcur" ] + a:000 )
		return
"endif

		if a:{argindex}=="default"
			let $MK_MAKEFILE=''
		else

			let filename = MATE_MakeFileName(a:{argindex})
			if filename==""
				let $MK_MAKEFILE=''
				let reason=""
				if a:{argindex}[0] == '~'
					let path=$HOME.strpart(a:{argindex},1)
				else
					let path=a:{argindex}
				endif
				if isdirectory(path)
					if $MK_OS=='unix' || exists("g:unixc")
						let sep="/"
					else
						let sep="\\\\"
					endif
					call MATE_makeRedirect( redirected, "MK_MAKEPATH=".substitute(a:{argindex},"\\",sep,"g") )
					redir END
					let redirected=0
					if !filereadable(cfg) || delete(cfg)==0
						exe "redir > ".cfg
						redir END
						make change_option
"						if rename( $MK_DEVEL."/etc/".$MK_COMPILER.".new", cfg )
"							let reason="Failed to apply the current settings."
"						else
"						endif
						" to reach correct error lines, change cwd to the project path
						exe "cd ".path
					else
						let reason="Failed to delete the previous configuration file."
					endif
				else
					let reason="Invalid makefile : ".path
				endif

				if reason != ""
" maintain make mode
"					let $MK_MODE=''
					call MATE_ErrorMessage( reason )
					if redirected
						redir END
					endif
					return
				endif
			else
				let $MK_MAKEFILE=a:{argindex}
				exe "cd ".$MK_DEVEL
			endif
		endif

		if a:0<argindex+1
			let mode=$MK_MODE
		else
			if $HOSTNAME=="yopy"
				let mode="r"
			else
				let mode=a:{argindex+1}
			endif
		endif


		if mode==""
			let $MK_MODE="d"
		else
			if mode!="d" && mode!="r"
				let $MK_MAKEFILE=''
				let $MK_MODE=''
				call MATE_ErrorMessage( "Invalid build mode :".mode )
				return
			endif

			let $MK_MODE=mode
		endif
	endif

	if redirected
		redir END
	endif

endfunction

function! MATE_SetDevice(...)
	if a:0<1
		echo $MK_DEVICE
	else
		" Temporarily replace global host device to the
		" new device to affect compiler specific configuration
		" which uses g:host_device
		let hostdev=g:host_device
"		let g:host_device=a:1
		let $MK_DEVICE=a:1
		call SetCompiler( $MK_COMPILER )
		let g:host_device=hostdev
	endif
endfunction

" This function can be called from compiler.vim for example, android:native
function! MATE_LoadCompiler(os,compiler,setbasepath)
	" $MK_COMPILER can be used in config scripts so this temporary replacement is done
	let oldc = $MK_COMPILER
	let $MK_COMPILER = a:compiler
	let ep = $MK_DEVEL."/etc/".a:os."/"
	if filereadable(ep."config.vim")
		exe "source ".ep."config.vim"
	endif

	" before loading compiler.vim, user config for compiler should be loaded
	" to override default symbols like MK_COMPILER_VERSION or MK_COMPILER_TAG
	let ep = ep.a:compiler."/"
	if filereadable(ep."config.vim")
		exe "source ".ep."config.vim"
	endif


" ignore this, etc/$MK_OS/$MK_COMPILER.cfg will control config
"	let p = $MK_DEVEL."/etc/".a:os."/".a:compiler."/compiler.vim"
"	if filereadable(p)
"		exe "source" p
"	endif

	let p = $MK_TEMPLATE_ROOT."/".a:os."/".a:compiler."/compiler.vim"
	if filereadable(p)
		exe "source" p
	endif
	if a:setbasepath
		call SetBasePath()
	endif
	" a:setbasepath is used here to decide to show message
	" but if SetBasePath and showing message should be done
	" separately, optional argument or distinguishable
	" value for a:setbasepath is needed
	if a:setbasepath
		if g:vimenter
			echo "[".a:compiler."][".a:os."][".$MK_DEVICE."] configured."
		endif
	else
		if exists("*MATE_CompilerPath")
			let $TOOLPATH=MATE_CompilerPath().g:__.$TOOLPATH
		endif
	endif

	let $MK_COMPILER = oldc
endfunction

function! SetCompiler(...)
	if a:0<1
		echo $MK_COMPILER $MK_COMPILER_TAG $MK_OS $MK_DEVICE
	else
		let compiler=a:1

		if compiler == "config"
			let cfg = $MK_DEVEL.g:_."etc".g:_.$MK_OS.g:_.$MK_COMPILER.g:_."config.vim"
			if filereadable(cfg)
				exe "e ".cfg
			else
				echo cfg." file not found"
			endif
		else
			if a:0>1
				let os=a:2
			else
				let os=$MK_OS
			endif
			call MATE_Invoke( "mtcomp", a:1, os )
			return


			let basepath = $MK_TEMPLATE_ROOT."/".os."/".compiler

			if !isdirectory(basepath)
				echo "[".os."] [".compiler."] : unavailable compiler."
				return
			endif

			" change envvar first so that SetBasePath works correctly
			let $MK_OS = os
			let $MK_COMPILER = compiler
			let $TOOLPATH=''
			let $MK_DEVICE=''
			let $LIB1=''
			let g:mate_compiler_path=1
			" MK_TEMPLATE is defined in symbol.mk but predefined here for convenience
			let $MK_TEMPLATE = $MK_TEMPLATE_ROOT.g:_.$MK_OS.g:_.$MK_COMPILER

			if exists("g:mate_makeprg_postfix")
				unlet g:mate_makeprg_postfix
			endif
			if exists("g:unixc")
				unlet g:unixc
			endif

			let $MK_COMPILER_VERSION=''
			let $MK_COMPILER_TAG=''
			let g:mate_default_compiler=''
			let g:mate_default_compiler_tag=''

			call MATE_LoadCompiler( $MK_OS, $MK_COMPILER, 1 )

	"		let p = basepath.'/device.vim'
	"		if filereadable(p)
	"			exe "source" p
	"		endif
			" consider typical source tree structure
			" XXX this will always cumulate the path
			let &path=&path.',./../inc,./../include'

			"call SetBasePath()
			if $PROJECT_PATH != ''
				let $PROJECT_PATH=''
			endif
			if $PROJECT_SUBPATH != ''
				let $PROJECT_SUBPATH=''
			endif
			call MATE_SetMakePrg()

			if exists("g:mate_jdk_path")
				let $PATH=$PATH.g:__.g:mate_jdk_path.g:_."bin"
			endif
		endif
	endif
endfunction

function! SetOutput()
	let $MK_OUT_ROOT=$MK_EXTRA.g:_.g:output_base_path.g:_.'out'
	let $MK_LIB_ROOT=$MK_EXTRA.g:_.g:output_base_path.g:_.'lib'
endfunction

function! GetCygwinEnv(var)
	return substitute(substitute(substitute(a:var,"\\","/","g"),"\\(.\\):","/cygdrive/\\1","g"),";",":","g")
endfunction

function! MATE_TargetPath()
	return g:host_os.g:_.$MK_CONFIG.g:_.g:host_device
endfunction

"function! GetCygwinEnv(var)
"	return substitute(substitute(substitute(a:var,"\\","/","g"),"\\(.\\):","/cygdrive/\\1","g"),";",":","g")
"endfunction

function! MATE_Invoke(name,...)
	let cmd=g:mate_host_shcmd.$MK_BASE.g:_.$MK_RELATIVE.g:_."make".g:_."template".g:_."etc".g:_."cmd".g:_.a:name.g:mate_host_shext
	let i = 1
	while i <= a:0
		let cmd = cmd." ".a:{i}
		let i = i + 1
	endwhile
	let o=system(cmd)
	let i = match(o,"\\*\\*\\*MATE")
	if i >= 0
		let o = strpart(o,i + 7)
	endif
	let o = substitute(o,"\\","\\\\\\\\","g")
	let i = 0
	let name = ""
	while i >= 0
		let next = stridx(o,"*",i)
		if next >= 0
			let len=next-i
			if strpart(o,next - 1,1) == "\n"
				" ignore the last trailing new line
				let len=len - 1
			endif
			let i = strpart(o,i,len)
			let next = next + 1
		endif

		if name == ""
			let name=i
		else
			if name == "MK_RESULT"
				echo i
			" unsetting variable is needed so don't check
			" i is empty string
			else
				exe "let $".name."="."\"".i."\""
			endif
			let name=""
		endif
		let i = next
	endwhile
"unlet next
unlet i
"unlet init
"unlet name
endfunction

if has("unix") "{
let host_os="unix"
let mate_host_shext=".sh"
let mate_host_shcmd="bash "
set encoding=utf-8
set fileencodings=euc-kr
set fileencoding=euc-kr
let $MK_OS='unix'
let $MK_COMPILER='gcc'
"let $MK_TEMPLATE_ROOT=MK_DEVEL.'/make/template'
"let mate_makeprg='sh\ '.MATE_ResolveBlank('$MK_DEVEL/make/make')
let mate_makeprg='sh "'.MK_DEVEL.'/make/make.sh"'
call MATE_Invoke("initecho")

exe "let $ORIGLDLIBPATH=$".$MATE_LDLIBPATH_SYM

let host_device=$MK_DEVICE


function! SetBasePath()
	let target_path=MATE_TargetPath()
	call DefinePath( $MK_OUT_ROOT.'/'.target_path.':'.$MK_ORIGPATH )
	exe "let $".$MATE_LDLIBPATH_SYM."=$ORIGLDLIBPATH.':'.$MK_LIB_ROOT.'/'.target_path.'/shared'"
endfunction

"call SetBasePath()

function! GetCygwinEnv(var)
	return a:var
endfunction

call MATE_InitializeOutput()

elseif g:winos "}{

function! GetDrive()
	let cwd = getcwd()
	let index=stridx( cwd, "\\" )
	return strpart( cwd, -index, index*2 )
endfunction

if has("win64")
	let host_os='win64'
elseif has("win32")
	let host_os='win32'
elseif has("win16")
	let host_os='win16'
endif
let mate_host_shext=".bat"
let mate_host_shcmd=""

call MATE_Invoke("initecho")
let &errorformat="\"%f\"\\\,\ line\ %l:\ %m,%*[ %#|>1 %#]%f(%l) : %m,".&errorformat

"jshint
let &errorformat="%f: line %l\\\, col\ %c\\\,\ %m,".&errorformat

if $OS==""
	set shell=cmd95\ /K\ doskey
else
	set shellpipe=>%s\ 2>&1
endif

if $LS_COLOR==''
	let $LS_COLORS=':di=47;34'
endif

"let $MK_OS='win32'
"let $MK_COMPILER='vcc'
"let $MK_TEMPLATE_ROOT=MK_DEVEL.'\make\template'
"let $MK_ORIGPATH=$PATH
let mate_makeprg=MK_DEVEL."\\make\\make.bat"

if isdirectory($FILE.'\devel\vs2005')
	let msdev_default='vs2005'
	let vs2005_installed=1
else
	let msdev_default='msdev'
	let vs2005_installed=0
endif

if $MK_DEVICE == ""
	if $PROCESSOR_ARCHITECTURE == "AMD64" || $PROCESSOR_ARCHITEW6432 == "AMD64"
		let $MK_DEVICE='x64'
	else
		let $MK_DEVICE='x86'
	endif
endif
let host_device=$MK_DEVICE

" TODO: this function is obsolete, settle exact install path
function! GetBasePath(path,name)
	if !isdirectory(a:path)
		let basepath=$FILE.'\devel\'.a:name
		if !isdirectory(basepath)
			if exists("$TOOL")
				return $TOOL.'\'.a:name
			endif
			if isdirectory('u:\cygwin') && isdirectory('u:\path')
				let $WINDEV='u:'
			endif
			if isdirectory('s:\util') && isdirectory('s:\include')
				let $SHARE='s:'
			endif
			if exists("$WINDEV")
				return $WINDEV.'\'.a:name
			endif
			return 'c:\tool\'.a:name
		endif
		return basepath
	endif
	return a:path
endfunction

function! MATE_WinOSCompilerPath(target_path, compiler, tag)
	return $MK_OUT_ROOT.'\'.a:target_path.'\'.a:compiler.a:tag.';'.$MK_LIB_ROOT.'\'.a:target_path.'\shared'.a:tag
endfunction

function! MATE_WinOSProgramFiles()
	if $PROCESSOR_ARCHITECTURE == "AMD64" || $PROCESSOR_ARCHITEW6432 == "AMD64"
		return substitute(split(system('set ProgramFiles(x86)'),'=')[1],"\n","","g")
	endif
	return $ProgramFiles
endfunction

function! MATE_CompilerPath()
	let target_path=MATE_TargetPath()
	if g:mate_compiler_path
		let newpath=MATE_WinOSCompilerPath(target_path,$MK_COMPILER,$MK_COMPILER_TAG).';'
	else
		let newpath=''
	endif
	if g:mate_default_compiler==''
		" first compiler is default compiler
		let g:mate_default_compiler=$MK_COMPILER
	endif
	return newpath.MATE_WinOSCompilerPath(target_path,g:mate_default_compiler,g:mate_default_compiler_tag)
endfunction

if !exists("g:winos_x86_program")
	let g:winos_x86_program = MATE_WinOSProgramFiles()
endif

function! SetBasePath()

	let $PATH=''
	if !exists("g:cygwin_base_path")
		let g:cygwin_base_path=GetBasePath("","cygwin")
	endif

	if $MK_SHELL == "1"
		let cygwin_path=''
	else
		let cygwin_path=g:cygwin_base_path.'\bin'
	endif
	if isdirectory(g:cygwin_base_path.'\usr\X11R6\bin')
		let cygwin_path=cygwin_path.';'.g:cygwin_base_path.'\usr\X11R6\bin'
	endif
	if isdirectory(g:cygwin_base_path.'\msys\1.0\bin')
		let cygwin_path=cygwin_path.';'.g:cygwin_base_path.'\msys\1.0\bin'
	endif

	let newpath = MATE_CompilerPath()

	" the priority of the cygwin path is lowered after msdev path because,
	" cygwin has 'link' command which has the same name of linker in msdev
"echo "MK_ORIGPATH=".$MK_ORIGPATH
"echo "PATH=".newpath
	call DefinePath( cygwin_path.';'.newpath.';'.$MK_ORIGPATH )
	let $LIB=$LIB1.';'.$MK_LIB_ROOT.'\'.MATE_TargetPath().'\'.$MK_COMPILER.$MK_COMPILER_TAG
	if isdirectory($FILE.'\devel\j2se')
		let $PATH=$PATH.';'.$FILE.'\devel\j2se\bin'
		if isdirectory($FILE.'\devel\j2se\jaxb')
			let $PATH=$PATH.';'.$FILE.'\devel\j2se\jaxb\bin'
			let $CLASSPATH=$CLASSPATH.';'.$FILE.'\devel\j2se\jaxb\lib\jaxb-impl.jar'.';'.$FILE.'\devel\j2se\jaxb\lib\jaxb-api.jar'
		endif
	endif
	if isdirectory($FILE.'\devel\python')
		let $PATH=$PATH.';'.$FILE.'\devel\python'
	endif
	if isdirectory($MK_DEVEL.'\cmd')
		let $PATH=$PATH.';'.$MK_DEVEL.'\cmd'
	endif

	if has("win32")
		if $MK_OS!="win32"
			let $PATH=$PATH.';'.$MK_OUT_ROOT.'\win32'
		endif
		if $WINDEV!=""
			let $PATH=$PATH.';'.$WINDEV.'\path;'.$WINDEV.'\jdk\bin'
		endif
	endif

	let $LIBRARY_PATH=GetCygwinEnv($MK_LIB_ROOT.'\win32\cygnus;'.g:cygwin_base_path.'\lib')
endfunction

call MATE_InitializeOutput()

" call SetCompiler after sourcing config.vim so the active compiler can be selected
if $MK_COMPILER == ""
	let $MK_COMPILER='vcc'
endif
if $MK_OS == ""
	let $MK_OS='win32'
endif


endif "} has("unix")

"let $PATH = system( mate_host_shext


if !exists(":BASE")
	let $MK_TEMPLATE_ROOT=MK_DEVEL.g:_.'make'.g:_.'template'
else
	let $MK_TEMPLATE_ROOT=$MK_DEVEL.g:_.'make'.g:_.'template'
endif

function! InitDevel()
	let $MK_DEVEL=g:MK_DEVEL
endfunction

function! MATE_SystemRootPath()
	return $MATE_DEVROOTPATH
endfunction

let default_extra=$MK_EXTRA


let g:mate_scm_recursive=1


if !exists("$MK_SCM")
	let $MK_SCM="cvs"
endif

if $MK_DEVICE==''
	if has("unix")
		let host=system("echo -n $HOSTTYPE")
		if host=="x86_64"
			let $MK_DEVICE='x64'
		else
			let $MK_DEVICE='x86'
		endif
	elseif $PROCESSOR_ARCHITECTURE == "AMD64" || $PROCESSOR_ARCHITEW6432 == "AMD64"
		let $MK_DEVICE='x64'
	else
		let $MK_DEVICE='x86'
	endif
endif

call InitDevel()
call SetCompiler( $MK_COMPILER,  $MK_OS )
"if !exists(":BASE")
"	call SetDevel( 's:' )
	call SetDevel( 'default' )
"endif

let &errorformat="%f: line %l: %m,".&errorformat.",symbol %#:%m,found %#:%m,required %#:%m"

" only for cygwin TODO
" preceding %-G will remove the matched line
let &errorformat="%-GIn file included from %f:%l:%m,".&errorformat

" for nodejs error, %A is beginning of multiline and %Z is end of multiline
let &errorformat="%-G    at %.%# (%f:%l:%m),%A%f:%l,%C%m,%C%.%#,%ZSyntaxError: %m,".&errorformat

let $CLASSPATH=$MK_LIB_ROOT."/java".__.$MK_OUT_ROOT."/java".__.$CLASSPATH

if exists("apache_lib_path")
let $CLASSPATH=$CLASSPATH.__.apache_lib_path."/commons-net.jar"
let $CLASSPATH=$CLASSPATH.__.apache_lib_path."/jakarta-oro.jar"
endif

if exists("axis_lib_path")
let $CLASSPATH=$CLASSPATH.__.axis_lib_path."/axis.jar".__.axis_lib_path."/commons-logging-1.0.4.jar".__.axis_lib_path."/commons-discovery-0.2.jar".__.axis_lib_path."/jaxrpc.jar".__.axis_lib_path."/saaj.jar".__.axis_lib_path."/wsdl4j-1.5.1.jar"
endif

if exists("jaf_lib_path")
let $CLASSPATH=$CLASSPATH.__.jaf_lib_path."/activation.jar"
endif

if exists("javamail_lib_path")
let $CLASSPATH=$CLASSPATH.__.javamail_lib_path."/mail.jar"
endif

if exists("java_user_lib")
let $CLASSPATH=$CLASSPATH.__.$MK_OUT_ROOT."/java".__.java_user_lib
endif

cd $MK_DEVEL
if $CVS_RSH==""
	let $CVS_RSH=GetCygwinEnv($MK_TEMPLATE_ROOT).'/etc/util/cvsrsh.sh'
endif


function! MATE_InitC()
	setlocal ts=4
	setlocal sw=4
	" folding
"	setlocal foldmethod=syntax
"	syn region cfComment start=+/\*+ end=+\*/+ transparent keepend extend fold 
"	syn region cfCode start="[^#]*{" end="}" transparent fold
"	syn region cfStruct start="^DECLARE_.*(.*)" end="DECLARE_END" transparent fold
"	syn region cfParam start="P_1" end="P_E" transparent fold
endfunction

au FileType c,cpp call MATE_InitC()
au FileType java,jav call MATE_InitC()
au FileType tex setlocal encoding=utf-8 fileencoding=utf-8
au BufRead,BufNewFile *.pj set filetype=pj | call MATE_InitC()
au BufRead,BufNewFile *.pjs set filetype=pjs | call MATE_InitC()
au BufRead,BufNewFile *.src set filetype=make
au BufRead,BufNewFile *.bun set filetype=make
au BufRead,BufNewFile *.mmk set filetype=make

function! EchoProject()
		echo "PROJECT : [" $MK_PROJECT "]"
endfunction

function! ClearProject()
"	if $MK_PROJECT!=""
"		let index = GetFileNameIndex($MK_PROJECT)
"let $MK_PROJECT='d:/k/dk/sd/hello.world'
"		let index = strridx( $MK_PROJECT, "." )
"		let filename = strpart( $MK_PROJECT, -index, index*2 ).".vim"
	if exists("*MATE_FinalizeProject")
		call MATE_FinalizeProject()
		delf MATE_FinalizeProject
	endif
"	endif
endfunction

"SetProject project [subdirrectory]
function! SetProject(...)
"call ClearProject()
"return
	if a:0<1
		call EchoProject()
	else
		if a:1=="default"
			call ClearProject()
			let $MK_PROJECT = ""
			cd $MK_DEVEL
		else
			let index = GetFileNameIndex(a:1)
			if $PROJECT_PATH == ""
				let project = "project"
			else
				let project = $PROJECT_PATH
			endif
			if index==0
				let project=$MK_BASE.g:_.project.g:_
			else
				let project=_
			endif
			let project=project.a:1
			if a:0>1
				let project=project.g:_.a:2
			else
				if $PROJECT_SUBPATH != ""
					let project=project.g:_.$PROJECT_SUBPATH
				endif
			endif

			" determine valid file extension
			if exists("g:msdev") && g:msdev=="vs2005"
				let project_file = project.".sln"
			else
				let project_file = project
			endif

			" initialize project
			if !isdirectory(project_file) && !filereadable(project_file)
				echo "Unvailable project [".project_file."]"
			else
				call ClearProject()
				let project_conf = project.g:_.a:1.".vim"
				if filereadable(project_conf)
					exe "source ".project_conf
					call InitializeProject()
					delf InitializeProject
				endif
				let $MK_PROJECT = project_file
				exe "cd ".project
				if filereadable("tags.vim")
					source tags.vim
				endif
			endif
		endif
	endif
endfunction

function! MATE_MakeOpen(src,...)
	if( a:0>0 )
		let make_name=a:1
	else
		let make_name=$MK_MAKEFILE
	endif
	if a:src
		let make_file = $MK_DEVEL.g:_."etc".g:_.make_name.".src"
	else
		let make_file = MATE_MakeFileName(make_name)
	endif
	if make_file==""
		echo "Invalid make file :" make_file
		return
	endif
	exe "e" make_file
endfunction


function! SetFindFilter(...)

	if a:0<1
		echo g:find_filter
		return
	endif

	if has("unix")
		let filter = ' \('
		let close = ' \)'
		let filter_option=" -iname '"
		let filter_or=' -or'
		let quot="'"
	else
		let filter = ''
		let close = ''
		let filter_option=' "'
		let filter_or=''
		let quot='"'
	endif

	let or=quot.filter_or.filter_option
	let default_filter = filter_option.'*.c'.or.'*.h'.or.'*.hpp'.or.'*.cpp'.or.'*.cxx'.or.'*.htm'.or.'*.html'.or.'*.xml'.or.'*.xsd'.or.'*.xsl'.or.'*.txt'.or.'*.tex'.or.'*.mk'.or.'*.mmk'.or.'*.d'.or.'*.java'.or.'*.pj'.or.'*.pjs'.or.'*.bat'.or.'*.rsg'.or.'*.rh'.or.'*.hrh'.quot.''.filter_or.filter_option.'*.py'.or.'*.sh'.or.'*.m'.or.'*.js'.or.'*.conf'.or.'*.cfg'.or.'*.vim'.or.'*.yml'.or.'*.yaml'.quot

	if a:1=="default" || a:1=="."
		let i = 2
		let filter = filter.default_filter
	else
		let i = 1
	endif

	while i<=a:0
		if i>1
			let filter = filter.filter_or
		endif
		"let filter = filter.filter_option.quot.''.a:{i}.quot
		let filter = filter.filter_option.a:{i}.quot
		let i = i + 1
	endwhile
	let g:find_filter=filter.close
endfunction

call SetFindFilter( "default" )

function! MATE_RecursiveCommandTempFile()
	return &dir.g:_."filelist.txt"
endfunction

function! MATE_RecursiveCommand(path,fl,cmd)
	let old_grepprg=&grepprg

	let path = a:path
	let lastchar = path[strlen(path)-1]
	if lastchar != '/' && lastchar != '\'
		let path = path.g:_
	endif

	let fl=a:fl
	if has("unix")
		if fl != ""
			let fl = " > ".fl
		endif
		let cmd = a:cmd
		if cmd != ""
			let cmd = "-exec ".cmd." \\{\\} \\;"
		endif
		let &grepprg="find '".path."'".g:find_filter." ".cmd.fl
		grep
	else
		let cwd=getcwd()
		if fl == ""
			let fl = MATE_RecursiveCommandTempFile()
		endif
		exe "cd ".path
		exe "silent !dir ".g:find_filter." /s /b > ".fl
		exe "cd ".cwd

		if a:cmd != ""
			let &grepprg=a:cmd
			grep
		endif

		if a:fl == ""
			call delete(fl)
		endif
	endif

	let &grepprg=old_grepprg
endfunction

function! MATE_RecursiveGrep(...)
	let expr=a:1
	if a:0<2
		let path=expand("%:p:h")
	else
		if a:2[0] == '~'
			let path=$HOME.strpart(a:2,1)
		else
			let path=a:2
		endif
	endif
	if has("unix")
		" one trailing blank must be needed"
		let opt = " "
		let wp=""
		let ws=""
	else
		let opt = "/R /F:".MATE_RecursiveCommandTempFile()
		if exists("g:grep_whole") && g:grep_whole
			let wp="\\<"
			let ws="\\>"
		else
			let wp=""
			let ws=""
		endif
	endif
	call MATE_RecursiveCommand(path,"",&grepprg." ".opt.' "'.wp.expr.ws.'"')
endfunction

function! MATE_RecursiveGrep_(...)
	let expr=a:1
	let old_grepprg=&grepprg
	if a:0<2
		let path=expand("%:p:h")
	else
		if a:2[0] == '~'
			let path=$HOME.strpart(a:2,1)
		else
			let path=a:2
		endif
	endif

	let lastchar = path[strlen(path)-1]
	if lastchar != '/' && lastchar != '\'
		let path = path.g:_
	endif

	if has("unix")
		let &grepprg="find '".path."'".g:find_filter." -exec ".&grepprg.' "'.expr.'" \{\} \;'
		grep
	else
		let cwd=getcwd()
		let fl=&dir.g:_."filelist.txt"
		exe "cd ".path
		exe "silent !dir ".g:find_filter." /s /b > ".fl
		exe "cd ".cwd
		let &grepprg=&grepprg." /F:".fl.' "'.expr.'"'
		grep
		call delete(fl)
	endif

	let &grepprg=old_grepprg
endfunction

function! GetFileNameIndex(path)
	" TODO: is there a function! which multiple tokens can be supplied ?
	" is there the string array to store tokens ?
	let cnt=0
	while cnt<2
		if cnt==0
			let token="/"
		else
			let token="\\"
		endif
		let index=strridx(a:path,token)
		if index!=-1
			return index
		endif
		let cnt = cnt+1
	endwhile
	return 0
endfunction

function! GetFileName(path)
	" TODO: is there a function! which multiple tokens can be supplied ?
	" is there the string array to store tokens ?
	let index = GetFileNameIndex(a:path)
	if index>0
		return strpart(a:path,index+1)
	endif
	return a:path
endfunction

function! CvsSetPath(path)
	let fullpath=$MK_DEVEL."/".a:path
	if isdirectory(fullpath)
		let curpath=getcwd()
		exe "cd" fullpath
		return curpath
	endif
	echo "Invalid directory."
	return ""
endfunction



function! EditStream( cmd, ... )
	if a:0<1
		let filename = expand("%")
	else
		if isdirectory(a:1)
			let curpath=getcwd()
			exe "!find ".a:1.g:find_filter." -mindepth 1 -exec sh ".$MK_TEMPLATE_ROOT."/etc/util/replace '".a:cmd."' \{\} \;"
			return
		endif
		let filename = a:1
	endif
	exe "!sh ".$MK_TEMPLATE_ROOT."/etc/util/replace '".a:cmd."' ".filename
endfunction

function! Copy( ... )
"	if a:0<1
		let mkdevel = $MK_DEVEL
"		let cvsroot = $CVSROOT
		if !exists("g:mate_copy_srcbasepath") || g:mate_copy_srcbasepath==""
			call InitDevel()
		else
			let $MK_DEVEL=g:mate_copy_srcbasepath
		endif
		if ( !exists("g:mate_copy_dstbasepath") || g:mate_copy_dstbasepath=="" )
			if a:0<1
				echo "Destination path is needed"
				return
			endif
			let dstpath=a:1
		else
			let dstpath=g:mate_copy_dstbasepath
		endif
		let filename = GetCygwinEnv(expand("%:p"))
		exe "!cp -f ".filename dstpath."/".MATE_RelativeToDevelPath(filename)
		let $MK_DEVEL=mkdevel
"		let $CVSROOT=cvsroot
"	else
"		let i=1
"		while i<=a:0
"			exe "!cp -f ".a:{i} a:path."/".a:{i}
"			let i=i+1
"		endwhile
"	endif
endfunction

function! NetRW(op,...)
"	Sample NETRW global formats
"	let netrw_server="ftp://machine/path"
"	let netrw_server="scp://user@machine/path"

	if g:netrw_server!=""

		if a:0>0
			let filename = a:1
			exe "view ".a:1
		else
			let filename = expand("%")
		endif

		if filename==""
			echo "Nothing to transfer."
		else
			if g:netrw_local_subdir!=""
				let basepath = g:netrw_local_subdir."/"
			endif
			let filename = GetCygwinEnv(substitute(filename,basepath,"",""))
			let l = line(".")
			let c = col(".")
			exe a:op." ".g:netrw_server."/".filename
			if g:netrw_sync!=""
				exe "silent !echo ".a:op." ".filename." >> netrw.log"
			endif
			call cursor( l, c )
"			echo "O.K"
		endif
	else
		echo "NETRW is not allowed for this source tree."
	endif
endfunction

function! NetSync()
	if g:netrw_sync==""
		echo "NetSync is not allowed for this source tree."
	else
		exe "!lsync netrw.log ".g:netrw_sync." delete"
	endif
endfunction

function! Date()
	let lc_time=v:lc_time
	"change the current time language to English
	if has("unix")
		language en_US.UTF-8
	else
		language EN
	endif
	exe "put='".strftime('%p %I:%M %Y.%m.%d')."'"
	"recover the current time language
	exe "language ".lc_time
endfunction

function! Output(...)
	if a:0>0
		let g:output_base_path=a:1
		call SetOutput()
		call SetBasePath()
	endif
	echo $MK_OUT_ROOT
	echo $MK_LIB_ROOT
endfunction

function! SetExtra(...)
	if( a:0<1 )
		echo $MK_EXTRA
	else
		if a:1=='default'
			let $MK_EXTRA=g:default_extra
		else
			if isdirectory(a:1)
				let $MK_EXTRA=a:1
			else
				echo "Invalid path"
				return
			endif
		endif
		call SetOutput()
		call SetBasePath()
	endif
endfunction

function! SetBase(...)
	if( a:0<1 )
		echo $MK_DEVEL $MK_TEMPLATE_ROOT $MK_EXTRA $MK_OUT_ROOT $MK_LIB_ROOT
		return "$MK_DEVEL/make/template/etc/config/empty.vim"
	endif
	if a:1=='default'
		call SetDevel('default')
	else
		call SetDevel(a:1.g:_.'devel')
	endif
	let $MK_TEMPLATE_ROOT=$MK_DEVEL.g:_.'make'.g:_.'template'
	call SetExtra(a:1)
	call SetTempPath()
	return ResetMate()
endfunction

function! Anchor(...)
	let desc=''
	if( a:0<1 )
		let url="http://"
	else
		let url=a:1
		if( a:0>1 )
			let desc=a:2
		endif
	endif
	exe 'put=\"<a target=_blank href=\\"'.url.'\\">'.desc.'</a>\"'
endfunction

function! MakeError(...)
	if a:0<1
		return
	else
		let filename=a:1
	endif
	let oldprg=&makeprg
	let &makeprg='cat '.filename
	make
	let &makeprg=oldprg
endfunction


command! -nargs=0 ORG : call MATE_Org()
command! -nargs=* -complete=dir DEVEL : call SetDevel(<f-args>)
command! -nargs=* PROJECT : call SetProject(<f-args>)
command! -nargs=* -complete=dir MAKE : call MATE_MakeCurrent(<f-args>)

command! -nargs=* MAKEFILE : call MATE_MakeOpen(0,<f-args>)
command! -nargs=* MAKESRC : call MATE_MakeOpen(1,<f-args>)
command! -nargs=* -complete=file MAKEERR : call MakeError(<f-args>)

function! MATE_FileList( ... )
"	let lst = "..".g:_.GetFileName(getcwd()).".lst"
	if a:0 < 1
		echo "Supply file name"
	else
		if a:0 > 1
			let path=a:2
		else
			let path=GetFileName(getcwd())
		endif
		let oldprg=&makeprg
		let &makeprg="bash ".GetCygwinEnv($MK_TEMPLATE_ROOT)."/etc/util/filelist.sh ".a:1." \"".GetCygwinEnv($MK_DEVEL."/etc/".path.".lst")."\""
		make
		let &makeprg=oldprg
	endif
endfunction
command! -nargs=* FILELIST : call MATE_FileList(<f-args>)

function! MATE_FileListGen( ... )
	if a:0 > 0
		let path=a:1
		if !isdirectory(path)
			let path=$MK_DEVEL.g:_.a:1
			if !isdirectory(path)
				let path=$MK_ALTERED_DEVEL.g:_.a:1
				if !isdirectory(path)
					echo "Invalid path"
					return
				endif
			endif
		endif
		let name=a:1
	else
		let path="."
		let name=GetFileName(getcwd())
	endif
	call MATE_RecursiveCommand(path,$MK_DEVEL.g:_."etc".g:_.name.".lst","")
"	exe "! find ".GetCygwinEnv(path)."/ ".g:find_filter." > ".$MK_DEVEL."/etc/".name.".lst"
endfunction
command! -nargs=* FILELISTGEN : call MATE_FileListGen(<f-args>)

function! MATE_GetMergeFile()
	return substitute(getline("."),"^\\[.*\\] *","","")
endfunction

function! MATE_MergeFileState(state)
	on
	exe "silent! s/^\\[.*\\] *//"
	if a:state != ""
		exe "s/^/[".a:state."] /"
	endif
	exe "+"
	call MATE_MergeFile()
endfunction

function! MATE_MergeFile( ... )
	if a:0>0
		wincmd b
		if a:1=="0"
			on
		elseif a:1=="diff"
			let f=MATE_GetMergeFile()
			exe "! diff -b -B ".GetCygwinEnv(g:mate_merge_origpath.g:_.f)." ".GetCygwinEnv(g:mate_merge_modpath.g:_.f)." >".g:mate_merge_diffpath.g:_.f.".diff"
		elseif a:1=="push"
			let f=MATE_GetMergeFile()
			exe "! cp ".GetCygwinEnv(g:mate_merge_origpath.'\'.f)." ".GetCygwinEnv(g:mate_merge_pushpath.g:_.f)
		elseif a:1=="pull"
			let f=MATE_GetMergeFile()
			exe "! cp ".GetCygwinEnv(g:mate_merge_pushpath.'\'.f)." ".GetCygwinEnv(g:mate_merge_origpath.g:_.f)
		elseif a:1=="done"
			call MATE_MergeFileState("DONE")
		elseif a:1=="skip"
			call MATE_MergeFileState("SKIP")
		elseif a:1=="clear"
			call MATE_MergeFileState("")
		elseif a:1=="reviewed"
			call MATE_MergeFileState("REVIEWED")
		else
			echo "Unknown instruction."
		endif
	else
		let l=MATE_GetMergeFile()
		exe "sview ".g:mate_merge_modpath.g:_.l
		exe "new ".g:mate_merge_origpath.g:_.l
		exe "new ".g:mate_merge_diffpath.g:_.l.".diff"
	endif
endfunction

command! -nargs=* MERGEFILE : call MATE_MergeFile(<f-args>)

function! MATE_MakeTouch( ... )
	if( a:0>0 )
		let make_name = a:1
	else
		let make_name = $MK_MAKEFILE
	endif
	let make_file = MATE_MakeFileName(make_name)
	if make_file == ""
		echo "Unavailable makefile."
	else
		exe "silent !touch ".filepath
	endif
endfunction

function! MakeUtil( ... )
	let mkfile=$MK_MAKEFILE
	if mkfile==""
		let $MK_MAKEFILE='template/etc/config/empty'
	endif
	let compiler=$MK_COMPILER
	let os = $MK_OS

	if a:1=='@'
		let index = 4
		call SetCompiler( a:2, a:3 )
	else
		let index = 1
	endif

	let $UTLCMD=a:{index}

	let n = 1
	let i = index
	while a:0 > i
		exe "let $UTLARG".n."='".a:{i+1}."'"
		let i = i + 1
		let n = n + 1
	endwhile
	if has("unix")
		" because screen can be invoked when it is connected to
		" a terminal, so applying '&' uniformly at the end of command
		" is not suitable
		exec "!".g:mate_makeprg." exec_util"
	else
		exec "silent ! ".g:mate_makeprg." exec_util"
	endif

	if a:1=='@'
		call SetCompiler( compiler, os )
	endif

	let $UTLCMD=""
	let n = 1
	let i = index
	while a:0 > i
		exe "let $UTLARG".n."=''"
		let i = i + 1
		let n = n + 1
	endwhile
	let $MK_MAKEFILE=mkfile
endfunction

command! -nargs=* -complete=file MAKEUTIL : call MakeUtil(<f-args>)

command! -nargs=* MAKETOUCH : call MATE_MakeTouch(<f-args>)

function! MATE_Config(...)
	if a:0>0
		let $MK_CONFIG=a:1
		call MATE_SetDevice($MK_DEVICE)
	endif
	echo "Current config is [".$MK_CONFIG."]"
endfunction
command! -nargs=? CONFIG : call MATE_Config(<f-args>)
command! -nargs=* COMPILER : call SetCompiler(<f-args>)
command! -nargs=? DEVICE : call MATE_SetDevice(<f-args>)

function! GrepOption(...)
	if has("unix")
		let &grepprg='grep -H -n'
		if a:0>0
			let i = 1
			while i<=a:0
				let o=a:{i}
				if o == "nocase"
					let o="-i"
				elseif o == "whole"
					let o="-w"
				else
					echo o." : unknown"
					let i = i + 1
					continue
				endif
				let &grepprg=&grepprg.' '.o
				let i = i + 1
			endwhile
		endif
	else
		let &grepprg='findstr -n'
		if exists("g:grep_whole")
			unlet g:grep_whole
		endif
		if a:0>0
			let i = 1
			while i<=a:0
				let o=a:{i}
				if o == "nocase"
					let o="/i"
				elseif o == "whole"
					let g:grep_whole=1
					let i = i + 1
					continue
				else
					echo o." : unknown"
					let i = i + 1
					continue
				endif
				let &grepprg=&grepprg.' '.o
				let i = i + 1
			endwhile
		endif
	endif
endfunction

call GrepOption()

command! -nargs=* GREPOPTION : call GrepOption(<f-args>)

" On cygwin environment, RGREP will not work if there is duplicated
" semi-colon (;;) before acquiring the path where the grep command! is present
command! -nargs=+ -complete=file RGREP : call MATE_RecursiveGrep(<f-args>)

command! -nargs=+ SED : call EditStream(<f-args>)

function! MATE_SCM(...)
	if a:0 == 0
		echo $MK_SCM
	else
		if a:1 == "set"
			if a:0 > 1 | let scm=a:2
			else | echo "Supply name of new SCM tool ( current : ".$MK_SCM." )" | return | endif
		else
			let scm=$MK_SCM
		endif
		let scmsh=$MK_TEMPLATE_ROOT.g:_."scm".g:_.scm.".sh"
		if filereadable(scmsh)
			if a:1 == "set"
				let $MK_SCM=scm
			else
				" the reason to source scm.sh is cygwin bash
				" locks file supplied its argument without
				" sourcing so if scm.sh itself is updated,
				" resource is busy so error :
				" 0 after -c "source scm.sh" means 1st arg for
				" sourced command so it will be unused
				call call("MATE_Command", [ "bash", 4, "-c", "\"source ".GetCygwinEnv(scmsh)."\"", 0 ] + a:000 )
			endif
		else
			echo "Invalid SCM [".scmsh."]"
		endif
	endif
endfunction

command! -nargs=* -complete=file SCM : call MATE_SCM(<f-args>)
"let scm=$MK_TEMPLATE_ROOT.g:_."scm".g:_.g:mate_scm_util.".sh" | if filereadable(scm) | call MATE_Command( "bash", 2, GetCygwinEnv(scm), <f-args> ) | else | echo "Invalid SCM [".scm."]" | endif



" call CvsCmd(1,$MK_TEMPLATE_ROOT._."etc"._."util"._."cvsundo",<f-args>)
command! -nargs=* -complete=file COPY : call Copy(<f-args>)
command! -nargs=* -complete=file FINDFILTER : call SetFindFilter(<f-args>)
command! -nargs=* -complete=file PUT : call NetRW("Nwrite",<f-args>)
command! -nargs=* -complete=file GET : call NetRW("Nread",<f-args>)
command! -nargs=0 SYNC : call NetSync() 
command! -nargs=0 DATE : call Date()
command! -nargs=* ANCHOR : call Anchor(<f-args>)
command! -nargs=* -complete=file OUTPUT : call Output(<f-args>)


function! MateExecute(...)
	let path=$MK_DEVEL.g:_."etc".g:_.g:host_os.g:_
	if a:0 > 0 " open given file name
		let cmd=path."cmd".g:mate_host_shext
		if filereadable(cmd)
			let argindex=2
			let arg=" c_".a:1
			while argindex<=a:0 
				let arg = arg." ".a:{argindex}
				let argindex=argindex+1
			endwhile
			exe "silent !".g:mate_host_shcmd.cmd.arg
		endif
	else " open current buffer according to its file extension
		let assoc=path."assoc".g:mate_host_shext
		if filereadable(assoc)
			exe "silent !".g:mate_host_shcmd.assoc." a_".expand("%:e") expand("%:p")
		else
			exe "silent !".expand("%")
		endif
	endif
endfunction
command! -nargs=? EXE : call MateExecute(<f-args>)

command! -nargs=* -complete=file EXTRA : call SetExtra(<f-args>)
command! -nargs=* -complete=file BASE : exe "source ".SetBase(<f-args>)

function! EscapeSpecialChar(cmd,c)
	return substitute(a:cmd,"\\([".a:c."]\\)","\\\\\\1","g")
endfunction

function! Calc( eq )
	let oldprg=&makeprg
	let &makeprg='pcalc "'.EscapeSpecialChar(a:eq,"%|").'"'
	make
	let &makeprg=oldprg
endfunction

command! -nargs=* QCALC : call Calc(<q-args>)
command! -nargs=* RCALC : exe 'r !pcalc "'.EscapeSpecialChar(<q-args>,"%").'"'
command! -nargs=* CALC : exe '!pcalc "'.EscapeSpecialChar(<q-args>,"%").'"'


function! MATE_RSync( ... )
	if a:0 >= 1
		if a:0<2
			
		else
			let dst = a:2
		endif
		let oldprg=&makeprg
		let &makeprg=g:cygwin_base_path.'\bin\bash '.GetCygwinEnv($MK_TEMPLATE_ROOT).'/etc/util/rsync.sh '.GetCygwinEnv($MK_DEVEL)." ".a:1

		if a:0>=2
			let &makeprg=&makeprg.' "'.GetCygwinEnv(a:2).'"'
		endif

		make
		let &makeprg=oldprg

		cl
	endif
endfunction

command! -nargs=* -complete=file RSYNC : call MATE_RSync(<f-args>)


function! MATE_Build( param )
	make
	cl
endfunction

command! BUILD : call MATE_Build(<q-args>)


function! ResetMate()
"	let $PATH=$MK_ORIGPATH
	let $ORIGPATH=''
	if !g:winos
		exe "let $".$MATE_LDLIBPATH_SYM."=$ORIGLDLIBPATH"
	endif
	if exists("g:mate_os_detail")
		unlet g:mate_os_detail
	endif
	if exists("g:mate_os_version")
		unlet g:mate_os_version
	endif
	return "$MK_DEVEL/make/template/etc/vim/mate.vim"
endfunction

" Now I don't know how to write multi-line command so call ResetMate and
" use its return value so the function ResetMate can be redefined by vim
command! -nargs=0 INIT : exe "source ".ResetMate()


function! ErrorFile(filename)
	let oldprg = &makeprg
	let &makeprg='cat '.a:filename
	make
	let &makeprg=oldprg
endfunction

command! -nargs=1 -complete=file ERROR : call ErrorFile(<f-args>)


function! VTblRef(...)
	if a:0 > 0
		exe "!vtblref ".expand("%")." ".a:1
	else
		exe "!vtblref ".expand("%")
	endif
endfunction

command! -nargs=* VTBLREF : call VTblRef(<f-args>) 

function! Mate_Doxygen(...)
	exe "!bash ".GetCygwinEnv($MK_TEMPLATE_ROOT)."/etc/util/doxygen.sh ".join(a:000)
endfunction

command! -nargs=* DOXYGEN : call Mate_Doxygen(<f-args>)
command! STRIPAPI : silent! %s/_API([[:blank:]]*\(.*\)[[:blank:]]*,[[:blank:]]*\(.*\)[[:blank:]]*,[[:blank:]]*(\(.*\))[[:blank:]]*)\(.*\)/\1 \2(\3);\4/g | %s/ __[FH]/ /g

